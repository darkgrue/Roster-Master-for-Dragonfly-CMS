<?php 
/***********************************************************************
  Roster Master for Dragonfly(TM) CMS
  **********************************************************************
  Copyright (C) 2005-2020 by Dark Grue

  Module main file.

  Based on Roster Master by Rex "SaintPeter" Schrader.

  With gratitude for the support from:
    The Roster Master Project (http://www.rostermaster.org/),
    The EverQuest II Community (http://forums.daybreakgames.com/eq2/),
    The Dragonfly CMS Community (http://www.dragonflycms.org), and
    Roster Master users worldwide.

  License:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or (at
  your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307 USA
***********************************************************************/
if (!defined('CPG_NUKE')) { exit; }

// For testing.
//ini_set('allow_url_fopen', '0');

// Get module configuration.
require('config.inc');


// *********************** Module Instantiation ************************
// Multiple instances of this module can be created by making a copy of
// the module directory and adding an underscore followed by an integer
// to the name of the directory (e.g. "Roster_Master_2"). The module
// will detect this and automagically configure itself.
//
// NOTE:
//	The module will check $mod_basename against its installation
//	directory.
//	Blocks that reference this module MUST be configured separately to
//	reference the new instance (e.g. naming to
//	"block-Guild_Wall_of_Fame_2.php").
$mod_basename = 'Roster_Master';
$mod_dirname = basename(dirname(__FILE__));

// Get module language file.
if (!defined('_ROSTERMASTER8')) { get_lang($mod_basename); }

$matches = array();
if (preg_match("/^{$mod_basename}(?:_(\d+))?$/", $mod_dirname, $matches)) {
	$suffix = (empty($matches[1])) ? '' : "_{$matches[1]}";
	$mod_iname = $mod_basename.$suffix;
} else {
	cpg_error('<strong>'._ERR_FATALCONFIG_RM."</strong><br /><br />\n"
		."The module installation directory, <nobr>\"{$mod_dirname}\"</nobr>, does not follow the expected instantation syntax of <nobr>\"{$mod_basename}[_<i>&lt;integer&gt;</i>]\"</nobr>.");
}

// REQUIRED: $mod_dirname
// REQUIRED: $mod_iname
// REQUIRED: $suffix
// *********************************************************************


// ************************** Global Variables *************************
// Page title.
$pagetitle .= $module_title;

// Global variables to select display of alts for current pageview.
$exclude_alts = (($config['show_alts'] == 0) && !(isset($_GET['show_all'])));
$baselink = $mod_dirname.((($config['show_alts'] == 0) && !$exclude_alts) ? '&amp;show_all=1' : '');
// Store guild name in configuration array for later use.
list($config['guild_name']) = $db->sql_ufetchrow("SELECT value FROM {$prefix}_roster_master_guild{$suffix}
	WHERE name='Guild Name'", SQL_NUM);


// ***************************** FUNCTIONS *****************************
// Include external function modules.
require_once("modules/{$mod_dirname}/functions/parser.inc");
require_once("modules/{$mod_dirname}/functions/rostermaster.inc");
require_once("modules/{$mod_dirname}/functions/questmaster.inc");
require_once("modules/{$mod_dirname}/functions/collectionmaster.inc");
if ($config['dynamic_sigs']) { require_once("modules/{$mod_dirname}/functions/sigmaster.inc"); }
require_once("modules/{$mod_dirname}/functions/utility.inc");

// Function:	retkey_in_array()
// What it does:	Find an value within an array of arrays with the haystack key returned.
function retkey_in_array($needle, $haystack) {
	foreach($haystack as $key => $value) {
		if (in_array($needle, $haystack[$key], TRUE)) {
			return $key;
		}
	}

	return FALSE;
}

// Function:	do_commands()
// What it does:	Displays the command menu header.
function do_commands() {
	global $rm, $qm, $baselink, $config, $mod_dirname, $cpgtpl;

	// Command setup, depending on verious config options and user status.
	if (($rm != 'rm_display') && ($rm != '')) {
		$commands[] = '<a href="'.getlink($baselink).'">'._VIEWROSTER.'</a>';
	}

	// Quest Master
	if (!(($rm == 'qm_display') && ($qm == 'heritage'))) {
		$commands[] = '<a href="'.getlink("{$baselink}&amp;rm=qm_display&amp;qm=heritage").'">'._TRACKERHERITAGE.'</a>';
	}
	if (!(($rm == 'qm_display') && ($qm == 'key'))) {
		$commands[] = '<a href="'.getlink("{$baselink}&amp;rm=qm_display&amp;qm=key").'">'._TRACKERKEY.'</a>';
	}
	if (!(($rm == 'qm_display') && ($qm == 'timeline'))) {
		$commands[] = '<a href="'.getlink("{$baselink}&amp;rm=qm_display&amp;qm=timeline").'">'._TRACKERTIMELINE.'</a>';
	}

	if (is_user()) {
		if ($config['logfile'] && ($rm != 'logfile')) {
			$commands[] = '<a href="'.getlink("{$baselink}&amp;rm=logfile").'">'._VIEWLOGFILE.'</a>';
		}

		if ($rm != 'rm_claim_char') {
			$commands[] = "<a href=\"".getlink("{$baselink}&amp;rm=rm_claim_char").'">'._MANAGECHAR.'</a>';
		}
	}

	$cpgtpl->assign_vars(array(
		'S_GUILD_NAME' => $config['guild_name'],
		'B_CAN_ADMIN' => can_admin($mod_dirname),
		'S_USER_COMMANDS' => '[&nbsp;'.implode('&nbsp;| ', $commands).'&nbsp;]'
		));

	if (can_admin($mod_dirname)) {
		if ($rm != 'rm_manage_claims') {
			$admin_commands[] = '<a href="'.getlink("{$baselink}&amp;rm=rm_manage_claims").'">'._MANAGEROSTER.'</a>';
		}
		$admin_commands[] = '<a href="'.getlink("{$baselink}&amp;force_update=1").'">'._FORCEUPDATE.'</a>';

		$cpgtpl->assign_vars(array('S_ADMIN_COMMANDS' => '[&nbsp;'.implode('&nbsp;| ', $admin_commands).'&nbsp;]'));
	}

	$cpgtpl->set_filenames(array('do_commands' => 'roster_master/do_commands.html'));
	$cpgtpl->display('do_commands');
}

// Function:	logfile()
// What it does:	Displays the contents of the file 'logfile.inc', if it exists.
function logfile() {
	global $config, $pagetitle;

	$pagetitle .= ' '._BC_DELIM.' '._LOG;

	$log_filename = dirname(__FILE__).'/logfile.inc';
	if (file_exists($log_filename)) {
		$log_contents = htmlprepare(file_get_contents($log_filename), TRUE, ENT_NOQUOTES, TRUE);
	} else {
		$log_contents = '<br /><center><i>'._LOG_NOLOGFILE.'</i></center>';
	}

	// Display code
	require('header.php');
	OpenTable();

	do_commands();

	OpenTable();
	if (empty($log_contents)) {
		$log_contents = '<br /><center><i>'._LOG_NOLOGCONTENT.'</i></center>';
	}
	echo $log_contents."<br /><br />\n";
	CloseTable();

	CloseTable();
}


// *************************** MODE SELECT *****************************
// Get the action type.
$rm = (isset($_GET['rm'])) ? $_GET['rm'] : ((isset($_POST['rm'])) ? $_POST['rm'] : '');

// Get the quest type and validate it.
if (strpos($rm, 'qm_') === 0) {
	if (isset($_GET['qm'])) {
		$qm = $_GET['qm'];
	} elseif (isset($_POST['qm'])) {
		$qm = $_POST['qm'];
	}
	if (!isset($qm) || !in_array($qm, array('heritage', 'key', 'timeline'))) {
		// Not valid quest type.
		cpg_error(_ERR_ACCESSCONT);
	}

	// Load quest data.
	require_once("modules/{$mod_dirname}/includes/quests_{$qm}.inc");
}

// Dispatch action.
switch ($rm) {
	// Roster Master
	case 'rm_display':
		update_data();
		rostermaster();
		break;
	case 'rm_claim_char':
		rm_claim_char();
		break;
	case 'rm_save_claim':
		rm_save_claim();
		break;
	case 'rm_edit_claim':
		rm_edit_claim();
		break;
	case 'rm_delete_claim':
		rm_delete_claim();
		break;
	case 'rm_manage_claims':
		rm_manage_claims();
		break;

	// Logfile
	case 'logfile':
		update_data();
		logfile();
		break;

	// Quest Master
	case 'qm_display':
		update_data();
		questmaster();
		break;
	case 'qm_subquery':
		update_data();
		qm_subquery();
		break;
	case 'qm_manage_claims':
		update_data();
		qm_manage_claims();
		break;
	case 'qm_save_claims':
		qm_save_claims();
		break;

	// Collection Master
	case 'cm_display':
		collectionmaster();
		break;

	// Signature Master
	case 'sm_sig':
		if (!$config['dynamic_sigs']) {
			// Signatures disabled in configuration.
			cpg_error(_ERR_ACCESSCONT);
		}
		fetchsignature();
		break;

	default:
		$rm = '';
		update_data();
		rostermaster();
}
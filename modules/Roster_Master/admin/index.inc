<?php
/***********************************************************************
  Roster Master for Dragonfly(TM) CMS
  **********************************************************************
  Copyright (C) 2005-2018 by Dark Grue

  Module main administration system file.

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
if (!defined('ADMIN_PAGES')) { exit; }


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
$mod_dirname = basename(dirname(dirname(__FILE__)));

// Get module language file.
if ($mod_dirname != $mod_basename) { get_lang($mod_basename); }

// Check for administrator privledges for this module instance.
if (!can_admin($mod_dirname)) { cpg_error(_ERR_ACCESSCONT); }

$matches = array();
if (preg_match("/^{$mod_basename}(?:_(\d+))?$/", $mod_dirname, $matches)) {
	$suffix = (empty($matches[1])) ? '' : "_{$matches[1]}";
	$mod_iname = $mod_basename.$suffix;
} else {
	cpg_error('<strong>'._ERR_FATALCONFIG_RM."</strong><br /><br />\n".sprintf(_ERR_INSTANTIATION, $mod_dirname, $mod_basename));
}

$mod_displayname = str_replace('_', ' ', $mod_basename).((empty($suffix)) ? '' : ' ('.ltrim($suffix, '_').')');

// REQUIRED: $mod_dirname
// REQUIRED: $mod_iname
// REQUIRED: $suffix
// REQUIRED: $mod_displayname
// *********************************************************************


// ************************** Global Variables *************************
// Get module configuration.
require("modules/{$mod_dirname}/config.inc");

// Enable developer functions.
// FIXME - TODO: Verify unset before releasing source.
$DEVELOPER_RM = TRUE;


// ***************************** FUNCTIONS *****************************
// Include external function modules.
require_once("modules/{$mod_dirname}/functions/parser.inc");
//require_once("modules/{$mod_dirname}/functions/rostermaster.inc");
require_once("modules/{$mod_dirname}/functions/questmaster.inc");
require_once("modules/{$mod_dirname}/functions/sigmaster.inc");

// Function:	rm_admin_menu()
// What it does:	Construct administration menu bar.
function rm_admin_menu($mode) {
	global $baselink, $config, $mod_displayname, $DEVELOPER_RM;

	// Build administation commands.
	if ($config['logfile']) {
		if ($mode == 'view_logfile') {
			$admin_commands[] = '[ <b>'._VIEWLOGFILE.'</b>';
		} else {
			$admin_commands[] = '[ <a href="'.adminlink('&mode=view_logfile').'">'._VIEWLOGFILE.'</a>';
		}
		if ($mode == 'purge_logfile') {
			$admin_commands[] = '<b>'._PURGELOGFILE.'</b> ]';
		} else {
			$admin_commands[] = '<a href="'.adminlink('&mode=purge_logfile').'">'._PURGELOGFILE.'</a> ]';
		}
	}
	if ($mode == 'optimize_tables') {
		$admin_commands[] = '[ <b>'._OPTIMZETABLES.'</b>';
	} else {
		$admin_commands[] = '[ <a href="'.adminlink('&mode=optimize_tables').'">'._OPTIMZETABLES.'</a>';
	}
	if ($mode == 'purge_table') {
		$admin_commands[] = '<b>'._PURGETABLE.'</b> ]';
	} else {
		$admin_commands[] = '<a href="'.adminlink('&mode=purge_table').'">'._PURGETABLE.'</a> ]';
	}
	if ($config['dynamic_sigs']) {
		if ($mode == 'sm_fill_sig') {
			$admin_commands[] = '[ <b>'._FILLSIGCACHE.'</b>';
		} else {
			$admin_commands[] = '[ <a href="'.adminlink('&mode=sm_fill_sig').'">'._FILLSIGCACHE.'</a>';
		}
		if ($mode == 'sm_purge_sig') {
			$admin_commands[] = '<b>'._PURGESIGCACHE.'</b> ]';
		} else {
			$admin_commands[] = '<a href="'.adminlink('&mode=sm_purge_sig').'">'._PURGESIGCACHE.'</a> ]';
		}
	}
	if ($mode == 'diagnostics') {
		$admin_commands[] = '[ <b>'._DIAGNOSTICS.'</b>';
	} else {
		$admin_commands[] = '[ <a href="'.adminlink('&mode=diagnostics').'">'._DIAGNOSTICS.'</a>';
	}
	if ($mode == 'diagnostics_php') {
		$admin_commands[] = '<b>'._PHPINFO.'</b>';
	} else {
		$admin_commands[] = '<a href="'.adminlink('&mode=diagnostics_php').'">'._PHPINFO.'</a>';
	}
	if ($mode == 'lookup_guildid') {
		$admin_commands[] = '<b>'._LOOKUPGID.'</b> ]';
	} else {
		$admin_commands[] = '<a href="'.adminlink('&mode=lookup_guildid').'">'._LOOKUPGID.'</a> ]';
	}

	// Build developer commands.
	if ($mode == 'build_questsdata') {
		$dev_commands[] = '[ <b>'._BUILDQUESTS.'</b>';
	} else {
		$dev_commands[] = '[ <a href="'.adminlink('&mode=build_questsdata').'">'._BUILDQUESTS.'</a>';
	}
	if ($mode == 'build_collectionsdata') {
		$dev_commands[] = '<b>'._BUILDCOLLECTIONS.'</b>';
	} else {
		$dev_commands[] = '<a href="'.adminlink('&mode=build_collectionsdata').'">'._BUILDCOLLECTIONS.'</a>';
	}
	if ($mode == 'build_collectionsicons') {
		$dev_commands[] = '<b>'._BUILDICONS.'</b> ]';
	} else {
		$dev_commands[] = '<a href="'.adminlink('&mode=build_collectionsicons').'">'._BUILDICONS.'</a> ]';
	}


	echo "<span class=\"gen\"><strong>{$mod_displayname} "._ADMINISTRATION."</strong></span><br />\n"
		.implode('&nbsp;| ', $admin_commands)."<br />\n<br />\n";
	if ($DEVELOPER_RM) {
		echo '<span class="gen"><strong>'._DEVELOPERTOOLS."</strong></span><br />\n"
			.implode('&nbsp;| ', $dev_commands)."<br />\n<br />\n";
	}
	echo '<b>'._ROSTER."</b><br />\n[ "
		.'<a href="'.getlink("{$baselink}&rm=rm_manage_claims").'">'._MANAGEROSTER.'</a> | '
		.'<a href="'.getlink("{$baselink}&force_update=1").'">'._FORCEUPDATE.'</a>'
		." ]<br />\n";
}

// Function:	view_logfile()
// What it does:	Displays the contents of the file 'logfile.inc', if it exists.
function view_logfile() {
	$log_filename = dirname(__FILE__).'/../logfile.inc';
	if (file_exists($log_filename)) {
		$log_contents = htmlprepare(file_get_contents($log_filename), TRUE, ENT_NOQUOTES, TRUE);
	} else {
		$log_contents = '<br /><center><i>'._LOG_NOLOGFILE.'</i></center>';
	}

	// Display code
	OpenTable();
	rm_admin_menu('view_logfile');
	echo "<hr />\n<br />\n"
		.'<span class="genmed"><strong>'._VIEWLOGFILE."</strong></span><br />\n"
		._VIEWLOG."<br /><br />\n";

	OpenTable();
	if (empty($log_contents)) {
		$log_contents = '<br /><center><i>'._LOG_NOLOGCONTENT.'</i></center>';
	}
	echo $log_contents."<br /><br />\n";
	CloseTable();
	CloseTable();
}

// Function:	purge_logfile()
// What it does:	Admin interface to purge the logfile.
// Requires:
//		$_POST['confirm'] -- verification token
function purge_logfile() {
	global $mod_dirname, $db, $prefix, $suffix, $baselink;

	if (isset($_POST['confirm'])) {
		// Truncate the logfile.
		$log_filename = dirname(__FILE__).'/../logfile.inc';
		if ($logfile = fopen($log_filename, 'w')) {
			fclose($logfile);
		} else {
			cpg_error('<strong>'._ERR_FATALCONFIG_RM."</strong><br /><br />\n"._ERR_LOGWRITE);
		}
		cpg_error(_LOGPRGECOMP, _PURGELOGFILE, adminlink($mod_dirname));
	} else if (isset($_POST['cancel'])) { cpg_error(_OPCANCEL, _PURGELOGFILE, adminlink($mod_dirname));
	} else {
		OpenTable();
		rm_admin_menu('purge_logfile');
		echo "<hr />\n<br />\n"
			.'<span class="genmed"><strong>'._PURGELOGFILE."</strong></span><br />\n"
			._PURGELOGDESC."<br /><br />\n";

		echo open_form(adminlink('&mode=purge_logfile'), '', _PURGELOGFILE);
		echo _PURGELOGCONF."<br /><br />\n"
			.'<div style="text-align:center"><input type="submit" name="confirm" value="'._YES."\" class=\"mainoption\" />&nbsp;&nbsp;&nbsp;\n"
			.'<input type="submit" name="cancel" value="'._NO."\" class=\"liteoption\" /></div>\n";
		echo close_form();
		CloseTable();
	}
}

// Function:	optimize_tables()
// What it does:	Admin interface to optimize the database tables by removing all widowed
//			and orphaned records from the database).
// Requires:
//		$_POST['confirm'] -- verification token
function optimize_tables() {
	global $db, $prefix, $user_prefix, $suffix, $mod_dirname, $baselink;

	if (isset($_POST['confirm'])) {
		// Fetch widowed Claims
		// Widowed claims are claims that no longer have characters associated with them.
		$result = $db->sql_query("SELECT rmu.characterId FROM {$prefix}_roster_master_users{$suffix} AS rmu
			LEFT JOIN {$prefix}_roster_master{$suffix} AS rm USING (characterId)
			LEFT JOIN {$user_prefix}_users AS cmsusers USING (user_id)
			WHERE rm.characterId IS NULL
			ORDER BY cmsusers.username", FALSE);
		while($char_info = $db->sql_fetchrow($result, SQL_ASSOC)) {
			// Delete quest statuses that no longer have characters associated with them.
			$db->sql_query("DELETE FROM {$prefix}roster_master_quest_status{$suffix} WHERE characterId='{$char_info['characterId']}'", TRUE);
		}
		// Fetch orphaned Claims.
		// Orphaned claims are claims that no longer have usernames associated
		// with them.
		$result = $db->sql_query("SELECT rmu.characterId FROM {$prefix}_roster_master_users{$suffix} AS rmu
			LEFT JOIN {$prefix}_roster_master{$suffix} AS rm USING (characterId)
			LEFT JOIN {$user_prefix}_users AS cmsusers USING (user_id)
			WHERE cmsusers.user_id IS NULL
			ORDER BY cmsusers.username", FALSE);
		while($char_info = $db->sql_fetchrow($result, SQL_ASSOC)) {
			// Delete claims.
			foreach(array('quest_status', 'collection_status') as $table) {
				$db->sql_query("DELETE FROM {$prefix}_roster_master_{$table}{$suffix} WHERE characterId='{$char_info['characterId']}'", TRUE);
			}
		}
		cpg_error(_OPTIMIZETABLESCOMP, _OPTIMZETABLES, adminlink($mod_dirname));
	} else if (isset($_POST['cancel'])) { cpg_error(_OPCANCEL, _OPTIMZETABLES, adminlink($mod_dirname));
	} else {
		OpenTable();
		rm_admin_menu('optimize_tables');
		echo "<hr />\n<br />\n"
			.'<span class="genmed"><strong>'._OPTIMZETABLES."</strong></span><br />\n"
			._OPTIMIZETABLESDESC."<br /><br />\n";

		echo open_form(adminlink('&mode=optimize_tables'), '', _OPTIMZETABLES);
		echo _OPTIMIZETABLESCONF."<br /><br />\n"
			.'<div style="text-align:center"><input type="submit" name="confirm" value="'._YES."\" class=\"mainoption\" />&nbsp;&nbsp;&nbsp;\n"
			.'<input type="submit" name="cancel" value="'._NO."\" class=\"liteoption\" /></div>\n";
		echo close_form();
		CloseTable();
	}
}

// Function:	purge_table()
// What it does:	Admin interface to purge the roster_master table (i.e. remove all parsed
//			roster information from the database). Claims are left intact.
// Requires:
//		$_POST['confirm'] -- verification token
function purge_table() {
	global $mod_dirname, $db, $prefix, $suffix, $baselink;

	if (isset($_POST['confirm'])) {
		// Empty out the roster table.
		$db->sql_query("TRUNCATE TABLE {$prefix}_roster_master{$suffix}");
		$db->sql_query("TRUNCATE TABLE {$prefix}_roster_master_quest_status{$suffix}");
		$db->sql_query("TRUNCATE TABLE {$prefix}_roster_master_collection_status{$suffix}");
		// Reset the date the table was last updated.
		$db->sql_query("UPDATE {$prefix}_roster_master_guild{$suffix}
			SET value='0'
			WHERE name='table_updated'", FALSE);
		cpg_error(_PURGETABLECOMP, _PURGETABLE, adminlink($mod_dirname));
	} else if (isset($_POST['cancel'])) { cpg_error(_OPCANCEL, _PURGETABLE, adminlink($mod_dirname));
	} else {
		OpenTable();
		rm_admin_menu('purge_table');
		echo "<hr />\n<br />\n"
			.'<span class="genmed"><strong>'._PURGETABLE."</strong></span><br />\n"
			._PURGETABLEDESC."<br /><br />\n";

		echo open_form(adminlink('&mode=purge_table'), '', _PURGETABLE);
		echo sprintf(_PURGETABLECONF, adminlink("modules"))."<br /><br /><br />\n"
			.'<div style="text-align:center"><input type="submit" name="confirm" value="'._YES."\" class=\"mainoption\" />&nbsp;&nbsp;&nbsp;\n"
			.'<input type="submit" name="cancel" value="'._NO."\" class=\"liteoption\" /></div>\n";
		echo close_form();
		CloseTable();
	}
}

// Function:	sm_fill_sigcache()
// What it does:	Admin interface to pregenerate the signature block cache (i.e. create
//			all images for valid characters and store them in the signature cache
//			directory).
//			User must be logged in as Admin to use.
// Requires:
//		$_GET['ok'] -- verification token
function sm_fill_sigcache() {
	global $mod_dirname, $baselink;

	if (isset($_POST['confirm'])) {
		// Fully populate the signature cache.
		list($count, $total, $time) = fillsigcache();
		cpg_error(_FILLSIGCACHECOMP."<br /><br />\n".sprintf(_NTC_SMCFILL, $count, $total, $time), _FILLSIGCACHE, adminlink($mod_dirname));
	} else if (isset($_POST['cancel'])) { cpg_error(_OPCANCEL, _FILLSIGCACHE, adminlink($mod_dirname));
	} else {
		OpenTable();
		rm_admin_menu('sm_fill_sig');
		echo "<hr />\n<br />\n"
			.'<span class="genmed"><strong>'._FILLSIGCACHE."</strong></span><br />\n"
			._FILLSIGCACHEDESC."<br /><br />\n";

		echo open_form(adminlink('&mode=sm_fill_sig'), '', _FILLSIGCACHE);
		echo _FILLSIGCACHECONF."<br /><br /><br />\n"
			.'<div style="text-align:center"><input type="submit" name="confirm" value="'._YES."\" class=\"mainoption\" />&nbsp;&nbsp;&nbsp;\n"
			.'<input type="submit" name="cancel" value="'._NO."\" class=\"liteoption\" /></div>\n";
		echo close_form();
		CloseTable();
	}
}

// Function:	sm_purge_sigcache()
// What it does:	Admin interface to purge the signature block cache (i.e. remove all
//			images from the signature cache directory).
// Requires:
//		$_GET['ok'] -- verification token
function sm_purge_sigcache() {
	global $mod_dirname, $db, $prefix, $suffix, $sig_savepath, $baselink;

	if (isset($_POST['confirm'])) {
		$count = 0;
		foreach(glob("{$sig_savepath}/sig_*.jpg") as $file) {
			@unlink($file);
			$count++;
		}
		cpg_error(sprintf(_PURGESIGCACHECOMP, $count), _PURGESIGCACHE, adminlink($mod_dirname));
	} else if (isset($_POST['cancel'])) { cpg_error(_OPCANCEL, _PURGESIGCACHE, adminlink($mod_dirname));
	} else {
		OpenTable();
		rm_admin_menu('sm_purge_sig');
		echo "<hr />\n<br />\n"
			.'<span class="genmed"><strong>'._PURGESIGCACHE."</strong></span><br />\n"
			._PURGESIGCACHEDESC."<br /><br />\n";

		echo open_form(adminlink('&mode=sm_purge_sig'), '', _PURGESIGCACHE);
		echo _PURGESIGCACHECONF."<br /><br /><br />\n"
			.'<div style="text-align:center"><input type="submit" name="confirm" value="'._YES."\" class=\"mainoption\" />&nbsp;&nbsp;&nbsp;\n"
			.'<input type="submit" name="cancel" value="'._NO."\" class=\"liteoption\" /></div>\n";
		echo close_form();
		CloseTable();
	}
}

// Function:	diagnostics_php()
// What it does:	Outputs information about PHP's configuration.
function diagnostics_php() {
	global $bgcolor1, $bgcolor2, $bgcolor3, $mod_dirname, $config, $db, $prefix, $BASEHREF, $baselink, $sig_savepath;

	OpenTable();
	rm_admin_menu('diagnostics_php');
	echo "<hr />\n<br />\n"
		.'<span class="genmed"><strong>PHP '._DIAGNOSTICS."</strong></span><br />\n"
		._PHPDIAGNOSTICSDESC."<br /><br />\n";

	OpenTable();

	echo '<style type="text/css">
		#phpinfo {background-color: #fff; color: #222; font-family: sans-serif}
		#phpinfo pre {margin: 0; font-family: monospace;}
		#phpinfo a:link {color: #009; text-decoration: none; background-color: #fff;}
		#phpinfo a:hover {text-decoration: underline;}
		#phpinfo table {border-collapse: collapse; border: 0; width: 934px; box-shadow: 1px 2px 3px #ccc;}
		#phpinfo .center {text-align: center;}
		#phpinfo .center table {margin: 1em auto; text-align: left;}
		#phpinfo .center th {text-align: center !important;}
		#phpinfo td, th {background-color: #99c; background-image: none; color: #222; height: auto; border: 1px solid #666; font-size: 75%; vertical-align: baseline; padding: 4px 5px;}
		#phpinfo h1 {font-size: 150%;}
		#phpinfo h2 {font-size: 125%;}
		#phpinfo .p {text-align: left;}
		#phpinfo .e {background-color: #ccf; width: 300px; font-weight: bold;}
		#phpinfo .h {background-color: #99c; font-weight: bold;}
		#phpinfo .v {background-color: #ddd; max-width: 300px; overflow-x: auto; word-wrap: break-word;}
		#phpinfo .v i {color: #999;}
		#phpinfo img {float: right; border: 0;}
		#phpinfo hr {width: 934px; background-color: #ccc; border: 0; height: 1px;}
		</style>';

	ob_start();
	phpinfo(INFO_ALL);
	$cache = ob_get_contents();
	ob_end_clean();

	$cache = str_replace('module_Zend Optimizer', 'module_Zend_Optimizer', preg_replace('%^.*<body>(.*)</body>.*$%ms', '$1', $cache));
	echo '<div id="phpinfo" width=100%>'.$cache.'</div>';

	CloseTable();

	CloseTable();
}

// Function:	diagnostics()
// What it does:	Perform system configuration diagnostics relevant to the Roster Master
//			module.
function diagnostics() {
	global $bgcolor1, $bgcolor2, $bgcolor3, $mod_dirname, $config, $db, $prefix, $BASEHREF, $baselink, $sig_savepath;

	OpenTable();
	rm_admin_menu('diagnostics');
	echo "<hr />\n<br />\n"
		.'<span class="genmed"><strong>'._DIAGNOSTICS."</strong></span><br />\n"
		._DIAGNOSTICSDESC."<br /><br />\n";

	OpenTable();
	echo '<table style="border:none;width:100%" cellpadding="2" cellspacing="2">';

	// DRAGONFLY
	echo "  <tr bgcolor=\"$bgcolor2\">"
		.'    <td style="text-align:left;white-space:nowrap"><span class="nav">Dragonfly</span></td>'
		.'  </tr>'
		.'  <tr>'
		.'    <td>'
		.'      <table class="forumline" style="width:100%" cellpadding="4" cellspacing="1">';
	// CMS Version
	echo '<tr><td>'._CMSVERSION.'</td><td>'.CPG_NUKE.'</td></tr>';
	// PHP Version
	echo '<tr><td class="row2">'._PHPVERSION.'</td><td class="row2">'.phpversion().'</td></tr>';
	echo '<tr><td>PHP_INT_SIZE</td><td>'.PHP_INT_SIZE.'</td></tr>';
	echo '<tr><td class="row2">PHP_INT_MAX</td><td class="row2">'.PHP_INT_MAX.'</td></tr>';	
	// Server Environment
	echo '<tr><td>HTTP_HOST</td><td>'.$_SERVER['HTTP_HOST'].'</td></tr>';
	echo '<tr><td class="row2">SCRIPT_NAME</td><td class="row2">'.$_SERVER['SCRIPT_NAME'].'</td></tr>';
	echo '<tr><td>SCRIPT_NAME (realpath)</td><td>'.realpath($_SERVER['SCRIPT_NAME']).'</td></tr>';
	echo '<tr><td class="row2">SCRIPT_FILENAME</td><td class="row2">'.$_SERVER['SCRIPT_FILENAME'].'</td></tr>';
	echo '<tr><td>SCRIPT_FILENAME (realpath)</td><td>'.realpath($_SERVER['SCRIPT_FILENAME']).'</td></tr>';
	echo '<tr><td class="row2">PHP_SELF</td><td class="row2">'.$_SERVER['PHP_SELF'].'</td></tr>';
	echo '<tr><td>PHP_SELF (realpath)</td><td>'.realpath($_SERVER['PHP_SELF']).'</td></tr>';
	echo '<tr><td class="row2">__FILE__</td><td class="row2">'.__FILE__.'</td></tr>';
	echo '<tr><td>__FILE__ (realpath)</td><td>'.realpath(__FILE__).'</td></tr>';
	echo '<tr><td class="row2">'._SITEROOT.'</td><td class="row2">'.substr($_SERVER['SCRIPT_NAME'],0,(strrpos($_SERVER['SCRIPT_NAME'],'/')+1)).'</td></tr>';
	echo '<tr><td>$_GET</td><td><pre style="text-align:left">'.htmlspecialchars(print_r($_GET, TRUE)).'</pre></td></tr>';
	echo '<tr><td class="row2">$_POST</td><td class="row2"><pre style="text-align:left">'.htmlspecialchars(print_r($_POST, TRUE)).'</pre></td></tr>';
	// ************* END DRAGONFLY *************
	echo '      </table>'
		.'  <br />'
		.'    </td>'
		.'  </tr>';

	// ROSTER MASTER
	echo "  <tr bgcolor=\"$bgcolor2\">"
		.'    <td style="text-align:left;white-space:nowrap"><span class="nav">Roster Master</span></td>'
		.'  </tr>'
		.'  <tr>'
		.'    <td>'
		.'      <table class="forumline" style="width:100%" cellpadding="4" cellspacing="1">';
	// RM4DF Schema Version
	list($version) = $db->sql_ufetchrow("SELECT version FROM {$prefix}_modules WHERE title='{$mod_dirname}'", SQL_NUM);
	echo "<tr><td>RM4DF Schema Version</td><td>{$version}</td></tr>";
	// $mod_dirname
	echo "<tr><td class=\"row2\">\$mod_dirname</td><td class=\"row2\">{$mod_dirname}</td></tr>";
	// RM4DF Module Absolute Link
	echo "<tr><td>RM4DF Module Absolute Link</td><td>{$BASEHREF}modules/{$mod_dirname}/</td></tr>";
	// RM4DF Configuration File
	echo '<tr><td class="row2">RM4DF Configuration File</td><td class="row2">'
		.((!preg_match('/file_get_contents/', @ini_get('suhosin.executor.func.blacklist')) && function_exists('file_get_contents'))
			? (($config_inc = file_get_contents("modules/{$mod_dirname}/config.inc"))
				? ((preg_match('/^.*<\?/', $config_inc)) ? '<span style="color:green">OK</span>' : '<b style="color:red">FAIL</b> - characters before PHP tag are not allowed') 
				: 'cannot open config.inc')
			: ((preg_match('/file_get_contents/', ini_get('disable_functions'))) ? '<b style="color:red">WARNING</b> - file_get_contents() disabled by configuration' : '<b style="color:red">WARNING</b> - file_get_contents() does not exist'))
		.'</td></tr>';
	// Check for socket connection support [fsockopen()].
	echo '<tr><td>Check fsockopen() function</td><td>'
		.((!preg_match('/fsockopen/', @ini_get('suhosin.executor.func.blacklist')) && function_exists('fsockopen')) ? '<span style="color:green">OK</span>' : ((preg_match('/fsockopen/', ini_get('disable_functions'))) ? '<b style="color:red">FAIL</b> - is disabled by configuration' : '<b style="color:red">FAIL</b> - does not exist'))
		.'</td></tr>';
	// Check for file support [fopen()].
	echo '<tr><td class="row2">Check fopen() function</td><td class="row2">'
		.((!preg_match('/fopen/', @ini_get('suhosin.executor.func.blacklist')) && function_exists('fopen')) ? '<span style="color:green">OK</span>' : ((preg_match('/fopen/', ini_get('disable_functions'))) ? '<b style="color:red">FAIL</b> - is disabled by configuration' : '<b style="color:red">FAIL</b> - does not exist'))
		.'</td></tr>';
	// Check for socket connection support [fwrite()].
	echo '<tr><td>Check fwrite() function</td><td>'
		.((!preg_match('/fwrite/', @ini_get('suhosin.executor.func.blacklist')) && function_exists('fwrite')) ? '<span style="color:green">OK</span>' : ((preg_match('/fwrite/', ini_get('disable_functions'))) ? '<b style="color:red">FAIL</b> - is disabled by configuration' : '<b style="color:red">FAIL</b>savepath
			- does not exist'))
		.'</td></tr>';
	// Check for socket connection support [fgets()].
	echo '<tr><td class="row2">Check fgets() function</td><td class="row2">'
		.((!preg_match('/fgets/', @ini_get('suhosin.executor.func.blacklist')) && function_exists('fgets')) ? '<span style="color:green">OK</span>' : ((preg_match('/fgets/', ini_get('disable_functions'))) ? '<b style="color:red">FAIL</b> - is disabled by configuration' : '<b style="color:red">FAIL</b> - does not exist'))
		.'</td></tr>';
	// Do end-to-end check to Census API.
	$statuscode = fetch_URL($config['json_guild_url'], TRUE);
	echo '<tr><td>Check Census API HTTP connection end-to-end [via fetch_URL()]</td><td>'
		.(($statuscode == '200') ? '<span style="color:green">OK</span>' : "<b style=\"color:red\">FAIL</b> - Recieved HTTP response status code $statuscode")
		.'</td></tr>';
	// Check if the cURL extension is loaded.
	echo '<tr><td class="row2">Check cURL extension [via extension_loaded()]</td><td class="row2">'
		.((extension_loaded('cURL')) ? '<span style="color:green">OK</span>' : '<b style="color:red">FAIL</b> - does not exist')
		.'</td></tr>';
	// Check for JSON decode support [json_decode()].
	echo '<tr><td>Check json_decode() function</td><td>'
		.((!preg_match('/json_decode/', @ini_get('suhosin.executor.func.blacklist')) && function_exists('json_decode')) ? '<span style="color:green">OK</span>' : ((preg_match('/json_decode/', ini_get('disable_functions'))) ? '<b style="color:red">FAIL</b> - is disabled by configuration' : '<b style="color:red">FAIL</b> - does not exist'))
		.'</td></tr>';
	// Check for JSON error support [json_last_error()].
	echo '<tr><td class="row2">Check json_last_error() function</td><td class="row2">'
		.((!preg_match('/json_last_error/', @ini_get('suhosin.executor.func.blacklist')) && function_exists('json_last_error')) ? '<span style="color:green">OK</span>' : ((preg_match('/json_last_error/', ini_get('disable_functions'))) ? '<b style="color:red">FAIL</b> - is disabled by configuration' : '<b style="color:red">FAIL</b> - does not exist'))
		.'</td></tr>';
	// Check for JSON error support [json_last_error_msg()].
	echo '<tr><td>Check json_last_error_msg() function</td><td>'
		.((!preg_match('/json_last_error_msg/', @ini_get('suhosin.executor.func.blacklist')) && function_exists('json_last_error_msg')) ? '<span style="color:green">OK</span>' : ((preg_match('/json_last_error_msg/', ini_get('disable_functions'))) ? '<b style="color:red">FAIL</b> - is disabled by configuration' : '<b style="color:red">FAIL</b> - does not exist'))
		.'</td></tr>';
	// ************* END ROSTER MASTER *************
	echo '      </table>'
		.'  <br />'
		.'    </td>'
		.'  </tr>';

	// SIGNATURE MASTER
	echo "  <tr bgcolor=\"$bgcolor2\">"
		.'    <td style="text-align:left;white-space:nowrap"><span class="nav">Signature Master</span></td>'
		.'  </tr>'
		.'  <tr>'
		.'    <td>'
		.'      <table class="forumline" style="width:100%" cellpadding="4" cellspacing="1">';
	// Check if the GD library extension is loaded.
	echo '<tr><td>GD Library Extension</td><td>'
		.((extension_loaded('gd')) ? '<span style="color:green">OK</span>' : '<b style="color:red">FAIL</b> - not loaded and/or installed')
		.'</td></tr>';
	// Check for FreeType support.
	if (!preg_match('/gd_info/', @ini_get('suhosin.executor.func.blacklist')) && function_exists('gd_info')) {
		$gdver_info = gd_info();
//		cpg_error('<pre style="text-align:left">'.print_r($ver_info, TRUE)."</pre>\n");
		echo '<tr><td class="row2">FreeType Support [via gd_info()]</td><td class="row2">'
			.(($gdver_info['FreeType Support']) ? '<span style="color:green">OK</span>' : '<b style="color:red">FAIL</b> - not installed')
			.'</td></tr>';
	} else {
		echo '<tr><td class="row2">FreeType Support [via phpinfo()]</td><td class="row2">';
		if (!preg_match('/phpinfo/', @ini_get('suhosin.executor.func.blacklist')) && function_exists('phpinfo')) {
			ob_start();
			phpinfo(INFO_MODULES);
			$php_info = strip_tags(ob_get_contents());
			ob_end_clean();
//			cpg_error('<pre style="text-align:left">'.$php_info."</pre>\n");
			$matches = array();
			preg_match('/gd\n\n(.*?)\n\n/s', $php_info, $matches);
			echo ((preg_match('/FreeType Support enabled/', $matches[1])) ? '<span style="color:green">OK</span>' : '<b style="color:red">FAIL</b> - not installed');
		} else {
			echo 'WARNING - indeterminite, phpinfo() disabled';
		}
		echo '</td></tr>';
	}
	// Check for FreeType support [imagettftext()].
	echo '<tr><td>Check imagettftext() function</td><td>'
		.((!preg_match('/imagettftext/', @ini_get('suhosin.executor.func.blacklist')) && function_exists('imagettftext')) ? '<span style="color:green">OK</span>' : ((preg_match('/imagettftext/', ini_get('disable_functions'))) ? '<b style="color:red">FAIL</b> - is disabled by configuration' : '<b style="color:red">FAIL</b> - does not exist'))
		.'</td></tr>';
	// Check image path is writable.
	echo "<tr><td class=\"row2\">\$sig_savepath directory ({$sig_savepath}) is writable</td><td class=\"row2\">"
		.((is_writable($sig_savepath)) ? '<span style="color:green">OK</span>' : '<b style="color:red">FAIL</b> - does not exist or unwritable')
		.'</td></tr>';
	// Check signature block image file is readable.
	echo "<tr><td>Signature block image file (modules/{$mod_dirname}/images/sigblocks/{$config['sigblock_basename']}.png) is readable</td><td>"
		.((is_readable("modules/{$mod_dirname}/images/sigblocks/{$config['sigblock_basename']}.png")) ? '<span style="color:green">OK</span>' : '<b style="color:red">FAIL</b> - unreadable')
		.'</td></tr>';
	// Check signature block template file is readable.
	echo "<tr><td class=\"row2\">Signature block template file (modules/{$mod_dirname}/images/sigblocks/{$config['sigblock_basename']}.txt) is readable</td><td class=\"row2\">"
		.((is_readable("modules/{$mod_dirname}/images/sigblocks/{$config['sigblock_basename']}.txt")) ? '<span style="color:green">OK</span>' : '<b style="color:red">FAIL</b> - unreadable')
		.'</td></tr>';
	// Check image URL.
	echo '<tr><td>Base signature image file link</td><td>'
		.getlink("{$baselink}&rm=sm_sig&characterId=nnnnnn", TRUE, TRUE)
		.'</td></tr>';
	// Check cached image URL.
	echo "<tr><td class=\"row2\">Direct (cached) signature image file link</td><td class=\"row2\">{$BASEHREF}modules/{$mod_dirname}/signatures/sig_nnnnnn.jpg</td></tr>";
	// Check config.inc setting.
	echo "<tr><td>\$config['dynamic_sigs']</td><td>{$config['dynamic_sigs']}</td></tr>";
	// ************* END SIGNATURE MASTER *************
	echo '      </table>'
		.'  <br />'
		.'    </td>'
		.'  </tr>';

	// SIGNATURE IMAGE REWRITE HACK
	echo "  <tr bgcolor=\"$bgcolor2\">"
		.'    <td style="text-align:left;white-space:nowrap"><span class="nav">Signature Image Rewrite Hack</span></td>'
		.'  </tr>'
		.'  <tr>'
		.'    <td>'
		.'      <table class="forumline" style="width:100%" cellpadding="4" cellspacing="1">';
	// Check that mod_rewrite is loaded.
	if (!preg_match('/apache_get_modules/', @ini_get('suhosin.executor.func.blacklist')) && function_exists('apache_get_modules')) {
		$load_mod = apache_get_modules();
//		cpg_error('<pre style="text-align:left">'.print_r($load_mod, TRUE)."</pre>\n");
		echo '<tr><td>Check mod_rewrite Apache module [via apache_get_modules()]</td><td>'
			.((in_array('mod_rewrite', $load_mod)) ? '<span style="color:green">OK</span>' : '<b style="color:red">FAIL</b> - not loaded or does not exist')
			.'</td></tr>';
	} else {
		echo '<tr><td>Check mod_rewrite Apache module [via phpinfo()]</td><td>';
		if (!preg_match('/phpinfo/', @ini_get('suhosin.executor.func.blacklist')) && function_exists('phpinfo')) {
			ob_start();
			phpinfo(INFO_MODULES);
			$php_info = strip_tags(ob_get_contents());
			ob_end_clean();
//			cpg_error('<pre style="text-align:left">'.$php_info."</pre>\n");
			echo ((preg_match('/Loaded Modules .*mod_rewrite/', $php_info)) ? '<span style="color:green">OK</span>' : '<b style="color:red">FAIL</b> - not installed (may be inaccurate result on older versions of PHP, when PHP is configured in CGI/FCGI, or on Win32)');
		} else {
			echo 'WARNING - indeterminite, phpinfo() disabled';
		}
		echo '</td></tr>';
	}
	// Check $BASEHREF value.
	echo "<tr><td class=\"row2\">\$BASEHREF</td><td class=\"row2\">{$BASEHREF}</td></tr>";
	// Rewrite hack signature image URL.
	echo "<tr><td>Rewrite hack signature image file link</td><td>{$BASEHREF}modules/{$mod_dirname}/sig_nnnnnn.jpg</td></tr>";
	// Do end-to-end rewrite check.
	$statuscode = fetch_URL("{$BASEHREF}modules/{$mod_dirname}/diagnostics.html", TRUE);
//	trigger_error("URL fetch returned {$statuscode} HTTP status.", E_USER_NOTICE);
	echo "<tr><td class=\"row2\">Check mod_rewrite end-to-end [verify .htaccess via fetch_URL() of {$BASEHREF}modules/{$mod_dirname}/diagnostics.html]</td><td class=\"row2\">"
		.(($statuscode == '200') ? '<span style="color:green">OK</span>' : "<b style=\"color:red\">FAIL</b> - Recieved HTTP response status code $statuscode")
		.'</td></tr>';
	// Do end-to-end rewrite check (image hotlink protection).
	$statuscode = fetch_URL("{$BASEHREF}modules/{$mod_dirname}/sig_1234567890.jpg", TRUE, 'http://www.domain.invalid/');
//	trigger_error("URL fetch returned {$statuscode} HTTP status.", E_USER_NOTICE);
	echo "<tr><td>Check for image hotlink protection interference [verify .htaccess via fetch_URL() of {$BASEHREF}modules/{$mod_dirname}/sig_1234567890.jpg with non-local referrer]</td><td>"
		.(($statuscode != '403') ? "<span style=\"color:green\">OK ({$statuscode})</span>" : "<b style=\"color:red\">FAIL</b> - Recieved HTTP response status code $statuscode")
		.'</td></tr>';
	$statuscode = fetch_URL("{$BASEHREF}modules/{$mod_dirname}/images/blank.png", TRUE, 'http://www.domain.invalid/');
//	trigger_error("URL fetch returned {$statuscode} HTTP status.", E_USER_NOTICE);
	echo "<tr><td class=\"row2\">Check for image hotlink protection enabled for rest of module [verify .htaccess via fetch_URL() of {$BASEHREF}modules/{$mod_dirname}/images/blank.png with non-local referrer]</td><td class=\"row2\">"
		.(($statuscode == '403') ? "<span style=\"color:green\">OK ({$statuscode})</span>" : "<b style=\"color:red\">FAIL</b> - Recieved HTTP response status code $statuscode")
		.'</td></tr>';
	// Check config.inc setting.
	echo "<tr><td>\$config['sig_image_rewrite_hack']</td><td>{$config['sig_image_rewrite_hack']}</td></tr>";
	// ************* END SIGNATURE IMAGE REWRITE HACK *************
	echo '      </table>'
		.'    </td>'
		.'  </tr>';

	echo '</table>';
	CloseTable();

	CloseTable();
}

// Function:	lookup_guildid()
// What it does:	Admin interface to look up the guild id given the server name and
//			(optionally partial) guild name.
// Requires:
//		$_POST['guildname'] -- guild name
//		$_POST['servername'] -- server name
function lookup_guildid() {
	global $config;

	$guildname = (isset($_POST['guildname'])) ? preg_replace('/[^a-z0-9 \[\]\\\^\$\.\|\?\*\+\(\)\{\}]+/i','', $_POST['guildname']) : '';
	$servername = (isset($_POST['servername'])) ? preg_replace('/[^a-z0-9 ]+/i','', $_POST['servername']) : '';

	OpenTable();
	rm_admin_menu('lookup_guildid');
	echo "<hr />\n<br />\n"
		.'<span class="genmed"><strong>'._LOOKUPGID."</strong></span><br />\n"
		._LOOKUPGIDDESC."<br /><br />\n";

	echo open_form(adminlink('&mode=lookup_guildid'), '', _SEARCH);
	echo '<table cols="3" border="0">'
		.'<tr style="border:none"><td style="border:none">Guild Name:</td><td colspan="2" style="border:none"><input size="40" name="guildname" value="'.$guildname.'"></td>'
		.'<tr style="border:none"><td style="border:none">Server Name:</td><td style="border:none">'
		."<select name=\"servername\" tabindex=\"16\">\n";
	$servers = array('Antonia Bayle',
		'Barren Sky',
		'Beta',
		'Deathtoll',
		'Halls of Fate',
		'Harla Dar',
		'Maj\'Dul',
		'Sebilis',
		'Skyfire',
		'Splitpaw',
		'Stormhold',
		'Storms',
		'Test',
		'Valor');
	foreach($servers as $server) {
		echo "<option value=\"$server\"".((preg_replace('/[^a-z0-9 ]+/i','', $server) == $servername) ? ' selected="selected"' : '').'>'.$server."</option>\n";
	}
	echo "</select></td>\n"
		.'<td style="border:none;text-align:right"><input name="rmsearch" value="Search" tabindex="10" type="submit"></td>'
		.'</table>';
	echo close_form();

	if (isset($_POST['servername'])) {
		$query = $config['census'].'count/eq2/guild/?name=i/'.urlencode($_POST['guildname']).'/&world='.urlencode($_POST['servername']).'&c:show=id,name&c:case=false';
		$xml = fetch_URL($query);
		$countobj = NEW SimpleXMLElement($xml);
		echo "<br />\n";
		if ($countobj['count'] == 0) {
			echo '<span style="color:red">'._ERR_GUILDNOTFOUND.'</span><br>';	
		} else {
			if ($countobj['count'] > 10) {
				echo '<div>'.sprintf(_LOOKUPGIDTRUNC, $countobj['count'], 10).'</div><br />';
			}
			$countobj = NULL;

			$query = $config['census'].'get/eq2/guild/?name=i/'.urlencode($_POST['guildname']).'/&world='.urlencode($_POST['servername']).'&c:show=id,name&c:limit=10&c:case=false';
			$xml = fetch_URL($query);
			$guildobj = NEW SimpleXMLElement($xml);

			echo '<table>'
				.'<thead><tr style="text-align:left;font-size:normal"><th>Name</th><th>Guild ID</th></tr></thead>';
			foreach($guildobj as $guild) {
				echo '<tr><td>'.$guild['name'].'</td><td><input readonly type="text" value="'.$guild['id'].'"></td></tr>';
			}
			$guildobj = NULL;
			echo '</table>';
		}
	}
	CloseTable();
}

// Function:	build_questsdata()
// What it does:	Developer interface to rebuild the $Timelines array from the Census.
//			This command takes a while to run and really only needs to be run during
//			development, so this command is available (to an admin), but hidden.
function build_questsdata() {
	global $mod_dirname, $config;

	// Display code
	OpenTable();
	rm_admin_menu('build_questsdata');
	echo "<hr />\n<br />\n"
		.'<span class="genmed"><strong>'._BUILDQUESTS."</strong></span><br />\n"
		._BUILDQUESTSDESC."<br /><br />\n";
	OpenTable();

	$QuestsData = array();

	foreach(array('heritage', 'key', 'timeline') as $series) {
		trigger_error("###### Loading series: {$series}...", E_USER_NOTICE);

		// Load quest timelines into $Timelines array.
		require_once("modules/{$mod_dirname}/includes/quests_{$series}.inc");

		foreach(array_keys($Timelines[$series]) as $timeline) {
			trigger_error("#### Walking timeline: {$timeline}...", E_USER_NOTICE);

			if (isset($Timelines[$series][$timeline]['steps'])) {
				foreach($Timelines[$series][$timeline]['steps'] as $crc => $quest) {
					trigger_error("## Parsing quest crc: {$crc}...", E_USER_NOTICE);

					$QuestsData[$crc] = parse_quest_JSON($crc);

					if ($QuestsData[$crc]['name'] != $quest['name']) { trigger_error("WARNING: quest {$crc} name, does not match Timeline...", E_USER_NOTICE); }

					// Create backwards references to timeline.
					$QuestsData[$crc]['in_series'] = $series;
					$QuestsData[$crc]['in_timeline'] = $timeline;
					$QuestsData[$crc]['in_step'] = $quest['step'];

//					cpg_error('<pre style="text-align:left">$quest = '.htmlspecialchars(print_r($quest, TRUE))."\n\n\$QuestsData".htmlspecialchars(print_r($QuestsData, TRUE))."</pre>\n");
				}
			}
//			cpg_error('<pre style="text-align:left">'.htmlspecialchars(var_export($QuestsData, TRUE)).";</pre>\n");
		}
	echo '<pre style="text-align:left">'.count($QuestsData)." elements parsed.\n\n\$Timelines['{$series}'] = ".htmlspecialchars(var_export($QuestsData, TRUE)).";\n\n</pre>\n";
	}

	CloseTable();
	CloseTable();
}

// Function:	build_collectionsdata()
// What it does:	Developer interface to rebuild the $CollectionsData array from the Census.
//			This command takes a while to run and really only needs to be run during
//			development, so this command is available (to an admin), but hidden.
function build_collectionsdata() {
	global $mod_dirname, $config;

	// Load array of class-restricted collections to fold into collection data.
        require_once("modules/{$mod_dirname}/includes/collections_data_class.inc");
	// Load array of meta-collections to fold into collection data.
        require_once("modules/{$mod_dirname}/includes/collections_data_meta.inc");

	// Display code
	OpenTable();
	rm_admin_menu('build_collectionsdata');
	echo "<hr />\n<br />\n"
		.'<span class="genmed"><strong>'._BUILDCOLLECTIONS."</strong></span><br />\n"
		._BUILDCOLLECTIONSDESC."<br /><br />\n";
	OpenTable();

	$CollectionsData = array();
	$data = '$CollectionsData = ';
	// The Census API internally limits the maximum returned values on the Collections collection to 100.
	$limit = 100;
	$start = 0;
	do {
		$parse = parse_collection_JSON($limit, $start);
		if (count($parse)) { $CollectionsData = $CollectionsData + $parse; }
		$start = $start + $limit;
	} while (count($parse) > 0);

	echo '<pre style="text-align:left">'.count($CollectionsData)." elements parsed.\n\n\$CollectionsData = ".htmlspecialchars(var_export($CollectionsData, TRUE)).";</pre>\n";

	CloseTable();
	CloseTable();
}

// Function:	build_collectionsicons()
// What it does:	Download and save to disk all unique collection icons.
//		This command takes a long time to run and really only needs to be run
//		during development, so this command is available (to an admin), but
//		hidden.
function build_collectionsicons() {
	global $config, $mod_dirname;

	// Load quest data.
	require_once("modules/{$mod_dirname}/includes/collections_data.inc");

	// Display code
	OpenTable();
	rm_admin_menu('build_collectionsicons');
	echo "<hr />\n<br />\n"
		.'<span class="genmed"><strong>'._BUILDICONS."</strong></span><br />\n"
		._BUILDICONSDESC."<br /><br />\n";

	if (extension_loaded('cURL')) {
		if (is_writable("modules/{$mod_dirname}/images/items/")) {
			$count = 0;
			$user_agent = 'Roster Master'.$config['version_string'];
			foreach ($CollectionsData as $collection) {
				foreach ($collection['reference_list'] as $item) {
					$filename = "modules/{$mod_dirname}/images/items/{$item['icon']}.png";
					if (!file_exists($filename)) {
						$fp = fopen($filename, 'w+');
						$ch = curl_init("{$config['census']}img/eq2/icons/{$item['icon']}/item");
						curl_setopt_array($ch, array(
							CURLOPT_CUSTOMREQUEST  => 'GET',       // set request type post or get
							CURLOPT_POST           => FALSE,       // set to GET
							CURLOPT_USERAGENT      => $user_agent, // set user agent
							CURLOPT_RETURNTRANSFER => TRUE,        // return web page
							CURLOPT_HEADER         => FALSE,       // return headers
							CURLOPT_FOLLOWLOCATION => TRUE,        // follow redirects
							CURLOPT_ENCODING       => '',          // handle all encodings
							CURLOPT_FAILONERROR    => TRUE,        // fail verbosely if the HTTP code returned is greater than or equal to 400
							CURLOPT_AUTOREFERER    => TRUE,        // set referer on redirect
							CURLOPT_CONNECTTIMEOUT => 60,          // timeout on connect
							CURLOPT_TIMEOUT        => 180,         // timeout on response
							CURLOPT_MAXREDIRS      => 10,          // stop after 10 redirects
							CURLOPT_SSL_VERIFYHOST => FALSE,       // don't verify SSL
							CURLOPT_SSL_VERIFYPEER => FALSE,       // don't verify SSL
							CURLOPT_FILETIME       => FALSE        // don't get the remote time of the retrieved document
						));

						// Save the icon to a file.
						curl_setopt($ch, CURLOPT_FILE, $fp);
						curl_exec($ch);
						curl_close($ch);
						fclose($fp);
						$count++;
					}
				}
			}

			OpenTable();
			echo "<pre style=\"text-align:left\">{$count} items written.</pre>\n";
			CloseTable();
		} else {
			cpg_error("Cannot write to <nobr>\"modules/{$mod_dirname}/images/items/\"</nobr> directory.");
		}
	} else {
		cpg_error("cURL not found.");
	}

	CloseTable();
}

// *********************************************************************
// Set page title.
$pagetitle .= ' '._BC_DELIM.' '.$mod_displayname;

// *************************** Mode Select *****************************
$mode = isset($_POST['mode']) ? $_POST['mode'] : (isset($_GET['mode']) ? $_GET['mode'] : '');

require('header.php');
GraphicAdmin('_AMENU5');

// Dispatch action.
switch ($mode) {
	case 'view_logfile':
		view_logfile();
		break;
	case 'purge_logfile':
		purge_logfile();
		break;
	case 'sync_questmaster':
		sync_questmaster();
		break;
	case 'optimize_tables':
		optimize_tables();
		break;
	case 'purge_table':
		purge_table();
		break;
	case 'sm_fill_sig':
		if (!$config['dynamic_sigs']) {
			// Signatures disabled in configuration.
			cpg_error(_ERR_ACCESSCONT);
		}
		sm_fill_sigcache();
		break;
	case 'sm_purge_sig':
		if (!$config['dynamic_sigs']) {
			// Signatures disabled in configuration.
			cpg_error(_ERR_ACCESSCONT);
		}
		sm_purge_sigcache();
		break;
	case 'diagnostics':
		diagnostics();
		break;
	case 'diagnostics_php':
		diagnostics_php();
		break;
	case 'lookup_guildid':
		lookup_guildid();
		break;
	case 'build_questsdata':
		build_questsdata();
		break;
	case 'build_collectionsdata':
		build_collectionsdata();
		break;
	case 'build_collectionsicons':
		build_collectionsicons();
		break;
	default:
		lookup_guildid();
}

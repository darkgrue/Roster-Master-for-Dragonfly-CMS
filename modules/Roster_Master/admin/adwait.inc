<?php
/***********************************************************************
  Roster Master for Dragonfly(TM) CMS
  **********************************************************************
  Copyright (C) 2005-2018 by Dark Grue

  Module administration system waiting notifications file.

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

if (!can_admin($mod_dirname)) { die('Access Denied2'); }

// Get module language file.
if ($mod_dirname != $mod_basename) { get_lang($mod_basename); }

$matches = array();
if (preg_match("/^{$mod_basename}(?:_(\d+))?$/", $mod_dirname, $matches)) {
	$suffix = (empty($matches[1])) ? '' : "_{$matches[1]}";
	$mod_iname = $mod_basename.$suffix;
} else {
	cpg_error('<strong>'._ERR_FATALCONFIG_RM."</strong><br /><br />\n"
		."The module installation directory, <nobr>\"{$mod_dirname}\"</nobr>, does not follow the expected instantation syntax of <nobr>\"{$mod_basename}[_<i>&lt;integer&gt;</i>]\"</nobr>.");
}

$mod_displayname = str_replace('_', ' ', $mod_basename).((empty($suffix)) ? '' : ' ('.ltrim($suffix, '_').')');

// REQUIRED: $mod_dirname
// REQUIRED: $mod_iname
// REQUIRED: $suffix
// REQUIRED: $mod_displayname
// *********************************************************************

if (empty($mod_dirname) || !file_exists(BASEDIR."modules/{$mod_dirname}")) {
	$content = 'ERROR';
	return trigger_error('The expected module directory'.((empty($mod_dirname)) ? '' : ', "'.BASEDIR."modules/{$mod_dirname}\",").' does not exist.', E_USER_WARNING);
}

if (!is_active($mod_dirname)) {
	$content = 'ERROR';
	return trigger_error("The {$mod_dirname} module is inactive.", E_USER_WARNING);
}

if (can_admin($mod_dirname)) {
	global $db, $prefix;

	// Get module language file.
	get_lang($mod_basename);

	list($unclaimed_count) = $db->sql_ufetchrow("SELECT COUNT(rm.characterId) FROM {$prefix}_roster_master{$suffix} AS rm
			LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId)
			WHERE rmu.characterId IS NULL", SQL_NUM);
	$content .= '<b>&#8226;</b>&nbsp;<a href="'.getlink("{$mod_dirname}&rm=rm_manage_claims")."\">{$mod_displayname} "._UNCLAIMEDCHARS."</a>: {$unclaimed_count}<br />";
}

<?php 
/***********************************************************************
  Roster Master for Dragonfly(TM) CMS
  **********************************************************************
  Copyright (C) 2005-2018 by Dark Grue

  EverQuest II guild roster management, quest tracker, and dynamic
  signature generator that integrates with the Dragonfly(TM) Content
  Management System (CMS).

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


// ************************** Global Variables *************************
$DEBUG = FALSE;


// ***************************** FUNCTIONS *****************************
// *********************** Module Instantiation ************************
// This block can reference alternative instances of its associated
// module by adding an underscore followed by an integer to the name of
// the file (e.g. "block-Guild_Wall_of_Fame_2.php"). The block will
// detect this and automagically configure itself.
//
// NOTE:
//	The block will check $blk_basename against its filename.
//	Modules that this block references MUST be configured separately
//	to create/reference the new instance.
$blk_basename = 'block-Guild_Wall_of_Fame';
$blk_filename = basename(__FILE__);
$mod_basename = 'Roster_Master';

$matches = array();
if (preg_match("/^{$blk_basename}(?:_(\d+))?\.php$/", $blk_filename, $matches)) {
	$suffix = (empty($matches[1])) ? '' : "_{$matches[1]}";
	$mod_dirname = $mod_basename.$suffix;
} else {
	$content = 'ERROR';
	return trigger_error("The block filename, <nobr>\"{$blk_filename}\"</nobr>, does not follow the expected instantation syntax of <nobr>\"{$blk_basename}[_<i>&lt;integer&gt;</i>].php\"</nobr>.", E_USER_WARNING);
}

// REQUIRED: $mod_dirname
// REQUIRED: $suffix
// *********************************************************************


if (empty($mod_dirname) || !file_exists(BASEDIR."modules/{$mod_dirname}")) {
	$content = 'ERROR';
	return trigger_error('The expected module directory'.((empty($mod_dirname)) ? '' : ', "'.BASEDIR."modules/{$mod_dirname}\",").' does not exist.', E_USER_WARNING);
}

if (!is_active($mod_dirname)) {
	$content = 'ERROR';
	return trigger_error("The {$mod_dirname} module is inactive.", E_USER_WARNING);
}

// Start function duration timer.
$timer_t = get_microtime();

global $db, $prefix;
$bid = (isset($block['bid'])) ? $block['bid'] : intval($bid);

$result = $db->sql_query("SELECT title, content, time FROM {$prefix}_blocks WHERE bid='$bid'", FALSE);
$row = $db->sql_fetchrow($result, SQL_ASSOC);
if ($db->sql_numrows($result) != 1) {
	$content = "<center>$blockfile: Error retrieving block #$bid.<br />"._BLOCKPROBLEM2."<br />\n";
	return;
}
$db->sql_freeresult($result);

$content =& $row['content'];

// Determine when the last time there was a data update.
list($last_updated) = $db->sql_ufetchrow("SELECT value FROM {$prefix}_roster_master_guild{$suffix}
	WHERE name='table_updated'", SQL_NUM);

if ((intval($last_updated) > intval($row['time'])) || $DEBUG) {
	// Start function duration timer.
	$timer_t = get_microtime();

	// Get module configuration.
	require(BASEDIR."modules/{$mod_dirname}/config.inc");

	// Get includes.
	require_once("modules/{$mod_dirname}/functions/utility.inc");

	if ($config['block_autoupdate'] == 0) {
		// Update the roster data.
		require_once("modules/{$mod_dirname}/functions/parser.inc");
		update_data();
	}

	// $exclude_alts is set based on the block_show_alts setting in config.inc
	$exclude_alts = !(bool)$config['block_show_alts'];

	// Timestamp the output.
	$current_time = time();
	$content = "\n<!-- Content cached: ".gmdate("Y-m-d H:i:s", $current_time)."Z -->\n";

	// Avoid polluting the JavaScript namespace by prefixing the
	// function name with the Block ID.
	$content .= '<script language="JavaScript" type="text/javascript">
<!-- Hide script from old browsers.
function bid'.$bid.'_togglelist(img) {
  var ulReturn=img.parentNode.parentNode.parentNode.parentNode.parentNode.getElementsByTagName("ul");

  if (ulReturn[0].style.display == "none") {
    img.src="modules/'.$mod_dirname.'/images/blocks/open.gif";
    ulReturn[0].style.display= "block";
  } else {
    img.src="modules/'.$mod_dirname.'/images/blocks/closed.gif";
    ulReturn[0].style.display= "none";
  }
}
// End -->
</script>'."\n";

	// ********** GENERAL GUILD STATS ********** 
	// Only shows if show_genstats in config.inc is set to 1.
	if ($config['show_genstats']) {	
		$content .= '<table border="0" cellpadding="0" cellspacing="0" width="100%">';

		// Guild name - from the guild collection.
		list($value) = $db->sql_ufetchrow("SELECT value FROM {$prefix}_roster_master_guild{$suffix}
			WHERE name='Guild Name'", SQL_NUM);
		$content .= "<tr><td colspan=\"2\"><b>Guild Name:</b></td></tr>\n<tr><td width=\"10\">&nbsp;</td><td><a href=\"{$config['guild_url']}\" target=\"_blank\">".((empty($value)) ? 'N/A' : $value)."</a></td></tr>\n";

		// Server name - from the guild collection.
		list($value) = $db->sql_ufetchrow("SELECT value FROM {$prefix}_roster_master_guild{$suffix}
			WHERE name='Server'", SQL_NUM);
		$content .= "<tr><td colspan=\"2\"><b>Server:</b></td></tr>\n<tr><td width=\"10\">&nbsp;</td><td>"
// FIXME: Update when/if server status and summary pages are available.
//			.((empty($value)) ? 'Unknown' :	"<a href=\"{$config['server_url']}{$value}/\" target=\"_blank\">".((empty($value)) ? 'N/A' : $value)."</a></td></tr>\n");
			.((empty($value)) ? 'Unknown' :	$value);

		// Guild level - from the guild collection.
		list($value) = $db->sql_ufetchrow("SELECT value FROM {$prefix}_roster_master_guild{$suffix}
			WHERE name='Guild Level'", SQL_NUM);
		$content .= '<tr><td colspan="2"><b>Guild Level:</b>&nbsp;'.((empty($value)) ? 'N/A' : number_format($value))."</td></tr>\n";

// FIXME: Update when/if Guild status is available.
/*
		// Guild status total - from the guild collection.
		list($value) = $db->sql_ufetchrow("SELECT value FROM {$prefix}_roster_master_guild{$suffix}
			WHERE name='Guild Status'", SQL_NUM);
		$content .= '<tr><td colspan="2"><b>Guild Status:</b>&nbsp;'.((empty($value)) ? 'N/A' : number_format($value))."</td></tr>\n";	
*/

		// Total Characters - varied wording depending on the block_show_alts configuration
		list($value) = $db->sql_ufetchrow("SELECT COUNT(*) FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : ''), SQL_NUM);
		if ($config['block_show_alts']) {
			$content .= '<tr><td colspan="2"><b>Total Characters:</b>&nbsp;'.number_format($value)."</td></tr>\n";
		} else {
			list($value2) = $db->sql_ufetchrow("SELECT COUNT(*) FROM {$prefix}_roster_master{$suffix}", SQL_NUM);
			$content .= "<tr><td colspan=\"2\"><b>Primary Characters:</b>&nbsp;".number_format($value).' ('.number_format($value2)."&nbsp;Total)</td></tr>\n";
		}

		// Calculate average adventurer level.
		list($value) = $db->sql_ufetchrow("SELECT AVG(rm.type_level) FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rm.type_level!=0 AND rmu.char_type='Primary'" : ' WHERE rm.tradeskill_level!=0 '), SQL_NUM);
		$content .= '<tr><td colspan="2"><b>Average Adv. Level:</b>&nbsp;'.number_format($value)."</td></tr>\n";

		// Calculate average artisan level.
 		list($value) = $db->sql_ufetchrow("SELECT AVG(rm.tradeskill_level) FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rm.tradeskill_level!=0 AND rmu.char_type='Primary'" : ' WHERE rm.tradeskill_level!=0'), SQL_NUM);
		$content .= '<tr><td colspan="2"><b>Average Art. Level:</b>&nbsp;'.number_format($value)."</td></tr>\n";

		$content .= "</table><hr />\n";
	}

	// ********** EXTENDED GUILD STATS **********
	// Only shows if show_extstats in config.inc is set to 1.
	if ($config['show_extstats']) {	
		$content .= '<table border="0" cellpadding="0" cellspacing="0" width="100%">';

		// Date formed - from the guild collection.
		list($value) = $db->sql_ufetchrow("SELECT value FROM {$prefix}_roster_master_guild{$suffix}
			WHERE name='Date Formed'", SQL_NUM);
		$content .= "<tr><td colspan=\"2\"><b>Date Formed:</b></td></tr>\n<tr><td width=\"10\">&nbsp;</td><td>".formatDateTime($value, "%a, %d %b %Y %H:%M:%S")."</td></tr>\n";

		// Calculate average quests completed.
 		list($value) = $db->sql_ufetchrow("SELECT AVG(rm.quests_complete) FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : ''), SQL_NUM);
		$content .= '<tr><td colspan="2"><b>Average Quests Completed:</b>&nbsp;'.number_format($value)."</td></tr>\n";

		// Calculate total rares harvested.
 		list($value) = $db->sql_ufetchrow("SELECT SUM(rm.statistics_rare_harvests) FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : ''), SQL_NUM);
		$content .= '<tr><td colspan="2"><b>Total Rares Harvested:</b>&nbsp;'.number_format($value)."</td></tr>\n";

		// Calculate total items crafted.
 		list($value) = $db->sql_ufetchrow("SELECT SUM(rm.statistics_items_crafted) FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : ''), SQL_NUM);
		$content .= '<tr><td colspan="2"><b>Total Items Crafted:</b>&nbsp;'.number_format($value)."</td></tr>\n";

		$content .= "</table><hr />\n";
	}

	// Arena
	if ($config['show_Arena']) {
		$content .= '<table border="0" cellpadding="0" cellspacing="0" width="100%">';

		// Calculate total Arena CTF wins.
 		list($value) = $db->sql_ufetchrow("SELECT SUM(rm.arena_ctf_wins) FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : ''), SQL_NUM);
		$content .= '<tr><td colspan="2"><b>Arena CTF Wins:</b>&nbsp;'.number_format($value)."</td></tr>\n";

		// Calculate total Arena CTF losses.
 		list($value) = $db->sql_ufetchrow("SELECT SUM(rm.arena_ctf_losses) FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : ''), SQL_NUM);
		$content .= '<tr><td colspan="2"><b>Arena CTF Losses:</b>&nbsp;'.number_format($value)."</td></tr>\n";

		// Calculate longest Arena CTF streak member.
 		list($name, $id, $value) = $db->sql_ufetchrow("SELECT rm.name_first, rm.characterId, rm.arena_ctf_streak FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : '')
			." ORDER BY rm.arena_ctf_streak DESC, rm.last_update DESC
			LIMIT 1", SQL_NUM);
		if ($value != 0) { $content .= "<tr><td colspan=\"2\"><b>Longest Arena CTF Streak:</b></td></tr><tr><td width=\"10\">&nbsp;</td><td><a href=\"{$config['char_url']}{$id}\" target=\"_blank\">$name</a> (".number_format($value).")</td></tr>\n"; }

		// Calculate total Arena Deathmatch wins.
 		list($value) = $db->sql_ufetchrow("SELECT SUM(rm.arena_deathmatch_wins) FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : ''), SQL_NUM);
		$content .= '<tr><td colspan="2"><b>Arena Deathmatch Wins:</b>&nbsp;'.number_format($value)."</td></tr>\n";

		// Calculate total Arena Deathmatch losses.
 		list($value) = $db->sql_ufetchrow("SELECT SUM(rm.arena_deathmatch_losses) FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : ''), SQL_NUM);
		$content .= '<tr><td colspan="2"><b>Arena Deathmatch Losses:</b>&nbsp;'.number_format($value)."</td></tr>\n";

		// Calculate longest Arena Deathmatch streak member.
 		list($name, $id, $value) = $db->sql_ufetchrow("SELECT rm.name_first, rm.characterId, rm.arena_deathmatch_streak FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : '')
			." ORDER BY rm.arena_deathmatch_streak DESC, rm.last_update DESC
			LIMIT 1", SQL_NUM);
		if ($value != 0) { $content .= "<tr><td colspan=\"2\"><b>Longest Arena Deathmatch Streak:</b></td></tr><tr><td width=\"10\">&nbsp;</td><td><a href=\"{$config['char_url']}{$id}\" target=\"_blank\">$name</a> (".number_format($value).")</td></tr>\n"; }

		// Calculate total Arena Idol wins.
 		list($value) = $db->sql_ufetchrow("SELECT SUM(rm.arena_idol_wins) FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : ''), SQL_NUM);
		$content .= '<tr><td colspan="2"><b>Arena Idol Wins:</b>&nbsp;'.number_format($value)."</td></tr>\n";

		// Calculate total Arena Idol losses.
 		list($value) = $db->sql_ufetchrow("SELECT SUM(rm.arena_idol_losses) FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : ''), SQL_NUM);
		$content .= '<tr><td colspan="2"><b>Arena Idol Losses:</b>&nbsp;'.number_format($value)."</td></tr>\n";

		// Calculate longest Arena Idol streak member.
 		list($name, $id, $value) = $db->sql_ufetchrow("SELECT rm.name_first, rm.characterId, rm.arena_idol_streak FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : '')
			." ORDER BY rm.arena_idol_streak DESC, rm.last_update DESC
			LIMIT 1", SQL_NUM);
		if ($value != 0) { $content .= "<tr><td colspan=\"2\"><b>Longest Arena Idol Streak:</b></td></tr><tr><td width=\"10\">&nbsp;</td><td><a href=\"{$config['char_url']}{$id}\" target=\"_blank\">$name</a> (".number_format($value).")</td></tr>\n"; }

		$content .= "</table><hr />\n";
	}

	// Total PvP Kills
	if ($config['show_PvP']) {
		$content .= '<table border="0" cellpadding="0" cellspacing="0" width="100%">';

		// Calculate total kills.
 		list($value) = $db->sql_ufetchrow("SELECT SUM(rm.statistics_kills) FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : ''), SQL_NUM);
			$content .= '<tr><td colspan="2"><b>Total Kills:</b>&nbsp;'.number_format($value)."</td></tr>\n";

		// Calculate total PVP kills.
 		list($value) = $db->sql_ufetchrow("SELECT (SUM(rm.pvp_wild_kills) + SUM(rm.pvp_city_kills)) FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : ''), SQL_NUM);
			$content .= '<tr><td colspan="2"><b>Total PvP Kills:</b>&nbsp;'.number_format($value)."</td></tr>\n";

		// Calculate longest PVP kill streak member.
 		list($name, $id, $value) = $db->sql_ufetchrow("SELECT rm.name_first, rm.characterId, rm.pvp_kill_streak FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : '')
			." ORDER BY rm.pvp_kill_streak DESC, rm.last_update DESC
			LIMIT 1", SQL_NUM);
		if ($value != 0) { $content .= "<tr><td colspan=\"2\"><b>Longest PVP Kill Streak:</b></td></tr><tr><td width=\"10\">&nbsp;</td><td><a href=\"{$config['char_url']}{$id}\" target=\"_blank\">$name</a> (".number_format($value).")</td></tr>\n"; }

		// Calculate total city PVP kills.
 		list($value) = $db->sql_ufetchrow("SELECT SUM(rm.pvp_city_kills) FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : ''), SQL_NUM);
		$content .= '<tr><td colspan="2"><b>Total City PvP Kills:</b>&nbsp;'.number_format($value)."</td></tr>\n";

		// Calculate total wild PVP kills.
 		list($value) = $db->sql_ufetchrow("SELECT SUM(rm.pvp_wild_kills) FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : ''), SQL_NUM);
		$content .= '<tr><td colspan="2"><b>Total Wild PvP Kills:</b>&nbsp;'.number_format($value)."</td></tr>\n";

		// Calculate total deaths.
 		list($value) = $db->sql_ufetchrow("SELECT SUM(rm.statistics_deaths) FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : ''), SQL_NUM);
		$content .= '<tr><td colspan="2"><b>Total Deaths:</b>&nbsp;'.number_format($value)."</td></tr>\n";

		// Calculate longest PVP death streak member.
 		list($name, $id, $value) = $db->sql_ufetchrow("SELECT rm.name_first, rm.characterId, rm.pvp_death_streak FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : '')
			." ORDER BY rm.pvp_death_streak DESC, rm.last_update DESC
			LIMIT 1", SQL_NUM);
		if ($value != 0) { $content .= "<tr><td colspan=\"2\"><b>Longest PVP Death Streak:</b></td></tr><tr><td width=\"10\">&nbsp;</td><td><a href=\"{$config['char_url']}{$id}\" target=\"_blank\">$name</a> (".number_format($value).")</td></tr>\n"; }

		$content .= "</table><hr />\n";

		$content .= '<table border="0" cellpadding="0" cellspacing="0" width="100%">';

		// Calculate total kills.
 		list($value) = $db->sql_ufetchrow("SELECT SUM(rm.statistics_kills) FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : ''), SQL_NUM);
			$content .= '<tr><td colspan="2"><b>Total Kills:</b>&nbsp;'.number_format($value)."</td></tr>\n";

		// Calculate total deaths.
 		list($value) = $db->sql_ufetchrow("SELECT SUM(rm.statistics_deaths) FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : ''), SQL_NUM);
		$content .= '<tr><td colspan="2"><b>Total Deaths:</b>&nbsp;'.number_format($value)."</td></tr>\n";

		// Kills vs. Deaths Ratio
 		list($value) = $db->sql_ufetchrow("SELECT (SUM(rm.statistics_kills) / SUM(rm.statistics_deaths)) FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : ''), SQL_NUM);
		$content .= '<tr><td colspan="2"><b>Kills vs. Deaths Ratio:</b>&nbsp;'.number_format($value)."</td></tr>\n";

		$content .= "</table><hr />\n";
	}

	// ********** LEADERS **********
	// Only shows if show_leaders in config.inc is set to 1.
	if ($config['show_leaders']) {	
		$content .= '<table border="0" cellpadding="0" cellspacing="0" width="100%">';

		// Calculate highest guild status contributor.
 		list($name, $id, $value) = $db->sql_ufetchrow("SELECT rm.name_first, rm.characterId, rm.guild_status FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : '')
			." ORDER BY rm.guild_status DESC, rm.last_update DESC
			LIMIT 1", SQL_NUM);
		if ($value != 0) { $content .= "<tr><td colspan=\"2\"><b>Highest Guild Status Contributor:</b></td></tr><tr><td width=\"10\">&nbsp;</td><td><a href=\"{$config['char_url']}{$id}\" target=\"_blank\">$name</a> (".number_format($value).")</td></tr>\n"; }

		// Calculate most quests complete member.
 		list($name, $id, $value) = $db->sql_ufetchrow("SELECT rm.name_first, rm.characterId, rm.quests_complete FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : '')
			." ORDER BY rm.quests_complete DESC, rm.last_update DESC
			LIMIT 1", SQL_NUM);
		if ($value != 0) { $content .= "<tr><td colspan=\"2\"><b>Most Quests Complete:</b></td></tr><tr><td width=\"10\">&nbsp;</td><td><a href=\"{$config['char_url']}{$id}\" target=\"_blank\">$name</a> (".number_format($value).")</td></tr>\n"; }

		// Calculate most collections complete member.
 		list($name, $id, $value) = $db->sql_ufetchrow("SELECT rm.name_first, rm.characterId, rm.collections_complete FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : '')
			." ORDER BY rm.collections_complete DESC, rm.last_update DESC
			LIMIT 1", SQL_NUM);
		if ($value != 0) { $content .= "<tr><td colspan=\"2\"><b>Most Collections Complete:</b></td></tr><tr><td width=\"10\">&nbsp;</td><td><a href=\"{$config['char_url']}{$id}\" target=\"_blank\">$name</a> (".number_format($value).")</td></tr>\n"; }

		// Calculate highest max melee hit member.
 		list($name, $id, $value) = $db->sql_ufetchrow("SELECT rm.name_first, rm.characterId, rm.statistics_max_melee_hit FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : '')
			." ORDER BY rm.statistics_max_melee_hit DESC, rm.last_update DESC
			LIMIT 1", SQL_NUM);
		if ($value != 0) { $content .= "<tr><td colspan=\"2\"><b>Highest Max Melee Hit:</b></td></tr><tr><td width=\"10\">&nbsp;</td><td><a href=\"{$config['char_url']}{$id}\" target=\"_blank\">$name</a> (".number_format($value).")</td></tr>\n"; }

		// Calculate highest max magic hit member.
 		list($name, $id, $value) = $db->sql_ufetchrow("SELECT rm.name_first, rm.characterId, rm.statistics_max_magic_hit FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : '')
			." ORDER BY rm.statistics_max_magic_hit DESC, rm.last_update DESC
			LIMIT 1", SQL_NUM);
		if ($value != 0) { $content .= "<tr><td colspan=\"2\"><b>Highest Max Magic Hit:</b></td></tr><tr><td width=\"10\">&nbsp;</td><td><a href=\"{$config['char_url']}{$id}\" target=\"_blank\">$name</a> (".number_format($value).")</td></tr>\n"; }

		// Calculate most rares collected member.
 		list($name, $id, $value) = $db->sql_ufetchrow("SELECT rm.name_first, rm.characterId, rm.statistics_rare_harvests FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : '')
			." ORDER BY rm.statistics_rare_harvests DESC, rm.last_update DESC
			LIMIT 1", SQL_NUM);
		if ($value != 0) { $content .= "<tr><td colspan=\"2\"><b>Most rares collected:</b></td></tr><tr><td width=\"10\">&nbsp;</td><td><a href=\"{$config['char_url']}{$id}\" target=\"_blank\">$name</a> (".number_format($value).")</td></tr>\n"; }

		// Calculate most items crafted member.
 		list($name, $id, $value) = $db->sql_ufetchrow("SELECT rm.name_first, rm.characterId, rm.statistics_items_crafted FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : '')
			." ORDER BY rm.statistics_items_crafted DESC, rm.last_update DESC
			LIMIT 1", SQL_NUM);
		if ($value != 0) { $content .= "<tr><td colspan=\"2\"><b>Most items crafted:</b></td></tr><tr><td width=\"10\">&nbsp;</td><td><a href=\"{$config['char_url']}{$id}\" target=\"_blank\">$name</a> (".number_format($value).")</td></tr>\n"; }

		// Calculate longest play time member.
 		list($name, $id, $value) = $db->sql_ufetchrow("SELECT rm.name_first, rm.characterId, rm.playedtime FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : '')
			." ORDER BY rm.playedtime DESC, rm.last_update DESC
			LIMIT 1", SQL_NUM);
		if ($value != 0) { $content .= "<tr><td colspan=\"2\"><b>Longest Time Played:</b></td></tr><tr><td width=\"10\">&nbsp;</td><td><a href=\"{$config['char_url']}{$id}\" target=\"_blank\">$name</a> (".duration($value, 5).")</td></tr>\n";	}

		$content .= "</table><hr />\n";
	}

	// ********** GUILD COMPOSITION **********
	// Only shows if show_breakdown in config.inc is set to 1.
	if($config['show_breakdown']) {
		$content .= "<b><u>Class Breakdown:</u></b><br />\n"
			."<ul style=\"list-style:none;margin:0 0 0 4px;padding:0\">\n";

		$result = $db->sql_query("SELECT type_class, COUNT(*) AS count FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : '')
			." GROUP BY rm.type_class", FALSE);
		while(list($class, $count) = $db->sql_fetchrow($result)) {
			$classes[$class] = (int)$count;
		}
		$db->sql_freeresult($result);
		// List Fighter Archetype
		$classes['fighters'] = 0;
		$mylist = '';
		foreach(array('Berserker', 'Guardian', 'Bruiser', 'Monk', 'Paladin', 'Shadow Knight') as $class) {
			if (isset($classes[$class])) {
				$classes['fighters'] += $classes[$class];
			} else {
				$classes[$class] = 0;
			}
			if ($config['breakdown_shownames']) {
				if ($classes[$class] != 0) {
					$mylist .= "    <li><table border=\"0\" cellpadding=\"0\" cellspacing=\"0\"><tr><td valign=\"top\"><img src=\"modules/{$mod_dirname}/images/blocks/closed.gif\" onclick=\"bid{$bid}_togglelist(this)\" style=\"border:0;margin:2px 3px 0 0;width:9px;height:9px;\" alt=\"\" /></td>"
						."<td>$class: {$classes[$class]}</td></tr></table>\n"
						."    <ul style=\"display:none;list-style:none;margin:0 0 2px 8px;padding:0\">\n";
					$result = $db->sql_query("SELECT rm.name_first, rm.characterId, rm.type_level FROM {$prefix}_roster_master{$suffix} AS rm"
						.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary' AND" : ' WHERE')
						." rm.type_class='{$class}'"
						." ORDER BY rm.type_level DESC", FALSE);
					while(list($name, $id, $level) = $db->sql_fetchrow($result)) {
						$mylist .= "      <li><a href=\"{$config['char_url']}{$id}\" target=\"_blank\">$name</a> ($level)</li>\n";
					}
					$db->sql_freeresult($result);
					$mylist .= "    </ul></li>\n";
				} else {
					$mylist .= "    <li><table border=\"0\" cellpadding=\"0\" cellspacing=\"0\"><tr><td valign=\"top\"><img src=\"modules/{$mod_dirname}/images/blocks/blank.png\" style=\"border:0;margin:2px 3px 0 0;width:9px;height:9px;\" alt=\"\" /></td>"
						."<td>$class: {$classes[$class]}</td></tr></table></li>\n";
				}
			} else {
				$mylist .= "    <li>$class: {$classes[$class]}</li>\n";
			}
		}
		$content .= "  <li><table border=\"0\" cellpadding=\"0\" cellspacing=\"0\"><tr><td valign=\"top\"><img src=\"modules/{$mod_dirname}/images/blocks/closed.gif\" onclick=\"bid{$bid}_togglelist(this)\" style=\"border:0;margin:2px 3px 0 0;width:9px;height:9px;\" alt=\"\" /></td>"
			."<td>Fighter:&nbsp;{$classes['fighters']}</td></tr></table>\n"
			."  <ul style=\"display:none;list-style:none;margin:0 0 2px 8px;padding:0\">\n"
			."$mylist  </ul></li>\n";
		// List Priest Archetype
		$classes['priests'] = 0;
		$mylist = '';
		foreach(array('Templar', 'Inquisitor', 'Warden', 'Fury', 'Defiler', 'Mystic', 'Channeler') as $class) {
			if (isset($classes[$class])) {
				$classes['priests'] += $classes[$class];
			} else {
				$classes[$class] = 0;
			}
			if ($config['breakdown_shownames']) {
				if ($classes[$class] != 0) {
					$mylist .= "    <li><table border=\"0\" cellpadding=\"0\" cellspacing=\"0\"><tr><td valign=\"top\"><img src=\"modules/{$mod_dirname}/images/blocks/closed.gif\" onclick=\"bid{$bid}_togglelist(this)\" style=\"border:0;margin:2px 3px 0 0;width:9px;height:9px;\" alt=\"\" /></td>"
						."<td>$class: {$classes[$class]}</td></tr></table>\n"
						."    <ul style=\"display:none;list-style:none;margin:0 0 2px 8px;padding:0\">\n";
					$result = $db->sql_query("SELECT rm.name_first, rm.characterId, rm.type_level FROM {$prefix}_roster_master{$suffix} AS rm"
						.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary' AND" : ' WHERE')
						." rm.type_class='{$class}'"
						." ORDER BY rm.type_level DESC", FALSE);
					while(list($name, $id, $level) = $db->sql_fetchrow($result)) {
						$mylist .= "      <li><a href=\"{$config['char_url']}{$id}\" target=\"_blank\">$name</a> ($level)</li>\n";
					}
					$db->sql_freeresult($result);
					$mylist .= "    </ul></li>\n";
				} else {
					$mylist .= "    <li><table border=\"0\" cellpadding=\"0\" cellspacing=\"0\"><tr><td valign=\"top\"><img src=\"modules/{$mod_dirname}/images/blocks/blank.png\" style=\"border:0;margin:2px 3px 0 0;width:9px;height:9px;\" alt=\"\" /></td>"
						."<td>$class: {$classes[$class]}</td></tr></table></li>\n";
				}
			} else {
				$mylist .= "    <li>$class: {$classes[$class]}</li>\n";
			}
		}
		$content .= "  <li><table border=\"0\" cellpadding=\"0\" cellspacing=\"0\"><tr><td valign=\"top\"><img src=\"modules/{$mod_dirname}/images/blocks/closed.gif\" onclick=\"bid{$bid}_togglelist(this)\" style=\"border:0;margin:2px 3px 0 0;width:9px;height:9px;\" alt=\"\" /></td>"
			."<td>Priest:&nbsp;{$classes['priests']}</td></tr></table>\n"
			."  <ul style=\"display:none;list-style:none;margin:0 0 2px 8px;padding:0\">\n"
			."$mylist  </ul></li>\n";
		// List Mage Archetype
		$classes['mages'] = 0;
		$mylist = '';
		foreach(array('Warlock', 'Wizard', 'Illusionist', 'Coercer', 'Necromancer', 'Conjuror') as $class) {
			if (isset($classes[$class])) {
				$classes['mages'] += $classes[$class];
			} else {
				$classes[$class] = 0;
			}
			if ($config['breakdown_shownames']) {
				if ($classes[$class] != 0) {
					$mylist .= "    <li><table border=\"0\" cellpadding=\"0\" cellspacing=\"0\"><tr><td valign=\"top\"><img src=\"modules/{$mod_dirname}/images/blocks/closed.gif\" onclick=\"bid{$bid}_togglelist(this)\" style=\"border:0;margin:2px 3px 0 0;width:9px;height:9px;\" alt=\"\" /></td>"
						."<td>$class: {$classes[$class]}</td></tr></table>\n"
						."    <ul style=\"display:none;list-style:none;margin:0 0 2px 8px;padding:0\">\n";
					$result = $db->sql_query("SELECT rm.name_first, rm.characterId, rm.type_level FROM {$prefix}_roster_master{$suffix} AS rm"
						.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary' AND" : ' WHERE')
						." rm.type_class='{$class}'"
						." ORDER BY rm.type_level DESC", FALSE);
					while(list($name, $id, $level) = $db->sql_fetchrow($result)) {
						$mylist .= "      <li><a href=\"{$config['char_url']}{$id}\" target=\"_blank\">$name</a> ($level)</li>\n";
					}
					$db->sql_freeresult($result);
					$mylist .= "    </ul></li>\n";
				} else {
					$mylist .= "    <li><table border=\"0\" cellpadding=\"0\" cellspacing=\"0\"><tr><td valign=\"top\"><img src=\"modules/{$mod_dirname}/images/blocks/blank.png\" style=\"border:0;margin:2px 3px 0 0;width:9px;height:9px;\" alt=\"\" /></td>"
						."<td>$class: {$classes[$class]}</td></tr></table></li>\n";
				}
			} else {
				$mylist .= "    <li>$class: {$classes[$class]}</li>\n";
			}
		}
		$content .= "  <li><table border=\"0\" cellpadding=\"0\" cellspacing=\"0\"><tr><td valign=\"top\"><img src=\"modules/{$mod_dirname}/images/blocks/closed.gif\" onclick=\"bid{$bid}_togglelist(this)\" style=\"border:0;margin:2px 3px 0 0;width:9px;height:9px;\" alt=\"\" /></td>"
			."<td>Mage:&nbsp;{$classes['mages']}</td></tr></table>\n"
			."  <ul style=\"display:none;list-style:none;margin:0 0 2px 8px;padding:0\">\n"
			."$mylist  </ul></li>\n";
		// List Scout Archetype
		$classes['scouts'] = 0;
		$mylist = '';
		foreach(array('Brigand', 'Swashbuckler', 'Dirge', 'Troubador', 'Assassin', 'Ranger', 'Beastlord') as $class) {
			if (isset($classes[$class])) {
				$classes['scouts'] += $classes[$class];
			} else {
				$classes[$class] = 0;
			}
			if ($config['breakdown_shownames']) {
				if ($classes[$class] != 0) {
					$mylist .= "    <li><table border=\"0\" cellpadding=\"0\" cellspacing=\"0\"><tr><td valign=\"top\"><img src=\"modules/{$mod_dirname}/images/blocks/closed.gif\" onclick=\"bid{$bid}_togglelist(this)\" style=\"border:0;margin:2px 3px 0 0;width:9px;height:9px;\" alt=\"\" /></td>"
						."<td>$class: {$classes[$class]}</td></tr></table>\n"
						."    <ul style=\"display:none;list-style:none;margin:0 0 2px 8px;padding:0\">\n";
					$result = $db->sql_query("SELECT rm.name_first, rm.characterId, rm.type_level FROM {$prefix}_roster_master{$suffix} AS rm"
						.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary' AND" : ' WHERE')
						." rm.type_class='{$class}'"
						." ORDER BY rm.type_level DESC", FALSE);
					while(list($name, $id, $level) = $db->sql_fetchrow($result)) {
						$mylist .= "      <li><a href=\"{$config['char_url']}{$id}\" target=\"_blank\">$name</a> ($level)</li>\n";
					}
					$db->sql_freeresult($result);
					$mylist .= "    </ul></li>\n";
				} else {
					$mylist .= "    <li><table border=\"0\" cellpadding=\"0\" cellspacing=\"0\"><tr><td valign=\"top\"><img src=\"modules/{$mod_dirname}/images/blocks/blank.png\" style=\"border:0;margin:2px 3px 0 0;width:9px;height:9px;\" alt=\"\" /></td>"
						."<td>$class: {$classes[$class]}</td></tr></table></li>\n";
				}
			} else {
				$mylist .= "    <li>$class: {$classes[$class]}</li>\n";
			}
		}
		$content .= "  <li><table border=\"0\" cellpadding=\"0\" cellspacing=\"0\"><tr><td valign=\"top\"><img src=\"modules/{$mod_dirname}/images/blocks/closed.gif\" onclick=\"bid{$bid}_togglelist(this)\" style=\"border:0;margin:2px 3px 0 0;width:9px;height:9px;\" alt=\"\" /></td>"
			."<td>Scout:&nbsp;{$classes['scouts']}</td></tr></table>\n"
			."  <ul style=\"display:none;list-style:none;margin:0 0 2px 8px;padding:0\">\n"
			."$mylist  </ul></li>\n";
		$content .= "</ul>\n";

		$content .= "<hr /><b><u>Trades Breakdown:</u></b><br />\n"
			."<ul style=\"list-style:none;margin:0 0 0 4px;padding:0\">\n";

		$result = $db->sql_query("SELECT tradeskill_class, COUNT(*) AS count FROM {$prefix}_roster_master{$suffix} AS rm"
			.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary'" : '')
			." GROUP BY rm.tradeskill_class", FALSE);
		while(list($class, $count) = $db->sql_fetchrow($result)) {
			$classes[$class] = (int)$count;
		}
		$db->sql_freeresult($result);
		// List Craftsman Archetype
		$classes['craftsmen'] = 0;
		$mylist = '';
		foreach(array('Craftsman', 'Provisioner', 'Woodworker', 'Carpenter') as $class) {
			if (isset($classes[$class])) {
				$classes['craftsmen'] += $classes[$class];
			} else {
				$classes[$class] = 0;
			}
			if ($config['breakdown_shownames']) {
				if ($classes[$class] != 0) {
					$mylist .= "    <li><table border=\"0\" cellpadding=\"0\" cellspacing=\"0\"><tr><td valign=\"top\"><img src=\"modules/{$mod_dirname}/images/blocks/closed.gif\" onclick=\"bid{$bid}_togglelist(this)\" style=\"border:0;margin:2px 3px 0 0;width:9px;height:9px;\" alt=\"\" /></td>"
						."<td>$class: {$classes[$class]}</td></tr></table>\n"
						."    <ul style=\"display:none;list-style:none;margin:0 0 2px 8px;padding:0\">\n";
					$result = $db->sql_query("SELECT rm.name_first, rm.characterId, rm.tradeskill_level FROM {$prefix}_roster_master{$suffix} AS rm"
						.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary' AND" : ' WHERE')
						." rm.tradeskill_class='{$class}'"
						." ORDER BY rm.tradeskill_level DESC", FALSE);
					while(list($name, $id, $level) = $db->sql_fetchrow($result)) {
						$mylist .= "      <li><a href=\"{$config['char_url']}{$id}\" target=\"_blank\">$name</a> ($level)</li>\n";
					}
					$db->sql_freeresult($result);
					$mylist .= "    </ul></li>\n";
				} else {
					$mylist .= "    <li><table border=\"0\" cellpadding=\"0\" cellspacing=\"0\"><tr><td valign=\"top\"><img src=\"modules/{$mod_dirname}/images/blocks/blank.png\" style=\"border:0;margin:2px 3px 0 0;width:9px;height:9px;\" alt=\"\" /></td>"
						."<td>$class: {$classes[$class]}</td></tr></table></li>\n";
				}
			} else {
				$mylist .= "    <li>$class: {$classes[$class]}</li>\n";
			}
		}
		$content .= "  <li><table border=\"0\" cellpadding=\"0\" cellspacing=\"0\"><tr><td valign=\"top\"><img src=\"modules/{$mod_dirname}/images/blocks/closed.gif\" onclick=\"bid{$bid}_togglelist(this)\" style=\"border:0;margin:2px 3px 0 0;width:9px;height:9px;\" alt=\"\" /></td>"
			."<td>Craftsman:&nbsp;{$classes['craftsmen']}</td></tr></table>\n"
			."  <ul style=\"display:none;list-style:none;margin:0 0 2px 8px;padding:0\">\n"
			."$mylist  </ul></li>\n";
		// List Outfitter Archetype
		$classes['outfitters'] = 0;
		$mylist = '';
		foreach(array('Outfitter', 'Armorer', 'Weaponsmith', 'Tailor') as $class) {
			if (isset($classes[$class])) {
				$classes['outfitters'] += $classes[$class];
			} else {
				$classes[$class] = 0;
			}
			if ($config['breakdown_shownames']) {
				if ($classes[$class] != 0) {
					$mylist .= "    <li><table border=\"0\" cellpadding=\"0\" cellspacing=\"0\"><tr><td valign=\"top\"><img src=\"modules/{$mod_dirname}/images/blocks/closed.gif\" onclick=\"bid{$bid}_togglelist(this)\" style=\"border:0;margin:2px 3px 0 0;width:9px;height:9px;\" alt=\"\" /></td>"
						."<td>$class: {$classes[$class]}</td></tr></table>\n"
						."    <ul style=\"display:none;list-style:none;margin:0 0 2px 8px;padding:0\">\n";
					$result = $db->sql_query("SELECT rm.name_first, rm.characterId, rm.tradeskill_level FROM {$prefix}_roster_master{$suffix} AS rm"
						.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary' AND" : ' WHERE')
						." rm.tradeskill_class='{$class}'"
						." ORDER BY rm.tradeskill_level DESC", FALSE);
					while(list($name, $id, $level) = $db->sql_fetchrow($result)) {
						$mylist .= "      <li><a href=\"{$config['char_url']}{$id}\" target=\"_blank\">$name</a> ($level)</li>\n";
					}
					$db->sql_freeresult($result);
					$mylist .= "    </ul></li>\n";
				} else {
					$mylist .= "    <li><table border=\"0\" cellpadding=\"0\" cellspacing=\"0\"><tr><td valign=\"top\"><img src=\"modules/{$mod_dirname}/images/blocks/blank.png\" style=\"border:0;margin:2px 3px 0 0;width:9px;height:9px;\" alt=\"\" /></td>"
						."<td>$class: {$classes[$class]}</td></tr></table></li>\n";
				}
			} else {
				$mylist .= "    <li>$class: {$classes[$class]}</li>\n";
			}
		}
		$content .= "  <li><table style=\"border-style:none\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\"><tr><td valign=\"top\"><img src=\"modules/{$mod_dirname}/images/blocks/closed.gif\" onclick=\"bid{$bid}_togglelist(this)\" style=\"border:0;margin:2px 3px 0 0;width:9px;height:9px;\" alt=\"\" /></td>"
			."<td>Outfitter:&nbsp;{$classes['outfitters']}</td></tr></table>\n"
			."  <ul style=\"display:none;list-style:none;margin:0 0 2px 8px;padding:0\">\n"
			."$mylist  </ul></li>\n";
		// List Scholar Archetype
		$classes['scholars'] = 0;
		$mylist = '';
		foreach(array('Scholar', 'Jeweler', 'Sage' , 'Alchemist') as $class) {
			if (isset($classes[$class])) {
				$classes['scholars'] += $classes[$class];
			} else {
				$classes[$class] = 0;
			}
			if ($config['breakdown_shownames']) {
				if ($classes[$class] != 0) {
					$mylist .= "    <li><table border=\"0\" cellpadding=\"0\" cellspacing=\"0\"><tr><td valign=\"top\"><img src=\"modules/{$mod_dirname}/images/blocks/closed.gif\" onclick=\"bid{$bid}_togglelist(this)\" style=\"border:0;margin:2px 3px 0 0;width:9px;height:9px;\" alt=\"\" /></td>"
						."<td>$class: {$classes[$class]}</td></tr></table>\n"
						."    <ul style=\"display:none;list-style:none;margin:0 0 2px 8px;padding:0\">\n";
					$result = $db->sql_query("SELECT rm.name_first, rm.characterId, rm.tradeskill_level FROM {$prefix}_roster_master{$suffix} AS rm"
						.(($exclude_alts) ? " LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId) WHERE rmu.char_type='Primary' AND" : ' WHERE')
						." rm.tradeskill_class='{$class}'"
						." ORDER BY rm.tradeskill_level DESC", FALSE);
					while(list($name, $id, $level) = $db->sql_fetchrow($result)) {
						$mylist .= "      <li><a href=\"{$config['char_url']}{$id}\" target=\"_blank\">$name</a> ($level)</li>\n";
					}
					$db->sql_freeresult($result);
					$mylist .= "    </ul></li>\n";
				} else {
					$mylist .= "    <li><table border=\"0\" cellpadding=\"0\" cellspacing=\"0\"><tr><td valign=\"top\"><img src=\"modules/{$mod_dirname}/images/blocks/blank.png\" style=\"border:0;margin:2px 3px 0 0;width:9px;height:9px;\" alt=\"\" /></td>"
						."<td>$class: {$classes[$class]}</td></tr></table></li>\n";
				}
			} else {
				$mylist .= "    <li>$class: {$classes[$class]}</li>\n";
			}
		}
		$content .= "  <li><table style=\"border-style:none\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\"><tr><td valign=\"top\"><img src=\"modules/{$mod_dirname}/images/blocks/closed.gif\" onclick=\"bid{$bid}_togglelist(this)\" style=\"border:0;margin:2px 3px 0 0;width:9px;height:9px;\" alt=\"\" /></td>"
			."<td>Scholar:&nbsp;{$classes['scholars']}</td></tr></table>\n"
			."  <ul style=\"display:none;list-style:none;margin:0 0 2px 8px;padding:0\">\n"
			."$mylist  </ul></li>\n";
		$content .= "</ul>\n";
	}
	// Cache content and retrieval date back to the DB.
	$db->sql_query("UPDATE {$prefix}_blocks SET content='".Fix_Quotes($content)."', time='{$current_time}' WHERE bid='$bid'", FALSE);

	// Calculate function duration in seconds.
	$error = sprintf('Time to generate block data: %.4f sec.', (get_microtime() - $timer_t));
	trigger_error($error, E_USER_NOTICE);
} else {
	$content = "\n<!-- Content retrieved from cache, roster last updated ".gmdate("Y-m-d H:i:s", $last_updated)."Z -->\n".$content;
}

// Calculate function duration in seconds.
$error = sprintf('Time to display block: %.4f sec.', (get_microtime() - $timer_t));
trigger_error($error, E_USER_NOTICE);

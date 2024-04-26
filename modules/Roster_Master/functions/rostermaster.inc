<?php 
/***********************************************************************
  Roster Master for Dragonfly(TM) CMS
  **********************************************************************
  Copyright (C) 2005-2020 by Dark Grue

  Roster Master module. Displays guild roster, character claims,
  and claim management interface.

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


// ************************** Global Variables *************************
// Establish the global variable to hold the table update time.
$table_updated = 0;

// List for selecting sort options. This associates sort options with
// DB field names and column heading titles. The order this array is
// enumerated in controls the order of options in the the sort order
// pickbox.
$sort_options = array(
	'u' => array(
		'field' => 'username',
		'title' => _USERNAME_RM,
		'shorttitle' => _USERNAMET,
		'default_srt' => 'a',
		'inv_srt' => FALSE),
	'ct' => array(
		'field' => 'char_type',
		'title' => _CHARACTERTYPE,
		'shorttitle' => _CHARACTERTYPET,
		'default_srt' => 'd',
		'inv_srt' => TRUE),
	'n' => array(
		'field' => 'rm.name_first',
		'title' => _NAME,
		'shorttitle' => _NAMET,
		'default_srt' => 'a',
		'inv_srt' => FALSE),
	'gr' => array(
		'field' => 'guild_rank',
		'title' => _GUILDRANK,
		'shorttitle' => _GUILDRANKT,
		'default_srt' => 'd',
		'inv_srt' => TRUE),
	'r' => array(
		'field' => 'type_race',
		'title' => _RACE,
		'shorttitle' => _RACET,
		'default_srt' => 'a',
		'inv_srt' => FALSE),
	'ac' => array(
		'field' => 'type_class',
		'title' => _ADVCLASS,
		'shorttitle' => _ADVCLASST,
		'default_srt' => 'a',
		'inv_srt' => FALSE),
	'al' => array(
		'field' => 'type_level',
		'title' => _ADVLEVEL,
		'shorttitle' => _ADVLEVELT,
		'default_srt' => 'd',
		'inv_srt' => FALSE),
	'tc' => array(
		'field' => 'tradeskill_class',
		'title' => _ARTCLASS,
		'shorttitle' => _ARTCLASST,
		'default_srt' => 'a',
		'inv_srt' => FALSE),
	'tl' => array(
		'field' => 'tradeskill_level',
		'title' => _ARTLEVEL,
		'shorttitle' => _ARTLEVELT,
		'default_srt' => 'd',
		'inv_srt' => FALSE),
	'2tsa' => array(
		'field' => 'adorning',
		'title' => _ADORNING,
		'shorttitle' => _ADORNINGT,
		'default_srt' => 'd',
		'inv_srt' => FALSE),
	'2tst' => array(
		'field' => 'tinkering',
		'title' => _TINKERING,
		'shorttitle' => _TINKERINGT,
		'default_srt' => 'd',
		'inv_srt' => FALSE),
	'q' => array(
		'field' => 'quests_complete',
		'title' => _QUESTS,
		'shorttitle' => _QUESTST,
		'default_srt' => 'd',
		'inv_srt' => FALSE),
	's' => array(
		'field' => 'guild_status',
		'title' => _STATUSPOINTS,
		'shorttitle' => _STATUSPOINTST,
		'default_srt' => 'd',
		'inv_srt' => FALSE));

// Initialize globals.
$s1 = $s1d = $s2 = $s2d = '';
$gb = FALSE;
$columncount = 0;


// ***************************** FUNCTIONS *****************************
// Function:	read_summary_db()
// What it does:	Reads the contents of the database into an array,
//			which it returns.
function read_summary_db() {
	global $db, $prefix, $user_prefix, $suffix, $config;

	// Fetch guild ranks from the table.
	$result = $db->sql_query("SELECT * FROM {$prefix}_roster_master_guild_rank{$suffix}", FALSE);
	$config['lookup_grank'] = array();
	// Put $config['lookup_grank'] into the global array.
	$row = array();
	while($row = $db->sql_fetchrow($result)) {
		$config['lookup_grank'][$row['grank']] = $row['name'];
	}
	$db->sql_freeresult($result);

	// Fetch all of the data from the table.
	$result = $db->sql_query("SELECT * FROM {$prefix}_roster_master_guild{$suffix}", FALSE);

	// Read the data into an array.
	$row = array();
	$data_array = array();
	while($row = $db->sql_fetchrow($result, SQL_ASSOC)) {
		$data_array[$row['name']] = $row['value'];
	}
	$db->sql_freeresult($result);

	// Return the data.
	return $data_array;
}

// Function:	read_roster_db()
// What it does:	Reads the contents of the database into an consolidated array of character data,
//			which it returns.
function read_roster_db() {
	global $userinfo, $db, $prefix, $user_prefix, $suffix;

	$data_array = array();

	// Fetch all of the roster data from the tables.
	$result = $db->sql_query("SELECT rm.characterId, rm.name_first, rm.last_update, rm.misc_last_update FROM {$prefix}_roster_master{$suffix} AS rm", FALSE);

	// Read the data into an array.
	while($char_info = $db->sql_fetchrow($result, SQL_ASSOC)) {
		$data_array[$char_info['characterId']] = array(
			'name_first' => $char_info['name_first'],
			'last_update' => $char_info['last_update'],
			'misc_last_update' => $char_info['misc_last_update']
		);
	}
	$db->sql_freeresult($result);

	// Return the data.
	return $data_array;
}

// Function:	read_char_roster_db()
// What it does:	Reads the contents of the database into an consolidated array of character data,
//			which it returns.
function read_char_roster_db($sco = FALSE, $exclude_alts = FALSE, $order_by = '') {
	global $userinfo, $db, $prefix, $user_prefix, $suffix;

	$data_array = array();

	// Fetch all of the roster data from the tables.
	// roster_master -> roster_master_users -> users
	$query = "SELECT cmsusers.username, cmsusers.user_id, rmu.char_type, rm.*
		FROM {$prefix}_roster_master{$suffix} AS rm
		LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId)
		LEFT JOIN {$user_prefix}_users AS cmsusers USING (user_id)"
		.(($sco || $exclude_alts) ? ' WHERE' : '')
		.(($exclude_alts) ? " rmu.char_type='Primary'".(($sco) ? ' AND' : '') : '')
		.(($sco) ? ' rmu.user_id='.(int)$userinfo['user_id'] : '')
		.(($order_by) ? " ORDER BY {$order_by}" : '');
//	trigger_error("\$query = {$query}", E_USER_NOTICE);
	$result = $db->sql_query($query, FALSE);

	// Read the data into an array.
	while($char_info = $db->sql_fetchrow($result, SQL_ASSOC)) {
		$data_array[$char_info['characterId']] = $char_info;
	}
	$db->sql_freeresult($result);

	// Return the data.
	return $data_array;
}

// Function:	header_link()
// What it does:	Constructs the heading link for sort selection
//			and places sort direction indicators.
function header_link($selectors) {
	global $config, $s1, $s1d, $s2, $s2d, $gb, $sco, $columncount, $sort_options, $baselink;

	// Explicitly cast parameter to array.
	if (!is_array($selectors)) {
		settype($selectors, 'array');
	}

	$indicator_on = ($config['sort_indicator'] == 0) ? 0 : ((count($selectors) > 1) || count(array_intersect(array($s1, $s2), $selectors)));
	$plural = count($selectors) > 1;
	$content = '';
	foreach($selectors as $selector) {
		$segment = '<a href="'
			.getlink("{$baselink}&s1={$selector}&s1d="
				.(($selector == $s1) ? (($s1d == 'd') ? 'a' : 'd') : $sort_options[$selector]['default_srt'])
				."&s2={$selector}"
				.(($gb) ? '&gb=1' : '')
				.((empty($sco)) ? '' : '&sco=1'))
			.'" class="statlink">'
			.((($config['short_titles'] != 'NEVER') && ($plural || ($config['short_titles'] == 'ALWAYS')) && isset($sort_options[$selector]['shorttitle']))
				? $sort_options[$selector]['shorttitle']
				: $sort_options[$selector]['title'])
			.'</a>';
		if (!empty($content)) { $segment = "($segment)"; }
		if ($indicator_on) {
			if (($selector == $s1) || ($selector == $s2)) {
				$segment .= '&nbsp;<a href="'
					.getlink("{$baselink}&s1={$s1}&s1d="
					.(($selector == $s1) ? (($s1d == 'd') ? 'a' : 'd') : $s1d)
					."&s2={$s2}&s2d="
					.(($selector == $s2) ? (($s2d == 'd') ? 'a' : 'd') : $s2d)
					.(($gb) ? '&gb=1' : '')
					.((empty($sco)) ? '' : '&sco=1'))
					.'" class="statlink">'
					.(($s1 != $s2) ? '[' : '')
					.(($s1 != $s2) ? (($selector == $s1) ? '1' : '2') : '').(($selector == $s1) ? (($s1d == 'd') ? '&darr;' : '&uarr;').(($s1 != $s2) ? ']' : '').'</a>'
					: (($s2d == 'd') ? '&darr;' : '&uarr;').']</a>');
			}
		}
		$content .= ($plural || $indicator_on) ? ((($content == '') ? '' : '&nbsp;').$segment) : $segment;
	}

	$columncount++;

	return $content;
}

// Function:	format_time_duration()
// What it does:	Calculate elapsed time in ISO 8601 format.
function format_time_duration($duration) {
	// Special case, zero-length duration.
	if (empty($duration)) return 'PT0S';

	// 60*60*24*365 seconds in a year.
	$yearsDiff = floor($duration / 31536000);
	$duration -= $yearsDiff * 31536000;
	// ISO 8601 "P" intervals with a D specification not associated with a
	// specific date assume 30-day months.
	// 60*60*24*30 seconds in a month.
	$monthsDiff = floor($duration / 2592000);
	$duration -= $monthsDiff * 2592000;
	// 60*60*24*7 seconds in a week.
	//$weeksDiff = floor($duration / 604800);
	//$duration -= $weeksDiff * 604800;
	// 60*60*24 seconds in a day.
	$daysDiff = floor($duration / 86400);
	$duration -= $daysDiff * 86400;
	// 60*60 seconds in an hour.
	$hrsDiff = floor($duration / 3600);
	$duration -= $hrsDiff * 3600;
	// 60 seconds in a minute.
	$minsDiff = floor($duration / 60);
	$duration -= $minsDiff * 60;
	$secsDiff = $duration;

	// Periods of Time, no Specific Start or End:
	// This is the only format for which variable width values are used. The
	// value starts with "P", and is followed by a list of periods, each
	// appended by a single letter designator: "Y" for years, "M" for
	// months, "D" for days, "W" for weeks, "H" for hours, "M" for minutes,
	// and "S" for seconds. Time components must be preceded by the "T"
	// character. 
	return 'P'.
	(($yearsDiff) ? "{$yearsDiff}Y" : '').
	(($monthsDiff) ? "{$monthsDiff}M" : '').
	//(($weeksDiff) ? "{$weeksDiff}W" : '').
	(($daysDiff) ? "{$daysDiff}D" : '').
	(($secsDiff || $minsDiff || $hrsDiff) ? 'T'.
	(($hrsDiff) ? "{$hrsDiff}H" : '').
	(($minsDiff) ? "{$minsDiff}M" : '').
	(($secsDiff) ? "{$secsDiff}S" : '') : '');
}

// Function:	rostermaster()
// What it does:	Reads, sorts, and displays the roster.
// Requires:
//		$_GET or $_POST['s1'] -- sort
//		$_GET or $_POST['s1d'] -- sort direction
//		$_GET or $_POST['s1'] -- secondary sort
//		$_GET or $_POST['s1d'] -- secondary sort direction
//		$_GET or $_POST['gb'] -- group by
//		$_GET or $_POST['sco'] -- show claimed characters only
function rostermaster() {
	// Start function duration timer.
	$timer_t = get_microtime(TRUE);

	global $s1, $s1d, $s2, $s2d, $gb, $sco, $columncount,
		$config, $db, $prefix, $suffix, $baselink,
		$exclude_alts, $sort_options, $table_updated, $mod_dirname, $userinfo;

	// Grab the sort values from the server environment.
	$s1 = (empty($_GET['s1'])) ? ((isset($_POST['s1'])) ? $_POST['s1'] : '') : $_GET['s1'];
	$s1d = (empty($_GET['s1d'])) ? ((isset($_POST['s1d'])) ? $_POST['s1d'] : '') : $_GET['s1d'];
	$s2 = (empty($_GET['s2'])) ? ((isset($_POST['s2'])) ? $_POST['s2'] : '') : $_GET['s2'];
	$s2d = (empty($_GET['s2d'])) ? ((isset($_POST['s2d'])) ? $_POST['s2d'] : '') : $_GET['s2d'];
	$gb = ((isset($_GET['gb']) && (($_GET['gb'] == '1') || ($_GET['gb'] == '0')))
		? (($_GET['gb'] == '1') ? TRUE : FALSE)
		: ((isset($_POST['gb']) && (($_POST['gb'] == '1') || ($_POST['gb'] == '0')))
			? (($_POST['gb'] == '1') ? TRUE : FALSE)
			: (($config['group_by']) ? TRUE : FALSE)));
	$sco = (empty($_GET['sco'])) ? ((isset($_POST['sco'])) ? (bool)$_POST['sco'] : FALSE) : (bool)$_GET['sco'];

	require('header.php');
	OpenTable();

	do_commands();

	// SORT CODE
	// Set default search options.
	if (empty($sort_options[$s1]) || ($exclude_alts && ($s1 == 'ct'))) {
		$s1 = $config['pri_sort_default'];
		$s1d = $config['pri_sort_dir_default'];
	}
	// Sanity check primary sort options and set if necessary.
	if (empty($sort_options[$s1])) {
		$s1 = 'gr';	// guild rank
	}
	if (($s1d != 'a') && ($s1d != 'd')) {
		$s1d = 'd'; // decending
	}

	// Set s2 = s1 if a column header was clicked or switched from Display All view to Display Primary.
	if (($s2 == $s1) || ($exclude_alts && ($s2 == 'ct'))) {
		$s2 = $s1;
		$s2d = $s1d;
	}

	// Set default secondary search options.
	if (empty($sort_options[$s2])) {
		$s2 = $config['sec_sort_default'];
		$s2d = $config['sec_sort_dir_default'];
	}
	// Sanity check secondary sort options and set if necessary.
	if (empty($sort_options[$s2])) {
		$s2 = 'al'; // adventurer level
	}
	if (($s2d != 'a') && ($s2d != 'd')) {
		$s2d = 'd'; // decending
	}

	// Fetch the guild summary data out of the database tables
	$summary_data_array = read_summary_db();

	// Construct the ORDER BY clause(s) for sorting.
	// Force NULL fields to always sort last.
	$orderby = '';
	$direction = (($s1d == 'd') ? (($sort_options[$s1]['inv_srt']) ? 'ASC' : 'DESC') : (($sort_options[$s1]['inv_srt']) ? 'DESC' : 'ASC'));
	switch ($s1) {
		case 'gr':
			$orderby .= "CASE WHEN {$sort_options[$s1]['field']}=666 THEN 1 ELSE 0 END ASC, ";
			break;
		case 'r':
			$orderby .= "CASE WHEN {$sort_options[$s1]['field']}='' THEN 1 WHEN {$sort_options[$s1]['field']} IS NULL THEN 1 ELSE 0 END ASC, ";
			break;
		case 'ac':
		case 'tc':
			$orderby .= "CASE WHEN {$sort_options[$s1]['field']}='Unskilled' THEN 1 WHEN LENGTH({$sort_options[$s1]['field']})>1 THEN 0 ELSE 2 END ASC, ";
			break;
	}
	$orderby .= "{$sort_options[$s1]['field']} {$direction}";
	if ($s1 != $s2) {
		$orderby .= ', ';
		$direction = (($s2d == 'd') ? (($sort_options[$s2]['inv_srt']) ? 'ASC' : 'DESC') : (($sort_options[$s2]['inv_srt']) ? 'DESC' : 'ASC'));
		switch ($s2) {
			case 'gr':
				$orderby .= "CASE WHEN {$sort_options[$s2]['field']}=666 THEN 1 ELSE 0 END ASC, ";
				break;
			case 'r':
				$orderby .= "CASE WHEN {$sort_options[$s2]['field']}='' THEN 1 WHEN {$sort_options[$s2]['field']} IS NULL THEN 1 ELSE 0 END ASC, ";
				break;
			case 'ac':
			case 'tc':
				$orderby .= "CASE WHEN {$sort_options[$s2]['field']}='Unskilled' THEN 1 WHEN LENGTH({$sort_options[$s2]['field']})>1 THEN 0 ELSE 2 END ASC, ";
				break;
		}
		$orderby .= "{$sort_options[$s2]['field']} {$direction}";
	}
//	trigger_error("\$orderby = {$orderby}", E_USER_NOTICE);

	// Fetch the guild roster data out of the database tables
	$data_array = read_char_roster_db($sco, $exclude_alts, $orderby);

	// Add page element to flip between full list and Primary only.
	$relink = '';
	if ($config['show_alts'] == 0) {
		$relink = $baselink
			.((empty($s1)) ? '' : "&s1={$s1}")
			.((empty($s1d)) ? '' : "&s1d={$s1d}")
			.((empty($s2)) ? '' : "&s2={$s2}")
			.((empty($s2d)) ? '' : "&s2d={$s2d}")
			.(($gb) ? '&gb=1' : '')
			.((empty($sco)) ? '' : '&sco=1');
		if ($exclude_alts) {
			$relink = getlink((strpos($relink, 'show_all=')) ? str_replace('&show_all=0', '&show_all=1', $relink) : $relink.'&show_all=1');
		} else {
			$relink = getlink((strpos($relink, 'show_all=')) ? str_replace('&show_all=1', '', $relink) : $relink);
		}
//		cpg_error('<pre style="text-align:left">'.htmlspecialchars($baselink)."\ns1: $s1\ns1d: $s1d\ns2: $s2\ns2d: $s2d\n".htmlspecialchars($relink)."</pre>\n");
	}

	// Open Table
	if ($config['show_updated'] || $config['show_adv_sort']) {
		if ($config['show_updated']) {
			$expiry = ($config['cache_update_time']) ? time() - (intval($table_updated) + $config['cache_update_time']) : 0;
//			cpg_error("time: ".time().", last_updated: $table_updated, expiry: $expiry");
			if ($exclude_alts) {
				list($total) = $db->sql_ufetchrow("SELECT COUNT(*) FROM {$prefix}_roster_master{$suffix}", SQL_NUM);
				$cpgtpl->assign_vars(array(
					'S_TOTAL_ROSTER' => $total
				));
			}
			$cpgtpl->assign_vars(array(
				'U_GUILD_URL' => $config['guild_url'],
				'S_GUILD_NAME' => $summary_data_array['Guild Name'],
// FIXME - TODO: Update when/if server status and summary pages are available.
//				'U_SERVER_URL' => $config['server_url'].$summary_data_array['Server'].'/',
				'U_SERVER_URL' => '',
				'S_SERVER_NAME' => $summary_data_array['Server'],
				'S_LAST_UPDATED' => formatDateTime($table_updated - date('Z'), $config['updated_date_fmt']),
				'S_UPDATE_WARNING' => (($expiry > 0) ? "&nbsp;<img src=\"modules/{$mod_dirname}/images/icon_exclamation.png\" style=\"border:0;width:16px;height:16px\" alt=\"".sprintf(_ERR_ROSTEREXPIRED, format_time_duration($expiry)).'" title="'.sprintf(_ERR_ROSTEREXPIRED, format_time_duration($expiry)).'" />' : ''),
				'S_GUILD_LEVEL' => $summary_data_array['Guild Level'],
				'S_TOTAL_LISTED' => count($data_array)
			));
		}
		if ($config['show_adv_sort']) {
			// Construct the primary and secondary sort options lists.
			$sort_options_pri = '';
			$sort_options_sec = '';
			foreach(array_keys($sort_options) as $key => $option) {
				// Do not display Character Type if we're only showing Primary.
				if (($option == 'ct') && $exclude_alts) { continue; }
				// Do not display Username if excluded by config.
				if (($option == 'u') && !in_array('u', $config['columns'])) { continue; }
				$sort_options_pri .= "<option value=\"$option\"".(($option == $s1) ? ' selected="selected"' : '').">{$sort_options[$option]['title']}</option>";
				$sort_options_sec .= "<option value=\"$option\"".(($option == $s2) ? ' selected="selected"' : '').">{$sort_options[$option]['title']}</option>";
			}
			$cpgtpl->assign_vars(array(
				'U_ROSTER_DISPLAY' => getlink($mod_dirname.((($config['show_alts'] == 0) && !$exclude_alts) ? "&show_all=1" : '')),
				'S1_SORT_OPTIONS' => $sort_options_pri,
				'S1_A_CHECKED' => (($s1d != 'd') ? ' checked="checked"' : ''),
				'S1_D_CHECKED' => (($s1d == 'd') ? ' checked="checked"' : ''),
				'GROUPED_SELECTED' => (($gb) ? ' selected="selected"' : ''),
				'S2_SORT_OPTIONS' => $sort_options_sec,
				'S2_A_CHECKED' => (($s2d != 'd') ? ' checked="checked"' : ''),
				'S2_D_CHECKED' => (($s2d == 'd') ? ' checked="checked"' : ''),
				'SCO_CHECKED' => (($sco) ? ' checked="checked"' : '')
			));
		}
	}

	$cpgtpl->assign_vars(array(
		'S_RELINK' => $relink,
		'B_EXCLUDE_ALTS' => $exclude_alts,
		'B_SHOW_UPDATED' => $config['show_updated'],
		'B_SHOW_ADV_SORT' => $config['show_adv_sort'],
		'S_TARGET' => (($config['popup']) ? ' target="_blank"' : ''),
		'S_VERSIONBANNER' => "<table class=\"forumline\" width=\"100%\">\n"
			."<tr><td style=\"text-align:center\"><span style=\"font-size:65%\"><a href=\"http://www.theclenchedfist.com/projects/rm4df\">Roster Master for Dragonfly (RM4DF)</a> {$config['version_string']}</span></td></tr>\n"
			."</table>\n"
	));

	// Groupable fields.
	$groupable_fields = array('ct', 'gr', 'r', 'ac', 'tc');
	// Display row headers.
	$endcount = count($config['columns']) - 1;
	foreach($config['columns'] as $column) {
		// Do not display Character Type if we're only showing Primary.
		if ($exclude_alts && ($column == 'ct')) { continue; }
		// Do not display column if we're grouping by.
		if ($gb && ($column == $s1) && in_array($s1, $groupable_fields)) { continue; }

		switch ($column) {
			case 'a':
				$header = _AVATAR_RM;
				$columncount++;
				break;
			case 'acl':
				$header = header_link((!($gb && ($s1 == 'ac'))) ? array('ac', 'al') : 'al');
				break;
			case 'tcl':
				$header = header_link((!($gb && ($s1 == 'tc'))) ? array('tc', 'tl') : 'tl');
				break;
			case '2tsc':
				$header = header_link(array('2tsa', '2tst'));
				break;
			default:
				$header = header_link($column);
		}
		$cpgtpl->assign_block_vars('header', array(
			'S_COLCLASS' => (($columncount == 0) ? 'thCornerL' : (($columncount == $endcount) ? 'thCornerR' : 'thTop')),
			'S_HEADER' => $header
		));
	}

	// Display rows of data.
	if (in_array('a', $config['columns'])) {
		$avatarimages = array();
		switch (strtoupper($config['avatars'])) {
			case 'ARCH':
				$avatarimages = glob("modules/{$mod_dirname}/images/avatars/archetypes/*.png");
				break;
			case 'CLASS':
				$avatarimages = glob("modules/{$mod_dirname}/images/avatars/classes/*.png");
				break;
			case 'RANK':
				$avatarimages = glob("modules/{$mod_dirname}/images/avatars/ranks/*.png");
				break;
			case 'EQ2':
				$avatarimages = glob("modules/{$mod_dirname}/images/avatars/eq2/*.png");
				break;
		}
	}		
	$classesimages = glob("modules/{$mod_dirname}/images/roster/classes/*.png");
	$racesimages = glob("modules/{$mod_dirname}/images/roster/races/*.png");
	$ranksimages = glob("modules/{$mod_dirname}/images/roster/ranks/*.png");
	$tradeskillsimages = glob("modules/{$mod_dirname}/images/roster/tradeskills/*.png");
/*	
	cpg_error('<pre style="text-align:left">'
		.'$avatarimages = '.htmlspecialchars(print_r($classesimages, TRUE))."\n\n"
		.'$classesimages = '.htmlspecialchars(print_r($classesimages, TRUE))."\n\n"
		.'$racesimages = '.htmlspecialchars(print_r($racesimages, TRUE))."\n\n"
		.'$ranksimages = '.htmlspecialchars(print_r($ranksimages, TRUE))."\n\n"
		.'$tradeskillsimages = '.htmlspecialchars(print_r($tradeskillsimages, TRUE))."\n\n"
		."</pre>\n");
*/

	// Twofer: Make sure we know when the group has changed, even if it's
	// NULL. Casts can be useful, and so can shibboleths.
	$currentfield = -21524111;
	$groupheader = '';
	$i = 0;
	foreach($data_array as $char_info) {
		$groupheader = '';
		// Groupable fields
		if ($gb && in_array($s1, $groupable_fields) && ($char_info[$sort_options[$s1]['field']] !== $currentfield)) {
			$currentfield = $char_info[$sort_options[$s1]['field']];
			switch ($s1) {
				case 'gr':
					$groupheader = ((empty($config['lookup_grank'][$char_info['guild_rank']])) ? _UNKNOWN_RM : $config['lookup_grank'][$char_info['guild_rank']]);
					break;
				default:
					$groupheader = (($currentfield == '') ? _NONE : $currentfield);
			}
			$i = 0;
		} else {
			$groupheader = '';
		}

		// Determine avatar file.
		$avatar_file = '';
		if (in_array('a', $config['columns'])) {
			// It's tempting, but don't use getimagesize() here, it'd be a huge performance hit.
			if (file_exists("modules/{$mod_dirname}/images/avatars/characterId/{$char_info['characterId']}.png")) {
				// Use avatar file named as Character ID.
				$avatar_file = "modules/{$mod_dirname}/images/avatars/characterId/{$char_info['characterId']}.png";
			} else {
				// Establish default.
				$avatar_file = "modules/{$mod_dirname}/images/avatars/anon.png";

				$is_officer = in_array($char_info['guild_rank'], $config['officers']);
				switch (strtoupper($config['avatars'])) {
					case 'ARCH':
						if ($archetype = mb_strtolower(retkey_in_array($char_info['type_class'], $config['archetypes']))) {
							if ($is_officer && in_array("modules/{$mod_dirname}/images/avatars/archetypes/officer_{$archetype}.png", $avatarimages)) {
								$avatar_file = "modules/{$mod_dirname}/images/avatars/archetypes/officer_{$archetype}.png";
							} else if (in_array("modules/{$mod_dirname}/images/avatars/archetypes/{$archetype}.png", $avatarimages)) {
									$avatar_file = "modules/{$mod_dirname}/images/avatars/archetypes/{$archetype}.png";
							}
						}
						break;
					case 'CLASS':
						$class = mb_strtolower(str_replace(' ', '', $char_info['type_class']));
						if ($is_officer && in_array("modules/{$mod_dirname}/images/avatars/classes/officer_{$class}.png", $avatarimages)) {
							$avatar_file = "modules/{$mod_dirname}/images/avatars/classes/officer_{$class}.png";
						} else if (in_array("modules/{$mod_dirname}/images/avatars/classes/{$class}.png", $avatarimages)) {
							$avatar_file = "modules/{$mod_dirname}/images/avatars/classes/{$class}.png";
						}
						break;
					case 'RANK':
						if (in_array("modules/{$mod_dirname}/images/avatars/ranks/rank-{$char_info['guild_rank']}.png", $avatarimages)) {
							$avatar_file = "modules/{$mod_dirname}/images/avatars/ranks/rank-{$char_info['guild_rank']}.png";
						} else if ($is_officer) {
							$avatar_file = "modules/{$mod_dirname}/images/avatars/ranks/officer_anon.png";
						}
						break;
					case 'EQ2':
						// Pick a random color EQ2 icon.
						if (count($avatarimages)) {
							$avatar_file = $avatarimages[array_rand($images, 1)];
						}
						break;
				}
			}
		}

		// Determine list of alts.
		$altlist = array();
		$alts = '';
		if (!empty($char_info['account_link_id'])) {
			$result = $db->sql_query("SELECT name_last, name_first, characterId
				FROM {$prefix}_roster_master{$suffix}
				WHERE characterId!=".(int)$char_info['characterId'].' AND account_link_id='.(int)$char_info['account_link_id'], FALSE);
			// Only populate list if it contains alts (it's possible to have an altlist that only contains the character it's attached to).
			if ($db->sql_numrows($result) > 0) {
				while($alt_info = $db->sql_fetchrow($result, SQL_ASSOC)) {
					//$altlist[] = (($alt_info['characterId'] == 0 ) ? $alt_info['name_first'] : "<a href=\"{$config['char_url']}{$alt_info['characterId']}\"".(($config['popup']) ? ' target="_blank"' : '').">{$alt_info['name_first']}</a>");
					$altlist[] = $alt_info['name_first'];
				}
				$db->sql_freeresult($result);

				// Do a natural sort on list and concatenate.
				natsort($altlist);
				$alts = implode(', ', $altlist);
			}
		}			

		$cpgtpl->assign_block_vars('character_row', array(
			'S_ROWCLASS' => ((!($i++ % 2)) ? 'row1' : 'row2'),
			'S_GROUPHEADER' => $groupheader,
			'S_COLSPAN' => $columncount,
			'U_CHAR_PROFILE' => (($char_info['characterId'] == 0 ) ? '' : $config['char_url'].$char_info['characterId']),
			'U_AVATAR_IMG' => $avatar_file,
			'S_AVATAR_WIDTH' => $config['avatar_size']['width'],
			'S_AVATAR_HEIGHT' => $config['avatar_size']['height'],
			'S_CHAR_USERNAME' => ((isset($char_info['username'])) ? $char_info['username'] : ''),
			'S_CHAR_USERID' => ((isset($char_info['user_id'])) ? $char_info['user_id'] : ''),
			'U_CHAR_USERID' => getlink('Your_Account&profile='.$char_info['user_id']),
			'S_CHAR_TYPE' => ((isset($char_info['char_type'])) ? $char_info['char_type'] : '<i>'._NONE.'</i>'),
			'U_ALTS_IMG' => "modules/{$mod_dirname}/images/icon_alts.png",
			'U_COLLECTIONS' => getlink("{$baselink}&rm=cm_display&characterId={$char_info['characterId']}"),
			'U_COLLECTIONS_IMG' => "modules/{$mod_dirname}/images/icon_collection.png",
			'S_COLLECTIONS_TITLE' => _TRACKERCOLLECT,
			'U_EDIT_CLAIM' => ((isset($char_info['user_id']) && ($char_info['user_id'] == $userinfo['user_id'])) ? getlink("{$baselink}&rm=rm_edit_claim&characterId={$char_info['characterId']}") : ''),
			'U_EDIT_CLAIM_IMG' => "modules/{$mod_dirname}/images/icon_edit.png",
			'U_CLASS_IMG' => ((in_array("modules/{$mod_dirname}/images/roster/classes/".mb_strtolower($char_info['type_class']).'.png', $classesimages)) ? "modules/{$mod_dirname}/images/roster/classes/".mb_strtolower(str_replace(' ', '', $char_info['type_class'])).'.png' : "modules/{$mod_dirname}/images/roster/classes/unknown.png"),
			'U_RACE_IMG' => ((in_array("modules/{$mod_dirname}/images/roster/races/".mb_strtolower(str_replace(' ', '', $char_info['type_race'])."-{$char_info['type_gender']}").'.png', $racesimages)) ? "modules/{$mod_dirname}/images/roster/races/".mb_strtolower(str_replace(' ', '', "{$char_info['type_race']}-{$char_info['type_gender']}")).'.png' : "modules/{$mod_dirname}/images/roster/races/unknown.png"),
			'U_RANK_IMG' => ((in_array("modules/{$mod_dirname}/images/roster/ranks/"."rank-{$char_info['guild_rank']}.png", $ranksimages)) ? "modules/{$mod_dirname}/images/roster/ranks/rank-{$char_info['guild_rank']}.png" : "modules/{$mod_dirname}/images/roster/ranks/unknown.png"),
			'U_TRADESKILL_IMG' => ((in_array("modules/{$mod_dirname}/images/roster/tradeskills/".mb_strtolower($char_info['tradeskill_class']).'.png', $tradeskillsimages)) ? "modules/{$mod_dirname}/images/roster/tradeskills/".mb_strtolower($char_info['tradeskill_class']).'.png' : "modules/{$mod_dirname}/images/roster/tradeskills/unknown.png"),
			'S_ALTLIST' => $alts,
			// character collection
			'S_CHAR_PLAYEDTIME' => ((empty($char_info['playedtime'])) ? _UNKNOWN_RM : format_time_duration($char_info['playedtime'])),
			'S_CHAR_LAST_UPDATE' => ((empty($char_info['last_update'])) ? _UNKNOWN_RM : formatDateTime($char_info['last_update'], "%a, %d %b %Y %H:%M:%S")),
			// pvp
			'S_CHAR_PVP_WILD_KILLS' => number_format($char_info['pvp_wild_kills']),
			'S_CHAR_PVP_TITLE_RANK' => number_format($char_info['pvp_title_rank']),
			'S_CHAR_PVP_KILL_STREAK' => number_format($char_info['pvp_kill_streak']),
			'S_CHAR_PVP_DEATHS' => number_format($char_info['pvp_deaths']),
			'S_CHAR_PVP_DEATH_STREAK' => number_format($char_info['pvp_death_streak']),
			'S_CHAR_PVP_CITY_KILLS' => number_format($char_info['pvp_city_kills']),
			//secondarytradeskills
			'S_CHAR_2TS_AD_LEVEL' => intval($char_info['adorning']),
			'S_CHAR_2TS_TINK_LEVEL' => intval($char_info['tinkering']),
			// statistics
			'S_CHAR_STATISTICS_KILLS' => number_format($char_info['statistics_kills']),
			'S_CHAR_STATISTICS_DEATHS' => number_format($char_info['statistics_deaths']),
			'S_CHAR_STATISTICS_MAX_MELEE_HIT' => number_format($char_info['statistics_max_melee_hit']),
			'S_CHAR_STATISTICS_MAX_MAGIC_HIT' => number_format($char_info['statistics_max_magic_hit']),
			'S_CHAR_STATISTICS_RARE_HARVESTS' => number_format($char_info['statistics_rare_harvests']),
			'S_CHAR_STATISTICS_ITEMS_CRAFTED' => number_format($char_info['statistics_items_crafted']),
			// type
			'S_CHAR_TYPE_RACE' => ((empty($char_info['type_race'])) ? _UNKNOWN_RM : $char_info['type_race']),
			'S_CHAR_ADV_LEVEL' => intval($char_info['type_level']),
			'S_CHAR_TYPE_GENDER' => ((empty($char_info['type_gender'])) ? _UNKNOWN_RM : $char_info['type_gender']),
			'S_CHAR_TYPE_DEITY' => ((empty($char_info['type_deity'])) ? _UNKNOWN_RM : $char_info['type_deity']),
			'S_CHAR_ADV_CLASS' => ((empty($char_info['type_class'])) ? _UNKNOWN_RM : $char_info['type_class']),
			'S_CHAR_TYPE_BIRTHDATE_UTC' => ((empty($char_info['type_birthdate_utc'])) ? _UNKNOWN_RM : formatDateTime($char_info['type_birthdate_utc'], "%a, %d %b %Y %H:%M:%S")),
			// tradeskills
			'S_CHAR_ART_CLASS' => ((empty($char_info['tradeskill_class'])) ? _UNKNOWN_RM : $char_info['tradeskill_class']),
			'S_CHAR_ART_LEVEL' => intval($char_info['tradeskill_level']),
			// achievements
			'S_CHAR_ACHIEVEMENTS_TOTAL_POINTS' => number_format($char_info['achievements_total_points']),
			'S_CHAR_ACHIEVEMENTS_TOTAL_COUNT' => number_format($char_info['achievements_total_count']),
			'S_CHAR_ACHIEVEMENTS_POINTS' => number_format($char_info['achievements_points']),
			'S_CHAR_ACHIEVEMENTS_COMPLETED' => number_format($char_info['achievements_completed']),
			// locationdata
			'S_CHAR_LOCATIONDATA_ZONENAME' => htmlspecialchars($char_info['locationdata_zonename']),		
			'S_CHAR_LOCATIONDATA_BINDZONE' => htmlspecialchars($char_info['locationdata_bindzone']),
			// arena
			'S_CHAR_ARENA_CTF_DEATHS' => number_format($char_info['arena_ctf_deaths']),
			'S_CHAR_ARENA_CTF_WINS' => number_format($char_info['arena_ctf_wins']),
			'S_CHAR_ARENA_CTF_TIMEPLAYED' => number_format($char_info['arena_ctf_timeplayed']),
			'S_CHAR_ARENA_CTF_STREAK' => number_format($char_info['arena_ctf_streak']),
			'S_CHAR_ARENA_CTF_MATCHES' => number_format($char_info['arena_ctf_matches']),
			'S_CHAR_ARENA_CTF_LOSSES' => number_format($char_info['arena_ctf_losses']),
			'S_CHAR_ARENA_CTF_KILLS' => number_format($char_info['arena_ctf_kills']),
			'S_CHAR_ARENA_CTF_CAPTURES' => number_format($char_info['arena_ctf_captures']),
			'S_CHAR_ARENA_DEATHMATCH_DEATHS' => number_format($char_info['arena_deathmatch_deaths']),
			'S_CHAR_ARENA_DEATHMATCH_WINS' => number_format($char_info['arena_deathmatch_wins']),
			'S_CHAR_ARENA_DEATHMATCH_TIMEPLAYED' => number_format($char_info['arena_deathmatch_timeplayed']),
			'S_CHAR_ARENA_DEATHMATCH_STREAK' => number_format($char_info['arena_deathmatch_streak']),
			'S_CHAR_ARENA_DEATHMATCH_MATCHES' => number_format($char_info['arena_deathmatch_matches']),
			'S_CHAR_ARENA_DEATHMATCH_LOSSES' => number_format($char_info['arena_deathmatch_losses']),
			'S_CHAR_ARENA_DEATHMATCH_KILLS' => number_format($char_info['arena_deathmatch_kills']),
			'S_CHAR_ARENA_IDOL_DEATHS' => number_format($char_info['arena_idol_deaths']),
			'S_CHAR_ARENA_IDOL_WINS' => number_format($char_info['arena_idol_wins']),
			'S_CHAR_ARENA_IDOL_TIMEPLAYED' => number_format($char_info['arena_idol_timeplayed']),
			'S_CHAR_ARENA_IDOL_STREAK' => number_format($char_info['arena_idol_streak']),
			'S_CHAR_ARENA_IDOL_MATCHES' => number_format($char_info['arena_idol_matches']),
			'S_CHAR_ARENA_IDOL_LOSSES' => number_format($char_info['arena_idol_losses']),
			'S_CHAR_ARENA_IDOL_KILLS' => number_format($char_info['arena_idol_kills']),
			'S_CHAR_ARENA_IDOL_DESTROYED' => number_format($char_info['arena_idol_destroyed']),
			// account
			'S_ACCOUNT_AGE' => duration($char_info['account_age']),
			// guild		
			'S_CHAR_GUILD_STATUS' => number_format($char_info['guild_status']),
			'S_CHAR_GUILD_JOINED' => ((empty($char_info['guild_joined'])) ? _UNKNOWN_RM : formatDateTime($char_info['guild_joined'], "%a, %d %b %Y %H:%M:%S")),
			'S_CHAR_GUILD_RANK' => intval($char_info['guild_rank']),
			'S_GUILD_RANKNAME' => ((empty($config['lookup_grank'][$char_info['guild_rank']])) ? _UNKNOWN_RM : $config['lookup_grank'][$char_info['guild_rank']]),
			// name		
			'S_CHAR_NAME_SUFFIX' => htmlspecialchars($char_info['name_suffix']),
			'S_CHAR_NAME_PREFIX' => htmlspecialchars($char_info['name_prefix']),
			'S_CHAR_NAME_LAST' => htmlspecialchars($char_info['name_last']),
			'S_CHAR_NAME_FIRST' => htmlspecialchars($char_info['name_first']),
			// collections
			'S_CHAR_COLLECTIONS_COMPLETE' => number_format($char_info['collections_complete']),
			'S_CHAR_COLLECTIONS_ACTIVE' => number_format($char_info['collections_active']),
			// quests
			'S_CHAR_QUESTS_COMPLETE' => number_format($char_info['quests_complete']),
			'S_CHAR_QUESTS_ACTIVE' => number_format($char_info['quests_active'])
		));

			foreach($config['columns'] as $column) {
			// Do not display Character Type if we're only showing Primary.
			if ($exclude_alts && ($column == 'ct')) { continue; }
			// Do not display column if we're grouping by.
			if ($gb && ($column == $s1) && in_array($s1, $groupable_fields)) { continue; }

			$cpgtpl->assign_block_vars('character_row.character_column', array(
				'S_COLTYPE' => $column
			));
		}
	}
//cpg_error('<pre style="text-align:left">'.htmlspecialchars(print_r($cpgtpl, TRUE)).'</pre>');

	$cpgtpl->set_filenames(array('rostermaster' => 'roster_master/rostermaster.html'));
	$cpgtpl->display('rostermaster');

	CloseTable();

	// Log the performance statistics.
	$error = sprintf(_NTC_RSTRDISPLAY, 'rostermaster()', (get_microtime(TRUE) - $timer_t));
	trigger_error($error, E_USER_NOTICE);
	if ($config['logfile'] & 64) { $GLOBALS['log'][] = $error; }
}

// Function:	rm_save_claim()
// What it does:	Takes the "POST"ed variables and ensures they are exploit-free
//			(SQL Injection) and no duplicates for that char name, then writes the
//			claim to the database.
// Requires:
//		$_POST['characterId'] -- character ID
//		$_POST['char_type'] -- character type
//		$_SERVER['HTTP_REFERER'] -- return link
function rm_save_claim() {
	global $mod_dirname, $userinfo, $db, $prefix, $user_prefix, $suffix, $baselink;

	if (is_user() || can_admin($mod_dirname)) {
		// Get form variables and untaint.
		$characterId = (isset($_POST['characterId'])) ? intval($_POST['characterId']) : '';
		if (!isset($_POST['char_type']) || !in_array($_POST['char_type'], array('Primary', 'Secondary', 'Utility'))) {
			// Not valid character type.
			cpg_error(_ERR_ACCESSCONT);
		}
		$char_type = (isset($_POST['char_type'])) ? Fix_Quotes($_POST['char_type'], TRUE) : 'Primary';
		$posted_username = (isset($_POST['username'])) ? Fix_Quotes($_POST['username'], TRUE) : '';

		// Set username either as the posted value, or as the user logged in.
		$username = (!empty($posted_username) && can_admin($mod_dirname)) ? $posted_username : $userinfo['username'];

		// Check for non-existent username.
		$result = $db->sql_query("SELECT user_id, username FROM {$user_prefix}_users WHERE username='$username'", FALSE);
		if ($db->sql_numrows($result) != 1) {
			cpg_error('<strong>'._ERR_CLAIMSAVEFAIL."</strong><br /><br />\n"
				."Username '$username' does not exist.", _ERROR, ((empty($_SERVER['HTTP_REFERER'])) ? getlink($baselink) : $_SERVER['HTTP_REFERER']));
		}
		// Get canonical version of username (normalize capitalization).
		list($user_id, $username) = $db->sql_fetchrow($result);
		$db->sql_freeresult($result);
		// Verify that the named is owned by the user, or that the user is an Admin.
		if (($userinfo['user_id'] != $user_id) && !can_admin($mod_dirname)) {
			cpg_error(_ERR_ACCESSCONT);
		}

		// Check for non-existent character.
		$result = $db->sql_query("SELECT * FROM {$prefix}_roster_master{$suffix} WHERE characterId=".(int)$characterId, FALSE);
		if (!$db->sql_numrows($result)) {
			cpg_error('<strong>'._ERR_CLAIMSAVEFAIL."</strong><br /><br />\n"
				."Character ID '$characterId' does not exist.", _ERROR, ((empty($_SERVER['HTTP_REFERER'])) ? getlink($baselink) : $_SERVER['HTTP_REFERER']));
		}
		$db->sql_freeresult($result);

		// Check for duplicate entries.
		$result = $db->sql_query("SELECT * FROM {$prefix}_roster_master_users{$suffix} WHERE characterId=".(int)$characterId, FALSE);
		switch ($db->sql_numrows($result)) {
			case 0:
				// Create new character claim.
				$db->sql_query("INSERT INTO {$prefix}_roster_master_users{$suffix} SET characterId='{$characterId}', user_id='{$user_id}', char_type='{$char_type}'", FALSE);
				break;
			case 1:
				// Update existing claim.
				$db->sql_query("UPDATE {$prefix}_roster_master_users{$suffix}
					SET char_type='{$char_type}'
					WHERE characterId='{$characterId}'", FALSE);
				break;
			default:
				cpg_error(_ERR_ACCESSCONT
					.(($db->sql_numrows($result) > 1) ? "<br />\n"._ERR_DBCE : ''));
		}
		$db->sql_freeresult($result);

		// Autoreturn back to the calling page - could be claim char or list
		// char.
		(isset($_SERVER['HTTP_REFERER'])) ? url_redirect($_SERVER['HTTP_REFERER']) : url_redirect(getlink($baselink));
	} else {
		// Not a user.
		cpg_error(_ERR_ACCESSCONT);
	}
}

// Function:	rm_edit_claim()
// What it Does:	Grabs and verifies that characterId is exploit free, verifies the character
//			exists, and presents the input form to user.
// Requires:	$_GET['characterId'] -- character ID to edit
function rm_edit_claim() {
	global $pagetitle, $mod_dirname, $config, $userinfo, $db, $prefix, $user_prefix, $suffix, $BASEHREF, $baselink, $sig_savepath;

	$pagetitle .= ' '._BC_DELIM.' '._EDITCHARCLAIM;

	// Get the character id from the URL, ensure it's an integer.
	$characterId = (isset($_GET['characterId'])) ? intval($_GET['characterId']) : 0;

	if (is_user() || can_admin($mod_dirname)) {
		$username = $userinfo['username'];

		// Ensure the entry exists and determine the owner.
		$result = $db->sql_query("SELECT rmu.*, rm.characterId, rm.name_first, rm.guild_rank
			FROM {$prefix}_roster_master_users{$suffix} AS rmu
			INNER JOIN {$prefix}_roster_master{$suffix} AS rm USING (characterId)
			INNER JOIN {$user_prefix}_users AS cmsusers USING (user_id)
			WHERE rmu.characterId=".(int)$characterId, FALSE);
		if ($db->sql_numrows($result) == 1) {
			$char_info = $db->sql_fetchrow($result, SQL_ASSOC);
			$db->sql_freeresult($result);

			// Verify that the named is owned by the editor, or that the editor is an Admin.
			if (($char_info['user_id'] == $userinfo['user_id']) || can_admin($mod_dirname)) {
				require('header.php');
				OpenTable();

				do_commands();

				$cpgtpl->assign_vars(array(
					'U_BASELINK' => getlink($baselink),
					'S_TARGET' => (($config['popup']) ? ' target="_blank"' : ''),
					'U_CHAR_PROFILE' => (($char_info['characterId'] == 0 ) ? '' : $config['char_url'].$char_info['characterId']),
					'S_CHAR_NAME_FIRST' => $char_info['name_first'],
					'S_SELECT_LIST' => '<option'.(($char_info['char_type'] == 'Primary') ? ' selected="selected"' : '').'>Primary</option>'
						.'<option'.(($char_info['char_type'] == 'Secondary') ? ' selected="selected"' : '').'>Secondary</option>'
						.'<option'.(($char_info['char_type'] == 'Utility') ? ' selected="selected"' : '').'>Utility</option>',
					'S_CHAR_CHARACTERID' => $char_info['characterId']
				));

				if ($config['dynamic_sigs'] && !in_array($char_info['guild_rank'], $config['exclude_ranks'])) {
					// Get final signature size by reading the sigblock base's file attributes.
					$block_file = "modules/{$mod_dirname}/images/sigblocks/{$config['sigblock_basename']}.png";
					$block_file = (is_file($block_file)) ? $block_file : "modules/{$mod_dirname}/images/sigblocks/default_sigblock.png";
					list($width, $height, $type, $attr) = getimagesize($block_file);
					$cpgtpl->assign_vars(array(
						'B_SIGBLOCK' => true,
						'U_SIG_PREVIEW' => getlink("{$baselink}&rm=sm_sig&preview=1&characterId={$char_info['characterId']}"),
						'S_WIDTH' => $width,
						'S_HEIGHT' => $height,
						'U_SIGLINK' => (($config['sig_image_rewrite_hack']) ? "{$BASEHREF}modules/{$mod_dirname}/sig_{$char_info['characterId']}.jpg" : getlink("{$baselink}&rm=sm_sig&characterId={$char_info['characterId']}", TRUE, TRUE))
					));
					if (!$config['sig_image_rewrite_hack'] && is_file("{$sig_savepath}/sig_{$char_info['characterId']}.jpg")) {
						$cpgtpl->assign_vars(array(
							'U_SIG_CACHE' => "{$BASEHREF}modules/{$mod_dirname}/signatures/sig_{$char_info['characterId']}.jpg"
						));
					} else {
						$cpgtpl->assign_vars(array(
							'U_SIG_CACHE' => ''
						));
					}
				} else {
					$cpgtpl->assign_vars(array(
						'B_SIGBLOCK' => false
					));
				}

				$cpgtpl->set_filenames(array('rm_edit_claim' => 'roster_master/rm_edit_claim.html'));
				$cpgtpl->display('rm_edit_claim');

				CloseTable();
			} else {
				// Not owner.
				cpg_error(_ERR_ACCESSCONT);
			}
		} else {
			cpg_error(_ERR_ACCESSCONT
				.(($db->sql_numrows($result) > 1) ? "<br />\n"._ERR_DBCE : ''));
		}
	} else {
		// Not a user.
		cpg_error(_ERR_ACCESSCONT);
	}
}

// Function:	rm_delete_claim()
// What it Does:	Grabs and verifies that characterId is exploit free, verifies the character
//			exists, deletes character from claims database, and returns to calling
//			entity (Via HTTP Referer).
// Requires:	$_GET['characterId'] -- character ID to delete
//		$_POST['confirm'] -- verification token
function rm_delete_claim() {
	global $pagetitle, $userinfo, $db, $prefix, $user_prefix, $suffix, $mod_dirname, $baselink;

	$pagetitle .= ' '._BC_DELIM.' '._DELCLAIM;

	// Get the character id from the URL, ensure it's an integer.
	$characterId = (isset($_GET['characterId'])) ? intval($_GET['characterId']) : 0;

	if (isset($_POST['confirm'])) {
		$username = $userinfo['username'];

		// Ensure the entry exists and determine the owner.
		$result = $db->sql_query("SELECT rmu.characterId
			FROM {$prefix}_roster_master_users{$suffix} AS rmu
			INNER JOIN {$user_prefix}_users AS cmsusers USING (user_id)
			WHERE rmu.characterId=".(int)$characterId.' AND rmu.user_id='.(int)$userinfo['user_id'], FALSE);
		// Verify that the named is owned by the deleter, or that the deleter is an Admin.
		if (($db->sql_numrows($result) == 1) || can_admin($mod_dirname)) {
			$db->sql_freeresult($result);

			// Delete claims.
			foreach (array('users', 'quest_status') as $table) {
				$db->sql_query("DELETE FROM {$prefix}_roster_master_{$table}{$suffix} WHERE characterId='{$characterId}'", TRUE);
			}

			// Autoreturn back to the calling page.
			url_redirect((isset($_POST['returnpath'])) ? base64_decode($_POST['returnpath']) : $baselink);
		} else {
			cpg_error(_ERR_ACCESSCONT
				.(($db->sql_numrows($result) > 1) ? "<br />\n"._ERR_DBCE : ''));
		}
	} else if (isset($_POST['cancel'])) {
		// Autoreturn back to the calling page.
		url_redirect((isset($_POST['returnpath'])) ? base64_decode($_POST['returnpath']) : $baselink);
	} else {
		list($name) = $db->sql_ufetchrow("SELECT name_first FROM {$prefix}_roster_master{$suffix} WHERE characterId=".(int)$characterId, SQL_NUM);

		require('header.php');
		OpenTable();

		echo '<center><b>'._DELCLAIM."</b><br /><br />\n"
			."Do you wish to delete the claim for Character ID#{$characterId}"
			.((empty($name)) ? '' : " ($name)")
			."?<br /><br />\n"
			."NOTE: This will delete the character claim, Heritage Quest claims, and dynamic signature configurations.<br /><br />\n"
			."<div style=\"display:inline\">\n"
			.'<form style="display:inline;margin:0" action="'.getlink("{$baselink}&rm=rm_delete_claim&characterId={$characterId}").'" method="post" enctype="multipart/form-data" accept-charset="utf-8">'
			// Calculate return path.
			.'<input type="hidden" name="returnpath" value="'.((isset($_SERVER['HTTP_REFERER'])) ? base64_encode($_SERVER['HTTP_REFERER']) : '').'" />'
			.'<input type="submit" name="confirm" value="'._YES."\" class=\"mainoption\" />\n"
			.'<input type="submit" name="cancel" value="'._NO."\" class=\"liteoption\" />\n"
			."</form>\n"
			."</div>\n</center>\n";

		CloseTable();
	}
}

// Function:	rm_claim_char()
// What it does:	User Interface for claiming characters.
//			Requires the user be logged in.
//			Lists the user's already claimed characters and provides management
//			options.
function rm_claim_char() {
	global $pagetitle, $config, $db, $prefix, $suffix, $mod_dirname, $userinfo, $baselink;

	$pagetitle .= ' '._BC_DELIM.' '._MANAGECHAR;

	$target = ($config['popup']) ? ' target="_blank"' : '';

	if (is_user() || can_admin($mod_dirname)) {
		$username = $userinfo['username'];

		require('header.php');
		OpenTable();

		do_commands();

		// Build list of unclaimed characters.
		$select_list = '';
		$result = $db->sql_query("SELECT rm.characterId, rm.name_first
			FROM {$prefix}_roster_master{$suffix} AS rm
			LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId)
			WHERE rmu.characterId IS NULL
			ORDER BY rm.name_first", FALSE);
		while($char_info = $db->sql_fetchrow($result, SQL_ASSOC)) {
			$select_list .= "<option value=\"{$char_info['characterId']}\">{$char_info['name_first']} ({$char_info['characterId']})</option>";
		}
		$db->sql_freeresult($result);

		$cpgtpl->assign_vars(array(
			'U_BASELINK' => getlink($baselink),
			'S_SELECT_LIST' => $select_list,
			'S_TARGET' => (($config['popup']) ? ' target="_blank"' : '')
		));

		// Display current Claims.
		$result = $db->sql_query("SELECT *
			FROM {$prefix}_roster_master{$suffix} AS rm
			INNER JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId)
			WHERE user_id=".(int)$userinfo['user_id'].'
			ORDER BY rm.name_first, rmu.char_type', FALSE);
		if ($db->sql_numrows($result) > 0) {
			// Loop through each character, displaying it and management options.
			$i = 0;
			while($char_info = $db->sql_fetchrow($result, SQL_ASSOC)) {
				$cpgtpl->assign_block_vars('claims', array(
					'B_NONE' => false,
					'S_ROWCLASS' => ((!($i++ % 2)) ? 'row1' : 'row2'),
					'U_CHAR_PROFILE' => (($char_info['characterId'] == 0 ) ? '' : $config['char_url'].$char_info['characterId']),
					'S_CHAR_NAME_FIRST' => $char_info['name_first'],
					'S_CHAR_NAME_LAST' => ((empty($char_info['name_last'])) ? '' : " {$char_info['name_last']}"),
					'S_CHAR_TYPE' => $char_info['char_type'],
					'U_CLAIM_EDIT' => getlink("{$baselink}&rm=rm_edit_claim&characterId={$char_info['characterId']}"),
					'U_CLAIM_DELETE' => getlink("{$baselink}&rm=rm_delete_claim&characterId={$char_info['characterId']}")
				));
				if ($config['dynamic_sigs'] && !in_array($char_info['guild_rank'], $config['exclude_ranks'])) {
					$cpgtpl->assign_block_vars('claims.manage_claims', array(
						'U_COLLECTIONS' => getlink("{$baselink}&rm=cm_display&characterId={$char_info['characterId']}"),
						'U_COLLECTIONS_IMG' => "modules/{$mod_dirname}/images/icon_collection.png",
						'S_COLLECTIONS_TITLE' => _TRACKERCOLLECT,
						'U_SIG' => getlink("{$baselink}&rm=sm_sig&characterId={$char_info['characterId']}"),
						'U_SIG_IMG' => "modules/{$mod_dirname}/images/icon_sig.png",
						'S_SIG_TITLE' => _SIGLINK
					));
				}
			}
		} else {
			$cpgtpl->assign_block_vars('claims', array(
				'B_NONE' => true
			));
		}
		$db->sql_freeresult($result);

		$cpgtpl->set_filenames(array('rm_claim_char' => 'roster_master/rm_claim_char.html'));
		$cpgtpl->display('rm_claim_char');

		CloseTable();
	} else {
		// Not a user.
		cpg_error(_ERR_ACCESSCONT);
	}
}

// Function:	rm_manage_claims()
// What it does:	Admin Interface for viewing all claims and deleting them.
//			User must be logged in as Admin to use.
//			Non-Admins are redirected back to the main roster list.
//			Reads the database, and outputs all the rows.
function rm_manage_claims() {
	global $pagetitle, $config, $mod_dirname, $db, $prefix, $user_prefix, $suffix, $baselink;

	$pagetitle .= ' '._BC_DELIM.' '._MANAGEROSTER;

	if (can_admin($mod_dirname)) {
		require('header.php');
		OpenTable();

		do_commands();

		$cpgtpl->assign_vars(array(
			'S_TARGET' => (($config['popup']) ? ' target="_blank"' : '')
		));

		// Build list of unclaimed characters.
		$select_list = '';
		$result = $db->sql_query("SELECT rm.characterId, rm.name_first
			FROM {$prefix}_roster_master{$suffix} AS rm
			LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId)
			WHERE rmu.characterId IS NULL
			ORDER BY rm.name_first", FALSE);
		while($char_info = $db->sql_fetchrow($result, SQL_ASSOC)) {
			$select_list .= "<option value=\"{$char_info['characterId']}\">{$char_info['name_first']} ({$char_info['characterId']})</option>";
		}
		$db->sql_freeresult($result);
		// Enter New Claim Form
		if (!empty($select_list)) {
			$cpgtpl->assign_vars(array(
				'B_NONE' => false,
				'U_BASELINK' => getlink($baselink),
				'U_SEARCHUSER' => getlink('Forums&file=search&mode=searchuser&popup=1', TRUE, TRUE),
				'S_SELECT_LIST' => $select_list
			));
		} else {
			$cpgtpl->assign_vars(array(
				'B_NONE' => true
			));
		}

		// Display current Claims.
		// Exclude widowed/orphaned Claims.
		$result = $db->sql_query("SELECT cmsusers.username, rm.characterId, rm.name_first, rm.guild_rank, rmu.*
			FROM {$prefix}_roster_master_users{$suffix} AS rmu
			INNER JOIN {$prefix}_roster_master{$suffix} AS rm USING (characterId)
			INNER JOIN {$user_prefix}_users AS cmsusers USING (user_id)
			ORDER BY cmsusers.username, rmu.char_type", FALSE);
		if ($db->sql_numrows($result) > 0) {
			// Loop through each character, displaying it and management options.
			$i = 0;
			while($char_info = $db->sql_fetchrow($result, SQL_ASSOC)) {
				$cpgtpl->assign_block_vars('claims', array(
					'B_NONE' => false,
					'S_ROWCLASS' => ((!($i++ % 2)) ? 'row1' : 'row2'),
					'U_CHAR_USERID' => getlink('Your_Account&profile='.$char_info['user_id']),
					'S_CHAR_USERNAME' => $char_info['username'],
					'U_CHAR_PROFILE' => (($char_info['characterId'] == 0 ) ? '' : $config['char_url'].$char_info['characterId']),
					'S_CHAR_NAME_FIRST' => $char_info['name_first'],
					'S_CHAR_NAME_LAST' => ((empty($char_info['name_last'])) ? '' : " {$char_info['name_last']}"),
					'S_CHAR_TYPE' => $char_info['char_type'],
					'U_CLAIM_EDIT' => getlink("{$baselink}&rm=rm_edit_claim&characterId={$char_info['characterId']}"),
					'U_CLAIM_DELETE' => getlink("{$baselink}&rm=rm_delete_claim&characterId={$char_info['characterId']}")
				));
				if ($config['dynamic_sigs'] && !in_array($char_info['guild_rank'], $config['exclude_ranks'])) {
					$cpgtpl->assign_block_vars('claims.manage_claims', array(
						'U_MANAGE_CLAIMS' => getlink("{$baselink}&rm=sm_sig&characterId={$char_info['characterId']}"),
						'U_MANAGE_CLAIMS_IMG' => "modules/{$mod_dirname}/images/icon_sig.png",
						'S_MANAGE_CLAIMS_TITLE' => _SIGLINK
					));
				}
			}
		} else {
			$cpgtpl->assign_block_vars('claims', array(
				'B_NONE' => true
			));
		}
		$db->sql_freeresult($result);

		// Display widowed Claims
		// Widowed claims are claims that no longer have characters associated
		// with them.
		$result = $db->sql_query("SELECT rm.characterId, rmu.*, cmsusers.username
			FROM {$prefix}_roster_master_users{$suffix} AS rmu
			LEFT JOIN {$prefix}_roster_master{$suffix} AS rm USING (characterId)
			LEFT JOIN {$user_prefix}_users AS cmsusers USING (user_id)
			WHERE rm.characterId IS NULL
			ORDER BY cmsusers.username", FALSE);
		if ($db->sql_numrows($result) > 0) {
			// Loop through each character, displaying it and a delete option.
			$i = 0;
			while($char_info = $db->sql_fetchrow($result, SQL_ASSOC)) {
				$cpgtpl->assign_block_vars('widowed_claims', array(
					'B_NONE' => false,
					'S_ROWCLASS' => ((!($i++ % 2)) ? 'row1' : 'row2'),
					'S_CHAR_USERID' => ((isset($char_info['user_id'])) ? $char_info['user_id'] : ''),
					'U_CHAR_USERID' => getlink('Your_Account&profile='.$char_info['user_id']),
					'S_CHAR_USERNAME' => ((isset($char_info['username'])) ? $char_info['username'] : ''),
					'U_CHAR_PROFILE' => (($char_info['characterId'] == 0 ) ? '' : $config['char_url'].$char_info['characterId']),
					'S_CHAR_NAME_FIRST' => ((isset($char_info['name_first'])) ? $char_info['name_first'] : ''),
					'S_CHAR_CHARACTERID' => $char_info['characterId'],
					'S_CHAR_TYPE' => ((isset($char_info['char_type'])) ? $char_info['char_type'] : '<i>'._NONE.'</i>'),
					'U_CLAIM_DELETE' => getlink("{$baselink}&rm=rm_delete_claim&characterId={$char_info['characterId']}")
				));
			}
		} else {
			$cpgtpl->assign_block_vars('widowed_claims', array(
				'B_NONE' => true
			));
		}
		$db->sql_freeresult($result);

		// Display orphaned Claims.
		// Orphaned claims are claims that no longer have usernames associated
		// with them.
		$result = $db->sql_query("SELECT rm.name_first, rmu.*, cmsusers.username
			FROM {$prefix}_roster_master_users{$suffix} AS rmu
			LEFT JOIN {$prefix}_roster_master{$suffix} AS rm USING (characterId)
			LEFT JOIN {$user_prefix}_users AS cmsusers USING (user_id)
			WHERE cmsusers.user_id IS NULL
			ORDER BY cmsusers.username", FALSE);
		if ($db->sql_numrows($result) > 0) {
			// Loop through each character, displaying it and a delete option.
			$i = 0;
			while($char_info = $db->sql_fetchrow($result, SQL_ASSOC)) {
				$cpgtpl->assign_block_vars('orphaned_claims', array(
					'B_NONE' => false,
					'S_ROWCLASS' => ((!($i++ % 2)) ? 'row1' : 'row2'),
					'S_CHAR_USERID' => ((isset($char_info['user_id'])) ? $char_info['user_id'] : ''),
					'U_CHAR_USERID' => getlink('Your_Account&profile='.$char_info['user_id']),
					'S_CHAR_USERNAME' => ((isset($char_info['username'])) ? $char_info['username'] : ''),
					'S_CHAR_NAME_FIRST' => ((isset($char_info['name_first'])) ? $char_info['name_first'] : ''),
					'S_CHAR_TYPE' => ((isset($char_info['char_type'])) ? $char_info['char_type'] : '<i>'._NONE.'</i>'),
					'U_CLAIM_DELETE' => getlink("{$baselink}&rm=rm_delete_claim&characterId={$char_info['characterId']}")
				));
			}
		} else {
			$cpgtpl->assign_block_vars('orphaned_claims', array(
				'B_NONE' => true
			));
		}
		$db->sql_freeresult($result);

		$cpgtpl->set_filenames(array('rm_manage_claims' => 'roster_master/rm_manage_claims.html'));
		$cpgtpl->display('rm_manage_claims');

		CloseTable();
	} else {
		// Not an admin.
		cpg_error(_ERR_ACCESSCONT);
	}
}

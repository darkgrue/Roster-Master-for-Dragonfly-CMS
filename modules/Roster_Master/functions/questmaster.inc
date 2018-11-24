<?php 
/***********************************************************************
  Roster Master for Dragonfly(TM) CMS
  **********************************************************************
  Copyright (C) 2005-2018 by Dark Grue

  Quest Master management module. Tracks Heritage, Access, and Timeline
  Quests for roster characters.

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
// List for selecting quest progress.
// Leave alt tag and tag close off of icon string.
$ProgressOptions = array(
	0 => array(
		'label' => _DONTHAVE,
		'icon' => "src=\"modules/{$mod_dirname}/images/blank.png\" style=\"border:0;width:16px;height:16px\""),
	1 => array(
		'label' => _INPROGRESS,
		'icon' => "src=\"modules/{$mod_dirname}/images/in_progress.png\" style=\"border:0;width:16px;height:16px\""),
	2 => array(
		'label' => _COMPLETED,
		'icon' => "src=\"modules/{$mod_dirname}/images/completed.png\" style=\"border:0;width:16px;height:16px\""));

// List of game expansions.
$Expansions = array(
	0 => array(
		'name' => 'The Shattered Lands',
		'shortname' => 'TSL',
		'url' => 'http://eq2.wikia.com/wiki/Shattered_Lands'),
	1 => array(
		'name' => 'Desert of Flames',
		'shortname' => 'DoF',
		'url' => 'http://eq2.wikia.com/wiki/Desert_of_Flames'),
	2 => array(
		'name' => 'Kingdom of Sky',
		'shortname' => 'KoS',
		'url' => 'http://eq2.wikia.com/wiki/Kingdom_of_Sky'),
	3 => array(
		'name' => 'Echoes of Faydwer',
		'shortname' => 'EoF',
		'url' => 'http://eq2.wikia.com/wiki/Echoes_of_Faydwer'),
	4 => array(
		'name' => 'Rise of Kunark',
		'shortname' => 'RoK',
		'url' => 'http://eq2.wikia.com/wiki/Category:Rise_of_Kunark'),
	5 => array(
		'name' => 'The Shadow Odyssey',
		'shortname' => 'TSO',
		'url' => 'http://eq2.wikia.com/wiki/Shadow_Odyssey'),
	6 => array(
		'name' => 'Sentinel\'s Fate',
		'shortname' => 'SF',
		'url' => 'http://eq2.wikia.com/wiki/Category:Sentinel%27s_Fate'),
	7 => array(
		'name' => 'Destiny of Velious',
		'shortname' => 'DoV',
		'url' => 'http://eq2.wikia.com/wiki/Destiny_of_Velious'),
	8 => array(
		'name' => 'Age of Discovery',
		'shortname' => 'AoD',
		'url' => 'http://eq2.wikia.com/wiki/Age_of_Discovery'),
	9 => array(
		'name' => 'Chains of Eternity',
		'shortname' => 'CoE',
		'url' => 'http://eq2.wikia.com/wiki/Chains_of_Eternity'),
	10 => array(
		'name' => 'Tears of Veeshan',
		'shortname' => 'ToV',
		'url' => 'http://eq2.wikia.com/wiki/Tears_of_Veeshan'),
	11 => array(
		'name' => 'Altar of Malice',
		'shortname' => 'AoM',
		'url' => 'http://eq2.wikia.com/wiki/Altar_of_Malice'),
	12 => array(
		'name' => 'Terrors of Thalumbra',
		'shortname' => 'ToT',
		'url' => 'http://eq2.wikia.com/wiki/Terrors_of_Thalumbra'),
	13 => array(
		'name' => 'Kunark Ascending',
		'shortname' => 'KA',
		'url' => 'http://eq2.wikia.com/wiki/LU103'),
	14 => array(
		'name' => 'Planes of Prophecy',
		'shortname' => 'PoP',
		'url' => 'http://eq2.wikia.com/wiki/Planes_of_Prophecy'),
	15 => array(
		'name' => '	Chaos Descending',
		'shortname' => 'CD',
		'url' => 'https://eq2.fandom.com/wiki/Chaos_Descending')
	);

// Load quest data.
require_once("modules/{$mod_dirname}/includes/quests_data.inc");


// ***************************** FUNCTIONS *****************************
// Function:	read_char_quest_status_db()
// What it does:	Reads the contents of the quest status database into an consolidated array of character data,
//			which it returns.
function read_char_quest_status_db($qm, $sco = FALSE, $exclude_alts = FALSE, $order_by = '') {
	global $userinfo, $db, $prefix, $user_prefix, $suffix, $Timelines, $QuestsData;

	$data_array = array();

	// Fetch all of the data from the tables.
	$query = "SELECT rm.characterId, rm.name_first, rm.type_level, rmu.user_id, rmu.char_type, qs.crc, qs.stage_num, qs.completion_date
		FROM {$prefix}_roster_master_quest_status{$suffix} AS qs
		INNER JOIN {$prefix}_roster_master{$suffix} AS rm USING (characterId)
		LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId)"
		.(($sco || $exclude_alts) ? ' WHERE' : '')
		.(($exclude_alts) ? " rmu.char_type='Primary'".(($sco) ? ' AND' : '') : '')
		.(($sco) ? ' rmu.user_id='.(int)$userinfo['user_id'] : '')
		.(($order_by) ? " ORDER BY ".Fix_Quotes($order_by) : '');
//	trigger_error("\$query = {$query}", E_USER_NOTICE);
	$result = $db->sql_query($query, FALSE);

	// Read the data into an array.
	while($quest_status = $db->sql_fetchrow($result, SQL_ASSOC)) {
//		cpg_error('<pre style="text-align:left">'.count($quest_status)." elements parsed.\n\n".htmlspecialchars(print_r($quest_status, TRUE))."</pre>\n");
		$crc = $quest_status['crc'];
		$series = $QuestsData[$crc]['in_series'];
		if ($series != $qm) { continue; }
		$timeline = $QuestsData[$crc]['in_timeline'];
		if (!isset($data_array[$quest_status['characterId']])) {
			$data_array[$quest_status['characterId']] = array(
				'name_first' => $quest_status['name_first'],
				'type_level' => $quest_status['type_level'],
				'user_id' => $quest_status['user_id'],
				'char_type' => $quest_status['char_type'],
			);
		}
		if (!isset($data_array[$quest_status['characterId']]['quest_list'][$timeline]) || $QuestsData[$crc]['in_step'] > $data_array[$quest_status['characterId']]['quest_list'][$timeline]['in_step']) {
			$data_array[$quest_status['characterId']]['quest_list'][$timeline] = array(
				'in_step' => $QuestsData[$crc]['in_step'],
				'crc' => $crc,
				'stage_num' => $quest_status['stage_num'],
				'completion_date' => $quest_status['completion_date'],
				'tl_complete' => ((($QuestsData[$crc]['in_step'] == (count($Timelines[$series][$timeline]['steps']) - 1)) && ($quest_status['stage_num'] > 8000)) ? TRUE : FALSE)
			);
		}
	}
	$db->sql_freeresult($result);

	// Return the data.
	return $data_array;
}

// Function:	qm_subquery()
// What it does:	Report on character status for a particular quest.
// Requires:
//		$_GET['timeline'] -- quest ID
//		$_GET['sco'] -- schow claimed characters only
function qm_subquery() {
	global $qm, $mod_dirname, $userinfo, $db, $prefix, $user_prefix, $suffix, $pagetitle, $exclude_alts, $config, $Timelines, $QuestsData;
	
	// Get the quest id from the URL, ensure it's valid.
	$timeline = (isset($_GET['timeline'])) ? (string)$_GET['timeline'] : '';
	if (!isset($Timelines[$qm][$timeline])) {
		// Not a quest.
		cpg_error(_ERR_ACCESSCONT);
	}
	// Get option to show Claimed characters only.
	$sco = (empty($_GET['sco'])) ? ((isset($_POST['sco'])) ? (bool)$_POST['sco'] : FALSE) : (bool)$_GET['sco'];
	
	$pagetitle .= ' '._BC_DELIM.' '.constant('_TRACKER'.strtoupper($qm)).' '._BC_DELIM.' '.$Timelines[$qm][$timeline]['name'];

	require('header.php');
	OpenTable();

	// Display the commands.
	do_commands();

	$cpgtpl->assign_vars(array(
		'U_QUEST' => $Timelines[$qm][$timeline]['quest_url'],
		'S_TARGET' => (($config['popup']) ? ' target="_blank"' : ''),
		'S_QUEST' => $timeline,
		'S_QUEST_NAME' => $Timelines[$qm][$timeline]['name'],
		'U_QUEST_REWARD_IMG' => "modules/{$mod_dirname}/images/quests/".(($qm == 'key') ? 'key' : $timeline).'.jpg',
		'U_QUEST_REWARD' => $Timelines[$qm][$timeline]['reward_url'],
		'S_QUEST_REWARD' => $Timelines[$qm][$timeline]['reward'],
	));

	// Retrieve character quest status data.
	$roster_data = read_char_quest_status_db($qm, $sco, $exclude_alts, 'rm.type_level DESC, rm.name_first');
//	cpg_error('<pre style="text-align:left">'.count($roster_data)." elements parsed.\n\n".htmlspecialchars(print_r($roster_data, TRUE))."</pre>\n");

	// Loop through each character.
	foreach($roster_data as $characterId => $char_info) {
		if (empty($char_info['quest_list'][$timeline])) {
			// Timeline not started.
			$cpgtpl->assign_block_vars('not_started', array(
				'U_CHAR_PROFILE' => $config['char_url'].$characterId,
				'S_CHAR_NAME_FIRST' => $char_info['name_first'],
				'S_CHAR_ADV_LEVEL' => $char_info['type_level']
			));
		} elseif ($char_info['quest_list'][$timeline]['tl_complete'] == TRUE) {
			// Timeline completed.
			$cpgtpl->assign_block_vars('completed', array(
				'U_CHAR_PROFILE' => $config['char_url'].$characterId,
				'S_CHAR_NAME_FIRST' => $char_info['name_first'],
				'S_CHAR_ADV_LEVEL' => $char_info['type_level']
			));
		}
	}
	unset($roster_data);

	// Timeline in progress.
	foreach($Timelines[$qm][$timeline]['steps'] as $crc => $quest) {
		$cpgtpl->assign_block_vars('in_progress', array(
			'S_STEP' => _STEP.' '.(string)($quest['step'] + 1).': '.$quest['name'],
		));

		$query = "SELECT rm.characterId, rm.name_first, rm.type_level, rmu.user_id, rmu.char_type, qs.crc, qs.stage_num, qs.completion_date
			FROM {$prefix}_roster_master_quest_status{$suffix} AS qs
			INNER JOIN {$prefix}_roster_master{$suffix} AS rm USING (characterId)
			LEFT JOIN {$prefix}_roster_master_users{$suffix} AS rmu USING (characterId)
			WHERE qs.crc='".(string)$crc."' AND qs.stage_num<8000"
			.(($exclude_alts) ? " AND rmu.char_type='Primary'" : '')
			.(($sco) ? ' AND rmu.user_id='.(int)$userinfo['user_id'] : '')
			." ORDER BY qs.stage_num, rm.type_level DESC, rm.name_first";
		trigger_error("\$query = {$query}", E_USER_NOTICE);
		$result = $db->sql_query($query, FALSE);

		// Read the data into an array.
		$stage = 0;
		$not_first = FALSE;
		while($char_info = $db->sql_fetchrow($result, SQL_ASSOC)) {
//			cpg_error('<pre style="text-align:left">'.count($char_info)." elements parsed.\n\n".htmlspecialchars(print_r($char_info, TRUE))."</pre>\n");
			$cpgtpl->assign_block_vars('in_progress.column', array(
				'B_SEPARATOR' => (($not_first && $char_info['stage_num'] > $stage) ? TRUE : FALSE),
				'U_CHAR_PROFILE' => $config['char_url'].$char_info['characterId'],
				'S_CHAR_NAME_FIRST' => $char_info['name_first'],
				'S_CHAR_ADV_LEVEL' => $char_info['type_level'],
				'S_CHAR_QUEST_STAGE' => '('._STAGE.' '.(string)($char_info['stage_num'] + 1).') '.$QuestsData[$crc]['stage_list'][$char_info['stage_num']]['description']
			));
			$stage = $char_info['stage_num'];
			$not_first = TRUE;
		}
		$db->sql_freeresult($result);
	}

	$cpgtpl->set_filenames(array('qm_subquery' => 'roster_master/qm_subquery.html'));
	$cpgtpl->display('qm_subquery');

	CloseTable();
}

// Function:	questmaster()
// What it does:	Reads, sorts, and displays the quest master list.
// Requires:
//		$_GET or $_POST['s1'] -- sort
//		$_GET or $_POST['s1d'] -- sort direction
//		$_GET or $_POST['xs'] -- show selected expansions
//		$_GET or $_POST['sco'] -- show claimed characters only
function questmaster() {
	// Start function duration timer.
	$timer_t = get_microtime(TRUE);

	global $qm, $baselink, $db, $prefix, $user_prefix, $suffix, $pagetitle, $bgcolor2, $exclude_alts, $module_id, $mod_dirname, $userinfo, $config, $ProgressOptions, $Expansions, $Timelines, $QuestsData;

	$pagetitle .= ' '._BC_DELIM.' '.constant('_TRACKER'.strtoupper($qm));

	// Grab the sort values from the server environment.
	$s1 = (empty($_GET['s1'])) ? ((isset($_POST['s1'])) ? $_POST['s1'] : '') : $_GET['s1'];
	$s1d = (empty($_GET['s1d'])) ? ((isset($_POST['s1d'])) ? $_POST['s1d'] : '') : $_GET['s1d'];
	$xs = ((empty($_GET['xs']))
		? ((isset($_POST['xs'])) ? $_POST['xs'] : array_keys($Expansions))
		: array_keys(str_split(str_pad(base_convert($_GET['xs'], 16, 2), count($Expansions), '0', STR_PAD_LEFT), 1), 1)
	);
	$sco = (empty($_GET['sco'])) ? ((isset($_POST['sco'])) ? (bool)$_POST['sco'] : FALSE) : (bool)$_GET['sco'];

	// SORT CODE
	// Default the search.
	if (($s1 != 'name_first') && ($s1 != 'type_level') && empty($Timelines[$qm][$s1])) { $s1 = 'name_first'; } // name
	if ($s1d != 'd') { $s1d = 'a'; } // ascending

	// Construct the sort options list. NOTE: be aware of hardcoded field names in selector list when changing schema.
	$sort_options_pri = '<option value="name"'.(($s1 == 'name_first') ? ' selected="selected"' : '').'>'._NAME.'</option>';
	$sort_options_pri .= '<option value="type_level"'.(($s1 == 'type_level') ? ' selected="selected"' : '').'>'._ADVLEVEL.'</option>';
	foreach(array_keys($Timelines[$qm]) as $quest) {
		if (in_array($Timelines[$qm][$quest]['expansion'], array_values($xs))) {
			$sort_options_pri .= "<option value=\"{$quest}\"".(($quest == $s1) ? ' selected="selected"' : '').">{$Timelines[$qm][$quest]['reward']}</option>";
		}
	}

	// Construct the checkbox list for Expansions and binary string used for relinking.
	$xs_r = '';
	foreach($Expansions as $k => $v) {
		$checked = in_array($k, $xs);
		$checkboxes[] = "<label title=\"{$v['name']}\">{$v['shortname']}&nbsp;<input type=\"checkbox\" value=\"{$k}\""
			.(($checked) ? ' checked="checked"' : '')
			.' name="xs[]" style="border-style:none;bottom:1px;position:relative;vertical-align:middle" /></label>';
		$xs_r .= (($checked) ? '1' : '0');
	}

	require('header.php');
	OpenTable();

	// Display the commands.
	do_commands();

	// Include external function modules containing the table JavaScript.
	if ($config['qm_scrolltable'] || $config['qm_rowhighlight']) {
		include("modules/{$mod_dirname}/includes/table_js.inc");
	}

	// Reconstruct page URL link for re-sort and option flips.
	$relink = '';
	$relink = $baselink
		."&rm=qm_display&qm={$qm}&s1={$s1}&s1d={$s1d}"
		.'&xs='.base_convert($xs_r, 2, 16)
		.((empty($sco)) ? '' : '&sco=1')
		.((($config['show_alts'] == 0) && !$exclude_alts) ? '&show_all=1' : '');
//	cpg_error("<pre style=\"text-align:left\">\$baselink = ".htmlspecialchars($baselink)."\n\$s1 = {$s1}\n\$s1d = {$s1d}\n\$xs = ".print_r($xs, TRUE)."\$xs_r = {$xs_r}\n\$sco = {$sco}\n\$relink = {$relink}</pre>\n");

	// Start populating template variables.
	$cpgtpl->assign_vars(array(
		'I_MID' => $module_id,
		'S_ROWHIGHLIGHT' => " onmouseover=\"mid{$module_id}_trackTableHighlight(event, this, '{$bgcolor2}', false)\""
			." onmouseout=\"mid{$module_id}_highlightTableRow(0)\""
			." onclick=\"mid{$module_id}_trackTableHighlight(event, this, '{$bgcolor2}', true)\"",
		'S_TARGET' => (($config['popup']) ? ' target="_blank"' : ''),
		'S1_SORT_FIELD' => $s1,
		'U_S1_SORT_SWAP' => getlink((strpos($relink, 's1d=a')) ? str_replace('&s1d=a', '&s1d=d', $relink) : str_replace('&s1d=d', '&s1d=a', $relink)),
		'S1_SORT_INDICATOR' => (($s1d == 'a') ? '&uarr;' : '&darr;'),
		'S_HRSTYLE' => (($config['qm_scrolltable']) ? 'cursor:move;' : ''),
		'S_HRRESIZE' => (($config['qm_scrolltable']) ? " onmousedown=\"mid{$module_id}_getPosition(event); return false;\"" : ''),
		'S1_EXPANSION_OPTIONS' => implode('&nbsp;&nbsp;', $checkboxes),
		'S1_SORT_OPTIONS' => $sort_options_pri,
		'U_QUEST_DISPLAY' => getlink("{$mod_dirname}&rm=qm_display&qm={$qm}".((($config['show_alts'] == 0) && !$exclude_alts) ? '&show_all=1' : '')),
		'S1_A_CHECKED' => (($s1d != 'd') ? ' checked="checked"' : ''),
		'S1_D_CHECKED' => (($s1d == 'd') ? ' checked="checked"' : ''),
		'SCO_CHECKED' => (($sco) ? ' checked="checked"' : ''),
		'S_SCROLLTABLE' => "<script type=\"text/javascript\">\n"
			."<!-- Hide script from old browsers.\n"
			."  // Wrap the onunload method to keep compatibility with other scripts\n"
			."  // that may be running on the page.\n"
			."  var mid{$module_id}_oldOnLoad = (typeof window.onload == \"function\") ? window.onload : function() {};\n"
			."  window.onload = function() { mid{$module_id}_oldOnLoad(); mid{$module_id}_makeScrollableTable(\"QuestMaster\"); };\n"
			."// End -->\n"
			."</script>\n"
	));

	// Add page element to flip between full list and Primary only.
	if ($config['show_alts'] == 0) {
		if ($exclude_alts) {
			// Link to full list (show_all=1).
			$cpgtpl->assign_vars(array(
				'U_TOGGLE_ALTS' => getlink((strpos($relink, 'show_all=')) ? str_replace('&show_all=0', '&show_all=1', $relink) : $relink.'&show_all=1'),
				'S_ALTS_SHOWING' => _SHOWPRI,
				'S_ALTS_TOGGLE' => _DISPALL
			));
		} else {
			// Link to short list (show_all=0 or '').
			$cpgtpl->assign_vars(array(
				'U_TOGGLE_ALTS' => getlink((strpos($relink, 'show_all=')) ? str_replace('&show_all=1', '', $relink) : $relink),
				'S_ALTS_SHOWING' => _SHOWALL,
				'S_ALTS_TOGGLE' => _DISPPRI
			));
		}
	} else {
		// Link to short list (show_all=0 or '').
		$cpgtpl->assign_vars(array(
			'U_TOGGLE_ALTS' => '',
		));
	}

	// Display row headers.
	$i = 1;
	foreach(array_keys($Timelines[$qm]) as $timeline) {
		if (in_array($Timelines[$qm][$timeline]['expansion'], $xs)) {
			$cpgtpl->assign_block_vars('header1', array(
				'S_COLCLASS' => (($i++ < count($Timelines[$qm])) ? 'thTop' : 'thCornerR'),
				'U_QUEST_REWARD_IMG' => "modules/{$mod_dirname}/images/quests/".(($qm == 'key') ? 'key' : $timeline).'.jpg',
				'U_QUEST_REWARD' => $Timelines[$qm][$timeline]['reward_url'],
				'S_QUEST_REWARD' => $Timelines[$qm][$timeline]['reward']
			));
			$cpgtpl->assign_block_vars('header2', array(
				'S_QUEST' => ((empty($Timelines[$qm][$timeline]['display'])) ? $timeline : $Timelines[$qm][$timeline]['display']),
				'U_QUEST' => $Timelines[$qm][$timeline]['quest_url'],
				'S_QUEST_NAME' => $Timelines[$qm][$timeline]['name'],
			));
			$cpgtpl->assign_block_vars('header3', array(
				'U_SUBQUERY' => getlink("{$baselink}&rm=qm_subquery&qm={$qm}&timeline={$timeline}".((empty($sco)) ? '' : '&sco=1').((($config['show_alts'] == 0) && !$exclude_alts) ? '&show_all=1' : '')),
				'S_QUEST_NAME' => $Timelines[$qm][$timeline]['name']
			));
		}
	}

	// Retrieve character quest status data.
	if (($s1 == 'name_first') || ($s1 == 'type_level')) {
		// Sort by name or level.
		$roster_data = read_char_quest_status_db($qm, $sco, $exclude_alts, 'rm.'.$s1.(($s1d == 'd') ? ' DESC' : ' ASC'));
	} else {
		// Sort by timeline.
		$roster_data = read_char_quest_status_db($qm, $sco, $exclude_alts, 'rm.type_level DESC, rm.name_first');

		// Obtain a list of columns.
		$in_step = array();
		$stage_num = array();
		foreach ($roster_data as $key => $row) {
			if (isset($row['quest_list'][$s1])) {
				$in_step[$key] = $row['quest_list'][$s1]['in_step'];
				$stage_num[$key] = $row['quest_list'][$s1]['stage_num'];
			} else {
				$in_step[$key] = -1;
				$stage_num[$key] = -1;
			}
		}
		// Sort and reindex.
		$sort_order = (($s1d == 'd') ? SORT_DESC : SORT_ASC);
		array_multisort($in_step, $sort_order, $stage_num, $sort_order, $roster_data);
	}
//	cpg_error('<pre style="text-align:left">'.count($roster_data)." elements parsed.\n\n".htmlspecialchars(print_r($roster_data, TRUE))."</pre>\n");

	// Display Rows of data.
	if (count($roster_data) > 0) {
		// Loop through each character.
		$i = 0;
		foreach($roster_data as $characterId => $char_info) {
			$cpgtpl->assign_block_vars('character_row', array(
				'B_NONE' => FALSE,
				'S_ROWCLASS' => ((!($i++ % 2)) ? 'row1' : 'row2'),
				'U_CHAR_PROFILE' => ((empty($characterId)) ? '' : $config['char_url'].$characterId),
				'S_CHAR_NAME_FIRST' => $char_info['name_first'],
				'S_CHAR_ADV_LEVEL' => $char_info['type_level'],
			));
			// Loop through each timeline.
			foreach(array_keys($Timelines[$qm]) as $timeline) {
				// Iterate only over selected expansions.
				if (in_array($Timelines[$qm][$timeline]['expansion'], $xs)) {
					if (empty($char_info['quest_list'][$timeline])) {
						// Timeline not started.
						$cpgtpl->assign_block_vars('character_row.quest_column', array(
							'S_BACKGROUNDCOLOR' => ((($s1 != 'name_first') && ($s1 != 'type_level') && ($s1 == $timeline)) ? ";background-color:{$config['rowhighlight_color']}" : ''),
							'I_CHAR_QUEST_STEP' => -1
						));
					} elseif ($char_info['quest_list'][$timeline]['tl_complete'] == TRUE) {
							// Timeline completed.
							$cpgtpl->assign_block_vars('character_row.quest_column', array(
								'S_BACKGROUNDCOLOR' => ((($s1 != 'name_first') && ($s1 != 'type_level') && ($s1 == $timeline)) ? ";background-color:{$config['rowhighlight_color']}" : ''),
								'I_CHAR_QUEST_STEP' => $char_info['quest_list'][$timeline]['in_step'] + 1,
								'B_TL_COMPLETE' => TRUE,
								'S_PROGRESS_IMG' => $ProgressOptions[2]['icon'],
								'S_QUEST' => $QuestsData[$char_info['quest_list'][$timeline]['crc']]['name'],
								'S_PROGRESS_TITLE' => $ProgressOptions[2]['label']
							));
					} else {
						// Timeline in progress.
						$cpgtpl->assign_block_vars('character_row.quest_column', array(
							'S_BACKGROUNDCOLOR' => ((($s1 != 'name_first') && ($s1 != 'type_level') && ($s1 == $timeline)) ? ";background-color:{$config['rowhighlight_color']}" : ''),
							'I_CHAR_QUEST_STEP' => $char_info['quest_list'][$timeline]['in_step'] + 1,
							'I_CHAR_QUEST_STAGE' => $char_info['quest_list'][$timeline]['stage_num'] + 1,
							'B_TL_COMPLETE' => FALSE,
							'S_PROGRESS_IMG' => $ProgressOptions[1]['icon'],
							'S_QUEST' => $QuestsData[$char_info['quest_list'][$timeline]['crc']]['name'],
							'S_PROGRESS_TITLE' => $ProgressOptions[1]['label']
						));
					}
				}
			}
		}
	} else {
		$cpgtpl->assign_block_vars('character_row', array(
			'B_NONE' => TRUE,
			'S_COLSPAN' => strval(count($Timelines[$qm]) + 1)
		));
	}

	// Display table legend.
	for ($i = count($ProgressOptions) - 1; $i >= 0; $i--) {
		$cpgtpl->assign_block_vars('table_legend', array(
			'S_PROGRESS_IMG' => $ProgressOptions[$i]['icon'],
			'S_PROGRESS_TITLE' => $ProgressOptions[$i]['label']
		));
	}

	$cpgtpl->set_filenames(array('questmaster' => 'roster_master/questmaster.html'));
	$cpgtpl->display('questmaster');

	CloseTable();

	// Log the performance statistics.
	$error = sprintf(_NTC_RSTRDISPLAY, "questmaster(), {$qm}", (get_microtime(TRUE) - $timer_t));
	trigger_error($error, E_USER_NOTICE);
	if ($config['logfile'] & 64) { $GLOBALS['log'][] = $error; }
}

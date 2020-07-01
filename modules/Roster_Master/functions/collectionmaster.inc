<?php 
/***********************************************************************
  Roster Master for Dragonfly(TM) CMS
  **********************************************************************
  Copyright (C) 2005-2020 by Dark Grue

  Collection Master management module. Tracks Collection Quests for
  roster characters.

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
// Load collection quest data.
require_once("modules/{$mod_dirname}/includes/collections_data.inc");


// ***************************** FUNCTIONS *****************************
// Function:	read_char_collection_status_db()
// What it does:	Reads the contents of the quest status database into an consolidated array of character data,
//			which it returns.
function read_char_collection_status_db($characterId) {
	global $userinfo, $db, $prefix, $user_prefix, $suffix, $CollectionsData;

	$data_array = array();

	// Fetch all of the data from the tables.
	$query = "SELECT * FROM {$prefix}_roster_master_collection_status{$suffix} WHERE characterId=".(int)$characterId;
//	trigger_error("\$query = {$query}", E_USER_NOTICE);
	$result = $db->sql_query($query, FALSE);

	// Read the data into an array.
	while($collection_status = $db->sql_fetchrow($result, SQL_ASSOC)) {
//		cpg_error('<pre style="text-align:left">'.htmlspecialchars(print_r($collection_status, TRUE))."</pre>\n");
		$data_array[$collection_status['crc']] = explode(',', $collection_status['item_list']);
	}
	$db->sql_freeresult($result);

	// Return the data.
	return $data_array;
}

// Function:	collectionmaster()
// What it does:	Reads, sorts, and displays the character collection quest master list.
// Requires:
//		$_GET or $_POST['characterId'] -- character ID
//		$_GET or $_POST['s1'] -- sort
//		$_GET or $_POST['s1d'] -- sort direction
//		$_GET or $_POST['siq'] -- show inactive quests
//		$_GET or $_POST['scq'] -- show completed quests
function collectionmaster() {
	// Start function duration timer.
	$timer_t = get_microtime(TRUE);

	global $baselink, $db, $prefix, $user_prefix, $suffix, $pagetitle, $bgcolor2, $module_id, $mod_dirname, $userinfo, $config, $CollectionsData;

	$pagetitle .= ' '._BC_DELIM.' '._TRACKERCOLLECT;

	// Grab the sort values from the server environment.
	$characterId = (empty($_GET['characterId'])) ? ((isset($_POST['characterId'])) ? intval($_POST['characterId']) : '') : intval($_GET['characterId']);
	$s1 = (empty($_GET['s1'])) ? ((isset($_POST['s1'])) ? $_POST['s1'] : '') : $_GET['s1'];
	$s1d = (empty($_GET['s1d'])) ? ((isset($_POST['s1d'])) ? $_POST['s1d'] : '') : $_GET['s1d'];
	$siq = (empty($_GET['siq'])) ? ((isset($_POST['siq'])) ? (bool)$_POST['siq'] : FALSE) : (bool)$_GET['siq'];
	$scq = (empty($_GET['scq'])) ? ((isset($_POST['scq'])) ? (bool)$_POST['scq'] : FALSE) : (bool)$_GET['scq'];

	// Check for non-existent character.
	$result = $db->sql_query("SELECT * FROM {$prefix}_roster_master{$suffix} WHERE characterId=".(int)$characterId, FALSE);
	if (!$db->sql_numrows($result)) {
		cpg_error('<strong>'._ERR_CLAIMREADFAIL."</strong><br /><br />\n"
			."Character ID '$characterId' does not exist.", _ERROR, ((empty($_SERVER['HTTP_REFERER'])) ? getlink($baselink) : $_SERVER['HTTP_REFERER']));
	}
	$db->sql_freeresult($result);

	// SORT CODE
	// Default the search.
	if (($s1 != 'name') && ($s1 != 'category') && ($s1 != 'level') && ($s1 != 'count')) { $s1 = 'name'; } // name
	if ($s1d != 'd') { $s1d = 'a'; } // ascending

	// Construct the sort options list. NOTE: be aware of hardcoded field names in selector list when changing schema.
	$sort_options_pri = '<option value="name"'.(($s1 == 'name') ? ' selected="selected"' : '').'>'._NAME.'</option>';
	$sort_options_pri .= '<option value="category"'.(($s1 == 'category') ? ' selected="selected"' : '').'>'._CATEGORY.'</option>';
	$sort_options_pri .= '<option value="level"'.(($s1 == 'level') ? ' selected="selected"' : '').'>'._LEVEL.'</option>';
	$sort_options_pri .= '<option value="count"'.(($s1 == 'count') ? ' selected="selected"' : '').'>'._COUNT.'</option>';

	require('header.php');
	OpenTable();

	// Display the commands.
	do_commands();

	list($name_first, $name_last, $type_level, $type_class, $collections_active, $collections_complete) =
		$db->sql_ufetchrow("SELECT name_first, name_last, type_level, type_class, collections_active, collections_complete
		FROM {$prefix}_roster_master{$suffix}
		WHERE characterId=".(int)$characterId
		.' LIMIT 1', SQL_NUM);

	// Start populating template variables.
	$cpgtpl->assign_vars(array(
		'I_MID' => $module_id,
		'U_ROSTER_DISPLAY' => getlink($mod_dirname."&rm=cm_display&characterId={$characterId}"),
		'S1_SORT_FIELD' => $s1,
		'S1_SORT_OPTIONS' => $sort_options_pri,
		'S1_A_CHECKED' => (($s1d != 'd') ? ' checked="checked"' : ''),
		'S1_D_CHECKED' => (($s1d == 'd') ? ' checked="checked"' : ''),
		'SIQ_CHECKED' => (($siq) ? ' checked="checked"' : ''),
		'SCQ_CHECKED' => (($scq) ? ' checked="checked"' : ''),
		'U_CHAR_PROFILE' => $config['char_url'].$characterId,
		'S_TARGET' => (($config['popup']) ? ' target="_blank"' : ''),
		'S_CHAR_NAME_FIRST' => $name_first,
		'S_CHAR_NAME_LAST' => ((empty($name_last)) ? '' : " {$name_last}"),
		'S_CHAR_ADV_LEVEL' => $type_level,
		'S_CHAR_CLASS' => $type_class,
		'S_COLLECTIONS_ACTIVE' => $collections_active,
		'S_COLLECTIONS_COMPLETE' => $collections_complete
	));

	// Obtain a list of columns.
	$category = array();
	$name = array();
	$level = array();
	foreach ($CollectionsData as $key => $row) {
		$category[$key] = $row['category'];
		$name[$key] = $row['name'];
		$level[$key] = $row['level'];
	}

	// Get character collection data.
	$char_info = read_char_collection_status_db($characterId);
//	cpg_error('<pre style="text-align:left">'.count($char_info)." elements parsed.\n\n".htmlspecialchars(print_r($char_info, TRUE))."</pre>\n");

	// Sort and re-index.
	$sort_order = (($s1d == 'd') ? SORT_DESC : SORT_ASC);
	if ($s1 == 'name') { array_multisort($name, $sort_order, SORT_NATURAL, $CollectionsData); }
	elseif ($s1 == 'category') { array_multisort($category, $sort_order, $name, SORT_ASC, $level, SORT_NATURAL, $CollectionsData); }
	elseif ($s1 == 'level') { array_multisort($level, $sort_order, $name, SORT_ASC, SORT_NATURAL, $CollectionsData); }
	elseif ($s1 == 'count') {
		// Build index from completion count.
		foreach ($CollectionsData as $key => $quest) {
			$CollectionsCount[$key] = ((empty($char_info[$key])) ? 0 : count($char_info[$key]));
		}
		array_multisort($CollectionsCount, $sort_order, $name, SORT_ASC, SORT_NATURAL, $CollectionsData);
		unset($CollectionsCount);
	}

//	cpg_error('<pre style="text-align:left">'.count($CollectionsCount)." elements parsed.\n\n".htmlspecialchars(print_r($CollectionsCount, TRUE))."</pre>\n");
//	cpg_error('<pre style="text-align:left">'.count($CollectionsData)." elements parsed.\n\n".htmlspecialchars(print_r($CollectionsData, TRUE))."</pre>\n");

	// Display Rows of data.
	$i = 0;
	foreach ($CollectionsData as $quest) {
		$completed = (empty($char_info[$quest['crc']])) ? FALSE : array_equal($char_info[$quest['crc']], array_keys($quest['reference_list']), FALSE);
/*
		if (count($char_info[$quest['crc']]) == count(array_keys($quest['reference_list']))) {
			cpg_error('<pre style="text-align:left">'
				.count($quest).' $quest elements parsed, '.count($quest['reference_list'])." \$quest['reference_list'] elements parsed.\n"
				.'$quest = '.htmlspecialchars(print_r($quest, TRUE))."\n\n"
				.count($quest['reference_list'])." elements parsed.\narray_keys(\$quest['reference_list']) = ".htmlspecialchars(var_dump_r(array_values(array_keys($quest['reference_list']))))."\n\n"
				.count($char_info[$quest['crc']])." elements parsed.\n\$char_info[\$quest['crc'] = ".htmlspecialchars(var_dump_r($char_info[$quest['crc']]))
				."\n\n\$completed = ".(int)$completed."</pre>\n");
		}
*/
		if ((empty($char_info[$quest['crc']]) && $siq) || (!empty($char_info[$quest['crc']]) && !$completed) || ($completed && $scq)) {
			$cpgtpl->assign_block_vars('collection_row', array(
				'S_ROWCLASS' => ((!($i++ % 2)) ? 'row1' : 'row2'),
				'U_COLLECTION' => 'http://eq2.wikia.com/wiki/'.rawurlencode($quest['name']),
				'S_CATEGORY' => $quest['category'],
				'S_NAME' => $quest['name'],
				'B_ISCLASS' => $quest['isclass'],
				'B_ISMETA' => $quest['ismeta'],
				'S_LEVEL' => $quest['level'],
				'S_COUNT' => ((empty($char_info[$quest['crc']])) ? 0 : count($char_info[$quest['crc']])),
				'S_TOTAL' => count($quest['reference_list'])
			));
			foreach ($quest['reference_list'] as $collection => $item) {
/*
				cpg_error('<pre style="text-align:left">'
					."\$collection = {$collection}\n\n".count($item)." elements parsed.\n\n\$item = ".htmlspecialchars(print_r($item, TRUE))
					.count($char_info[$quest['crc']])." elements parsed.\n\n\$char_info[\$quest['crc']] = ".htmlspecialchars(print_r($char_info[$quest['crc']], TRUE))
					."</pre>\n");
*/
				$cpgtpl->assign_block_vars('collection_row.item_column', array(
					'B_HAS' => ((empty($char_info[$quest['crc']])) ? FALSE : in_array($collection, $char_info[$quest['crc']])),
					'U_ITEM' => 'http://eq2.wikia.com/wiki/'.rawurlencode($item['name']),
					'U_ITEM_IMG' => ((file_exists("modules/{$mod_dirname}/images/items/{$item['icon']}.png")) ? "modules/{$mod_dirname}/images/items/{$item['icon']}.png" : "modules/{$mod_dirname}/images/blankitem.png"),
					'S_NAME' => $item['name']
				));
			}
		}
//		cpg_error('<pre style="text-align:left">'.htmlspecialchars(print_r($cpgtpl->_tpldata['collection_row'], TRUE))."</pre>\n");
	}

	$cpgtpl->assign_vars(array(
		'S_SHOWING' => $i,
		'S_TOTAL' => $collections_active + $collections_complete,
		'S_DONTHAVE' => count($CollectionsData) - ($collections_active + $collections_complete),
		'S_CENSUS_TOTAL' => count($CollectionsData)
	));

	$cpgtpl->set_filenames(array('cm_display' => 'roster_master/collectionmaster.html'));
	$cpgtpl->display('cm_display');
//	cpg_error('<pre style="text-align:left">'.count($cpgtpl)." elements parsed.\n\n".htmlspecialchars(print_r($cpgtpl, TRUE))."</pre>\n");

	CloseTable();

	// Log the performance statistics.
	$error = sprintf(_NTC_RSTRDISPLAY, 'cm_display()', (get_microtime(TRUE) - $timer_t));
	trigger_error($error, E_USER_NOTICE);
	if ($config['logfile'] & 64) { $GLOBALS['log'][] = $error; }
}

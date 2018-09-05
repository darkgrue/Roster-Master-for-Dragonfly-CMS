<?php
/* ********************************************
  Guild_Event_Manager
  ********************************************
  Copyright (C) 2006 by Stremok

  Support for this module can be found at:
  http://www.eq2caladrius.com/
  
  You must be running the current version of Roster
  Master for Dragonfly by DarkGrue in order for this
  module to behave properly.
  
  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  GEM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
******************************************** */


// Error message for people trying to access pages they aren't allowed into.
function EditGroups($eid) {
	global $db, $prefix, $userinfo, $gemAdminID, $bgcolor1, $bgcolor2, $bgcolor3;
	if ($_POST['none']) {
		url_redirect(getlink("&pg=15&eid=$eid"), true);
	} elseif ($_POST['save'] || $_POST['more']) {
		$newGroup = floor(Fix_Quotes($_POST['groupslot']) / 10);
		$newSlot = (Fix_Quotes($_POST['groupslot']) - ($newGroup * 10));
		if($db->sql_numrows("SELECT * FROM ".$prefix."_GEM_outsiders WHERE gemOutID = '".Fix_Quotes($_POST['charid'])."' AND gemEventID = '".$eid."'")){
			$outuserid = $db->sql_query("SELECT * FROM ".$prefix."_GEM_outsiders WHERE gemOutID = '".Fix_Quotes($_POST['charid'])."' AND gemEventID = '".$eid."'");
			$db->sql_query("UPDATE ".$prefix."_GEM_signups
							SET gemGroupID = '".$newGroup."', gemSlotID = '".$newSlot."'
							WHERE gemCharID = '0' AND gemEventID = '".$eid."' AND gemUserID = '".$outuserid['gemUserID']."'");
		} else {
			$db->sql_query("UPDATE ".$prefix."_GEM_signups
							SET gemGroupID = '".$newGroup."', gemSlotID = '".$newSlot."'
							WHERE gemCharID = '".Fix_Quotes($_POST['charid'])."' AND gemEventID = '".$eid."'");
		}
		switch (true) {
			case ($_POST['more']):
				url_redirect(getlink("&pg=17&eid=$eid"), true);
				break;
			case ($_POST['save']):
				url_redirect(getlink("&pg=15&eid=$eid"), true);
				break;
		}
	}
	$SignupArray = SignupData($eid, _GEM_GROUP);
	$sqlGetEvent = $db->sql_query("SELECT * FROM ".$prefix."_GEM_events WHERE gemEventID = '".$eid."'");
	$EventInfo = $db->sql_fetchrow($sqlGetEvent);
	$Participants = $SignupArray[1];
	$UsedSlots = $SignupArray[2];
	switch ($EventInfo['gemMaxChars']) {
		case '6':
			$FullPosList = array(11,12,13,14,15,16);
			break;
		case '12':
			$FullPosList = array(11,12,13,14,15,16,21,22,23,24,25,26);
			break;
		case '18':
			$FullPosList = array(11,12,13,14,15,16,21,22,23,24,25,26,31,32,33,34,35,36);
			break;
		case '24':
			$FullPosList = array(11,12,13,14,15,16,21,22,23,24,25,26,31,32,33,34,35,36,41,42,43,44,45,46);
			break;
	}
	foreach($FullPosList as $slot) {
		if ((sizeof($UsedSlots) == 0) || !in_array($slot, $UsedSlots)) {
			$Available[] = $slot;
		}
	}
	$numChar = 0;
	foreach($Participants as $char) {
		$sql_out_disp_chars = "SELECT * FROM ".$prefix."_GEM_outsiders WHERE gemOutID = '".$char."' AND gemEventID = '".$eid."'";
		$sql_rm_disp_chars = "SELECT * FROM ".$prefix."_roster_master WHERE characterId = '".$char."'";
		if ($db->sql_numrows($sql_out_disp_chars)) {
			$out_disp_chars = $db->sql_query($sql_out_disp_chars);
			while($row = $db->sql_fetchrow($out_disp_chars)) {	
				$char_name = $row['gemCharName'];
				$char_level = $row['gemCharLevel'];
				$char_class = $row['gemCharClass'];
				$char_id = $row['gemOutID'];
				$charData[] = array($char_name, $char_level, $char_class, $char_id);
				$numChar++;
			}
		} else {
			$rm_disp_chars = $db->sql_query($sql_rm_disp_chars);
			while($row = $db->sql_fetchrow($rm_disp_chars)) {	
				$char_name = $row['name_first'];
				$char_level = $row['type_level'];
				$char_class = $row['type_class'];
				$char_id = $row['characterId'];
				$charData[] = array($char_name, $char_level, $char_class, $char_id);
				$numChar++;
			}		
		}
	}
	$buttonArray = array('more'=>_GEM_SAVE_CHANGES2, 'save'=>_GEM_SAVE_CHANGES, 'none'=>_GEM_NO_CHANGES);
	OpenTable();
	echo '
		<table width="100%">
			<tr>
				<th>'.$EventInfo['gemTitle'].' :: '._GEM_GROUP_SETUP.'</th>
			</tr>
			<tr>
				<td align="center">
					'.$SignupArray[0].'
				</td>
			</tr>
			<tr>
				<td><br /><br /><hr /><br /></td>
			</tr>
			<tr>
				<td align="center">
					<form method="post" action="'.htmlprepare(get_uri()).'" style="padding: 0px; margin: 0px;">
						<select name="charid">';
	foreach($charData as $value) {
		echo '				<option value="'.$value[3].'">'.$value[0].' '._GEM_A_LEVEL.' '.$value[1].' '.$value[2].'</option>';
	}
	echo '				</select>
						<select name="groupslot">';
	echo '				<option value="57">'._GEM_GROUP5.'</option>';
	foreach($Available as $value) {
		$groupID = floor($value / 10);
		$slotID = ($value - ($groupID * 10));
		echo '				<option value="'.$value.'">'.sprintf(_GEM_GROUP_SLOT, $groupID, $slotID).'</option>';
	}	
	echo '				</select><br /><br />';
	foreach($buttonArray as $key=>$value) {
		echo '					<input type="submit" name="'.$key.'" value="'.$value.'" class="liteoption" />';	
	}
	echo '
					</form>
				</td>
			</tr>
		</table>';
	Footer();
	CloseTable();
}

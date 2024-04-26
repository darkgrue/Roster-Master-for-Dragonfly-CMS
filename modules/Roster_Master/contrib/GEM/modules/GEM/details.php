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


// Page to display event information
function EventDetails($eid) {
	global $userinfo, $db, $prefix, $module_name, $CurUserGroup, $gemCreateID, $gemOutsiderOpt, $gemAdminID, $gemCommentID, $gemSignupID, $gemDKPOpt, $gemPubClass, $gemDefList, $gemAutoslots, $bgcolor1, $bgcolor2, $bgcolor3, $l10n_gmt_regions;
	$now = gmtime();
	$gemDefList = ($gemDefList == 1) ? _GEM_GROUP : _GEM_CLASS;
	if (Fix_Quotes($_POST['signuptype']) == _GEM_CLASS || Fix_Quotes($_POST['signuptype']) == _GEM_GROUP) {
		$SignupType = Fix_Quotes($_POST['signuptype']);
	} else {
		$SignupType = $gemDefList;
	}
	$SignupChoice = ($SignupType == _GEM_CLASS) ? _GEM_GROUP : _GEM_CLASS;
	$sqlGetEvent = $db->sql_query("SELECT * FROM ".$prefix."_GEM_events WHERE gemEventID = '".$eid."'");
	$EventInfo = $db->sql_fetchrow($sqlGetEvent);
	$gemOutStatus = ($EventInfo['gemOutSign'] == 1) ? _YES : _NO;
	$sqlGetCat = $db->sql_query("SELECT * FROM ".$prefix."_GEM_cats WHERE gemCatTitle = '".$EventInfo['gemEventType']."'");
	$CatInfo = $db->sql_fetchrow($sqlGetCat);
	$sqlGetUsername = $db->sql_query("SELECT * FROM ".$prefix."_users WHERE user_id = '".$EventInfo['gemOwnerID']."'");
	$OwnerName = $db->sql_fetchrow($sqlGetUsername);
	$timestamp = $EventInfo['gemStartTime'];
	$timestamp2 = $EventInfo['gemEndTime'];
	$duration = $EventInfo['gemEndTime'] - $EventInfo['gemStartTime'];
	$duration = TimeGap($duration);
	$gemDKPValue = $EventInfo['gemDKPValue'];
	// See if a character has signed up.
	$charSignup = Fix_Quotes($_POST['char_signup']);
	$charSelect = Fix_Quotes($_POST['p_char_select']);
	if ($charSignup == 'submitted' && $charSelect != 'none') {
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
		$AssignedSlot = '11';
		$slotCounter = 0;
		$CharToSignup = Fix_Quotes($_POST['p_char_select']);
		$gemOutName = Fix_Quotes($_POST['p_gemOutName']);
		$gemOutLevel = Fix_Quotes($_POST['p_gemOutLevel']);
		$gemOutClass = Fix_Quotes($_POST['p_gemOutClass']);
		$getPrevSignups = $db->sql_query("SELECT * FROM ".$prefix."_GEM_signups WHERE gemEventID = '".$eid."' ORDER BY gemGroupID, gemSlotID");
		while($row = $db->sql_fetchrow($getPrevSignups)) {
			$tmpGroup = $row['gemGroupID'];
			$tmpSlot = $row['gemSlotID'];
			$position = $tmpGroup.$tmpSlot;
			$TakenSlots[] = $position;
		}
		if ($gemAutoslots == 1) {
			for ($i=0; $i<=$EventInfo['gemMaxChars']; $i++) {
				if (in_array($AssignedSlot, $TakenSlots)) {
					$slotCounter++;
					$AssignedSlot = $FullPosList[$slotCounter];
				} elseif ($i == $EventInfo['gemMaxChars']) {
					$group_id = 5;
					$slot_id = 7;
					break;				
				} else {
					$group_id = floor($AssignedSlot / 10);
					$slot_id = $AssignedSlot - ($group_id * 10);
					break;
				}
			}
		} else {
			$group_id = 5;
			$slot_id = 7;
		}
		if ($CharToSignup != 0) {
			$db->sql_query("
				INSERT INTO ".$prefix."_GEM_signups 
				(gemUserID, 
				gemCharID, 
				gemEventID,
				gemGroupID, 
				gemSlotID, 
				gemSignupDate)
				VALUES
				('".$userinfo['user_id']."',
				'".$CharToSignup."',
				'".$eid."',
				'".$group_id."',
				'".$slot_id."',
				'".gmtime()."')");
		} else {
			$arrayName = explode(' ',$gemOutName);
			$gemOutNameTmp = strtolower($arrayName[0]);
			$gemOutName = ucfirst($gemOutNameTmp);
			$db->sql_query("
				INSERT INTO ".$prefix."_GEM_signups 
				(gemUserID, 
				gemCharID, 
				gemEventID,
				gemGroupID, 
				gemSlotID, 
				gemSignupDate)
				VALUES
				('".$userinfo['user_id']."',
				'0',
				'".$eid."',
				'".$group_id."',
				'".$slot_id."',
				'".gmtime()."')");
			$db->sql_query("
				INSERT INTO ".$prefix."_GEM_outsiders 
				(gemUserID, 
				gemCharName,
				gemCharLevel,
				gemCharClass,
				gemEventID,
				gemGroupID, 
				gemSlotID, 
				gemSignupDate)
				VALUES
				('".$userinfo['user_id']."',
				'".$gemOutName."',
				'".$gemOutLevel."',
				'".$gemOutClass."',
				'".$eid."',
				'".$group_id."',
				'".$slot_id."',
				'".gmtime()."')");
		}
		$thisPage = get_uri();
		url_redirect($thisPage);
	} elseif ($charSignup == 'submitted' && $charSelect == 'none') {
		$thisPage = get_uri();
		url_redirect($thisPage);
	}	
	// See if a character has been removed.
	$charRemoval = Fix_Quotes($_POST['char_removal']);
	$charRemSelect = Fix_Quotes($_POST['p_char_remove']);
	if ($charRemoval == 'submitted' && $charRemSelect != 'none') {
		$db->sql_query("DELETE FROM ".$prefix."_GEM_signups WHERE (gemEventID = '".$eid."' AND gemCharID = '".$charRemSelect."')");
		$thisPage = get_uri();
		url_redirect($thisPage);
	} elseif ($charRemoval == 'submitted' && $charRemSelect == 'none') {
		$thisPage = get_uri();
		url_redirect($thisPage);
	}
	if ($SignupType != _GEM_GROUP && $SignupType != _GEM_CLASS) {
		$SignupType = _GEM_CLASS;
	}
	// Generate an array of participants
	$EventDataArr = SignupData($eid, $SignupType);
	$groupDisp = $EventDataArr[0];
	$ChosenCharID = $EventDataArr[1];
	$numSignups = $EventDataArr[3];
	// If user has permission to signup for event, generate form.
	if (in_array($gemSignupID, $CurUserGroup) && $EventInfo['gemEndTime'] > $now) {
		// Find out if the user has any claimed characters on roster master.
		$sql_rm_users = "SELECT * FROM ".$prefix."_roster_master_users WHERE user_id = '".$userinfo['user_id']."' ORDER BY char_type DESC";
		$rm_user = $db->sql_query($sql_rm_users);
		while($row = $db->sql_fetchrow($rm_user)) {
			// Build a list of user claimed characters not already signed up for this event.
			if ((sizeof($ChosenCharID) == 0) || !in_array($row['characterId'], $ChosenCharID, false)) {
				$char_id[] = $row['characterId'];
			// Build a list of user characters availabe to be removed from event.
			} else {
				$char_rem_id[] = $row['characterId'];
			}
		}
		if (sizeof($char_id) == 0) {
			$SelectMsg = _GEM_NO_CHARS_AVAIL;
			$SelectDisable = 'disabled';
		} else {
			$SelectMsg = _GEM_MAKE_CHAR_SEL;
			$SelectDisable = '';
		}
		if (in_array($gemAdminID, $CurUserGroup)) {
			$char_rem_id = $ChosenCharID;
			if (sizeof($char_rem_id) == 0) {
				$RemoveMsg = _GEM_NO_CHARS_AVAIL;
				$RemoveDisable = 'disabled';
			} else {
//cpg_error('<pre style="text-align:left">'.print_r($char_rem_id, TRUE)."</pre>\n");
				$RemoveMsg = _GEM_MAKE_CHAR_SEL;
				$RemoveDisable = '';
			}
		} else {
			if (sizeof($char_rem_id) == 0) {
				$RemoveMsg = _GEM_NO_CHARS_AVAIL;
				$RemoveDisable = 'disabled';
			} else {
				$RemoveMsg = _GEM_MAKE_CHAR_SEL;
				$RemoveDisable = '';
			}
		}
		// Build form for signing up characters.
		$signupContent = '
			<tr>
				<td align="center">
					<form method="post" action="'.htmlprepare(get_uri()).'" style="padding: 0px; margin: 0px;">
						<select name="p_char_select" '.$SelectDisable.'>
							<option value="none" >'.$SelectMsg.'.</option>';
		foreach($char_id as $char_sel_id) {
			$sql_rm_chars = "SELECT * FROM ".$prefix."_roster_master WHERE characterId = '".$char_sel_id."'";
			$rm_chars = $db->sql_query($sql_rm_chars);
			while($row = $db->sql_fetchrow($rm_chars)) {
				$char_name = $row['name_first'];
				$char_level = $row['type_level'];
				$char_class = $row['type_class'];
				$grank = $row['guild_rank'];
				$signupContent .= '<option value="'.$char_sel_id.'" >'.$char_name.' '._GEM_A_LEVEL.' '.$char_level.' '.$char_class.'</option>';
			}
		}
		$signupContent .= '
						</select>
						<input type="hidden" name="char_signup" value="submitted" />
						<input type="submit" value="'._GEM_SIGN_ME_UP.'" '.$SelectDisable.' class="liteoption" />
					</form>
					<br />
				</td>
			</tr>';
		// Build form for removing characters.
		$removeContent = '
			<tr>
				<td align="center">
					<form method="post" action="'.htmlprepare(get_uri()).'" style="padding: 0px; margin: 0px;">
						<select name="p_char_remove" '.$RemoveDisable.'>
							<option value="none" >'.$RemoveMsg.'.</option>';
		if (sizeof($char_rem_id) > 0) {
//cpg_error('<pre style="text-align:left">'.print_r($char_rem_id, TRUE)."</pre>\n");
			foreach($char_rem_id as $char_sel_id) {
				$sql_rm_chars = "SELECT * FROM ".$prefix."_roster_master WHERE characterId = '".$char_sel_id."'";
				$rm_chars = $db->sql_query($sql_rm_chars);
				while($row = $db->sql_fetchrow($rm_chars)) {
					$char_name = $row['name_first'];
					$char_level = $row['type_level'];
					$char_class = $row['type_class'];
					$grank = $row['guild_rank'];
					$removeContent .= '<option value="'.$char_sel_id.'" >'.$char_name.' '._GEM_A_LEVEL.' '.$char_level.' '.$char_class.'</option>';
				}
			}
		}
		$removeContent .= '
						</select>
						<input type="hidden" name="char_removal" value="submitted" />
						<input type="submit" value="'._GEM_TAKE_ME_OFF.'" '.$RemoveDisable.' class="liteoption" />
					</form>
				</td>
			</tr>';
	} elseif (in_array($gemOutsiderOpt, $CurUserGroup) && $EventInfo['gemEndTime'] > $now) { 
		$signupContent = '
			<tr>
				<td align="center">
					<form method="post" action="'.htmlprepare(get_uri()).'" style="padding: 0px; margin: 0px;">
						Character Name: <input type="text" name="p_gemOutName" />
						Level: <select name="p_gemOutLevel">';
									for ($i=1; $i<=80; $i++) {
										$signupContent .= '<option value="'.$i.'">'.$i.'</option>';
									}
		$signupContent .= '
						</select>
						Level: <select name="p_gemOutClass">
							<option value="Assassin">Assassin</option>
							<option value="Berzerker">Berzerker</option>
							<option value="Brigand">Brigand</option>
							<option value="Bruiser">Bruiser</option>
							<option value="Coercer">Coercer</option>
							<option value="Conjuror">Conjuror</option>
							<option value="Defiler">Defiler</option>
							<option value="Dirge">Dirge</option>
							<option value="Fury">Fury</option>
							<option value="Guardian">Guardian</option>
							<option value="Illusionist">Illusionist</option>
							<option value="Inquisitor">Inquisitor</option>
							<option value="Monk">Monk</option>
							<option value="Mystic">Mystic</option>
							<option value="Necromancer">Necromancer</option>
							<option value="Paladin">Paladin</option>
							<option value="Ranger">Ranger</option>
							<option value="Shadow Knight">Shadow Knight</option>
							<option value="Swashbuckler">Swashbuckler</option>
							<option value="Templar">Templar</option>
							<option value="Troubador">Troubador</option>
							<option value="Warden">Warden</option>
							<option value="Warlock">Warlock</option>
							<option value="Wizard">Wizard</option>	
						</select>
						<input type="hidden" name="char_signup" value="submitted" />
						<input type="hidden" name="p_char_select" value="0" />
						<input type="submit" value="'._GEM_SIGN_ME_UP.'" class="liteoption" />
					</form>
				</td>
			</tr>';
	} else {
		$signupContent =  '';
		$removeContent = '';
	}
	// Add link for admins to administer group data.
	if (in_array($gemAdminID, $CurUserGroup)) {
		$groupContent = '
			<tr>
				<td align="center">
					<br /><br />
					<form method="post" action="'.getlink("&amp;pg=17&amp;eid=$eid").'" style="padding: 0px; margin: 0px;">
						<input type="hidden" name="edit_group" value="submitted" />
						<input type="submit" value="'._GEM_EDIT_GROUP.'" class="liteoption" />
					</form>
				</td>
			</tr>';
	} else {
		$groupContent = '';
	}
	// Generate an array of comments
	$numComments = 0;
	$getComments = $db->sql_query("SELECT * FROM ".$prefix."_GEM_comments WHERE gemEventID = '".$eid."' ORDER BY gemComID");
	while($row = $db->sql_fetchrow($getComments)) {
		$numComments++;
		$Comments[] = $row;
	}
	if ($Comments) {
		foreach($Comments as $value) {
			$getCPGname = $db->sql_query("SELECT * FROM ".$prefix."_users WHERE user_id = '".$value['gemUserID']."'");
			$gemUserName = $db->sql_fetchrow($getCPGname);
			$ComTime = $value['gemComDate'];
			$com_id = $value['gemComID'];
			$commentContent .= '
			<tr><td>
				<br />
				<div style="border: 1px solid '.$bgcolor3.'; padding: 2px; background-color: '.$bgcolor2.';">
					<font class="content"><b>&quot;'.$value["gemComTitle"].'&quot;</b><br />
					'._BY.' <a href="Your_Account/profile='.$value["gemUserID"].'.html">'.$gemUserName["username"].'</a> '._ON.' '.L10NTime::date(_GEM_DATE6, $ComTime, $userinfo["user_dst"], $userinfo["user_timezone"]).' '.substr($l10n_gmt_regions[$userinfo["user_timezone"]], strpos($l10n_gmt_regions[$userinfo["user_timezone"]], "("), strpos($l10n_gmt_regions[$userinfo["user_timezone"]], ")")).')</font>
				</div>';
				if (in_array($gemCommentID, $CurUserGroup)) {
					$commentContent .= '
						<div style="border: 1px solid '.$bgcolor3.'; padding: 2px; background-color: '.$bgcolor1.';">
							<font class="content">
							'.decode_bb_all(encode_bbcode($value["gemComment"]), 1, true).'<br><br>
							<div align="right">
								<a href="'.getlink("&amp;pg=18&amp;eid=$eid&amp;rci=$com_id").'">'._GEM_REPLY.'</a>';
								if ($value['gemUserID'] == $userinfo['user_id'] || in_array($gemAdminID, $CurUserGroup)) {
									$commentContent .= '
									| <a href="'.getlink("&amp;pg=18&amp;eid=$eid&amp;eci=$com_id").'">'._EDIT.'</a>
									| <a href="'.getlink("&amp;pg=18&amp;eid=$eid&amp;dci=$com_id").'">'._DELETE.'</a>';
								}
								$commentContent .= '
								</font><br>
							</div>
						</div>';
				}
				$commentContent .= '
				<br />
			</td></tr>';
		}
	}
	// Add link for commenters to comment.
	if (in_array($gemCommentID, $CurUserGroup)) {
		$commentButton = '
			<tr>
				<td align="center">
					<br />
					<form method="post" action="'.getlink("&amp;pg=18&amp;eid=$eid").'" style="padding: 0px; margin: 0px;">
						<input type="submit" value="'._GEM_LEAVE_COM.'" class="liteoption" />
					</form>
				</td>
			</tr>';
	} else {
		$commentButton = '';
	}
	$togglecolor = ($togglecolor == $bgcolor2) ? $bgcolor1 : $bgcolor2;
	//Add info about non guildmember participation.
	if ($gemOutsiderOpt != 0) {
		$gemOutDetail =
			'<tr>
				<td style="background-color: '.$togglecolor.';">'._GEM_ALLOW_OUT.':</td>
				<td style="background-color: '.$togglecolor.';">'.$gemOutStatus.'</td>
			</tr>';
			$togglecolor = ($togglecolor == $bgcolor2) ? $bgcolor1 : $bgcolor2; 
	} else {
		$gemOutDetail = '';
	}
	//Add info about dkp.
	if ($gemDKPOpt != 0) {
		$gemDKPDetail .=
			'<tr>
				<td style="background-color: '.$togglecolor.';">'._GEM_DKP_VALUE.':</td>
				<td style="background-color: '.$togglecolor.';">'.$gemDKPValue.'</td>
			</tr>';
			$togglecolor = ($togglecolor == $bgcolor2) ? $bgcolor1 : $bgcolor2; 
	} else {
		$gemDKPDetail .= '';
	}
	//Add info about event editing.
	if ($EventInfo['gemEditorID'] != 0) {
		$getEditorName = $db->sql_query("SELECT * FROM ".$prefix."_users WHERE user_id = '".$EventInfo['gemEditorID']."'");
		$gemEditorName = $db->sql_fetchrow($getEditorName);
		$gemEditDetail .=
			'<tr>
				<td style="background-color: '.$togglecolor.';" colspan="100%"><center><i>Last edited by '.$gemEditorName['username'].' on '.GEMdate2(_GEM_DATE1, $EventInfo['gemLastEditTime'], $userinfo["user_dst"], $userinfo["user_timezone"]).' '.substr($l10n_gmt_regions[$userinfo["user_timezone"]], strpos($l10n_gmt_regions[$userinfo["user_timezone"]], "("), strpos($l10n_gmt_regions[$userinfo["user_timezone"]], ")")).').</i></center></td>
			</tr>';
			$togglecolor = ($togglecolor == $bgcolor2) ? $bgcolor1 : $bgcolor2; 
	} else {
		$gemEditDetail .= '';
	}
	OpenTable();
	PrintMainMenu();
	echo '
		<table width="100%">
			<tr>
				<th id="EventTitle">'._GEM_EVENT_TITLE.': &quot;'.$EventInfo["gemTitle"].'&quot;</th>
			</tr>
			<tr>
				<td>
					<table>
						<tr>
							<td width="100%">
								<table style="border-collapse: collapse;">
									<tr>
										<td style="background-color: '.$bgcolor2.';">'._GEM_EVENT_OWNER.':</td>
										<td style="background-color: '.$bgcolor2.';">'.$OwnerName['username'].'</td>
									</tr>
									<tr>
										<td style="background-color: '.$bgcolor1.';">'._GEM_START_TIME.':</td>
										<td style="background-color: '.$bgcolor1.';">'.GEMdate(_GEM_DATE2, $timestamp, $userinfo["user_dst"], $userinfo["user_timezone"]).' '.substr($l10n_gmt_regions[$userinfo["user_timezone"]], strpos($l10n_gmt_regions[$userinfo["user_timezone"]], "("), strpos($l10n_gmt_regions[$userinfo["user_timezone"]], ")")).')</td>
									</tr>
									<tr>
										<td style="background-color: '.$bgcolor2.';">'._GEM_DURATION.':</td>
										<td style="background-color: '.$bgcolor2.';">'.$duration[2].' '._GEM_HOURS_SM.'</td>
									</tr>
									<tr>
										<td style="background-color: '.$bgcolor1.';">'._GEM_EVENT_CHARS.':</td>
										<td style="background-color: '.$bgcolor1.';">'.$EventInfo["gemMaxChars"].'</td>
									</tr>
									<tr>
										<td style="background-color: '.$bgcolor2.';">'._GEM_MIN_LEVEL.':</td>
										<td style="background-color: '.$bgcolor2.';">'.$EventInfo["gemMinLevel"].'</td>
									</tr>
									<tr>
										<td style="background-color: '.$bgcolor1.';">'._GEM_EVENT_TYPE.':</td>
										<td style="background-color: '.$bgcolor1.';">'.$EventInfo["gemEventType"].'</td>
									</tr>
									'.$gemOutDetail.$gemDKPDetail.$gemEditDetail;
									if (is_admin()) {
										/*      echo '<tr>
												<td colspan="100%"><a href="'.getlink("&amp;pg=15&amp;eid=$eid").'">Top</a></td>
											</tr>';     */
										echo '
											<tr>
												<td align="center"><br/><a href="'.getlink("&amp;pg=15&amp;eid=$eid#EventSignup").'">Signups</a> ('.$numSignups.')</td>
												<td align="center"><br/><a href="'.getlink("&amp;pg=15&amp;eid=$eid#EventComments").'">Comments</a> ('.$numComments.')</td>
											</tr>';
									}									
								echo '
								</table>
							</td>
							<td><img src="images/'.$module_name.'/'.$CatInfo["gemCatImage"].'" alt="'.$CatInfo["gemCatTitle"].'" title="'.$CatInfo["gemCatTitle"].'" /></td>
						</tr>
					</table>
				</td>
			</tr>';
			echo '
			<tr>
				<td><br /></td>
			</tr>
			<tr>
				<td>
					<table width="100%" style="border-collapse: collapse;">
						<tr>
							<td class="postbody" style="background-color: '.$bgcolor1.'; border: 1px solid '.$bgcolor3.';"><center><h1><u>-'._GEM_EVENT_DESC.'-</u></h1></center><br /><br />
							&nbsp;'.decode_bb_all(encode_bbcode($EventInfo["gemDesc"]), 1, true).'<br /><br /></td>
						</tr>
					</table>
				</td>
			</tr>';
			if ((in_array($gemAdminID, $CurUserGroup) || $userinfo['user_id'] == $EventInfo['gemOwnerID'] || $gemAdminID == 1)) {
				echo '
					<tr>
						<td><br /><br /></td>
					</tr>
					<tr>
						<td align="center">
							<table align="center">
								<tr>
									<td>
										<form method="post" action="'.getlink("&amp;pg=3&amp;eid=$eid").'" style="padding: 0px; margin: 0px;">
											<input type="hidden" name="edit" value="'.$eid.'" />
											<input type="submit" value="'._GEM_EDIT.'"  name="editEvent" class="liteoption" />
										</form>
									</td>
									<td>
										<form method="post" action="'.getlink("&amp;pg=16&amp;eid=$eid").'" style="padding: 0px; margin: 0px;">
											<input type="submit" value="'._GEM_DELETE.'" name="deleteEvent" class="liteoption" />
										</form>';
										if (in_array($gemAdminID, $CurUserGroup)) {
											echo '
													<td>
														<form method="post" action="'.getlink("&amp;pg=17&amp;eid=$eid").'" style="padding: 0px; margin: 0px;">
															<input type="hidden" name="edit_group" value="submitted" />
															<input type="submit" value="'._GEM_EDIT_GROUP.'" name="editGroup" class="liteoption" />
														</form>
													</td>';
										}
		echo '						</td>
								</tr>
							</table>
						</td>
					</tr>';
			}
		echo '
			<tr>
				<td><br /><br /></td>
			</tr>
			<tr>
				<th id="EventSignup">'._GEM_SIGNED_UP.'</th>
			</tr>
			<tr>
				<td align="right">
					<form method="post" action="'.htmlprepare(get_uri()).'" style="padding: 0px; margin: 0px;">
						'._GEM_SORT_BY.':
						<select name="signuptype">
							<option value="'.$SignupType.'">'.$SignupType.'</option>
							<option value="'.$SignupChoice.'">'.$SignupChoice.'</option>
						</select>
						<input type="submit" value="'._GO.'" class="liteoption" />
					</form>
				</td>
			</tr>
			'.$groupDisp.'
			<tr>
				<td><br /><br /></td>
			</tr>
			'.$signupContent.'
			'.$removeContent.'
			<tr>
				<td><br /><br /></td>
			</tr>
			<tr>
				<th id="EventComments">'._GEM_EVENT_COMMENTS.'</th>
			</tr>
			'.$commentContent.'
			'.$commentButton.'
			<tr>
				<td><br /><br /></td>
			</tr>
		</table>';
	Footer();
	CloseTable();
}

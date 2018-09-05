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


// Build a listing for event detail screen
function SignupData($eid, $SignupType) {
	global $prefix, $userinfo, $db, $bgcolor1, $bgcolor2, $bgcolor3, $gemPubClass, $gemAdmClass, $pg;
	$sqlGetEvent = $db->sql_query("SELECT * FROM ".$prefix."_GEM_events WHERE gemEventID = '".$eid."'");
	$EventInfo = $db->sql_fetchrow($sqlGetEvent);
	$numSignups = 0;
	$numGroup1 = 0;
	$numGroup2 = 0;
	$numGroup3 = 0;
	$numGroup4 = 0;
	$numGroup5 = 0;
	$rowcolor = $bgcolor2;
	switch ($SignupType) {
		case (_GEM_GROUP):
			$Group1Head = _GEM_GROUP1;
			$Group2Head = _GEM_GROUP2;
			$Group3Head = _GEM_GROUP3;
			$Group4Head = _GEM_GROUP4;
			$Group5Head = _GEM_GROUP5;
			$getSignups = $db->sql_query("SELECT * FROM ".$prefix."_GEM_signups WHERE gemEventID = '".$eid."' ORDER BY gemGroupID, gemSlotID");
			while($row1 = $db->sql_fetchrow($getSignups)) {
				$numSignups++;
				$GroupID = $row1['gemGroupID'];
				$SlotID = $row1['gemSlotID'];
				$SignupDate = GEMdate2(_GEM_DATE7, $row1['gemSignupDate'], $userinfo["user_dst"], $userinfo["user_timezone"]);
				$UsedSlot[] = $GroupID.$SlotID;
				// Pull Character data from Roster Master if guildmate
				if ($row1['gemCharID'] != 0) {
					$sql_rm_disp_chars = "SELECT * FROM ".$prefix."_roster_master WHERE characterId = '".$row1['gemCharID']."'";
					$rm_disp_chars = $db->sql_query($sql_rm_disp_chars);
					while($row = $db->sql_fetchrow($rm_disp_chars)) {
						$ChosenCharID[] = $row['characterId'];
						$disp_char_name = $row['name_first'];
						$disp_rank = $row['guild_rank'];
						$disp_adv_level = $row['type_level'];
						$disp_adv_class = $row['type_class'];
						if ($disp_adv_class == 'Shadow Knight') { 
							$imgPath = 'ShadowKnight'; 
						} else {
							$imgPath = $disp_adv_class;
						}
						switch ($pg) {
							case '7':
								if ($gemAdmClass == 0) {
									$classData = '<img src="modules/GEM/images/'.$imgPath.'.gif" alt="'.$disp_adv_class.'" title="'.$disp_adv_class.'" />';
								} elseif ($gemAdmClass == 1) {
									$classData = '<div style="font-size: 16px;">'.$disp_adv_class.'</div>';
								} else {
									$classData = '<div style="font-size: 16px;">'.$disp_adv_class.'&nbsp;&nbsp;&nbsp;<img src="modules/GEM/images/'.$imgPath.'.gif" alt="'.$disp_adv_class.'" title="'.$disp_adv_class.'" /></div>';
								}
								break;
							default:
								if ($gemPubClass == 0) {
									$classData = '<img src="modules/GEM/images/'.$imgPath.'.gif" alt="'.$disp_adv_class.'" title="'.$disp_adv_class.'" />';
								} elseif ($gemPubClass == 1) {
									$classData = '<div style="font-size: 16px;">'.$disp_adv_class.'</div>';
								} else {
									$classData = '<div style="font-size: 16px;">'.$disp_adv_class.'&nbsp;&nbsp;&nbsp;<img src="modules/GEM/images/'.$imgPath.'.gif" alt="'.$disp_adv_class.'" title="'.$disp_adv_class.'" /></div>';
								}
								break;
						}
						$rowdata = '
							<tr>
								<td align="center" style="background-color: '.$bgcolor1.'; border-left: 1px solid '.$bgcolor3.'; border-bottom: 1px solid '.$bgcolor3.';"><div style="font-size: 16px;">'.$disp_char_name.'</div></td>
								<td align="center" style="background-color: '.$bgcolor1.'; border-bottom: 1px solid '.$bgcolor3.';"><div style="font-size: 16px;">'.$disp_rank.'</div></td>
								<td align="center" style="background-color: '.$bgcolor1.'; border-bottom: 1px solid '.$bgcolor3.';"><div style="font-size: 16px;">'.$disp_adv_level.'</div></td>
								<td align="center" style="background-color: '.$bgcolor1.'; border-bottom: 1px solid '.$bgcolor3.';">'.$classData.'</td>
								<td align="center" style="background-color: '.$bgcolor1.'; border-right: 1px solid '.$bgcolor3.'; border-bottom: 1px solid '.$bgcolor3.';">'.$SignupDate.'</td>
							</tr>';
						switch (true) {
							case ($GroupID == '1'):
								$arrGroup1[$SlotID] = $rowdata;
								$numGroup1++;
								break;
							case ($GroupID == '2'):
								$arrGroup2[$SlotID] = $rowdata;
								$numGroup2++;
								break;
							case ($GroupID == '3'):
								$arrGroup3[$SlotID] = $rowdata;
								$numGroup3++;
								break;
							case ($GroupID == '4'):
								$arrGroup4[$SlotID] = $rowdata;
								$numGroup4++;
								break;
							case ($GroupID == '5'):
								$arrGroup5[] = $rowdata;
								$numGroup5++;
								break;
						}
					}
				// Pull Character data from outsider table if non guildie
				} else {
					$sqlOutData = $db->sql_query("SELECT * FROM ".$prefix."_GEM_outsiders WHERE gemEventId = '".$row1['gemEventID']."' AND gemUserID = '".$row1['gemUserID']."'");
					$ChosenCharID[] = $sqlOutData['gemOutID'];
					$disp_char_name = $sqlOutData['gemCharName'];
					$disp_rank = _GEM_NOT_AVAIL;
					$disp_adv_level = $sqlOutData['gemCharLevel'];
					$disp_adv_class = $sqlOutData['gemCharClass'];	
					if ($disp_adv_class == 'Shadow Knight') { 
						$imgPath = 'ShadowKnight'; 
					} else {
						$imgPath = $disp_adv_class;
					}
					switch ($pg) {
						case '7':
							if ($gemAdmClass == 0) {
								$classData = '<img src="modules/GEM/images/'.$imgPath.'.gif" alt="'.$disp_adv_class.'" title="'.$disp_adv_class.'" />';
							} elseif ($gemAdmClass == 1) {
								$classData = '<div style="font-size: 16px;">'.$disp_adv_class.'</div>';
							} else {
								$classData = '<div style="font-size: 16px;">'.$disp_adv_class.'&nbsp;&nbsp;&nbsp;<img src="modules/GEM/images/'.$imgPath.'.gif" alt="'.$disp_adv_class.'" title="'.$disp_adv_class.'" /></div>';
							}
							break;
						default:
							if ($gemPubClass == 0) {
								$classData = '<img src="modules/GEM/images/'.$imgPath.'.gif" alt="'.$disp_adv_class.'" title="'.$disp_adv_class.'" />';
							} elseif ($gemPubClass == 1) {
								$classData = '<div style="font-size: 16px;">'.$disp_adv_class.'</div>';
							} else {
								$classData = '<div style="font-size: 16px;">'.$disp_adv_class.'&nbsp;&nbsp;&nbsp;<img src="modules/GEM/images/'.$imgPath.'.gif" alt="'.$disp_adv_class.'" title="'.$disp_adv_class.'" /></div>';
							}
							break;
					}
					$rowdata = '
						<tr>
							<td align="center" style="background-color: '.$bgcolor1.'; border-left: 1px solid '.$bgcolor3.'; border-bottom: 1px solid '.$bgcolor3.';"><div style="font-size: 16px;">'.$disp_char_name.'</div></td>
							<td align="center" style="background-color: '.$bgcolor1.'; border-bottom: 1px solid '.$bgcolor3.';"><div style="font-size: 16px;">'.$disp_rank.'</div></td>
							<td align="center" style="background-color: '.$bgcolor1.'; border-bottom: 1px solid '.$bgcolor3.';"><div style="font-size: 16px;">'.$disp_adv_level.'</div></td>
							<td align="center" style="background-color: '.$bgcolor1.'; border-bottom: 1px solid '.$bgcolor3.';">'.$classData.'</td>
							<td align="center" style="background-color: '.$bgcolor1.'; border-right: 1px solid '.$bgcolor3.'; border-bottom: 1px solid '.$bgcolor3.';">'.$SignupDate.'</td>
						</tr>';	
					switch (true) {
						case ($GroupID == '1'):
							$arrGroup1[$SlotID] = $rowdata;
							$numGroup1++;
							break;
						case ($GroupID == '2'):
							$arrGroup2[$SlotID] = $rowdata;
							$numGroup2++;
							break;
						case ($GroupID == '3'):
							$arrGroup3[$SlotID] = $rowdata;
							$numGroup3++;
							break;
						case ($GroupID == '4'):
							$arrGroup4[$SlotID] = $rowdata;
							$numGroup4++;
							break;
						case ($GroupID == '5'):
							$arrGroup5[] = $rowdata;
							$numGroup5++;
							break;
					}						
				}
			}
			break;
		case (_GEM_CLASS):
			$Group1Head = '<img src="modules/GEM/images/fighter.gif" alt="'._GEM_FIGHTERS.'" title="'._GEM_FIGHTERS.'" />';
			$Group2Head = '<img src="modules/GEM/images/priest.gif" alt="'._GEM_PRIESTS.'" title="'._GEM_PRIESTS.'" />';
			$Group3Head = '<img src="modules/GEM/images/mage.gif" alt="'._GEM_MAGES.'" title="'._GEM_MAGES.'" />';
			$Group4Head = '<img src="modules/GEM/images/scout.gif" alt="'._GEM_SCOUTS.'" title="'._GEM_SCOUTS.'" />';
			$getSignups = $db->sql_query("SELECT * FROM ".$prefix."_GEM_signups WHERE gemEventID = '".$eid."' ORDER BY gemSignupDate");
			while($row1 = $db->sql_fetchrow($getSignups)) {
				$numSignups++;
				$SignupDate = GEMdate2(_GEM_DATE7, $row1['gemSignupDate'], $userinfo["user_dst"], $userinfo["user_timezone"]);
				if ($row1['gemCharID'] != 0) {
					$sql_rm_disp_chars = "SELECT * FROM ".$prefix."_roster_master WHERE characterId = '".$row1['gemCharID']."'";
					$rm_disp_chars = $db->sql_query($sql_rm_disp_chars);
					while($row = $db->sql_fetchrow($rm_disp_chars)) {
						$ChosenCharID[] = $row['characterId'];
						$disp_char_name = $row['name_first'];
						$disp_rank = $row['guild_rank'];
						$disp_adv_level = $row['type_level'];
						$disp_adv_class = $row['type_class'];
						if ($disp_adv_class == 'Shadow Knight') { 
							$imgPath = 'ShadowKnight'; 
						} else {
							$imgPath = $disp_adv_class;
						}
						if ($gemPubClass == 0) {
							$classData = '<img src="modules/GEM/images/'.$imgPath.'.gif" alt="'.$disp_adv_class.'" title="'.$disp_adv_class.'" />';
						} elseif ($gemPubClass == 1) {
							$classData = '<div style="font-size: 16px;">'.$disp_adv_class.'</div>';
						} else {
							$classData = '<div style="font-size: 16px;">'.$disp_adv_class.'&nbsp;&nbsp;&nbsp;<img src="modules/GEM/images/'.$imgPath.'.gif" alt="'.$disp_adv_class.'" title="'.$disp_adv_class.'" /></div>';
						}
						$rowdata = '
							<tr>
								<td align="center" style="background: '.$bgcolor1.'; border-left: 1px solid '.$bgcolor3.'; border-bottom: 1px solid '.$bgcolor3.';"><div style="font-size: 16px;">'.$disp_char_name.'</div></td>
								<td align="center" style="background: '.$bgcolor1.'; border-bottom: 1px solid '.$bgcolor3.';"><div style="font-size: 16px;">'.$disp_rank.'</div></td>
								<td align="center" style="background: '.$bgcolor1.'; border-bottom: 1px solid '.$bgcolor3.';"><div style="font-size: 16px;">'.$disp_adv_level.'</div></td>
								<td align="center" style="background: '.$bgcolor1.'; border-bottom: 1px solid '.$bgcolor3.';">'.$classData.'</td>
								<td align="center" style="background: '.$bgcolor1.'; border-right: 1px solid '.$bgcolor3.'; border-bottom: 1px solid '.$bgcolor3.';">'.$SignupDate.'</td>
							</tr>';
						switch (true) {
							case ($row['Adv_Class'] == 'Berserker' || $row['Adv_Class'] == 'Guardian' || $row['Adv_Class'] == 'Bruiser' || $row['Adv_Class'] == 'Monk' || $row['Adv_Class'] == 'Paladin' || $row['Adv_Class'] == 'Shadow Knight'):
								$arrGroup1[$numGroup1] = $rowdata;
								$numGroup1++;
								break;
							case ($row['Adv_Class'] == 'Templar' || $row['Adv_Class'] == 'Inquisitor' || $row['Adv_Class'] == 'Warden' || $row['Adv_Class'] == 'Fury' || $row['Adv_Class'] == 'Defiler' || $row['Adv_Class'] == 'Mystic'):
								$arrGroup2[$numGroup2] = $rowdata;
								$numGroup2++;
								break;
							case ($row['Adv_Class'] == 'Warlock' || $row['Adv_Class'] == 'Wizard' || $row['Adv_Class'] == 'Illusionist' || $row['Adv_Class'] == 'Coercer' || $row['Adv_Class'] == 'Necromancer' || $row['Adv_Class'] == 'Conjuror'):
								$arrGroup3[$numGroup3] = $rowdata;
								$numGroup3++;
								break;
							case ($row['Adv_Class'] == 'Brigand' || $row['Adv_Class'] == 'Swashbuckler' || $row['Adv_Class'] == 'Dirge' || $row['Adv_Class'] == 'Troubador' || $row['Adv_Class'] == 'Assassin' || $row['Adv_Class'] == 'Ranger'):
								$arrGroup4[$numGroup4] = $rowdata;
								$numGroup4++;
								break;
						}
						unset($rowdata);
					}
				} else {
					$sql_out_disp_chars = "SELECT * FROM ".$prefix."_GEM_outsiders WHERE gemEventId = '".$row1['gemEventID']."' AND gemUserID = '".$row1['gemUserID']."'";
					$out_disp_chars = $db->sql_query($sql_out_disp_chars);
					while($row = $db->sql_fetchrow($out_disp_chars)) {
						$ChosenCharID[] = $row['gemOutID'];
						$disp_char_name = $row['gemCharName'];
						$disp_rank = _GEM_NOT_AVAIL;
						$disp_adv_level = $row['gemCharLevel'];
						$disp_adv_class = $row['gemCharClass'];
						if ($disp_adv_class == 'Shadow Knight') { 
							$imgPath = 'ShadowKnight'; 
						} else {
							$imgPath = $disp_adv_class;
						}
						if ($gemPubClass == 0) {
							$classData = '<img src="modules/GEM/images/'.$imgPath.'.gif" alt="'.$disp_adv_class.'" title="'.$disp_adv_class.'" />';
						} elseif ($gemPubClass == 1) {
							$classData = '<div style="font-size: 16px;">'.$disp_adv_class.'</div>';
						} else {
							$classData = '<div style="font-size: 16px;">'.$disp_adv_class.'&nbsp;&nbsp;&nbsp;<img src="modules/GEM/images/'.$imgPath.'.gif" alt="'.$disp_adv_class.'" title="'.$disp_adv_class.'" /></div>';
						}
						$rowdata = '
							<tr>
								<td align="center" style="background: '.$bgcolor1.'; border-left: 1px solid '.$bgcolor3.'; border-bottom: 1px solid '.$bgcolor3.';"><div style="font-size: 16px;">'.$disp_char_name.'</div></td>
								<td align="center" style="background: '.$bgcolor1.'; border-bottom: 1px solid '.$bgcolor3.';"><div style="font-size: 16px;">'.$disp_rank.'</div></td>
								<td align="center" style="background: '.$bgcolor1.'; border-bottom: 1px solid '.$bgcolor3.';"><div style="font-size: 16px;">'.$disp_adv_level.'</div></td>
								<td align="center" style="background: '.$bgcolor1.'; border-bottom: 1px solid '.$bgcolor3.';">'.$classData.'</td>
								<td align="center" style="background: '.$bgcolor1.'; border-right: 1px solid '.$bgcolor3.'; border-bottom: 1px solid '.$bgcolor3.';">'.$SignupDate.'</td>
							</tr>';
						switch (true) {
							case ($row['gemCharClass'] == 'Berserker' || $row['gemCharClass'] == 'Guardian' || $row['gemCharClass'] == 'Bruiser' || $row['gemCharClass'] == 'Monk' || $row['gemCharClass'] == 'Paladin' || $row['gemCharClass'] == 'Shadow Knight'):
								$arrGroup1[$numGroup1] = $rowdata;
								$numGroup1++;
								break;
							case ($row['gemCharClass'] == 'Templar' || $row['gemCharClass'] == 'Inquisitor' || $row['gemCharClass'] == 'Warden' || $row['gemCharClass'] == 'Fury' || $row['gemCharClass'] == 'Defiler' || $row['gemCharClass'] == 'Mystic'):
								$arrGroup2[$numGroup2] = $rowdata;
								$numGroup2++;
								break;
							case ($row['gemCharClass'] == 'Warlock' || $row['gemCharClass'] == 'Wizard' || $row['gemCharClass'] == 'Illusionist' || $row['gemCharClass'] == 'Coercer' || $row['gemCharClass'] == 'Necromancer' || $row['gemCharClass'] == 'Conjuror'):
								$arrGroup3[$numGroup3] = $rowdata;
								$numGroup3++;
								break;
							case ($row['gemCharClass'] == 'Brigand' || $row['gemCharClass'] == 'Swashbuckler' || $row['gemCharClass'] == 'Dirge' || $row['gemCharClass'] == 'Troubador' || $row['gemCharClass'] == 'Assassin' || $row['gemCharClass'] == 'Ranger'):
								$arrGroup4[$numGroup4] = $rowdata;
								$numGroup4++;
								break;
						}
						unset($rowdata);
					}
				}
			}
			break;
	}
	$groupDisp = '<tr><td><br /><br /><table align="center" width="90%" cellspacing="0" style="border-collapse: seperated; border-spacing: 0 0;">';
	$ColHeading = '
			<tr>
				<td align="center" style="background-color: '.$bgcolor3.';"><strong><u>'._GEM_CHAR_NAME.'</u></strong></td>
				<td align="center" style="background-color: '.$bgcolor3.';"><strong><u>'._GEM_RANK.'</u></strong></td>
				<td align="center" style="background-color: '.$bgcolor3.';"><strong><u>'._GEM_CHAR_LVL.'</u></strong></td>
				<td align="center" style="background-color: '.$bgcolor3.';"><strong><u>'._GEM_CLASS.'</u></strong></td>
				<td align="center" style="background-color: '.$bgcolor3.';"><strong><u>'._GEM_SIGN_DATE.'</u></strong></td>
			</tr>';
	$SpaceRow = '
			<tr>
				<td colspan="100%"><br /><br /><br /></td>
			</tr>';
	$NoSignupHead = '
		<tr>
			<td colspan="100%" align="center" style="background-color: '.$bgcolor1.'; border-bottom: 1px solid '.$bgcolor3.'; border-left: 1px solid '.$bgcolor3.'; border-right: 1px solid '.$bgcolor3.'; border-top: 1px solid '.$bgcolor3.';">'._GEM_NO_SIGNUPS.'</td>
		</tr>';
	$unknown = '
		<tr>
			<td align="center" style="background: '.$bgcolor1.'; border-left: 1px solid '.$bgcolor3.'; border-bottom: 1px solid '.$bgcolor3.';"><div style="font-size: 16px;">-----</div></td>
			<td align="center" style="background: '.$bgcolor1.'; border-bottom: 1px solid '.$bgcolor3.';"><div style="font-size: 16px;">-----</div></td>
			<td align="center" style="background: '.$bgcolor1.'; border-bottom: 1px solid '.$bgcolor3.';"><div style="font-size: 16px;">-----</div></td>
			<td align="center" style="background: '.$bgcolor1.'; border-right: 1px solid '.$bgcolor3.'; border-bottom: 1px solid '.$bgcolor3.';"><img src="modules/GEM/images/question.gif" alt="-----" title="-----" /></td>
			<td align="center" style="background: '.$bgcolor1.'; border-bottom: 1px solid '.$bgcolor3.';"><div style="font-size: 16px;">-----</div></td>
		</tr>';
	$numGroups = array($numGroup1, $numGroup2, $numGroup3, $numGroup4, $numGroup5);
	$groupHeads = array($Group1Head, $Group2Head, $Group3Head, $Group4Head, $Group5Head);
	$groupDataArr = array($arrGroup1, $arrGroup2, $arrGroup3, $arrGroup4, $arrGroup5);
	switch ($SignupType) {
		case _GEM_CLASS:
			for ($i = 0; $i <= 3; $i++) {
				$groupDisp .= '
					<tr>
						<td colspan="100%"><span class="maintitle">'.$groupHeads[$i].'</span></td>
					</tr>'.$ColHeading;	
				if ($numGroups[$i] > 0) {
					foreach($groupDataArr[$i] as $value) {
						$groupDisp .= $value;
					}
				} else {
					$groupDisp .= $NoSignupHead;
				}
				if ($i != 3) {
					$groupDisp .= $SpaceRow;
				}
			}	
			break;
		case _GEM_GROUP:
			$gemGroups = ($EventInfo['gemMaxChars'] / 6);
			for ($i = 0; $i < $gemGroups; $i++) {
				$groupDisp .= '
					<tr>
						<td colspan="100%" align="center"><span class="maintitle">'.$groupHeads[$i].'</span></td>
					</tr>'.$ColHeading;	
					if ($numGroups[$i] > 0) {
						$slotnum = 1;
						for ($x=1; $x<=6; $x++) {
							if ($groupDataArr[$i][$slotnum]) {
								$groupDisp .= $groupDataArr[$i][$slotnum];
							} else {
								$groupDisp .= $unknown;
							}
							$slotnum++;
						}
					} else {
						$groupDisp .= $NoSignupHead;
					}
				$groupDisp .= $SpaceRow;
			}
			$groupDisp .= '
				<tr>
					<td colspan="100%" align="center"><span class="maintitle">'.$groupHeads[4].'</span></td>
				</tr>'.$ColHeading;	
			if (sizeof($groupDataArr[4]) != 0) {
				foreach($groupDataArr[4] as $value) {
					$groupDisp .= $value;
				}
			} else {
				$groupDisp .= $NoSignupHead;
			}
			break;
	}
	$groupDisp .= '</table></td></tr>';
	$DataArray[0] = $groupDisp;
	$DataArray[1] = $ChosenCharID;
	if (strlen($UsedSlot) > 0) {
		$DataArray[2] = $UsedSlot;
	}
	$DataArray[3] = $numSignups;
	return $DataArray;
}

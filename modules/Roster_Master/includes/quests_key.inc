<?php 
/***********************************************************************
  Roster Master for Dragonfly(TM) CMS
  **********************************************************************
  Copyright (C) 2005-2018 by Dark Grue

  Access Quest data for the Quest Master management module.

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


// Array storing Access Quest data.
$Timelines['key'] = array (
	'SV' => array (
		'display' => 'ShVale',
		'name' => 'Vale of the Shattering: Dispersing the Unkempt',
		'quest_url' => 'http://eq2.wikia.com/wiki/Vale_of_the_Shattering:_Dispersing_the_Unkempt',
		'expansion' => 0,
		'req_starter' => FALSE,
		'reward' => 'Access to Shattered Vale',
		'reward_url' => 'http://eq2.wikia.com/wiki/Shattered_Vale',
		'steps' => array(
			2481550516 => array (
				'step' => 0,
				'name' => 'Vale of the Shattering: Dispersing the Unkempt'
			),
		),
	),
	'DMBC_A' => array (
		'display' => 'DMBC:A',
		'name' => 'The D\'Morte Family Crest',
		'quest_url' => 'http://eq2.wikia.com/wiki/D%27Morte_Family_Crest',
		'expansion' => 0,
		'req_starter' => TRUE,
		'reward' => 'Access to D\'Morte Burial Chambers: Acquisition',
		'reward_url' => 'http://eq2.wikia.com/wiki/Acquisition',
		'steps' => array(
			2849492472 => array (
				'step' => 0,
				'name' => 'The D\'Morte Family Crest'
			),
		),
	),
	'TotKK' => array (
		'display' => 'TotKK',
		'name' => 'Fire and Ice',
		'quest_url' => 'http://eq2.wikia.com/wiki/Fire_and_Ice',
		'expansion' => 0,
		'req_starter' => FALSE,
		'reward' => 'Access to Drayek\'s_Chamber: Throne of the Kromise King (Raid x4)',
		'reward_url' => 'http://eq2.wikia.com/wiki/Drayek%27s_Chamber',
		'steps' => array(
			3555079531 => array (
				'step' => 0,
				'name' => 'Fire and Ice'
			),
		),
	),
	'DFC' => array (
		'display' => 'DFC',
		'name' => 'Operation Green Hood',
		'quest_url' => 'http://eq2.wikia.com/wiki/Operation_Green_Hood',
		'expansion' => 0,
		'req_starter' => FALSE,
		'reward' => 'Access to Deathfist Citadel',
		'reward_url' => 'http://eq2.wikia.com/wiki/Deathfist_Citadel',
		'steps' => array(
			453346416 => array (
				'step' => 0,
				'name' => 'Operation Green Hood'
			),
		),
	),
	'Tonmerk' => array (
		'display' => 'Tonmerk',
		'name' => 'In Search of the Sepulcher of Jahnda',
		'quest_url' => 'http://eq2.wikia.com/wiki/In_Search_of_the_Sepulcher_of_Jahnda',
		'expansion' => 0,
		'req_starter' => FALSE,
		'reward' => 'Access to Tomb of Lord Tonmerk (Vermin\'s Snye)',
		'reward_url' => 'http://eq2.wikia.com/wiki/The_Sepulcher_of_Jhanda',
		'steps' => array(
			1817720687 => array (
				'step' => 0,
				'name' => 'In Search of the Sepulcher of Jahnda'
			),
		),
	),
	'DSHive' => array (
		'display' => 'DSHive',
		'name' => 'Forgotten Potion - Into the Hive',
		'quest_url' => 'http://eq2.wikia.com/wiki/Forgotten_Potion_-_Into_the_Hive',
		'expansion' => 0,
		'req_starter' => FALSE,
		'reward' => 'Access to Deathly Scarab Hive (Crypt of Betrayal)',
		'reward_url' => 'http://eq2.wikia.com/wiki/Deathly_Scarab_Hive',
		'steps' => array(
			1259642412 => array (
				'step' => 0,
				'name' => 'Forgotten Potion - Into the Hive'
			),
		),
	),
	'VotF' => array (
		'display' => 'VotF',
		'name' => 'Searching the Depths of Fallen Gate',
		'quest_url' => 'http://eq2.wikia.com/wiki/Searching_the_Depths_of_Fallen_Gate',
		'expansion' => 0,
		'req_starter' => TRUE,
		'reward' => 'Access to Vault of the Fallen (Fallen Gate)',
		'reward_url' => 'http://eq2.wikia.com/wiki/Vault_of_the_Fallen',
		'steps' => array(
			736002942 => array (
				'step' => 0,
				'name' => 'Searching the Depths of Fallen Gate'
			),
		),
	),
	'tGC' => array (
		'display' => 'tGC',
		'name' => 'The Gnoll Cave',
		'quest_url' => 'http://eq2.wikia.com/wiki/The_Gnoll_Cave',
		'expansion' => 0,
		'req_starter' => FALSE,
		'reward' => 'Access to A Gnoll Cave (Thundering Steppes)',
		'reward_url' => 'http://eq2.wikia.com/wiki/A_Gnoll_Cave',
		'steps' => array(
			2225139752 => array (
				'step' => 0,
				'name' => 'The Gnoll Cave'
			),
		),
	),
	'ColH' => array (
		'display' => 'ColH',
		'name' => 'The Torches of Forewarning and Onward to Cauldron Hollow',
		'quest_url' => 'http://eq2.wikia.com/wiki/The_Torches_of_Forewarning_and_Onward_to_Cauldron_Hollow',
		'expansion' => 0,
		'req_starter' => FALSE,
		'reward' => 'Access to Cauldron Hollow (Nektulos Forest)',
		'reward_url' => 'http://eq2.wikia.com/wiki/Cauldron_Hollow',
		'steps' => array(
			670192887 => array (
				'step' => 0,
				'name' => 'The Torches of Forewarning and Onward to Cauldron Hollow'
			),
		),
	),
	'DMBC_U' => array (
		'display' => 'DMBC:U',
		'name' => 'D\'Morte Burial Chambers',
		'quest_url' => 'http://eq2.wikia.com/wiki/D%27Morte_Burial_Chambers_%28Quest%29',
		'expansion' => 0,
		'req_starter' => TRUE,
		'reward' => 'Access to D\'Morte Burial Chambers: Unearthing',
		'reward_url' => 'http://eq2.wikia.com/wiki/D%27Morte_Burial_Chambers_%28Zone%29',
		'steps' => array(
			2247227986 => array (
				'step' => 0,
				'name' => 'D\'Morte Burial Chambers'
			),
		),
	),
	'RE_tOTR' => array (
		'display' => 'RE:tOTR',
		'name' => 'Secrets of Runnyeye',
		'quest_url' => 'http://eq2.wikia.com/wiki/Secrets_of_Runnyeye',
		'expansion' => 0,
		'req_starter' => FALSE,
		'reward' => 'Access to The Overlord\'s Throne Room (Runnyeye Citadel)',
		'reward_url' => 'http://eq2.wikia.com/wiki/Overlord%27s_Throne_Room',
		'steps' => array(
			2271580669 => array (
				'step' => 0,
				'name' => 'Secrets of Runnyeye'
			),
		),
	),
	'tLT' => array (
		'display' => 'tLT',
		'name' => 'Searching for the Silent City',
		'quest_url' => 'http://eq2.wikia.com/wiki/Searching_for_the_Silent_City',
		'expansion' => 1,
		'req_starter' => FALSE,
		'reward' => 'Access to The Living Tombs',
		'reward_url' => 'http://eq2.wikia.com/wiki/Living_Tombs',
		'steps' => array(
			1582964416 => array (
				'step' => 0,
				'name' => 'Searching for the Silent City'
			),
		),
	),
	'Poet' => array (
		'display' => 'Poet',
		'name' => 'Be Careful What You Wish For',
		'quest_url' => 'http://eq2.wikia.com/wiki/Be_Careful_What_You_Wish_For',
		'expansion' => 1,
		'req_starter' => FALSE,
		'reward' => 'Access to The Poet\'s Palace (The Shimmering Citadel)',
		'reward_url' => 'http://eq2.wikia.com/wiki/Poet%27s_Palace',
		'steps' => array(
			2304059206 => array (
				'step' => 0,
				'name' => 'Be Careful What You Wish For'
			),
		),
	),
	'Poet_R' => array (
		'display' => 'Poet:R',
		'name' => 'Capturing Day and Night',
		'quest_url' => 'http://eq2.wikia.com/wiki/Capturing_Day_and_Night',
		'expansion' => 1,
		'req_starter' => FALSE,
		'reward' => 'Access to The Poet\'s Palace: Return',
		'reward_url' => 'http://eq2.wikia.com/wiki/Poet%27s_Palace:_Return',
		'steps' => array(
			3425947420 => array (
				'step' => 0,
				'name' => 'Capturing Day and Night'
			),
		),
	),
	'FoL' => array (
		'display' => 'FoL',
		'name' => 'The Eyes of Anashti Sul',
		'quest_url' => 'http://eq2.wikia.com/wiki/The_Eyes_of_Anashti_Sul',
		'expansion' => 1,
		'req_starter' => FALSE,
		'reward' => 'Access to Fountain of Life',
		'reward_url' => 'http://eq2.wikia.com/wiki/Fountain_of_Life',
		'steps' => array(
			240650021 => array (
				'step' => 0,
				'name' => 'The Eyes of Anashti Sul'
			),
		),
	),
	'AoAx4' => array (
		'display' => 'AoAx4',
		'name' => 'Audience with the Guardian',
		'quest_url' => 'http://eq2.wikia.com/wiki/Audience_with_the_Guardian',
		'expansion' => 2,
		'req_starter' => FALSE,
		'reward' => 'Access to Ascent of the Awakened (Raid x4)',
		'reward_url' => 'http://eq2.wikia.com/wiki/Ascent_of_the_Awakened',
		'steps' => array(
			1551488254 => array (
				'step' => 0,
				'name' => 'Audience with the Guardian'
			),
		),
	),
	'RoKR' => array (
		'display' => 'RoKR',
		'name' => 'Rise of Kunark Raid Flagging Timeline',
		'quest_url' => 'http://eq2.wikia.com/wiki/Rise_of_Kunark_Raid_Flagging_Timeline',
		'expansion' => 4,
		'req_starter' => FALSE,
		'reward' => 'Access to Trakanon\'s Lair',
		'reward_url' => 'http://eq2.wikia.com/wiki/Trakanon%27s_Lair',
		'steps' => array (
			2872442041 => array (
				'step' => 0,
				'name' => 'The Fate of Norrath'
			),
			3694316591 => array (
				'step' => 1,
				'name' => 'The Amulet of Drolakis'
			),
			1161534869 => array (
				'step' => 2,
				'name' => 'Into the Bowels of the Beast'
			),
			842837251 => array (
				'step' => 3,
				'name' => 'The Secret of the Stone'
			),
			3449518442 => array (
				'step' => 4,
				'name' => 'Taking on Trakanon'
			),
		),
	),
	'BSV' => array (
		'display' => 'BSV',
		'name' => 'The Search for Bloodskull Valley',
		'quest_url' => 'http://eq2.wikia.com/wiki/The_Search_for_Bloodskull_Valley',
		'expansion' => 0,
		'req_starter' => FALSE,
		'reward' => 'Access to Bloodskull Valley: Maulic\'s Stronghold',
		'reward_url' => 'http://eq2.wikia.com/wiki/Bloodskull_Valley:_Maulic%27s_Stronghold',
		'steps' => array(
			3737641508 => array (
				'step' => 0,
				'name' => 'The Search for Bloodskull Valley'
			),
		),
	),
	'tTG' => array (
		'display' => 'tTG',
		'name' => 'The Training Grounds',
		'quest_url' => 'http://eq2.wikia.com/wiki/The_Training_Grounds',
		'expansion' => 0,
		'req_starter' => TRUE,
		'reward' => 'Access to Bloodskull Valley: The Training Grounds',
		'reward_url' => 'http://eq2.wikia.com/wiki/Bloodskull_Valley:_The_Training_Grounds',
		'steps' => array(
			1290769908 => array (
				'step' => 0,
				'name' => 'The Training Grounds'
			),
		),
	),
	'Drayek' => array (
		'display' => 'Drayek',
		'name' => 'Drayek\'s Chamber',
		'quest_url' => 'http://eq2.wikia.com/wiki/Drayek%27s_Chamber_%28Quest%29',
		'expansion' => 0,
		'req_starter' => FALSE,
		'reward' => 'Access to Drayek\'s Chamber',
		'reward_url' => 'http://eq2.wikia.com/wiki/Drayek%27s_Chamber',
		'steps' => array(
			1507294425 => array (
				'step' => 0,
				'name' => 'Drayek\'s Chamber'
			),
		),
	),
	'tPoAG' => array (
		'display' => 'tPoAG',
		'name' => 'Getting the Smarmy Sprocket Underway!',
		'quest_url' => 'http://eq2.wikia.com/wiki/Getting_the_Smarmy_Sprocket_Underway',
		'expansion' => 0,
		'req_starter' => FALSE,
		'reward' => 'Access to The Prison of Admiral Gridgadget',
		'reward_url' => 'http://eq2.wikia.com/wiki/Prison_of_Admiral_Grimgadget',
		'steps' => array(
			2644418390 => array (
				'step' => 0,
				'name' => 'Getting the Smarmy Sprocket Underway!'
			),
		),
	),
	'ToV' => array (
		'display' => 'ToV',
		'name' => 'A Key to the Past',
		'quest_url' => 'http://eq2.wikia.com/wiki/A_Key_to_the_Past',
		'expansion' => 0,
		'req_starter' => FALSE,
		'reward' => 'Access to Tomb of Valor (Stormhold)',
		'reward_url' => 'http://eq2.wikia.com/wiki/Tomb_of_Valor',
		'steps' => array(
			1001408625 => array (
				'step' => 0,
				'name' => 'A Key to the Past'
			),
		),
	),
	'SPaw' => array (
		'display' => 'SPaw',
		'name' => 'A Darkened Shard',
		'quest_url' => 'http://eq2.wikia.com/wiki/A_Darkened_Shard',
		'expansion' => 0,
		'req_starter' => TRUE,
		'reward' => 'Access to Sundered Splitpaw: Delving into the Darkness',
		'reward_url' => 'http://eq2.wikia.com/wiki/Sundered_Splitpaw:_Delving_Into_the_Darkness',
		'steps' => array(
			62103012 => array (
				'step' => 0,
				'name' => 'A Darkened Shard'
			),
		),
	),
	'CoD' => array (
		'display' => 'CoD',
		'name' => 'The Cove of Decay - Reforming Stormsunder',
		'quest_url' => 'http://eq2.wikia.com/wiki/The_Cove_of_Decay_-_Reforming_Stormsunder',
		'expansion' => 0,
		'req_starter' => FALSE,
		'reward' => 'Access to Cove of Decay',
		'reward_url' => 'http://eq2.wikia.com/wiki/Cove_of_Decay',
		'steps' => array(
			915766997 => array (
				'step' => 0,
				'name' => 'The Cove of Decay - Reforming Stormsunder'
			),
		),
	),
	'RoV_CoI' => array (
		'display' => 'RoV:CoI',
		'name' => 'Where Will This Lead Me? ',
		'quest_url' => 'http://eq2.wikia.com/wiki/Where_Will_This_Lead_Me%3F',
		'expansion' => 0,
		'req_starter' => TRUE,
		'reward' => 'Access to Ruins of Varsoon front door',
		'reward_url' => 'http://eq2.wikia.com/wiki/Ruins_of_Varsoon',
		'steps' => array(
			1248080770 => array (
				'step' => 0,
				'name' => 'Where Will This Lead Me? '
			),
		),
	),
);
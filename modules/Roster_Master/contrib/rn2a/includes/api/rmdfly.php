<?php
/**********************************************************************
 * This file is part of the RaidNinja project.
 **********************************************************************
 * Copyright (C) 2005-2007 Nate Bundy <msu.falcon <at> gmail.com>
 * Copyright (C) 2006-2007 Derek Lee <beldak.sd <at> gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $HeadURL: https://raidninja.svn.sourceforge.net/svnroot/raidninja/rn2/trunk/includes/api/rmdfly.php $
 * $Revision: 183 $
 * $Author: beldak $
 * $Date: 2007-06-29 13:37:18 -0500 (Fri, 29 Jun 2007) $
**********************************************************************/

// Make sure this file can't be accessed outside of RN
if ( !defined('IN_RN') )
{
	die("Direct access to this file is disallowed.");
}

//Set this to your Dragonfly/Roster_Master table prefix (must be on same db)


// EQ2 Dragonfly and Roster Master link
// Add a define in config.php if your Roster Master prefix differs from the default (cms_)
if (!defined(DFRM_PREFIX)) { define('DFRM_PREFIX', 'cms_'); }
define('IS_SIGNED_UP_PREFIX', DFRM_PREFIX.'roster_master_users');
define('IS_SIGNED_UP_CHARID', 'characterId');

		// Matches character name to ID
    function getCharacterName($character_id)
		{
			$query = "SELECT name_first AS name FROM ".DFRM_PREFIX."roster_master WHERE characterId = ".$character_id;
			$result = mysql_query($query);
			$row = mysql_fetch_array($result);
		  $charname = $row['name'];
			return $charname;
		}
		// Checks if a character belongs to a certain user
    function characterCheck($user_id, $character_id)
		{
			$bool = false;
			$query = "SELECT user_id FROM ".DFRM_PREFIX."roster_master_users WHERE characterId = ".$character_id;
			$result = mysql_query($query);
			$num_rows = mysql_num_rows($result);
			if ($num_rows > 0)
			{
				$temp_result = mysql_fetch_array($result);
				if ($user_id == $temp_result['user_id'])
				{
					$bool = true;
				}
			}
			
			return $bool;
		}
		
	  // Remap column names if different than 'name' and 'character_id'
 		function getCharacterAlphaList()
	  {
		  $query = "SELECT name_first AS name, characterId AS character_id FROM ".DFRM_PREFIX."roster_master ";
		  // Optional query to exclude certain ranks, uncomment next line to use.
      //$query .= "WHERE Rank <> 'Alts'";
      $query .= "ORDER BY name_first";
		  $result = mysql_query($query);
		  return $result;
	  }	  
	  
	  // Retrieves a user's signed up character ID
	  // Returns false if the user does not have a character signed up
	  function getSignedupCharacter($raid_id, $user_id)
	  {
		  $query = "SELECT cr.character_id FROM ".TABLE_PREFIX."character_raid AS cr JOIN ".DFRM_PREFIX."roster_master_users AS c ON cr.raid_id = $raid_id AND cr.character_id = c.characterId AND c.user_id = $user_id";
		  $result = mysql_query($query);
		  if (!$result)
		  {
			  $character = false;
		  }
		  else
		  {
  			$result = mysql_fetch_array($result);
			  $character = $result[0];
		  }
		  return $character;
	  }

  // Optimization for characters
  // All raids should use the same "list" of characters, so populate this list whenever
  // character objects are needed and use them for all raids
  // This should significantly reduce the amount of SQL queries needed, especially
  // with large numbers of raids
  class character_list
  {
	  var $characters = array(); // Array of all characters as character objects
	
	  // Constructs the character list
	  function character_list($user_id = '')
	  {		
		  $query = "SELECT * FROM ".DFRM_PREFIX."roster_master_users ";
		  //If you want to excluse all alts being able to signup for raids,
      //uncomment out the next line and 2nd line down change WHERE to AND
		  $query = $query."WHERE char_type = 'Primary' ";
		  if ($user_id != '') $query = $query."AND user_id = ".$user_id.' ';
		  //Delete this next line if you want alts to show up for signup
		  $query = $query."ORDER BY char_type, characterId";
		  $result = mysql_query($query);
		  $num_rows = mysql_num_rows($result);
		  for ($i = 0; $i < $num_rows; $i++)
		  {
			  $temp_result = mysql_fetch_array($result);
			  $temp_result['race_id'] == 0;
			  $query2 = "SELECT * FROM ".DFRM_PREFIX."roster_master WHERE characterId = ".$temp_result['characterId'];
			  $result2 = mysql_query($query2);
			  $temp_result2 = mysql_fetch_array($result2);

// Map class_id to the class in Roster Master.
  			  switch ($temp_result2['type_class']) {
                case "Dirge" : $temp_result2['class_id'] = 1; break;
	        case "Troubador": $temp_result2['class_id'] = 1; break;
	        case "Wizard": $temp_result2['class_id'] = 2; break;
	        case "Warlock": $temp_result2['class_id'] = 2; break;
	        case "Conjuror": $temp_result2['class_id'] = 2; break;
	        case "Necromancer": $temp_result2['class_id'] = 2; break;
	        case "Inquisitor": $temp_result2['class_id'] = 3; break;
	        case "Templar": $temp_result2['class_id'] = 3; break;
	        case "Paladin": $temp_result2['class_id'] = 4; break;
	        case "Shadow Knight": $temp_result2['class_id'] = 4; break;
	        case "Fury": $temp_result2['class_id'] = 5; break;
	        case "Warden": $temp_result2['class_id'] = 5; break;
	        case "Coercer": $temp_result2['class_id'] = 6; break;
	        case "Illusionist": $temp_result2['class_id'] = 6; break;
	        case "Bruiser": $temp_result2['class_id'] = 7; break;
	        case "Monk": $temp_result2['class_id'] = 7; break;
	        case "Brigand": $temp_result2['class_id'] = 7; break;
	        case "Swashbuckler": $temp_result2['class_id'] = 7; break;
	        case "Assassin": $temp_result2['class_id'] = 7; break;
	        case "Ranger": $temp_result2['class_id'] = 7; break;
	        case "Defiler": $temp_result2['class_id'] = 8; break;
	        case "Mystic": $temp_result2['class_id'] = 8; break;
	        case "Berserker": $temp_result2['class_id'] = 9; break;  
	        case "Guardian": $temp_result2['class_id'] = 9; break;
        }
			  $this->characters[$temp_result['characterId']] = new character($temp_result['characterId'], $temp_result['user_id'], $temp_result2['name_first'], $temp_result2['class_id'], $temp_result2['type_level'], $temp_result['race_id']);

		  }
	  }
	
	  function getCharacter($id)
	  {
		  return $this->characters[$id];
	  }
  }
?>

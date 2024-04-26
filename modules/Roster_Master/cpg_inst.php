<?php
/***********************************************************************
  Roster Master for Dragonfly(TM) CMS
  **********************************************************************
  Copyright (C) 2005-2020 by Dark Grue

  Dragonfly(TM) CMS module installer file.

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
if (!defined('ADMIN_MOD_INSTALL')) { exit; }


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
$mod_dirname = basename(dirname(__FILE__));

$matches = array();
if (preg_match("/^{$mod_basename}(?:_(\d+))?$/", $mod_dirname, $matches)) {
	$suffix = (empty($matches[1])) ? '' : "_{$matches[1]}";
} else {
	require('header.php');

	cpg_error("<strong>A fatal configuration error has occurred.</strong><br /><br />\n"
		."The module installation directory, <nobr>\"{$mod_dirname}\"</nobr>, does not follow the expected instantation syntax of <nobr>\"{$mod_basename}[_<i>&lt;integer&gt;</i>]\"</nobr>.");
}

// REQUIRED: $mod_dirname
// REQUIRED: $suffix
// *********************************************************************


eval("class $mod_dirname {
	var \$radmin;
	var \$version;
	var \$modname;
	var \$description;
	var \$author;
	var \$website;
	var \$dbtables;

	function __construct() {
		\$this->radmin = true;
		\$this->version = '9.1.1';
		\$this->modname = 'Roster Master';
		\$this->description = 'EverQuest II guild roster management, quest tracker, and dynamic signature generator.';
		\$this->author = 'Dark Grue';
		\$this->website = 'http://www.theclenchedfist.com/projects/rm4df/';
		\$this->prefix = strtolower(basename(dirname(__FILE__)));
		\$this->dbtables = array(
			'roster_master{$suffix}',
			'roster_master_guild{$suffix}',
			'roster_master_guild_rank{$suffix}',
			'roster_master_users{$suffix}',
			'roster_master_collection_status{$suffix}',
			'roster_master_quest_status{$suffix}');
	}

	function install() {
		global \$installer, \$db, \$prefix;

		\$installer->add_query('CREATE', 'roster_master{$suffix}', \"
			characterId BIGINT NOT NULL,
			last_update DECIMAL(19,7) DEFAULT 0,
			playedtime INTEGER DEFAULT 0,
			pvp_kvd INTEGER DEFAULT 0,
			pvp_deaths INTEGER DEFAULT 0,
			pvp_death_streak INTEGER DEFAULT 0,
			pvp_title_rank INTEGER DEFAULT 0,
			pvp_kill_streak INTEGER DEFAULT 0,
			pvp_last_killer BIGINT DEFAULT 0,
			pvp_total_kills INTEGER DEFAULT 0,
			pvp_city_kills INTEGER DEFAULT 0,
			pvp_wild_kills INTEGER DEFAULT 0,
			adorning SMALLINT DEFAULT 0,
			tinkering SMALLINT DEFAULT 0,
			collections_active INTEGER DEFAULT 0,
			collections_complete INTEGER DEFAULT 0,
			type_level SMALLINT DEFAULT 0,
			type_gender VARCHAR(16) DEFAULT '',
			type_birthdate_utc INTEGER DEFAULT 0,
			type_race VARCHAR(16) DEFAULT '',
			type_deity VARCHAR(16) DEFAULT '',
			type_class VARCHAR(16) DEFAULT '',
			tradeskill_class VARCHAR(16) DEFAULT '',
			tradeskill_level INTEGER DEFAULT 0,
			achievements_total_points INTEGER DEFAULT 0,
			achievements_total_count INTEGER DEFAULT 0,
			achievements_completed INTEGER DEFAULT 0,
			achievements_points INTEGER DEFAULT 0,
			statistics_kills INTEGER DEFAULT 0,
			statistics_deaths INTEGER DEFAULT 0,
			statistics_max_melee_hit BIGINT DEFAULT 0,
			statistics_items_crafted INTEGER DEFAULT 0,
			statistics_rare_harvests INTEGER DEFAULT 0,
			statistics_max_magic_hit BIGINT DEFAULT 0,
			locationdata_zonename VARCHAR(255) DEFAULT '',
			locationdata_bindzone VARCHAR(255) DEFAULT '',
			name_prefix VARCHAR(255) DEFAULT '',
			name_last VARCHAR(255) DEFAULT '',
			name_suffix VARCHAR(255) DEFAULT '',
			name_first VARCHAR(255) NOT NULL,
			arena_ctf_streak INTEGER DEFAULT 0,
			arena_ctf_timeplayed INTEGER DEFAULT 0,
			arena_ctf_kills INTEGER DEFAULT 0,
			arena_ctf_deaths INTEGER DEFAULT 0,
			arena_ctf_matches INTEGER DEFAULT 0,
			arena_ctf_captures INTEGER DEFAULT 0,
			arena_ctf_losses INTEGER DEFAULT 0,
			arena_ctf_wins INTEGER DEFAULT 0,
			arena_deathmatch_streak INTEGER DEFAULT 0,
			arena_deathmatch_timeplayed INTEGER DEFAULT 0,
			arena_deathmatch_kills INTEGER DEFAULT 0,
			arena_deathmatch_deaths INTEGER DEFAULT 0,
			arena_deathmatch_matches INTEGER DEFAULT 0,
			arena_deathmatch_wins INTEGER DEFAULT 0,
			arena_deathmatch_losses INTEGER DEFAULT 0,
			arena_idol_streak INTEGER DEFAULT 0,
			arena_idol_timeplayed INTEGER DEFAULT 0,
			arena_idol_kills INTEGER DEFAULT 0,
			arena_idol_deaths INTEGER DEFAULT 0,
			arena_idol_matches INTEGER DEFAULT 0,
			arena_idol_wins INTEGER DEFAULT 0,
			arena_idol_losses INTEGER DEFAULT 0,
			arena_idol_destroyed INTEGER DEFAULT 0,
			account_age INTEGER DEFAULT 0,
			account_link_id BIGINT DEFAULT 0,
			guild_status INTEGER DEFAULT 0,
			guild_joined BIGINT DEFAULT 0,
			guild_rank INTEGER DEFAULT 0,
			quests_active INTEGER DEFAULT 0,
			quests_complete INTEGER DEFAULT 0,
			misc_last_update DECIMAL(19,7) DEFAULT 0,
			PRIMARY KEY (characterId)\", 'roster_master{$suffix}');

		\$installer->add_query('CREATE', 'roster_master_guild{$suffix}', \"
			name VARCHAR(255) UNIQUE NOT NULL,
			value VARCHAR(255) NOT NULL\", 'roster_master_guild{$suffix}');

		\$installer->add_query('CREATE', 'roster_master_guild_rank{$suffix}', \"
			grank SMALLINT NOT NULL,
			name VARCHAR(255)\", 'roster_master_guild_rank{$suffix}');

		\$installer->add_query('CREATE', 'roster_master_users{$suffix}', \"
			characterId BIGINT NOT NULL,
			user_id MEDIUMINT UNSIGNED NOT NULL,
			char_type VARCHAR(16) NOT NULL CHECK (Char_type IN ('Primary', 'Secondary', 'Utility')),
			PRIMARY KEY (characterId)\", 'roster_master_users{$suffix}');

		\$installer->add_query('CREATE', 'roster_master_collection_status{$suffix}', \"
			id INTEGER NOT NULL AUTO_INCREMENT,
			characterId BIGINT NOT NULL,
			crc BIGINT NOT NULL,
			item_list VARCHAR(1024) NOT NULL,
			PRIMARY KEY (id)\", 'roster_master_collection_status{$suffix}');

		\$installer->add_query('CREATE', 'roster_master_quest_status{$suffix}', \"
			id INTEGER NOT NULL AUTO_INCREMENT,
			characterId BIGINT NOT NULL,
			crc BIGINT NOT NULL,
			stage_num INTEGER DEFAULT 0,
			completion_date BIGINT DEFAULT 0,
			PRIMARY KEY (id)\", 'roster_master_quest_status{$suffix}');

		// Change content field to accommodate large block cache.
		\$db->sql_query(\"ALTER TABLE {\$prefix}_blocks CHANGE content content MEDIUMTEXT\", TRUE);

		return TRUE;
	}

	function uninstall() {
		global \$db, \$prefix;

		foreach(array(
			'roster_master{$suffix}',
			'roster_master_guild{$suffix}',
			'roster_master_guild_rank{$suffix}',
			'roster_master_users{$suffix}',
			'roster_master_collection_status{$suffix}',
			'roster_master_quest_status{$suffix}') as \$table) {
			\$db->sql_query(\"DROP TABLE IF EXISTS {\$prefix}_{\$table}\", FALSE);
		}

		return TRUE;
	}


	function upgrade(\$prev_version) {
		global \$installer, \$db, \$prefix;

		if (version_compare(\$prev_version, '7.0.0', '<')) {
			require('header.php');

			cpg_error(\"<strong>Unable to upgrade from this version.</strong><br /><br />\n\"
				.\"Please backup your {\$prefix}_roster_master_users{$suffix} table, uninstall version \$prev_version, and install {\$this->version}.<br />\n\"
				.\"Note that claims will not be preserved, and must be restored from the {\$prefix}_roster_master_users{$suffix} table manually.\");

			return FALSE;
		}

		if (version_compare(\$prev_version, '8.0.0', '<')) {
			\$db->sql_query(\"TRUNCATE TABLE {\$prefix}_roster_master_guild{$suffix}\", FALSE);
			\$db->sql_query(\"CREATE TABLE {\$prefix}_roster_master_guild_rank{$suffix} (
				grank SMALLINT UNIQUE NOT NULL,
				name VARCHAR(255) NOT NULL
				)\", FALSE);
		}

		if (version_compare(\$prev_version, '9.1.0', '<')) {
			// Drop old quest tracking tables.
			\$db->sql_query(\"DROP TABLE IF EXISTS {\$prefix}_roster_master_heritage{$suffix}\", FALSE);
			\$db->sql_query(\"DROP TABLE IF EXISTS {\$prefix}_roster_master_key{$suffix}\", FALSE);
			\$db->sql_query(\"DROP TABLE IF EXISTS {\$prefix}_roster_master_timeline{$suffix}\", FALSE);
			// Add new roster table.
			\$db->sql_query(\"DROP TABLE IF EXISTS {\$prefix}_roster_master{$suffix}\", FALSE);
			\$db->sql_query(\"CREATE TABLE {\$prefix}_roster_master{$suffix} (
				characterId BIGINT NOT NULL,
				last_update DECIMAL(19,7) DEFAULT 0,
				playedtime INTEGER DEFAULT 0,
				pvp_kvd INTEGER DEFAULT 0,
				pvp_deaths INTEGER DEFAULT 0,
				pvp_death_streak INTEGER DEFAULT 0,
				pvp_title_rank INTEGER DEFAULT 0,
				pvp_kill_streak INTEGER DEFAULT 0,
				pvp_last_killer BIGINT DEFAULT 0,
				pvp_total_kills INTEGER DEFAULT 0,
				pvp_city_kills INTEGER DEFAULT 0,
				pvp_wild_kills INTEGER DEFAULT 0,
				adorning SMALLINT DEFAULT 0,
				tinkering SMALLINT DEFAULT 0,
				collections_active INTEGER DEFAULT 0,
				collections_complete INTEGER DEFAULT 0,
				type_level SMALLINT DEFAULT 0,
				type_gender VARCHAR(16) DEFAULT '',
				type_birthdate_utc INTEGER DEFAULT 0,
				type_race VARCHAR(16) DEFAULT '',
				type_deity VARCHAR(16) DEFAULT '',
				type_class VARCHAR(16) DEFAULT '',
				tradeskill_class VARCHAR(16) DEFAULT '',
				tradeskill_level INTEGER DEFAULT 0,
				achievements_total_points INTEGER DEFAULT 0,
				achievements_total_count INTEGER DEFAULT 0,
				achievements_completed INTEGER DEFAULT 0,
				achievements_points INTEGER DEFAULT 0,
				statistics_kills INTEGER DEFAULT 0,
				statistics_deaths INTEGER DEFAULT 0,
				statistics_max_melee_hit BIGINT DEFAULT 0,
				statistics_items_crafted INTEGER DEFAULT 0,
				statistics_rare_harvests INTEGER DEFAULT 0,
				statistics_max_magic_hit BIGINT DEFAULT 0,
				locationdata_zonename VARCHAR(255) DEFAULT '',
				locationdata_bindzone VARCHAR(255) DEFAULT '',
				name_prefix VARCHAR(255) DEFAULT '',
				name_last VARCHAR(255) DEFAULT '',
				name_suffix VARCHAR(255) DEFAULT '',
				name_first VARCHAR(255) NOT NULL,
				arena_ctf_streak INTEGER DEFAULT 0,
				arena_ctf_timeplayed INTEGER DEFAULT 0,
				arena_ctf_kills INTEGER DEFAULT 0,
				arena_ctf_deaths INTEGER DEFAULT 0,
				arena_ctf_matches INTEGER DEFAULT 0,
				arena_ctf_captures INTEGER DEFAULT 0,
				arena_ctf_losses INTEGER DEFAULT 0,
				arena_ctf_wins INTEGER DEFAULT 0,
				arena_deathmatch_streak INTEGER DEFAULT 0,
				arena_deathmatch_timeplayed INTEGER DEFAULT 0,
				arena_deathmatch_kills INTEGER DEFAULT 0,
				arena_deathmatch_deaths INTEGER DEFAULT 0,
				arena_deathmatch_matches INTEGER DEFAULT 0,
				arena_deathmatch_wins INTEGER DEFAULT 0,
				arena_deathmatch_losses INTEGER DEFAULT 0,
				arena_idol_streak INTEGER DEFAULT 0,
				arena_idol_timeplayed INTEGER DEFAULT 0,
				arena_idol_kills INTEGER DEFAULT 0,
				arena_idol_deaths INTEGER DEFAULT 0,
				arena_idol_matches INTEGER DEFAULT 0,
				arena_idol_wins INTEGER DEFAULT 0,
				arena_idol_losses INTEGER DEFAULT 0,
				arena_idol_destroyed INTEGER DEFAULT 0,
				account_age INTEGER DEFAULT 0,
				account_link_id BIGINT DEFAULT 0,
				guild_status INTEGER DEFAULT 0,
				guild_joined BIGINT DEFAULT 0,
				guild_rank INTEGER DEFAULT 0,
				quests_active INTEGER DEFAULT 0,
				quests_complete INTEGER DEFAULT 0,
				misc_last_update DECIMAL(19,7) DEFAULT 0,
				PRIMARY KEY (characterId))\", FALSE);
			// Add new collection status table.
			\$db->sql_query(\"DROP TABLE IF EXISTS {\$prefix}_roster_master_collection_status{$suffix}\", FALSE);
			\$db->sql_query(\"CREATE TABLE {\$prefix}_roster_master_collection_status{$suffix} (
				id INTEGER NOT NULL AUTO_INCREMENT,
				characterId BIGINT NOT NULL,
				crc BIGINT NOT NULL,
				item_list VARCHAR(1024) NOT NULL,
				PRIMARY KEY (id))\", FALSE);
			// Add new quest status table.
			\$db->sql_query(\"DROP TABLE IF EXISTS {\$prefix}_roster_master_quest_status{$suffix}\", FALSE);
			\$db->sql_query(\"CREATE TABLE {\$prefix}_roster_master_quest_status{$suffix} (
				id INTEGER NOT NULL AUTO_INCREMENT,
				characterId BIGINT NOT NULL,
				crc BIGINT NOT NULL,
				stage_num INTEGER DEFAULT 0,
				completion_date BIGINT DEFAULT 0,
				PRIMARY KEY (id))\", FALSE);
		}

		// Change content field to accommodate large block cache.
		\$db->sql_query(\"ALTER TABLE {\$prefix}_blocks CHANGE content content MEDIUMTEXT\", TRUE);

		return TRUE;
	}
}");

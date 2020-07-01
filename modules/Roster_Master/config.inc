<?php
/***********************************************************************
  Roster Master for Dragonfly(TM) CMS
  **********************************************************************
  Copyright (C) 2005-2020 by Dark Grue

  Configuration file for Roster Master for Dragonfly(TM) CMS module.

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


// *************************** Setup Options ***************************
// guild_id: Set this to the guild id of your guild on the everquest2.com site. It
//		can be found using the Lookup Guild ID function of the Roster Master
$config['guild_id'] = 2566680537;

// popup:
//		1 - Open external links in a new window. (Default)
//		0 - Open external links in the same window.
$config['popup'] = 1;

// ********************** Roster Feature Options ***********************
// officers: Defines Guild Ranks that are officers BY RANK NUMBER. Used by the avatar
//		feature of Roster Master and Signature Master.
//		Default is 'Leader', 'Senior Officer', 'Officer'.
$config['officers'] = array(0, 1, 2);

// show alts:
//		1 - Display all character types. (Default)
//		0 - Do not show Secondary/Utility character types by default. Allow full
//			list to display by link.
$config['show_alts'] = 1;

// show updated:
//		1 - Display a row at the top of the table with the last updated date
//			and total users. (Default)
//		0 - Do not show last updated/total users.
$config['show_updated'] = 1;

// last updated date format: Specifies the date format string for the roster last updated date.
//		See the PHP strftime() function for formatting options.
//		Default is '%A, %e %b, %Y, %r'.
$config['updated_date_fmt'] = '%A, %e %b, %Y, %r';

// show adv sort:
//		1 - Show advanced sort dropdowns and sort button. (Default)
//		0 - Do not show Advanced sort.
$config['show_adv_sort'] = 1;

// default sort options:
$config['pri_sort_default'] = 'gr';
$config['pri_sort_dir_default'] = 'd';
$config['sec_sort_default'] = 'al';
$config['sec_sort_dir_default'] = 'd';

// sort indicator:
//		1 - Show column sort direction indicators. (Default)
//		0 - Do not show column sort direction indicators.
$config['sort_indicator'] = 1;

// list grouping:
//		1 - Group list by primary sort option.
//		0 - Do not group list. (Default)
$config['group_by'] = 0;

// roster display columns:
//		Choices are 'a', 'u', 'ct', 'n', 'gr', 'r', 'ac', 'al', 'acl', 'tc', 'tl', 'tcl', '2tsa', '2tst', 'q', 's'.
//		Default is array('u', 'ct', 'n', 'gr', 'r', 'acl', 'tcl', '2tsa', '2tst', 'q', 's').
$config['columns'] = array('u', 'ct', 'a', 'n', 'gr', 'r', 'acl', 'tcl', '2tsa', '2tst', 'q', 's');

// short heading titles:
//		ALWAYS - Always use short heading titles.
//		NEVER - Never use short titles.
//		<NULL> - Use short heading titles only when columns are grouped. (Default)
$config['short_titles'] = '';

// roster avatars:
//		ARCH - Displays one of four icons, depending on the character's
//			class archetype (fighter, mage, priest, or scout). (Default)
//		CLASS - Uses one of 24 images, depending on the character's class.
//		EQ2 - Displays a randomly selected EQ2 icon.
//		RANK - Display one of two icons, officers are differentiated.
$config['avatars'] = 'ARCH';

// avatar size:
//		Sets the width and height attributes for avatars in the roster.
//		NOTE: This does not pre-scale the avatar image, if you are setting
//		100 x 100 images smaller, remember that the client will request (and
//		get) the full-sized image, then the client browser will scale it down
//		internally. If you intend to use smaller avatars, it is most
//		efficient to store them at the appropriate size. Also note that if
//		you use irregularly-size avatars, they will be forced to the
//		specified geometry (will will result in distortion of the image).
$config['avatar_size']['width'] = 50;
$config['avatar_size']['height'] = 50;

// cache update time:
//		The time, in seconds, after which the cache is refreshed on a page load.
//		It is suggested that you leave this at at least 1 hour so you don't hit
//		Daybreak's site more often than you have to.
//		Setting this to zero turns off automatic updating and allows a
//		forced update from localhost:
//		wget --bind-address=127.0.0.1 --output-document=/dev/null --quiet 'http://<yoursitename>/index.php?name=Roster_Master&force_update=1'
//		Default is 60 * 60 * 1 = 3600 seconds (1 hour)
$config['cache_update_time'] = 60 * 60 * 1;

// logfile:	Bitfield option, add to set options (i.e. 0 = never log, 511 = always
//		log everything). The server must have read/write access to the
//		modules/Roster_Master/logfile.inc file in order to use the logging
//		feature (e.g. chown apache:apache logfile.inc).
//		0 - Do not use the logfile (log nothing).
//		2 - Always write logfile, even if no changes would be logged.
//		8 - * Log fetch retry attempts.
//		16 - * Log fetch failures.
//		The following flags force logging, as statistics always are created:
//		64 - Log roster display timing statistics.
//		128 - Log parser timing statistics.
//		Default is 24.
$config['logfile'] = 24;

// exclude fields from logfile:
//		Don't log changes to these fields (reduces logfile chattiness).
$config['exclude_log_field'] = array('/^playedtime$/', '/^last_update$/', '/^account_age$/', '/^statistics_/', '/^locationdata_zonename$/');

// logfile date format:
//		Specifies the date format string for the logfile. See the PHP
//		date() function for formatting options.
//		Default is 'Y-m-d H:i:s' and 'Z'. NOTE: 'suffix' should be only
//		'Z' or 'UTC', as the time is stored in that timezone, changing
//		the suffix identifier does not actually reformat the time for any
//		other timezone.
$config['logfile_date_fmt'] = array(
	'format' => 'Y-m-d H:i:s',
	'suffix' => 'Z');

// number of retries:
//		After the first try, Roster Master will pause for 3^n seconds, where n
//		is the retry number.
//		Default is 0. SETTING THIS ABOVE 2 IS NOT RECOMMENDED!
$config['retries'] = 0;

// read file hack:
//		This hack forces Roster Master to read the roster from a local file, rather
//		than from the network. This assumes that the guild.json and character.json
//		files exist in the Roster Master module directory.
//		This feature is a *hack*, and unsupported. Please don't ask about it.
//		1 - Read roster data from the local JSON file.
//		0 - Read roster data from Census. (Default)
$config['read_file_hack'] = 0;

// **********************  Quest Tracker Options ***********************
// scrollable table:
//		1 - Use scrollable table in quest tracker (requires JavaScript). (Default)
//		0 - Do not use scrollable table in quest tracker.
$config['qm_scrolltable'] = 1;

// row highlighter:
//		1 - Use row highlighter in quest tracker (requires JavaScript). (Default)
//		0 - Do not use row highlighter in quest tracker.
$config['qm_rowhighlight'] = 1;

// row highlight color:
//		This is the color that the row gets set to when the user clicks on a row.
$config['rowhighlight_color'] = 'gold';

// ******************** Signature Generator Options ********************
// dynamic sigs:
//		This feature requires GD 2.0.1 or later and the FreeType library.
//		The server must have read/write access to the
//		modules/Roster_Master/signatures directory in order to use this feature
//		(chown apache:apache signatures/ is suggested).
//		You must turn ON Dragonfly's LEO feature in order to ensure that files
//		are cached properly by client browsers.
//		1 - Offer/create dynamic signatures. (Default)
//		0 - Disable signatures.
$config['dynamic_sigs'] = 1;

// exclude ranks:
//		Array of ranks to be excluded from the ability to create a signature BY 
//		RANK NUMBER. Add 666 to this to exclude characters that do not return
//		rank data.
//		Default is 'Recruit', 'Initiate'. 
$config['exclude_ranks'] = array(6, 7);

// signature block basename:
//		The base name (without file extension) of the signature block image
//		PNG file(s). This file must be located in the
//		"modules/{$mod_dirname}/images/sigblocks/" directory and readable by
//		the web server user. If this file cannot be found, Signature Master
//		will fall back to using "default_sigblock.png" as the file name
//		before giving up. The template that Signature Master will look for
//		must be named the same as the signature block image file or
//		sigblock_basename setting (case sensitive), but with the 'txt'
//		extension.
//		The 'default_sigblock' setting is the default RM4DF logo background image.
$config['sigblock_basename'] = 'default_sigblock';

// sigblock base image type:
//		ARCH - Uses one of four base images, depending on the character's
//			class archetype (fighter, mage, priest, or scout).
//		CLASS - Uses one of 24 base images, depending on the character's
//			class.
//		RANK - Display one of two base images, officers are differentiated.
//		<NULL> - Only one base image is used. (Default)
$config['sigblock_type'] = '';

// JPEG quality:
//		The quality of the output JPEG file. Ranges from 0 (worst quality,
//		smaller file) to 100 (best quality, biggest file). The default is 90.
//		Decreasing this value (the default IJG quality value is 75) can
//		decrease the resultant image filesize at the expense of increased
//		pixellation and other "noise"
$config['JPEG_quality'] = 90;

// force signature cache:
//		1 - Force selective regeneration of the signature block cache every time
//			the roster data is updated.
//			Using this feature may cause Roster Master to exceed the maximum
//			script execution time (see source for explanation).
//		0 - Do not force regeneration of the signature block cache. (Default)
$config['force_sig_cache'] = 0;

// signature image rewrite hack:
//		Experimental rewrite rule to work around BBCode IMG tag URI filters that
//		prevent linking directly to a URI containing a query string. The URL to
//		use should be of the form:
//		{$baselink}/modules/Roster_Master{_moduleInstanceID}/sig_{characterId}.jpg
//		but requires that Apache be complied with mod_rewrite.c enabled and
//		.htaccess modification of rewrite rules must be permitted (AllowOverride
//		FileInfo Options). Turning this feature on causes Signature Master to
//		display links to the signature images that masquerade as a link to a
//		JPEG file instead of a link to a Roster Master module query.
//		This feature is *experimental*, and has limited support. The rewrite
//		hack should "just work," (especially if Dragonfly's LEO feature is
//		working	correctly) but troubleshooting will require advanced
//		configuration and administration skills. Also note that you may need
//		to edit the .htaccess file in the Roster Master base directory
//		to set the RewriteBase option. Check the same directive in the
//		.htaccess file in the base Dragonfly directory for guidance. 
//		1 - Masquerade signature image as a link to a JPEG file.
//		0 - Link images directly to module query. (Default)
$config['sig_image_rewrite_hack'] = 0;

// *********************** Block Feature Options ***********************
// show alts:
//		1 - Display stats for all character types. (Default)
//		0 - Do not include stats for Secondary/Utility character types.
$config['block_show_alts'] = 1;

// show general guild stats:
//		1 - Display the general guild stats in block. (Default)
//		0 - Do not show general guild stats in block.
$config['show_genstats'] = 1;

// show extended guild stats:
//		1 - Display the extended guild stats in block. (Default)
//		0 - Do not show extended guild stats in block.
$config['show_extstats'] = 1;
// show Arena stats:
//		1 - Display Arena stats in block. (Default)
//		0 - Do not show Arena stats in block.
$config['show_Arena'] = 0;
// show PvP stats:
//		1 - Display PvP stats in block.
//		0 - Do not show PvP stats in block. (Default)
$config['show_PvP'] = 0;

// show leaderboard:
//		1 - Display the character leaderboard in block. (Default)
//		0 - Do not show the character leaderboard in block.
$config['show_leaders'] = 1;

// show class breakdown:
//		1 - Display the class breakdown in block. (Default)
//		0 - Do not show the class breakdown in block.
$config['show_breakdown'] = 1;

// breakdown shownames:
//		1 - Display character names in class breakdown.
//		0 - Do not list character names in class breakdown. (Default)
//			If you turn this feature on, the "content" field in the
//			blocks table must be of type MEDIUMTEXT, or the cached
//			block will be truncated.
$config['breakdown_shownames'] = 0;

// block autoupdate:
//		1 - Autoupdate module on block load. This can delay loading of main site
//			page if updating the data takes a long time.
//		0 - Do not update module on block load. (Default)
$config['block_autoupdate'] = 0;

// ***************** END OF USER-CONFIGURATION OPTIONS *****************

// Service ID prevents rate limiting to 10 requests per minute.
$config['service_id'] = 's:rm4df/';

// Server URL has to have server name calculated and appended within the module.
// EQ2Players requires authentication and does not support 3rd-party authentication.
$config['server_url'] = 'http://players.everquest2.com/';
$config['guild_url'] = "http://u.eq2wire.com/soe/guild_detail/{$config['guild_id']}/";
$config['census'] = "http://census.daybreakgames.com/{$config['service_id']}";
$config['json_guild_url'] = $config['census'].'get/eq2/guild/'.$config['guild_id'];
$config['json_roster_url'] = $config['census'].'get/eq2/character/?guild.id='.$config['guild_id']
	.'&c:limit=999'
	.'&c:show='
	.'visible,'
	.'dbid,'
	.'last_update,'
	.'displayname,'
	.'playedtime,'
	.'pvp,'
	.'secondarytradeskills,'
	.'collections,'
	.'type,'
	.'tradeskills,'
	.'achievements,'
	.'statistics,'
	.'locationdata,'
	.'name,'
	.'arena,'
	.'account,'
	.'guild,'
	.'quests'
	.'&c:join=character_misc^on:id'
	.'^inject_at:character_misc'
	.'^show:last_update\'collection_list\'quest_list.crc\'quest_list.stage_num\'completed_quest_list';
$config['char_url'] = 'http://u.eq2wire.com/soe/character_detail/';

// Array mapping classes onto archetypes.
$config['archetypes'] = array(
	'Fighter' => array('Fighter', 'Warrior', 'Berserker', 'Guardian', 'Brawler', 'Bruiser', 'Monk', 'Crusader', 'Paladin', 'Shadowknight'),
	'Mage' => array('Mage', 'Sorceror', 'Warlock', 'Wizard', 'Enchanter', 'Illusionist', 'Coercer', 'Summoner', 'Necromancer', 'Conjuror'),
	'Priest' => array('Priest', 'Cleric', 'Templar', 'Inquisitor', 'Druid', 'Warden', 'Fury', 'Shaman', 'Defiler', 'Mystic', 'Channeler'),
	'Scout' => array('Scout', 'Rogue', 'Brigand', 'Swashbuckler', 'Bard', 'Dirge', 'Troubador', 'Predator', 'Assassin', 'Ranger', 'Beastlord'));

// FIXME - TODO: Update version number before releasing source. Update sigblock templates with major version.
$config['version_string'] = 'v9 p1.2';

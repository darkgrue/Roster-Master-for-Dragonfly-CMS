<?php 
/***********************************************************************
  Roster Master for Dragonfly(TM) CMS
  **********************************************************************
  Copyright (C) 2005-2018 by Dark Grue

  Data parser module. Retrieves roster and guild summary data.

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
// Load quest data.
require_once("modules/{$mod_dirname}/includes/quests_data.inc");


// ***************************** FUNCTIONS *****************************
// Function:	formatBytes()
// What it does:	convert bytes to a more readable format.
function formatBytes($bytes, $precision = 2) {
	$type = array('', 'kilo', 'mega', 'giga', 'tera', 'peta', 'exa', 'zetta', 'yotta');
	$idx = 0;
	while($bytes >= 1024) {
		$bytes /= 1024;
		$idx++;
	}

	return(round($bytes, $precision).' '.$type[$idx].'bytes');
}

// Function:	fetch_URL2()
// What it does:	Opens a HTTP/1.0 connection to retrieve data from a network resource.
function fetch_URL2($url, $return_status = FALSE, $referer = NULL, $depth = 0) {
	// Split URL parameter into its components.
	$url_parsed = parse_url($url);
	$host = $url_parsed['host'];
	$port = (empty($url_parsed['port'])) ? 80 : $url_parsed['port'];
	$path = ((empty($url_parsed['path'])) ? '/' : $url_parsed['path'])
		.((empty($url_parsed['query'])) ? '' : "?{$url_parsed['query']}")
		.((empty($url_parsed['fragment'])) ? '' : "#{$url_parsed['fragment']}");

	if (!empty($host)) {
		// Initiate a socket connection, 30 second timeout on the socket.
		$errno = 0;
		$errstr = '';
		$timeout = 30;
		$resource = fsockopen($host, $port, $errno, $errstr, $timeout);

		if ($resource) {
			// Send HTTP/1.0 request.
			trigger_error(_ERR_URL.$url, E_USER_NOTICE);
			fwrite($resource,
				"GET $path HTTP/1.0\r\n"
				."Host: $host\r\n"
				.(($referer) ? "Referer: $referer\r\n" : '')
				."\r\n");

			// Set stream to non-blocking mode.
			if (stream_set_blocking($resource, FALSE)) {
				trigger_error(_ERR_STREAMBLOCKING, E_USER_NOTICE);
			}

			stream_set_timeout($resource, $timeout);
			$info = stream_get_meta_data($resource);

			// Read stream into a string.
			$response = '';
			while((!feof($resource)) && (!$info['timed_out'])) {
				$response .= fgets($resource, 4096);
				$info = stream_get_meta_data($resource);
			}

			if ($info['timed_out']) {
				trigger_error(_ERR_STREAMTIMEOUT, E_USER_NOTICE);
			}

			// Close the socket.
			fclose($resource);

			// Split header and body.
			$pos = strpos($response, "\r\n\r\n");

			// Malformed response, bail out.
			if($pos === FALSE) return NULL;

			$header = substr($response, 0, $pos);
			$body = substr($response, $pos + 4);

//			trigger_error('<pre style="text-align:left">'.htmlspecialchars($header)."</pre>\n", E_USER_NOTICE);
//			cpg_error('<pre style="text-align:left">'.htmlspecialchars($body)."</pre>\n");

			// Parse response headers.
			$headers = array();
			$char_infos = explode("\r\n", $header);
			foreach($char_infos as $char_info) {
				if(($pos = strpos($char_info, ':')) !== FALSE) {
					$headers[strtolower(trim(substr($char_info, 0, $pos)))] = trim(substr($char_info, $pos + 1));
				}
			}

			$statuscode = trim(substr($char_infos[0], 9, 4));
			$statusclass = substr($statuscode, 0, 1);
//			trigger_error('<pre style="text-align:left">'.print_r($headers, TRUE)."\nStatus-Code: $statuscode\nStatus-Class: $statusclass</pre>\n", E_USER_NOTICE);
			if (($statusclass != '2') && ($statusclass != '7')) {
				trigger_error(sprintf(_ERR_HTTPRESPONSEFAIL, $statuscode, htmlspecialchars($path)), E_USER_NOTICE);
				return $statuscode;
			}

			// Redirection?
			// Only allow a maximum of 5 redirects.
			if(isset($headers['location']) && ($depth < 5)) {
				trigger_error(_ERR_URLREDIRECT.htmlspecialchars($headers['location']), E_USER_NOTICE);
				return fetch_URL($headers['location'], $return_status, $referer, ++$depth);
			} else {
				if ($return_status) {
					return $statuscode;
				} else {
					return $body;
				}
			}
		}
	}

	return NULL;
}

// Function:	fetch_URL()
// What it does:	Opens a cURL session to retrieve a network resource.
function fetch_URL($url, $return_status = FALSE, $referer = NULL, $depth = 0) {
	global $config;

	if (extension_loaded('cURL')) {
		$ch = curl_init($url);
		$user_agent = 'Roster Master'.$config['version_string'];
		curl_setopt_array($ch, array(
			CURLOPT_CUSTOMREQUEST  => 'GET',       // set request type post or get
			CURLOPT_POST           => FALSE,       // set to GET
			CURLOPT_USERAGENT      => $user_agent, // set user agent
			CURLOPT_RETURNTRANSFER => TRUE,        // return web page
			CURLOPT_HEADER         => FALSE,       // return headers
			CURLOPT_FOLLOWLOCATION => TRUE,        // follow redirects
			CURLOPT_ENCODING       => '',          // handle all encodings
			CURLOPT_FAILONERROR    => TRUE,        // fail verbosely if the HTTP code returned is greater than or equal to 400
			CURLOPT_AUTOREFERER    => TRUE,        // set referer on redirect
			CURLOPT_CONNECTTIMEOUT => 60,          // timeout on connect
			CURLOPT_TIMEOUT        => 180,         // timeout on response
			CURLOPT_MAXREDIRS      => 10,          // stop after 10 redirects
			CURLOPT_SSL_VERIFYHOST => FALSE,       // don't verify SSL
			CURLOPT_SSL_VERIFYPEER => FALSE,       // don't verify SSL
			CURLOPT_FILETIME       => FALSE        // don't get the remote time of the retrieved document
		));
//		curl_setopt($ch, CURLOPT_HTTPHEADER, array('Accept-Encoding: gzip'));
		if ($referer) { curl_setopt($ch, CURLOPT_REFERER, $referer); }

		trigger_error(_ERR_URL.$url, E_USER_NOTICE);
		$content = curl_exec($ch);
//		cpg_error('<pre style="text-align:left">'.print_r(curl_getinfo($ch), TRUE)."</pre>\n");
//		cpg_error('<pre style="text-align:left">'.htmlspecialchars($content)."</pre>\n");

		if(curl_errno($ch)) {
			// FATAL: Log the fetch failure.
			$error = sprintf(_ERR_CURLERR, curl_error($ch));
			trigger_error($error, E_USER_NOTICE);
			if ($config['logfile'] & 16) { $GLOBALS['log'][] = $error; }

			if ($return_status) {
				$statuscode = curl_getinfo($ch, CURLINFO_HTTP_CODE);
				curl_close($ch);

				return $statuscode;
			} else {
				curl_close($ch);
				return NULL;
			}
		} else {
			$csd = (int)curl_getinfo($ch, CURLINFO_SIZE_DOWNLOAD);
			$cs = (int)strlen($content);
			trigger_error(sprintf(_ERR_CURLINFO_SIZE_DOWNLOAD, formatBytes($csd), formatBytes($cs), (($csd != 0) ? ((1 - ($csd / $cs)) * 100) : 0)), E_USER_NOTICE);

			if ($return_status) {
				$statuscode = curl_getinfo($ch, CURLINFO_HTTP_CODE);
				curl_close($ch);

				return $statuscode;
			} else {
				curl_close($ch);

				return $content;
			}
		}
	} else {
		trigger_error(_ERR_CURLNOTFOUND, E_USER_NOTICE);
		return fetch_URL2($url, $return_status, $referer, $depth);
	}
}		

// Function:	get_JSON()
// What it does:	Retrieves the JSON guild data from a file or the Census API.
function get_JSON($file = '', $url = '') {
	global $config, $mod_dirname;

	// Start data retrieval duration timer.
	$timer_r = get_microtime(TRUE);

	// Fetch and parse the JSON data.
	if ($config['read_file_hack'] && ($file != '')) {
		$filename = "modules/{$mod_dirname}/{$file}.json";
		if (file_exists($filename)) {
			$handle = @fopen($filename, 'r');
			if ($handle) {
				$raw_data = fread($handle, filesize($filename));
				fclose($handle);
			} else {
				// Failed to open file handle.
				trigger_error(sprintf(_ERR_FILEHANDLEFAIL, $filename), E_USER_NOTICE);

				return FALSE;
			}
		} else {
			// File does not exist.
			trigger_error(sprintf(_ERR_FILENOTFOUND, $filename), E_USER_NOTICE);

			return FALSE;
		}
	} else {
		// Increase the time a script is allowed to run.
		// This function has no effect when PHP is running in safe mode. There
		// is no workaround other than turning off safe mode or changing the
		// time limit in the php.ini file.
		// The current effective setting of safe mode and the PHP
		// max_execution_time directive can be viewed in the Dragonfly
		// administration panel at 
		// "Administration Menu > > System Info > > PHP Core".
		// NOTE: The Apache TimeOut directive (default of 300 seconds) is
		// another limiting factor, the shorter of the two durations will be
		// effective.
		set_time_limit(300);

		$i = 0;
		$rlimit = $config['retries'] + 1;
		do {
			if ($raw_data = fetch_URL($url)) {
				if(strpos($raw_data, 'Missing Service ID.') === FALSE) {
					// Got the data, bail out of the loop.
					break;
				} else {
					trigger_error(_ERR_NOSERVICEID, E_USER_NOTICE);

					// Sleep for 60 seconds.
					$stime = 60;
				}
			} else {
				// Sleep for 3^n seconds...
				$stime = pow(3, $i++);
			}

			if ($i < $rlimit) {
				// Log the fetch retry.
				$error = sprintf(_ERR_JSON_FAIL1, $stime);
				trigger_error($error, E_USER_NOTICE);
				if ($config['logfile'] & 8) { $GLOBALS['log'][] = $error; }

				// Sleep for holdoff period.
				sleep($stime);
			}
		} while($i < $rlimit);
	}

	// Calculate data retrieval duration in seconds.
	$timer_r = get_microtime(TRUE) - $timer_r;

	if (empty($raw_data)) {
		// Unable to read the URL as presented
		trigger_error(_ERR_JSON_FAIL2a."<br />\n"
			.sprintf(_ERR_JSON_FAIL2b, "<a href=\"{$url}\">{$url}</a>"), E_USER_NOTICE);

		// FATAL: Log the fetch failure.
		if ($config['logfile'] & 16) { $GLOBALS['log'][] = _ERR_JSON_FAIL2a; }

		return FALSE;
	}
//	cpg_error('<pre style="text-align:left">'.htmlspecialchars($raw_data)."</pre>\n");

/*
	// Remove unsupported characters, see http://www.php.net/chr for details.
	for ($i = 0; $i <= 31; ++$i) { $json = str_replace(chr($i), '', $json); }
	$json = str_replace(chr(127), '', $json);
	// Remove the BOM (Byte Order Mark).
	if (0 === strpos(bin2hex($json), 'efbbbf')) { $json = substr($json, 3); }
*/
	$json = json_decode($raw_data, TRUE);

//	cpg_error('<pre style="text-align:left">ERROR'.var_dump_r(json_last_error()).', '.json_last_error_msg()."</pre>\n");
//	cpg_error('<pre style="text-align:left">'.count($json)." elements parsed.\n\n\$json = ".htmlspecialchars(var_dump_r($json))."</pre>\n");
	if (json_last_error() !== JSON_ERROR_NONE) {
		trigger_error(json_last_error_msg());

		// FATAL: Log the fetch failure.
		if ($config['logfile'] & 16) { $GLOBALS['log'][] = _ERR_JSON_FAIL2a; }

		return FALSE;
	}
//	cpg_error('<pre style="text-align:left">'.count($json)." elements parsed.\n\n\$json = ".htmlspecialchars(var_dump_r($json))."</pre>\n");

	// Log the performance statistics.
	$error = sprintf(_NTC_GETJSON, $file, formatBytes(strlen($raw_data)), $timer_r);
	trigger_error($error, E_USER_NOTICE);
	if ($config['logfile'] & 128) { $GLOBALS['log'][] = $error; }

	return $json;
}

// Function:	parse_guild_JSON()
// What it does:	Parses the JSON guild collection data from the Census API.
function parse_guild_JSON() {
	// Start function duration timer.
	$timer_t = $timer_r = get_microtime(TRUE);

	global $config, $mod_dirname;

	// Get the JSON data.
	if (!($json = get_JSON('guild_list', $config['json_guild_url']))) {
		// Fetch failed, return.
		return FALSE;
	}
//	cpg_error('<pre style="text-align:left">'.count($json)." elements parsed.\n\n\$json = ".htmlspecialchars(var_dump_r($json))."</pre>\n");

	// Calculate function duration in seconds.
	$timer_r = get_microtime(TRUE) - $timer_r;

	// ####### PARSER STARTS #######

	$row = array();
	// #### guild: Parse guild information.
	$row['accounts'] = (int)$json['guild_list'][0]['accounts'];
	$row['alignment'] = (int)$json['guild_list'][0]['alignment']; // usage?
	$row['Date Formed'] = (int)$json['guild_list'][0]['dateformed'];
	//last_update
	$row['version'] = (int)$json['guild_list'][0]['version']; // usage?
	$row['worldid'] = (int)$json['guild_list'][0]['worldid'];
	$row['members'] = (int)$json['guild_list'][0]['members'];
	$row['Server'] = (string)$json['guild_list'][0]['world'];
	$row['Guild Name'] = (string)$json['guild_list'][0]['name'];
	$row['guildxp'] = (int)$json['guild_list'][0]['guildxp'];
	$row['Guild Level'] = (int)$json['guild_list'][0]['level'];
	$row['Guild Status'] = (int)$json['guild_list'][0]['guildstatus']; // not useful, always shows as 0 in the feed
	$row['guildid'] = (int)$json['guild_list'][0]['guildid']; // usage?
	$row['id'] = (int)$json['guild_list'][0]['id'];
	// #### rank_list: Parse guild ranks.
	$config['lookup_rank'] = array();
	if (!empty($json['guild_list'][0]['rank_list'])) {
		foreach($json['guild_list'][0]['rank_list'] as $rank) {
			$config['lookup_rank'][(int)$rank['id']] = (string)$rank['name'];
		}
	}
	// #### recruting: Not currently parsed.
	// #### member_list: Parse full memberlist (including private/missing ones).
	// #### member_list: Not currently parsed.
	// #### personalitytag_list: Not currently parsed.
	// #### event_list: Not currently parsed.

	// ###### PARSER ENDS ######

	// Calculate function duration in seconds.
	$timer_t = get_microtime(TRUE) - $timer_t;

	// Log the performace statistics.
	$error = sprintf(_NTC_JSONPARSE, 'guild_list', $timer_r, ($timer_t - $timer_r), $timer_t);
	trigger_error($error, E_USER_NOTICE);
	if ($config['logfile'] & 128) { $GLOBALS['log'][] = $error; }

	return $row;
}

// Function:	parse_character_JSON()
// What it does:	Parses the JSON character collection data from the Census API.
function parse_character_JSON() {
	// Start function duration timer.
	$timer_t = $timer_r = get_microtime(TRUE);

	global $config, $mod_dirname, $table_updated, $QuestsData;

	// Get the JSON data.
	if (!($json = get_JSON('character_list', $config['json_roster_url']))) {
		// Fetch failed, return.
		return FALSE;
	}
//	cpg_error('<pre style="text-align:left">'.count($json)." elements parsed.\n\n\$json = ".htmlspecialchars(var_dump_r($json))."</pre>\n");

	// Calculate function duration in seconds.
	$timer_r = get_microtime(TRUE) - $timer_r;

	// ###### PARSER STARTS ######
	$parsed_roster_data = array();

	// Read the contents of the database.
	$parsed_roster_data['roster_delete'] = read_roster_db();
//	cpg_error('<pre style="text-align:left">'.count($parsed_roster_data['roster_delete'])." elements parsed.\n\n\$parsed_roster_data['roster_delete'] = ".htmlspecialchars(print_r($parsed_roster_data['roster_delete'], TRUE))."</pre>\n");

	foreach($json['character_list'] as $charobj) {
		$row['characterId'] = $parsed_roster_data['roster']['character_list'][] = (int)$charobj['id'];
		$row['last_update'] = round($charobj['last_update'], 4);
		$previous_update = ((empty($parsed_roster_data['roster_delete'][$row['characterId']]['last_update'])) ? 0 : round($parsed_roster_data['roster_delete'][$row['characterId']]['last_update'], 4));
		$row['misc_last_update'] = round($charobj['character_misc']['last_update'], 4);
		$misc_previous_update = (float)((empty($parsed_roster_data['roster_delete'][$row['characterId']]['misc_last_update'])) ? 0 : round($parsed_roster_data['roster_delete'][$row['characterId']]['misc_last_update'], 4));
/*
		if (!empty($parsed_roster_data['roster_delete'][$row['characterId']])) {
			trigger_error("{$parsed_roster_data['roster_delete'][$row['characterId']]['name_first']} ({$row['characterId']}),<br />"
			."\$row['last_update'] = {$row['last_update']} (".date('d.m.Y H:i:s', $row['last_update'])."), \$previous_update = {$previous_update} (".date('d.m.Y H:i:s', $previous_update)."),<br />"
			."\$row['misc_last_update'] = {$row['misc_last_update']} (".date('d.m.Y H:i:s', $row['misc_last_update'])."), \$misc_previous_update = {$misc_previous_update} (".date('d.m.Y H:i:s', $misc_previous_update).")"
			, E_USER_NOTICE);
		}
*/

		if ($row['last_update'] > $previous_update) {
			// #### character: Parse character data.
			// visible
			// dbid
			//$row['last_update'] = (float)$charobj['last_update'];
			$row['playedtime'] = (int)$charobj['playedtime'];
			//$row['characterId'] =(int)$charobj['id'];
			// #### pvp: Parse character PvP statistics.
			$row['pvp_kvd'] = (int)$charobj['pvp']['kvd'];
			$row['pvp_deaths'] = (int)$charobj['pvp']['deaths'];
			$row['pvp_death_streak'] = (int)$charobj['pvp']['death_streak'];
			$row['pvp_title_rank'] = (int)$charobj['pvp']['title_rank'];
			$row['pvp_kill_streak'] = (int)$charobj['pvp']['kill_streak'];
			$row['pvp_last_killer'] = (int)$charobj['pvp']['last_killer'];
			$row['pvp_total_kills'] = (int)$charobj['pvp']['total_kills'];
			$row['pvp_city_kills'] = (int)$charobj['pvp']['city_kills'];
			$row['pvp_wild_kills'] = (int)$charobj['pvp']['wild_kills'];
			// #### wardrobeslot_list: Not currently parsed.
			// #### alternateadvancements: Not currently parsed.
			// #### equipmentslot_list: Not currently parsed.
			// #### spell_list: Not currently parsed.
			// #### dungeon_list: Not currently parsed.
			// #### secondarytradeskills: Parse secondary trade skill(s).
			$row['tinkering'] = (empty($charobj['secondarytradeskills']['tinkering']['value'])) ? 0 : $charobj['secondarytradeskills']['tinkering']['value'];
			$row['adorning'] = (empty($charobj['secondarytradeskills']['adorning']['value'])) ? 0 : $charobj['secondarytradeskills']['adorning']['value'];
			// #### equipped_mount: Not currently parsed.
			// #### stats: Not currently parsed.
			// #### mount_list: Not currently parsed.
			// #### collections: Parse character collection data.
			$row['collections_active'] = (int)$charobj['collections']['active'];
			$row['collections_complete'] = (int)$charobj['collections']['complete'];
			// #### resists: Not currently parsed.
			// #### ascension_list: Not currently parsed.
			// #### type: Parse character data.
			//classid
			//aa_level
			//ts_level
			//raceid
			$row['type_level'] = (int)$charobj['type']['level'];
			$row['type_gender'] = Fix_Quotes(ucfirst((string)$charobj['type']['gender']), FALSE, FALSE);
			//ts_class
			$row['type_birthdate_utc'] = (int)$charobj['type']['birthdate_utc'];
			$row['type_race'] = Fix_Quotes((string)$charobj['type']['race'], FALSE, FALSE);
			$row['type_deity'] = Fix_Quotes((string)$charobj['type']['deity'], FALSE, FALSE);
			$row['type_class'] = Fix_Quotes(ucfirst((string)$charobj['type']['class']), FALSE, FALSE);
			//alignment
			// #### tradeskills: Parse primary trade skill.
			if (empty($charobj['tradeskills'])) {
				$row['tradeskill_class'] = unskilled;
				$row['tradeskill_level'] = 1;
			} else {
				foreach($charobj['tradeskills'] as $key => $value) {
					$row['tradeskill_class'] = Fix_Quotes(ucfirst((string)$key), FALSE, FALSE);
					$row['tradeskill_level'] = (int)$value['level'];
					// There should be only one child node. Quit anyway.
					break;
				}
			}
			// #### achievements: Parse character achievement data.
			$row['achievements_total_points'] = (int)$charobj['achievements']['total_points'];
			$row['achievements_total_count'] = (int)$charobj['achievements']['total_count'];
			$row['achievements_completed'] = (int)$charobj['achievements']['completed'];
			$row['achievements_points'] = (int)$charobj['achievements']['points'];
			// #### appearanceslot_list: Not currently parsed.
			// #### orderedalternateadvancement_list: Not currently parsed.
			// #### appearance_mount: Not currently parsed.
			// #### statistics: Parse character statistics.
			$row['statistics_kills'] = (int)$charobj['statistics']['kills']['value'];
			$row['statistics_deaths'] = (int)$charobj['statistics']['deaths']['value'];
			$row['statistics_max_melee_hit'] = (int)$charobj['statistics']['max_melee_hit']['value'];
			$row['statistics_items_crafted'] = (int)$charobj['statistics']['items_crafted']['value'];
			$row['statistics_rare_harvests'] = (int)$charobj['statistics']['rare_harvests']['value'];
			$row['statistics_max_magic_hit'] = (int)$charobj['statistics']['max_magic_hit']['value'];
			// #### locationdata: Parse character location data.
			//homefaction
			//worldid
			//world
			//coords
			$row['locationdata_zonename'] = Fix_Quotes((string)$charobj['locationdata']['zonename'], FALSE, FALSE);
			$row['locationdata_bindzone'] = Fix_Quotes((string)$charobj['locationdata']['bindzone'], FALSE, FALSE);
			// #### name: Parse character name data.
			$row['name_prefix'] = (empty($charobj['name']['prefix'])) ? '' : Fix_Quotes(trim((string)$charobj['name']['prefix']), FALSE, FALSE);
			//first_lower
			$row['name_last'] = (empty($charobj['name']['last'])) ? '' : Fix_Quotes((string)$charobj['name']['last'], FALSE, FALSE);
			$row['name_suffix'] = (empty($charobj['name']['suffix'])) ? '' : Fix_Quotes(trim((string)$charobj['name']['suffix']), FALSE, FALSE);
			$row['name_first'] = Fix_Quotes((string)$charobj['name']['first'], FALSE, FALSE);
			// #### arena: Parse character Arena statistics.
			// ## arena.ctf: Parse character Arena CTF statistics.
			$row['arena_ctf_streak'] = (int)$charobj['arena']['ctf']['streak'];
			$row['arena_ctf_timeplayed'] = (int)$charobj['arena']['ctf']['timeplayed'];
			$row['arena_ctf_kills'] = (int)$charobj['arena']['ctf']['kills'];
			$row['arena_ctf_deaths'] = (int)$charobj['arena']['ctf']['deaths'];
			$row['arena_ctf_matches'] = (int)$charobj['arena']['ctf']['matches'];
			$row['arena_ctf_captures'] = (int)$charobj['arena']['ctf']['captures'];
			$row['arena_ctf_losses'] = (int)$charobj['arena']['ctf']['losses'];
			$row['arena_ctf_wins'] = (int)$charobj['arena']['ctf']['wins'];
			// arena.deathmatch: Parse character Arena deathmatch statistics.
			$row['arena_deathmatch_streak'] = (int)$charobj['arena']['deathmatch']['streak'];
			$row['arena_deathmatch_timeplayed'] = (int)$charobj['arena']['deathmatch']['timeplayed'];
			$row['arena_deathmatch_kills'] = (int)$charobj['arena']['deathmatch']['kills'];
			$row['arena_deathmatch_deaths'] = (int)$charobj['arena']['deathmatch']['deaths'];
			$row['arena_deathmatch_matches'] = (int)$charobj['arena']['deathmatch']['matches'];
			$row['arena_deathmatch_wins'] = (int)$charobj['arena']['deathmatch']['wins'];
			$row['arena_deathmatch_losses'] = (int)$charobj['arena']['deathmatch']['losses'];
			// arena.idol: Parse character Arena idol statistics.
			$row['arena_idol_streak'] = (int)$charobj['arena']['idol']['streak'];
			$row['arena_idol_timeplayed'] = (int)$charobj['arena']['idol']['timeplayed'];
			$row['arena_idol_kills'] = (int)$charobj['arena']['idol']['kills'];
			$row['arena_idol_deaths'] = (int)$charobj['arena']['idol']['deaths'];
			$row['arena_idol_matches'] = (int)$charobj['arena']['idol']['matches'];
			$row['arena_idol_wins'] = (int)$charobj['arena']['idol']['wins'];
			$row['arena_idol_losses'] = (int)$charobj['arena']['idol']['losses'];
			$row['arena_idol_destroyed'] = (int)$charobj['arena']['idol']['destroyed'];
			// #### account: Parse character account data.
			$row['account_age'] = (int)$charobj['account']['age'];
			$row['account_link_id'] = (isset($charobj['account']['link_id'])) ? (int)$charobj['account']['link_id'] : (int)'0';
			// #### guild: Parse character guild data.
			$row['guild_status'] = (int)$charobj['guild']['status'];
			//name
			//level
			$row['guild_joined'] = (int)$charobj['guild']['joined'];
			$row['guild_rank'] = (int)$charobj['guild']['rank'];
			//guildid
			//id
			// #### quests: Parse character quest data.
			$row['quests_active'] = (int)$charobj['quests']['active'];
			$row['quests_complete'] = (int)$charobj['quests']['complete'];

			// Save row to array.
			$parsed_roster_data['roster']['values'][$row['characterId']] = '(\''.implode('\',\'', array_values($row)).'\')';
		}
		// Unset the old storage array so we know who is new and who to delete.
		unset($parsed_roster_data['roster_delete'][$row['characterId']]);

		if ($row['misc_last_update'] > $misc_previous_update) {
			// #### character_misc: Parse character_misc collection data.
			//$row['misc_last_update'] = (float)$charobj['character_misc']['last_update'];
			// ## collection_list: Parse character collection quest data.
			if (!empty($charobj['character_misc']['collection_list'])) {
				foreach($charobj['character_misc']['collection_list'] as $collection) {
					if (!empty($collection['item_list'])){
						$list = array();
						foreach($collection['item_list'] as $item) {
							$list[] = (int)$item['crc'];
						}
						$parsed_roster_data['collection_list']['values'][] = '(\''
								.$row['characterId'].'\',\''
								.(int)$collection['crc'].'\',\''
								.implode(',', $list)
								.'\')';
					}
				}
			}
			// ## quest_list: Parse character active quest data.
			if (!empty($charobj['character_misc']['quest_list'])) {
				foreach($charobj['character_misc']['quest_list'] as $quest) {
					if (isset($QuestsData[(int)$quest['crc']])) {
						$parsed_roster_data['quest_list']['values'][] = '(\''
							.$row['characterId'].'\',\''
							.(int)$quest['crc'].'\',\''
							.(int)$quest['stage_num'].'\',\'0\')';
					}
				}
			}
			// ## known_recipe_list: Not currently parsed.
			// ## completed_quest_list: Parse character completed quest data.
			if (!empty($charobj['character_misc']['completed_quest_list'])) {
				foreach($charobj['character_misc']['completed_quest_list'] as $quest) {
					if (isset($QuestsData[(int)$quest['crc']])) {
						// Shibboleths rear their ugly heads once again...
						// 八千以上？それは何か間違いだ！故障だぜ！
						$parsed_roster_data['quest_list']['values'][] = '(\''
							.$row['characterId'].'\',\''
							.(int)$quest['crc'].'\',\'8001\',\''
							.((!empty($quest['completion_date'])) ? strtotime((string)$quest['completion_date']) : 0)
							.'\')';
					}
				}
			}
		}
	}
//	cpg_error('<pre style="text-align:left">'.count($parsed_roster_data)." elements parsed.\n\n\$parsed_roster_data = ".htmlspecialchars(print_r($parsed_roster_data, TRUE))."</pre>\n");
	
	// Prepare field lists for db operations.
	if (!empty($row)) {
		// Create field index off of last row.
		$parsed_roster_data['roster']['fields'] = '('.implode(',', array_keys($row)).')';
	}
	$parsed_roster_data['collection_list']['fields'] = '(characterId,crc,item_list)';
	$parsed_roster_data['quest_list']['fields'] = '(characterId,crc,stage_num,completion_date)';

	// ###### PARSER ENDS ######

	// Calculate function duration in seconds.
	$timer_t = get_microtime(TRUE) - $timer_t;

	// Log the performace statistics.
	$error = sprintf(_NTC_JSONPARSE, 'character_list', $timer_r, ($timer_t - $timer_r), $timer_t);
	trigger_error($error, E_USER_NOTICE);
	if ($config['logfile'] & 128) { $GLOBALS['log'][] = $error; }

	return $parsed_roster_data;
}

// Function:	parse_quest_JSON()
// What it does:	Parses the JSON Quest collection data from the Census API.
function parse_quest_JSON($crc) {
	// Start function duration timer.
	$timer_t = $timer_r = get_microtime(TRUE);

	global $config, $mod_dirname;

	// Get the JSON data.
	if (!($json = get_JSON('quest_list', "{$config['census']}get/eq2/quest/{$crc}"))) {
		// Fetch failed, return.
		return FALSE;
	}
//	cpg_error('<pre style="text-align:left">'.count($json)." elements parsed.\n\n\$json = ".htmlspecialchars(print_r($json, TRUE))."</pre>\n");

	// Calculate function duration in seconds.
	$timer_r = get_microtime(TRUE) - $timer_r;

	// ###### PARSER STARTS ######

	$row = array();
	foreach($json['quest_list'] as $questobj) {
		$row['category'] = strip_tags((string)$questobj['category']);
		$row['name'] = strip_tags((string)$questobj['name']);
		$row['level'] = (int)$questobj['level'];
		//scales_with_level
		//is_tradeskill
		//ts
		//last_update
		$row['crc'] = (int)$questobj['crc'];
		$row['completion_text'] = trim(strip_tags((string)$questobj['completion_text']));
		//shareable
		$row['starter_text'] = trim(strip_tags((string)$questobj['starter_text']));
		//complete_shareable
		//tier
		//repeatable
		//id
		// #### stage_list: Parse list of quest stages.
		$row['stage_list'] = array();
		if (!empty($questobj['stage_list'])) {
			foreach($questobj['stage_list'] as $stage) {
				$row['stage_list'][(int)$stage['num']] = array(
					'description' => trim(strip_tags((string)$stage['starter_text_list'][0])),
					'completed_text' => trim(strip_tags((string)$stage['completion_text_list'][0]))
				);
			}
		}
		// #### reward_list: Not currently parsed.
//		cpg_error('<pre style="text-align:left">'.htmlspecialchars(var_export($row, TRUE))."</pre>\n");
	}

	// ###### PARSER ENDS ######

	// Calculate function duration in seconds.
	$timer_t = get_microtime(TRUE) - $timer_t;

	// Log the performace statistics.
	$error = sprintf(_NTC_JSONPARSE, 'quest', $timer_r, ($timer_t - $timer_r), $timer_t);
	trigger_error($error, E_USER_NOTICE);
	if ($config['logfile'] & 128) { $GLOBALS['log'][] = $error; }

	return $row;
}

// Function:	parse_collection_JSON()
// What it does:	Parses the JSON Collection quest collection data from the Census API.
function parse_collection_JSON($limit = 1, $start = 0) {
	// Start function duration timer.
	$timer_t = $timer_r = get_microtime(TRUE);

	global $config, $mod_dirname, $CollectionsDataClass, $CollectionsDataMeta;

	// Get the JSON data.
	if (!($json = get_JSON('collection_list', "{$config['census']}get/eq2/collection/?c:limit={$limit}&c:start={$start}&c:sort=name"))) {
		// Fetch failed, return.
		return FALSE;
	}
//	cpg_error('<pre style="text-align:left">'.count($json)." elements parsed.\n\n\$json = ".htmlspecialchars(print_r($json))."</pre>\n");

	// Calculate function duration in seconds.
	$timer_r = get_microtime(TRUE) - $timer_r;

	// ###### PARSER STARTS ######

	$i = 0;

	$CollectionsData = array();
	$row = array();
	foreach($json['collection_list'] as $collectionobj) {
//		cpg_error('<pre style="text-align:left">'.htmlspecialchars(print_r($collectionobj, TRUE))."</pre>\n");
		$row['category'] = strip_tags((string)$collectionobj['category']);
		$row['name'] = strip_tags((string)$collectionobj['name']);
		$row['level'] = (int)$collectionobj['level'];
		//ts
		//last_update
		$row['crc'] = (int)$collectionobj['id'];
		// Collection is member of class-restricted collection.
		if (isset($CollectionsDataClass[$row['crc']])) {
			$row['isclass'] = TRUE;
			if ($CollectionsDataClass[$row['crc']] != $row['name']) {
				trigger_error("Class collection quest name mismatch for crc {$row['crc']}, check class collection reference!", E_USER_NOTICE);
			}
		} else {
			$row['isclass'] = FALSE;
		}
		// Collection is member of meta-collection.
		if (isset($CollectionsDataMeta[$row['crc']])) {
			$row['ismeta'] = TRUE;
			if ($CollectionsDataMeta[$row['crc']] != $row['name']) {
				trigger_error("Meta collection quest name mismatch for crc {$row['crc']}, check meta-collection reference!", E_USER_NOTICE);
			}
		} else {
			$row['ismeta'] = FALSE;
		}
		// #### reference_list: Parse list of collection members.
		$row['reference_list'] = array();
		if (!empty($collectionobj['reference_list'])) {
			foreach($collectionobj['reference_list'] as $list) {
				$row['reference_list'][(int)$list['id']] = array(
					'icon' => (int)$list['icon'],
					//'id' => (int)$list['id'],
					'name' => strip_tags((string)$list['name'])
				);
			}
		}
		// #### reward_list: Not currently parsed.

//		cpg_error('<pre style="text-align:left">'.htmlspecialchars(print_r($row, TRUE))."</pre>\n");
		$CollectionsData[$row['crc']] = $row;
	}

	// ###### PARSER ENDS ######

	// Calculate function duration in seconds.
	$timer_t = get_microtime(TRUE) - $timer_t;

	// Log the performace statistics.
	$error = sprintf(_NTC_JSONPARSE, 'collection_list', $timer_r, ($timer_t - $timer_r), $timer_t);
	trigger_error($error, E_USER_NOTICE);
	if ($config['logfile'] & 128) { $GLOBALS['log'][] = $error; }

	return $CollectionsData;
}

// Function:	update_data()
// What it does:	Determine what the last time there was a data update.
//			If it's more than $config['cache_update_time'] (seconds), fetches the
//			data from everquest2.com and update the local storage.
//			First, the function fetches and parses the everquest2.com pages,
//			reads the stored data from the local file/database,
//			loops through the new/stored data generating the update logs,
//			writes all the new data to the local file/database, and then
//			prepends the current log to the logfile.
// Requires:
//		$_GET['force_update'] -- verification token
//		$_SERVER['REMOTE_ADDR'] -- verification token
function update_data() {
	global $db, $prefix, $suffix, $table_updated, $config, $mod_dirname;

	// Start function duration timer.
	$timer_t = get_microtime(TRUE);

	// Minimally initialize the log.
	$current_time = time();
	$GLOBALS['log'] = array();

	// Fetch the last time the data table was updated.
	list($table_updated) = $db->sql_ufetchrow("SELECT value FROM {$prefix}_roster_master_guild{$suffix}
		WHERE name='table_updated'", SQL_NUM);
	trigger_error('Last table update performed '.date('d.m.Y H:i:s', $table_updated)." ({$table_updated})", E_USER_NOTICE);

	if (isset($_GET['force_update']) && ((($config['cache_update_time'] == 0) && ($_SERVER['REMOTE_ADDR'] == '127.0.0.1')) || can_admin($mod_dirname))) {
		if ($config['logfile'] & 2) { $GLOBALS['log'][] = "Update forced from {$_SERVER['REMOTE_ADDR']}."; }
	} else {
		// Automatic updating is disabled.
		if ($config['cache_update_time'] == 0) { return; }

		// Table's fresh, bail out.
		if (intval($table_updated) > (time() - $config['cache_update_time'])) { return; }
	}

	// Start function duration timer.
	$timer_gs = get_microtime(TRUE);
	$timer_pgs = get_microtime(TRUE);

	$add_log = array();
	$remove_log = array();

	// #### Parse guild collection data.
	if ($parsed_summary_data = parse_guild_JSON()) {
		// Calculate function duration in seconds.
		$timer_pgs = get_microtime(TRUE) - $timer_pgs;

		// Read current roster_master_guild_rank table contents and format into an array (added to $config['lookup_rank'] global).
		$result = $db->sql_query("SELECT * FROM {$prefix}_roster_master_guild_rank{$suffix}", FALSE);
		$old_lookup_rank = array();
		while($row = $db->sql_fetchrow($result)) {
			$old_lookup_rank[$row['rank']] = $row['name'];
			if (!empty($config['lookup_rank'][$row['rank']]) && ($old_lookup_rank[$row['rank']] != $config['lookup_rank'][$row['rank']])) {

				// Log field changes.
				$error = sprintf(_LOG_ROSTERFIELDCHANGE, _GUILDRANK, $row['rank'], $old_lookup_rank[$row['rank']], $config['lookup_rank'][$row['rank']]);
//				trigger_error($error, E_USER_NOTICE);
				$GLOBALS['log'][] = $error;
			}
		}
		$db->sql_freeresult($result);

/*
		cpg_error('<pre style="text-align:left">'
			.'$old_lookup_rank = '.htmlspecialchars(print_r($old_lookup_rank, TRUE)).count($old_lookup_rank)." elements parsed.\n\n"
			.'$config=[\'lookup_rank\'] = '.htmlspecialchars(print_r($config['lookup_rank'], TRUE)).count($config['lookup_rank'])." elements parsed.\n\n"
			.(($old_lookup_rank != $config['lookup_rank']) ? 'IS NOT EQUAL' : 'IS EQUAL')
			."</pre>\n");
*/
		// Calculate function duration in seconds.
		$timer_wgs = get_microtime(TRUE);

		// Write changes to the local database if guild rank names have changed.
		if ($old_lookup_rank != $config['lookup_rank']) {
			$db->sql_query("TRUNCATE TABLE {$prefix}_roster_master_guild_rank{$suffix}", FALSE);
			foreach($config['lookup_rank'] as $rank => $name) {
				$db->sql_query("INSERT INTO {$prefix}_roster_master_guild_rank{$suffix} (rank, name)
					VALUES ('".intval($rank).'\',\''.Fix_Quotes($config['lookup_rank'][$rank], FALSE, TRUE).'\')', FALSE);
			}
		}

		// Calculate function duration in seconds.
		$duration_a_wgs = get_microtime(TRUE) - $timer_wgs;

		// Free up array copies.
		unset($old_lookup_rank);

		// Read the contents of the guild summary database.
		$old_summary_data = read_summary_db();

		// Determine what has been updated, as well as determine the old value.
		$new_summary_data = array();
		foreach(array_keys($parsed_summary_data) as $field) {
			if (!isset($old_summary_data[$field]) || ($old_summary_data[$field] != htmlspecialchars($parsed_summary_data[$field], ENT_NOQUOTES, 'UTF-8'))) {
				$new_summary_data[] = '(\''
					.Fix_Quotes($field, FALSE, TRUE).'\',\''
					.Fix_Quotes($parsed_summary_data[$field], FALSE, TRUE)
					.'\')';
			}
		}

		// Set time that the table was last parsed.
		$new_summary_data[] = '(\'table_updated\',\''.$current_time.'\')';

/*
		cpg_error("<pre style=\"text-align:left\">GUILD SUMMARY PARSER\n\n"
			."PARSED:\n".print_r($parsed_summary_data, TRUE)."\n\n"
			."OLD:\n".print_r($old_summary_data, TRUE)."\n\n"
			."NEW:\n".print_r($new_summary_data, TRUE)."\n\n"
			."LOG:\n".print_r($GLOBALS['log'], TRUE)."</pre>");
*/

		// Start function duration timer.
		$timer_wgs = get_microtime(TRUE);

		// Perform upsert/merge of guild summary data changes to the local database.
		$query = "INSERT INTO {$prefix}_roster_master_guild{$suffix} (name, value)
				VALUES ".implode(',', $new_summary_data)."
				ON DUPLICATE KEY UPDATE value=VALUES(value)";
//		trigger_error("\$query = {$query}", E_USER_NOTICE);
		$db->sql_query($query, FALSE);

		// Calculate function duration in seconds.
		$duration_a_wgs = get_microtime(TRUE) - $timer_wgs;

		// Free up array copies.
		unset($parsed_summary_data, $old_summary_data, $new_summary_data);

		// Calculate function duration in seconds.
		$timer_gs = get_microtime(TRUE) - $timer_gs;
	} else {
		$error = sprintf(_ERR_PROCESSFAIL, 'Guild Summary');
		trigger_error($error, E_USER_NOTICE);
		$GLOBALS['log'][] = $error;
	}

	// Start function duration timer.
	$timer_pcc = get_microtime(TRUE);
	$timer_cc = get_microtime(TRUE);

	// #### Parse character collection data.
	$parsed_roster_data = array();
	if ($parsed_roster_data = parse_character_JSON()) {
//		cpg_error('<pre style="text-align:left">'.count($parsed_roster_data)." elements parsed.\n\n\$parsed_roster_data = ".htmlspecialchars(print_r($parsed_roster_data, TRUE))."</pre>\n");

		// Calculate function duration in seconds.
		$timer_pcc = get_microtime(TRUE) - $timer_pcc;

		
		// ## Write to database.
		//	NOTE: this performs multi-row updates and is bounded by the maximum
		//	query size (max_allowed_packet for MySQL, default of 1MB and
		//	configurable up to 1GB).

		// Start function duration timer.
		$timer_wcc = get_microtime(TRUE);
		$timer_dd = get_microtime(TRUE);

		// Go through those names that no longer appear in the parse and
		// delete them from the roster.
		$cdeleted = 0;
		$rowsdeleted = 0;
		if (!empty($parsed_roster_data['roster_delete'])) {
			foreach ($parsed_roster_data['roster_delete'] as $characterId => $data) {
				if (!isset($new_roster_data[$characterId])) {
					$db->sql_query("DELETE FROM {$prefix}_roster_master{$suffix} WHERE characterId='{$characterId}'");
					$db->sql_query("DELETE FROM {$prefix}_roster_master_collection_status{$suffix} WHERE characterId='{$characterId}'");
					$rowsdeleted += $db->affected_rows();
					$db->sql_query("DELETE FROM {$prefix}_roster_master_quest_status{$suffix} WHERE characterId='{$characterId}'");
					$rowsdeleted += $db->affected_rows();
					
					// Log the character removal.
					$remove_log[] = sprintf(_LOG_ROSTERDEL, $data['name_first']);
					$cdeleted++;
				}
			}
		}

		// Calculate function duration in seconds.
		$timer_dd = get_microtime(TRUE) - $timer_dd;

		// Start function duration timer.
		$duration_a_imp = 0;
		$duration_a_cdu = 0;
		$duration_a_qdu = 0;
		$timer_rdu = 0;

		// Update roster database record for character.
		if (!empty($parsed_roster_data['roster']['values'])) {
//			cpg_error('<pre style="text-align:left">'.count($char_info)." elements parsed.\n\n\$char_info = ".htmlspecialchars(print_r($char_info, TRUE))."</pre>\n");
			$timer_imp = get_microtime(TRUE);
			$query = "REPLACE INTO {$prefix}_roster_master{$suffix} {$parsed_roster_data['roster']['fields']}
				VALUES ".implode(',', $parsed_roster_data['roster']['values']);
			$duration_a_imp = get_microtime(TRUE) - $timer_imp;
//			trigger_error('length = '.formatBytes(strlen($query)).", \$query = {$query}", E_USER_NOTICE);

			$timer_rdu = get_microtime(TRUE);
			$db->sql_query($query, FALSE);
			$timer_rdu = get_microtime(TRUE) - $timer_rdu;

			// Log the performance statistics.
			$error = sprintf("REPLACE INTO {$prefix}_roster_master{$suffix} (".formatBytes(strlen($query)).', %.4f sec) complete.', $timer_rdu);
			trigger_error($error, E_USER_NOTICE);
			if ($config['logfile'] & 128) { $GLOBALS['log'][] = $error; }
		}

		// Update collection_list database record for character.
		if (!empty($parsed_roster_data['collection_list']['values'])) {
			// Limit query string length.
			$step = 10000;
			for ($i = 0; $i < count($parsed_roster_data['collection_list']['values']); $i += $step) {
//				cpg_error('<pre style="text-align:left">'.count($parsed_roster_data['collection_list']['values'])." elements parsed.\n\n\$parsed_roster_data['collection_list']['values'] = ".htmlspecialchars(print_r($parsed_roster_data['collection_list']['values'], TRUE))."</pre>\n");
				$timer_imp = get_microtime(TRUE);
				$query = "REPLACE INTO {$prefix}_roster_master_collection_status{$suffix} {$parsed_roster_data['collection_list']['fields']}
						VALUES ".implode(',', array_slice($parsed_roster_data['collection_list']['values'], $i, $step - 1));
				$duration_a_imp = get_microtime(TRUE) - $timer_imp;
//				trigger_error('length = '.formatBytes(strlen($query)).", \$query = {$query}", E_USER_NOTICE);

				$timer_cdu = get_microtime(TRUE);
				$db->sql_query($query, FALSE);
				$duration_a_cdu += get_microtime(TRUE) - $timer_cdu;

				// Log the performance statistics.
				$error = sprintf("REPLACE INTO {$prefix}_roster_master_collection_status{$suffix}: slice %d through %d of %d total records (".formatBytes(strlen($query)).', %.4f sec) complete.', $i, ($i + $step - 1), count($parsed_roster_data['collection_list']['values']), (get_microtime(TRUE) - $timer_cdu));
				trigger_error($error, E_USER_NOTICE);
				if ($config['logfile'] & 128) { $GLOBALS['log'][] = $error; }
			}
		}

		// Update collection_list database record for character.
		if (!empty($parsed_roster_data['quest_list']['values'])) {
			// Limit query string length.
			$step = 50000;
			for ($i = 0; $i < count($parsed_roster_data['quest_list']['values']); $i += $step) {
//				cpg_error('<pre style="text-align:left">'.count($parsed_roster_data['quest_list']['values'])." elements parsed.\n\n\$parsed_roster_data['quest_list']['values'] = ".htmlspecialchars(print_r($parsed_roster_data['quest_list']['values'], TRUE))."</pre>\n");
				$timer_imp = get_microtime(TRUE);
				$query = "REPLACE INTO {$prefix}_roster_master_quest_status{$suffix} {$parsed_roster_data['quest_list']['fields']}
						VALUES ".implode(',', array_slice($parsed_roster_data['quest_list']['values'], $i, $step - 1));
				$duration_a_imp = get_microtime(TRUE) - $timer_imp;
//				trigger_error('length = '.formatBytes(strlen($query)).", \$query = {$query}", E_USER_NOTICE);

				$timer_qdu = get_microtime(TRUE);
				$db->sql_query($query, FALSE);
				$duration_a_qdu += get_microtime(TRUE) - $timer_qdu;

				// Log the performance statistics.
				$error = sprintf("REPLACE INTO {$prefix}_roster_master_quest_status{$suffix}: slice %d through %d of %d total records (".formatBytes(strlen($query)).', %.4f sec) complete.', $i, ($i + $step - 1), count($parsed_roster_data['quest_list']['values']), (get_microtime(TRUE) - $timer_qdu));
				trigger_error($error, E_USER_NOTICE);
				if ($config['logfile'] & 128) { $GLOBALS['log'][] = $error; }
			}
		}

		// Update the date the table was last updated.
		$table_updated = $current_time;
		$db->sql_query("UPDATE {$prefix}_roster_master_guild{$suffix}
			SET value='{$table_updated}'
			WHERE name='table_updated'", FALSE);

		// Calculate function duration in seconds.
		$timer_wcc = get_microtime(TRUE) - $timer_wcc;
		$timer_cc = get_microtime(TRUE) - $timer_cc;

		$error = sprintf(_LOG_PROCESSCOMPLETE, ((empty($parsed_roster_data['roster']['values'])) ? 0 : count($parsed_roster_data['roster']['values'])), ((empty($parsed_roster_data['roster']['character_list']))? 0 : count($parsed_roster_data['roster']['character_list'])), $cdeleted, $rowsdeleted);
		trigger_error($error, E_USER_NOTICE);
		if ($config['logfile'] & 1) { $GLOBALS['log'][] = $error; }

		// Log the performace statistics.
		$error = sprintf(_NTC_UPDATEDATA, 
			$timer_gs, $timer_pgs, $duration_a_wgs, ($timer_gs - $timer_pgs - $duration_a_wgs),
			$timer_cc, $timer_pcc, $timer_wcc,
				$duration_a_imp, $timer_dd, $timer_rdu, $duration_a_cdu, $duration_a_qdu,
			($timer_cc - $timer_pcc - $timer_wcc),
			(get_microtime(TRUE) - $timer_t));
		trigger_error($error, E_USER_NOTICE);
		if ($config['logfile'] & 128) { $GLOBALS['log'][] = $error; }
	} else {
		$error = sprintf(_ERR_PROCESSFAIL, 'Character Collection');
		trigger_error($error, E_USER_NOTICE);
		$GLOBALS['log'][] = $error;
	}

	// Check for invalid signature blocks and remove them.
	if ($config['dynamic_sigs']) {
		reapsigcache();
		if ($config['force_sig_cache']) { fillsigcache(); }
	}

	// Log to file.
	if ($config['logfile'] && (count($GLOBALS['log']) || ($config['logfile'] & 2))) {
		$log_filename = dirname(__FILE__).'/../logfile.inc';
		$GLOBALS['log'] = array_merge($GLOBALS['log'], $add_log, $remove_log);
		array_unshift($GLOBALS['log'], _LOG_UPDATE.': '.date($config['logfile_date_fmt']['format'], $current_time).$config['logfile_date_fmt']['suffix'], '---------------------------------');
		$GLOBALS['log'][] = "---------------------------------\n\n";

		// Grab the old log and append it to the new one.
		if (file_exists($log_filename)) {
			$new_logfile = implode($GLOBALS['log'], "\n").htmlprepare(file_get_contents($log_filename), FALSE, ENT_NOQUOTES, TRUE);
		} else {
			$new_logfile = implode($GLOBALS['log'], "\n");
		}

		// Write out the logfile.
		if ($log_file = fopen($log_filename, 'w')) {
			fwrite($log_file, $new_logfile);
			fclose($log_file);
		} else {
			if (can_admin($mod_dirname)) {
				cpg_error('<strong>'._ERR_FATALCONFIG_RM."</strong><br /><br />\n"
					."Unable to open logfile.inc for writing.<br />\n"
					.'Make sure that the webserver has write permission set for this file AND the install folder.');
			} else {
				return NULL;
			}
		}
	}
}

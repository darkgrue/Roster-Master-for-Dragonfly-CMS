<?php 
/***********************************************************************
  Roster Master for Dragonfly(TM) CMS
  **********************************************************************
  Copyright (C) 2005-2018 by Dark Grue

  Signature Master module. Generates and manages dynamic graphical
  signatures.

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
// Signature Master sends a "HTTP/1.0 404 Not Found" in response to
// errors in order to save system resources and provide a relatively
// clean exit condition (which will "red X" included images on a page,
// rather than both with sending the Dragonfly error page, which could
// be confusing to a client expecting a binary image (or nothing at all)
// as the response.
// In order to override this behavior and provide verbose error messages,
// set $DEBUG to TRUE and view the link directly (rather than as an IMG
// tag). Do not run production servers with $DEBUG = TRUE.
$DEBUG = FALSE;

// Signature block image save path.
$sig_savepath = "modules/{$mod_dirname}/signatures";

// Global variable to hold image resource pointers.
$GD_destroy = array();


// ***************************** FUNCTIONS *****************************
// Function:	destroyimageresource()
// What it does:	This shutdown function cleans up the GD memory space.
//			All image resource identifiers must go into the global
//			$GD_destroy array.
function destroyimageresource() {
	global $GD_destroy;

	foreach($GD_destroy as $resource) {
		imagedestroy($resource);
	}
}

// Function:	fit_imagettftext()
// What it does:	Wrapper for imagettftext() function. Incrementally reduces fontsize to
//			fit a specified line size.
function fit_imagettftext($image, $size, $override, $targetwidth, $x, $y, $color, $fontfile, $text) {
	static $lastsize;

	if ($override == 'I') {
		// Inherit the last font size used.
		$size = $lastsize;
	} else {
		if ($override != 'A') {
			// Don't bother going below 4 point text.
			while($size > 5) {
				$bbox = imagettfbbox($size, 0, $fontfile, $text);
				$width = $bbox[2] -  $bbox[0];
				if ($width > $targetwidth) {
					$size--;
				} else {
					break;
				}
			}
		}
		// Save the last font size used.
		$lastsize = $size;
	}

	imagettftext($image, $size, 0, $x, $y, $color, $fontfile, $text);
}

// Function:	getsigimage()
// What it does:	Generates a dynamic signature block from character data.
function getsigimage($char_info) {
	global $DEBUG, $mod_dirname, $db, $prefix, $suffix, $config;

	// Copy additional elements into or modify character info arrary for
	// later use in template macros.
	// Currently available: 'FullName', 'Server', and 'guild_rankname'.
	$char_info['FullName'] = $char_info['name_first'].((empty($char_info['name_last'])) ? '' : " {$char_info['name_last']}");
	list($char_info['guild_name']) = $db->sql_ufetchrow("SELECT value FROM {$prefix}_roster_master_guild{$suffix} WHERE name='Guild Name'", SQL_NUM);
	list($char_info['Server']) = $db->sql_ufetchrow("SELECT value FROM {$prefix}_roster_master_guild{$suffix} WHERE name='Server'", SQL_NUM);
	if (empty($config['lookup_rank'])) {
		$result = $db->sql_query("SELECT * FROM {$prefix}_roster_master_guild_rank{$suffix}", FALSE);
		$config['lookup_rank'] = array();
		while($row = $db->sql_fetchrow($result)) {
			$config['lookup_rank'][$row['rank']] = $row['name'];
		}
		$db->sql_freeresult($result);
	}
	$char_info['guild_rankname'] =  ((empty($config['lookup_rank'][$char_info['guild_rank']])) ? _UNKNOWN_RM : $config['lookup_rank'][$char_info['guild_rank']]);
//	cpg_error('<pre style="text-align:left">'.print_r($char_info, TRUE)."</pre>\n");

	// Select signature block image file.
	// Establish default
	$block_file = "modules/{$mod_dirname}/images/sigblocks/{$config['sigblock_basename']}.png";
	$block_file = (is_file($block_file)) ? $block_file : "modules/{$mod_dirname}/images/sigblocks/default_sigblock.png";
	// Get block basename to place attributes off the end of.
	switch (strtoupper($config['sigblock_type'])) {
		case 'ARCH':
			if ($archetype = MB::tolower(retkey_in_array($char_info['type_class'], $config['archetypes']))) {
				if (in_array($char_info['guild_rank'], $config['officers']) && file_exists("modules/{$mod_dirname}/images/sigblocks/{$config['sigblock_basename']}_officer_".str_replace(' ', '_', $archetype).'.png')) {
					$block_file = "modules/{$mod_dirname}/images/sigblocks/{$config['sigblock_basename']}_officer_".str_replace(' ', '_', $archetype).'.png';
				} else if (file_exists("modules/{$mod_dirname}/images/sigblocks/{$config['sigblock_basename']}_".str_replace(' ', '_', $archetype).'.png')) {
					$block_file = "modules/{$mod_dirname}/images/sigblocks/{$config['sigblock_basename']}_".str_replace(' ', '_', $archetype).'.png';
				}
			}
			break;
		case 'CLASS':
			if ($class = MB::tolower($char_info['type_class'])) {
				if (in_array($char_info['guild_rank'], $config['officers']) && file_exists("modules/{$mod_dirname}/images/sigblocks/{$config['sigblock_basename']}_officer_".str_replace(' ', '_', $class).'.png')) {
					$block_file = "modules/{$mod_dirname}/images/sigblocks/{$config['sigblock_basename']}_officer_".str_replace(' ', '_', $class).'.png';
				} else if (file_exists("modules/{$mod_dirname}/images/sigblocks/{$config['sigblock_basename']}_".str_replace(' ', '_', $class).'.png')) {
					$block_file = "modules/{$mod_dirname}/images/sigblocks/{$config['sigblock_basename']}_".str_replace(' ', '_', $class).'.png';
				}
			}
			break;
		case 'RANK':
			if (in_array($char_info['guild_rank'], $config['officers']) && file_exists("modules/{$mod_dirname}/images/sigblocks/{$config['sigblock_basename']}_officer.png")) {
				$block_file = "modules/{$mod_dirname}/images/sigblocks/{$config['sigblock_basename']}_officer.png";
			} else if (file_exists("modules/{$mod_dirname}/images/sigblocks/{$config['sigblock_basename']}")) {
				$block_file = "modules/{$mod_dirname}/images/sigblocks/{$config['sigblock_basename']}";
			}
			break;
	}
	// Read signature block image file.
	if (is_file($block_file)) {
		$what = getimagesize($block_file);
		$img_block_width =& $what[0];
		$img_block_height =& $what[1];

		switch($what['mime']) {
			case 'image/png':
				$GD_destroy[] = $img_block_src = imagecreatefrompng($block_file);
				break;
			case 'image/jpeg':
				$GD_destroy[] = $img_block_src = imagecreatefromjpeg($block_file);
				break;
			case 'image/gif':
				$GD_destroy[] = $img_block_src = imagecreatefromgif($block_file);
				break;
			default:
				if ($DEBUG) { cpg_error("Could not allocate signature block image file \"{$block_file}\"."); }
				header('HTTP/1.0 404 Not Found');
				exit();
		}
		$GD_destroy[] = $img_block = imagecreatetruecolor($img_block_width, $img_block_height);
		imagecopy($img_block, $img_block_src, 0, 0, 0, 0, $img_block_width, $img_block_height);
	} else {
		if ($DEBUG) { cpg_error("Could not find signature block image file \"{$block_file}\"."); }
		header('HTTP/1.0 404 Not Found');
		exit();
	}

	// Read signature block template file.
	$block_basename = basename($block_file, '.png');
	$template_file = "modules/{$mod_dirname}/images/sigblocks/{$block_basename}.txt";
	if (is_file($template_file)) {
		$errors = '';

		// Get the template file into an array.
		$lines = file($template_file);

		// Loop through our array.
		foreach($lines as $line_num => $line) {
			$matches = array();
			if (preg_match('/^(?:\s*|\s*\/\/.*)$/', $line)) {
				// Blank line or comment, ignore.
				continue;
			} else if (preg_match('/^\s*BORDER_MARGIN\s+(\d+)(?:|\s+\/\/.*)/i', $line, $matches)) {
				if (empty($template['BORDER_MARGIN'])) {
					$template['BORDER_MARGIN'] = "{$matches[1]}";
				} else {
					// Compile error listing.
					$errors .= 'Line #'.($line_num + 1).": BORDER_MARGIN already defined previously.\n";
				}
			} else if (preg_match('/^\s*TEXT_LEFTINDENT\s+(\d+)/i', $line, $matches)) {
				if (empty($template['TEXT_LEFTINDENT'])) {
					$template['TEXT_LEFTINDENT'] = "{$matches[1]}";
				} else {
					// Compile error listing.
					$errors .= 'Line #'.($line_num + 1).": TEXT_LEFTINDENT already defined previously.\n";
				}
			} else if (preg_match('/^\s*TEXT_RIGHTINDENT\s+(\d+)/i', $line, $matches)) {
				if (empty($template['TEXT_RIGHTINDENT'])) {
					$template['TEXT_RIGHTINDENT'] = "{$matches[1]}";
				} else {
					// Compile error listing.
					$errors .= 'Line #'.($line_num + 1).": TEXT_RIGHTINDENT already defined previously.\n";
				}
			} else if (preg_match('/^\s*COLOR\s+(\w+)\s+([0-9A-Fa-f]{2})([0-9A-Fa-f]{2})([0-9A-Fa-f]{2})/i', $line, $matches)) {
				// Create a color identifier representing the color composed of the
				// given hexidecimal color triplet.
				if (empty($color[$matches[1]])) {
					$color[$matches[1]] = imagecolorallocate($img_block, hexdec($matches[2]), hexdec($matches[3]), hexdec($matches[4]));
				} else {
					// Compile error listing.
					$errors .= 'Line #'.($line_num + 1).": Color \"{$matches[1]}\" already defined previously.\n";
				}
			} else if (preg_match('/^\s*AVATAR\s+(DEFAULT|ARCH|CLASS|RANK|EQ2)\s+(TOP|MIDDLE|BOTTOM)\s+(LEFT|RIGHT)/i', $line, $matches)) {
				$template['AVATAR'] = "{$matches[1]} {$matches[2]} {$matches[3]}";
			} else if (preg_match('/^\s*RANK\s+(\d+)\s+(\d+)/i', $line, $matches)) {
				$template['RANK'] = "{$matches[1]} {$matches[2]}";
			} else if (preg_match('/^\s*TEXT\s+(\d+)([A|I]?)\s+(\d+)\s+(\d+)\s+(\w+)\s+([\w\.]+)\s+(.*)/i', $line, $matches)) {
				$template['TEXT'][] = "{$matches[1]}{$matches[2]} {$matches[3]} {$matches[4]} {$matches[5]} {$matches[6]} {$matches[7]}";
			} else {
				// Compile error listing.
				$errors .= 'Line #'.($line_num + 1).": {$line}\n";
			}
		}

		// Output template errors.
		if ($DEBUG && !empty($errors)) {
			cpg_error("Error parsing signature block template file \"$template_file\"<br />\n"
				."<pre style=\"text-align:left\">{$errors}</pre>");
		}

		// Minimally initialize template.
		if (empty($template['BORDER_MARGIN'])) { $template['BORDER_MARGIN'] = 0; }
		if (empty($template['TEXT_LEFTINDENT'])) { $template['TEXT_LEFTINDENT'] = 0; }
		if (empty($template['TEXT_RIGHTINDENT'])) { $template['TEXT_RIGHTINDENT'] = 0; }
//		cpg_error("<pre style=\"text-align:left\">Parsed file: {$template_file}\n\n".print_r($template, TRUE).'</pre>');
	} else {
		if ($DEBUG) { cpg_error("Could not find signature block template file \"{$template_file}\"."); }
		header('HTTP/1.0 404 Not Found');
		exit();
	}

	// Set default text margins up inside the block.
	$text_leftmargin = $template['BORDER_MARGIN'] + $template['TEXT_LEFTINDENT'];
	$text_rightmargin = $img_block_width - $template['TEXT_RIGHTINDENT'] - $template['BORDER_MARGIN'];

	// Place avatar image.
	if (!empty($template['AVATAR']) && (preg_match('/^(DEFAULT|ARCH|CLASS|RANK|EQ2)\s+(TOP|MIDDLE|BOTTOM)\s+(LEFT|RIGHT)/i', $template['AVATAR'], $matches))) {
		if (file_exists("modules/{$mod_dirname}/images/avatars/characterId/{$char_info['characterId']}.png")) {
			// Use avatar file named as Character ID.
			$avatar_file = "modules/{$mod_dirname}/images/avatars/characterId/{$char_info['characterId']}.png";
		} else {
			// Establish default.
			$avatar_file = "modules/{$mod_dirname}/images/avatars/anon.png";

			switch (strtoupper($matches[1])) {
				case 'ARCH':
					if ($archetype = MB::tolower(retkey_in_array($char_info['type_class'], $config['archetypes']))) {
						if (in_array($char_info['guild_rank'], $config['officers']) && file_exists("modules/{$mod_dirname}/images/avatars/ranks/officer_".str_replace(' ', '_', $archetype).'.png')) {
							$avatar_file = "modules/{$mod_dirname}/images/avatars/ranks/officer_".str_replace(' ', '_', $archetype).'.png';
						} else if (file_exists("modules/{$mod_dirname}/images/avatars/archetypes/".str_replace(' ', '_', $archetype).'.png')) {
							$avatar_file = "modules/{$mod_dirname}/images/avatars/archetypes/".str_replace(' ', '_', $archetype).'.png';
						}
					}
					break;
				case 'CLASS':
					if ($class = MB::tolower($char_info['type_class'])) {
						if (in_array($char_info['guild_rank'], $config['officers']) && file_exists("modules/{$mod_dirname}/images/avatars/ranks/officer_".str_replace(' ', '_', $class).'.png')) {
							$avatar_file = "modules/{$mod_dirname}/images/avatars/ranks/officer_".str_replace(' ', '_', $class).'.png';
      						} else if (file_exists("modules/{$mod_dirname}/images/avatars/classes/".str_replace(' ', '_', $class).'.png')) {
							$avatar_file = "modules/{$mod_dirname}/images/avatars/classes/".str_replace(' ', '_', $class).'.png';
						}
					}
					break;
				case 'RANK':
					if (in_array($char_info['guild_rank'], $config['officers']) && file_exists("modules/{$mod_dirname}/images/avatars/ranks/officer_anon.png")) {
						$avatar_file = "modules/{$mod_dirname}/images/avatars/ranks/officer_anon.png";
					} else if (file_exists("modules/{$mod_dirname}/images/avatars/anon.png")) {
						$avatar_file = "modules/{$mod_dirname}/images/avatars/anon.png";
					}
					break;
				case 'EQ2':
					// Pick a random color EQ2 icon.
					$images = glob("modules/{$mod_dirname}/images/avatars/eq2_*.png");
					$avatar_file = $images[array_rand($images, 1)];
					break;
			}
		}
		if (is_file($avatar_file)) {
			$what = getimagesize($avatar_file);
			$img_avatar_width = $what[0];
			$img_avatar_height = $what[1];

			switch($what['mime']) {
				case 'image/png':
					$GD_destroy[] = $img_avatar_src = imagecreatefrompng($avatar_file);
					break;
				case 'image/jpeg':
					$GD_destroy[] = $img_avatar_src = imagecreatefromjpeg($avatar_file);
					break;
				case 'image/gif':
					$GD_destroy[] = $img_avatar_src = imagecreatefromgif($avatar_file);
					break;
				default:
					if ($DEBUG) { cpg_error("Could not allocate avatar image file \"{$avatar_file}\"."); }
					header('HTTP/1.0 404 Not Found');
					exit();
			}

			$usable_height = $img_block_height - (2 * $template['BORDER_MARGIN']);

			if ($img_avatar_height > $usable_height) {
				$new_avatar_width = (int)(($usable_height / $img_avatar_height) * $img_avatar_width);

				// Create temporary image and copy resampled avatar into it.
				$GD_destroy[] = $img_tmp = imagecreatetruecolor($new_avatar_width, $usable_height);

				// Set the flag to save full alpha channel information.
				imagesavealpha($img_tmp, TRUE);

				// resource dst_image, resource src_image, int dst_x, int dst_y, int src_x, int src_y, int dst_w, int dst_h, int src_w, int src_h
				imagecopyresampled($img_tmp, $img_avatar_src, 0, 0, 0, 0, $new_avatar_width, $usable_height, $img_avatar_width, $img_avatar_height);

				$img_avatar_src = $img_tmp;
				$img_avatar_width = $new_avatar_width;
				$img_avatar_height = $usable_height;
			}

			// Select y-position.
			switch (strtoupper($matches[2])) {
				case 'TOP':
					$y_pos = $template['BORDER_MARGIN'];
					break;
				case 'MIDDLE':
					$y_pos = ($img_block_height - $img_avatar_height) / 2;
					break;
				case 'BOTTOM':
					$y_pos = $img_block_height - ($img_avatar_height + $template['BORDER_MARGIN']);
					break;
			}

			// Select y-position.
			switch (strtoupper($matches[3])) {
				case 'LEFT':
					$x_pos = ($img_avatar_width < $usable_height) ? ($template['BORDER_MARGIN'] + (($img_block_height - $img_avatar_width) / 2)) : $template['BORDER_MARGIN'];
					$text_leftmargin = $template['BORDER_MARGIN'] + $template['TEXT_LEFTINDENT']
						+ ((($x_pos + $img_avatar_width) > $template['TEXT_LEFTINDENT']) ? $x_pos + $img_avatar_width : 0);
					$text_rightmargin = $img_block_width - $template['TEXT_RIGHTINDENT'] - $template['BORDER_MARGIN'];
					break;
				case 'RIGHT':
					$x_pos = (($img_avatar_width > $usable_height) ? ($img_block_width - $img_avatar_width) : ($img_block_width - ($img_avatar_width + $img_block_height) / 2)) - $template['BORDER_MARGIN'];
					$text_leftmargin = $template['BORDER_MARGIN'] + $template['TEXT_LEFTINDENT'];
					$text_rightmargin = $x_pos;
					break;
			}

			// Set the flag to save full alpha channel information.
			imagesavealpha($img_avatar_src, TRUE);

			// Copy character avatar into base image.
			// resource dst_im, resource src_im, int dst_x, int dst_y, int src_x, int src_y, int src_w, int src_h
			imagecopy($img_block, $img_avatar_src, $x_pos, $y_pos, 0, 0, $img_avatar_width, $img_avatar_height);
		} else {
			if ($DEBUG) { cpg_error("Could not find avatar image file \"{$avatar_file}\"."); }
		}
	}

	// Place guild rank insignia.
	if (!empty($template['RANK']) && (preg_match('/^(\d+)\s+(\d+)/', $template['RANK'], $matches))) {
		// Read guild rank insignia image file.
		$rank_file = "modules/{$mod_dirname}/images/sigblocks/{$block_basename}_rank_{$char_info['guild_rank']}.png";
		$rank_file = (is_file($rank_file)) ? $rank_file : "modules/{$mod_dirname}/images/sigblocks/default_rank_{$char_info['guild_rank']}.png";
		if (is_file($rank_file)) {
			$what = getimagesize($rank_file);
			$img_rank_width = $what[0];
			$img_rank_height = $what[1];

			switch($what['mime']) {
				case 'image/png':
					$GD_destroy[] = $img_rank_src = imagecreatefrompng($rank_file);
					break;
				case 'image/jpeg':
					$GD_destroy[] = $img_rank_src = imagecreatefromjpeg($rank_file);
					break;
				case 'image/gif':
					$GD_destroy[] = $img_rank_src = imagecreatefromgif($rank_file);
					break;
				default:
					header('HTTP/1.0 404 Not Found');
					if ($DEBUG) { cpg_error("Could not allocate guild rank image file \"{$rank_file}\"."); }
					exit();
			}

			// Set the flag to save full alpha channel information.
			imagesavealpha($img_rank_src, TRUE);

			// Copy guild rank insignia into base image.
			// resource dst_im, resource src_im, int dst_x, int dst_y, int src_x, int src_y, int src_w, int src_h
			imagecopy($img_block, $img_rank_src, $matches[1], $matches[2], 0, 0, $img_rank_width, $img_rank_height);
		} else {
			if ($DEBUG) { cpg_error("Could not find guild rank insignia image file \"{$rank_file}\"."); }
		}
	}

	// Place text lines.
	foreach($template['TEXT'] as $line) {
		if (preg_match('/^(\d+)([A|I]?)\s+(\d+)\s+(\d+)\s+(\w+)\s+([\w\.]+)\s+(.*)/', $line, $matches)) {
			// Test color name.
			if (!is_int($color[$matches[5]])) {
				if ($DEBUG) { cpg_error("Could not find template value for the color \"{$matches[5]}\"."); }
				header('HTTP/1.0 404 Not Found');
				exit();
			}

			// Test font filename.
			if (!is_file("modules/{$mod_dirname}/fonts/{$matches[6]}")) {
				if ($DEBUG) { cpg_error("Could not find font file \"modules/{$mod_dirname}/fonts/{$matches[6]}\"."); }
				header('HTTP/1.0 404 Not Found');
				exit();
			}

			// Replace template data macros with roster data.
			$matches[7] = preg_replace_callback('/\{\$(\w+)\}/',
				function($m) use (&$char_info) { return $char_info[$m[1]]; },
				$matches[7]);

			// resource image, float size, int targetwidth, int x, int y, int color, string fontfile, string text
			$text_x = $text_leftmargin + $matches[3];
			$text_width = $text_rightmargin - $text_x;
			fit_imagettftext($img_block, $matches[1], $matches[2], $text_width, $text_x, $matches[4], $color[$matches[5]], "modules/{$mod_dirname}/fonts/{$matches[6]}", $matches[7]);
		}
	}

	return $img_block;
}

// Function:	fillsigcache()
// What it does:	Create all images for valid characters and store them in the signature
//			cache.
function fillsigcache() {
	// Start function duration timer.
	$timer_t = get_microtime(TRUE);

	global $db, $prefix, $suffix, $sig_savepath, $baselink, $config;

	// Register a reaper function for execution on shutdown.
	register_shutdown_function('destroyimageresource');

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

	$result = $db->sql_query("SELECT rm.* FROM {$prefix}_roster_master{$suffix} AS rm", FALSE);
	$count = $total = 0;
	while($char_info = $db->sql_fetchrow($result, SQL_ASSOC)) {
		$sig_file = "{$sig_savepath}/sig_{$char_info['characterId']}.jpg";
//		cpg_error('<pre style="text-align:left">'."{$sig_file}\n\n".htmlspecialchars(print_r($char_info, TRUE))."</pre>\n");

		// Check exclusion list.
		if (in_array($char_info['guild_rank'], $config['exclude_ranks'])) {
			// Character is in exclusion list, delete cache file if it exists and skip.
			if (is_file($sig_file)) {
				@unlink($sig_file);
			}
			continue;
		}
		$total++;

		if (is_file($sig_file) && ($mtime = filemtime($sig_file)) && ($mtime >= $char_info['last_update'])) {
			// Cache file is contemporary with roster data, cache is still valid.
			continue;
		}

		$img_block = getsigimage($char_info);

		// Cache the new signature block file and free up image resource memory.
		imagejpeg($img_block, $sig_file, $config['JPEG_quality']);
		$count++;
	}
	$db->sql_freeresult($result);
	return array($count, $total, (get_microtime(TRUE) - $timer_t));
}

// Function:	reapsigcache()
// What it does:	Purge the signature block cache of invalid signatures (i.e. remove all
//			images from the signature cache directory that do not belong to the
//			roster).
function reapsigcache() {
	// Start function duration timer.
	$timer_t = get_microtime(TRUE);

	global $db, $prefix, $suffix, $sig_savepath, $config;

	$result = $db->sql_query("SELECT characterId, guild_rank, last_update FROM {$prefix}_roster_master{$suffix}", FALSE);
	$count = $total = 0;
	while($char_info = $db->sql_fetchrow($result, SQL_ASSOC)) {
		$ids[$char_info['characterId']]['guild_rank'] = $char_info['guild_rank'];
		$ids[$char_info['characterId']]['last_update'] = $char_info['last_update'];
		$total++;
	}
	$db->sql_freeresult($result);

//	cpg_error('<pre style="text-align:left">'.print_r($ids, TRUE)."</pre>\n");

	foreach(glob("{$sig_savepath}/sig_*.jpg") as $file) {
		$matches = array();
		preg_match('/^.*?\/sig_(\d+).jpg$/', $file, $matches);
		if (is_file($file) && (!isset($ids[$matches[1]]) || (in_array($ids[$matches[1]]['guild_rank'], $config['exclude_ranks'])) || (($mtime = filemtime($file)) && ($ids[$matches[1]]['last_update'] > $mtime)))) {
			@unlink($file);
			$count++;
		}
	}

	// Log the performance statistics.
	$error = sprintf("Time to reap signature block cache (%d of %d images): %.4f sec.", $count, $total, (get_microtime(TRUE) - $timer_t));
	trigger_error($error, E_USER_NOTICE);
	if ($config['logfile'] & 128) {
		$GLOBALS['log'][] = $error;
	}
}

// Function:	fetchsignature()
// What it does:	Generates a dynamic signature block JPEG and implements cache logic.
// Requires:	$_GET['characterId'] -- Character ID
function fetchsignature() {
	global $DEBUG, $sig_savepath, $mod_dirname, $db, $prefix, $suffix, $config;

	// Don't completely initialize the CMS.
	define('XMLFEED', 1);

	// Register a reaper function for execution on shutdown.
	register_shutdown_function('destroyimageresource');

	// Get the character id from the URL, ensure it's an integer.
	$characterId = (isset($_GET['characterId'])) ? floatval($_GET['characterId']) : 0;

	// Calculate signature image file based on $sig_savepath and $characterId.
	$sig_file = "{$sig_savepath}/sig_{$characterId}.jpg";

	// ************ CACHE LOGIC ***********

	// Check for forced refresh/preview.
	$preview = 0;
	$skipcache = 0;

	if (isset($_GET['preview'])) {
		if (is_user() || can_admin($mod_dirname)) {
			$preview = intval($_GET['preview']);
			$skipcache = 1;
		} else {
			// Not a user.
			cpg_error(_ERR_ACCESSCONT);
		}
	} else {
		if (isset($_GET['refresh'])) {
			if (is_user() || can_admin($mod_dirname)) {
				$skipcache = intval($_GET['refresh']);
			} else {
				// Not a user.
				cpg_error(_ERR_ACCESSCONT);
			}
		}
	}

	// Retrieve the record matching the Character ID from the roster.
	$result = $db->sql_query("SELECT rm.* FROM {$prefix}_roster_master{$suffix} AS rm WHERE rm.characterId=".(int)$characterId, FALSE);
	if (!$db->sql_numrows($result)) {
		if ($DEBUG) { cpg_error("Signature request for invalid characterId \"{$characterId}\"."); }
		header('HTTP/1.0 404 Not Found');
		exit();
	}
	$char_info = $db->sql_fetchrow($result, SQL_ASSOC);
	$db->sql_freeresult($result);

	// Check exclusion list.
	if (in_array($char_info['guild_rank'], $config['exclude_ranks'])) {
		if ($DEBUG) { cpg_error('Character in exclusion list. No signature generated.'); }
		header('HTTP/1.0 404 Not Found');
		exit();
	}

//	cpg_error("SMSTATUS: \$sig_file = {$sig_file} (readable: ".is_readable($sig_file)."), \$characterId = {$characterId}, \$preview = {$preview}, \$skipcache = {$skipcache}");
	if (!$skipcache && !$DEBUG) {
		if (is_file($sig_file) && ($mtime = filemtime($sig_file)) && ($mtime >= $char_info['last_update'])) {
			// Cache file is contemporary with roster data, cache is still valid.
			$if_modified_since = preg_replace('/;.*$/', '', $_SERVER['HTTP_IF_MODIFIED_SINCE']);
			if (date('D, d M Y H:i:s \G\M\T', $mtime) == $if_modified_since) {
				// File is younger than If-Modified-Since, send 304 response.
				header('HTTP/1.0 304 Not Modified');
				header('X-SMCacheStatus: Browser cache valid');
				exit;
			} else {
				// Cache file has changed after If-Modified-Since time, send the cached
				// signature image.
				header('X-SMCacheStatus: Cached from server file');
				// Send appropriate headers.
				header('Cache-Control: public', TRUE);
				header('Connection: close');
				header('Content-Type: image/jpeg');
				header('Content-Length: '.filesize($sig_file));
				header('Expires: '.date('D, d M Y H:i:s \G\M\T', (time() + $config['cache_update_time'])), TRUE);
				header('Last-Modified: '.date('D, d M Y H:i:s \G\M\T', $mtime));
				header('Pragma: none', TRUE);

				// Push cached file to client.
				@readfile($sig_file);
				exit;
			}
		}
	}

	// ********** END CACHE LOGIC *********

	$img_block = getsigimage($char_info);

	// Start a new output buffer.
	ob_start(); 

	imagejpeg($img_block, NULL, $config['JPEG_quality']);
	$img_data = ob_get_contents();
	$img_length = ob_get_length();

	ob_end_clean();

	if ($preview || $DEBUG) {
		header('X-SMCacheStatus: Preview mode');
		// Send appropriate headers.
		header('Cache-Control: no-store', TRUE);
		header('Connection: close');
		header('Content-Type: image/jpeg');
		header("Content-Length: $img_length");
		header('Expires: 0', TRUE);
		header('Pragma: private', TRUE);
	} else {
		// Cache the new signature block file and free up image resource memory.
		imagejpeg($img_block, $sig_file, $config['JPEG_quality']);
		chmod($sig_file, 0664);
		$mtime = filemtime($sig_file);

		header('X-SMCacheStatus: Generated');
		// Send appropriate headers.
		header('Cache-Control: public', TRUE);
		header('Connection: close');
		header('Content-Type: image/jpeg');
		header("Content-Length: $img_length");
		header('Expires: '.date('D, d M Y H:i:s \G\M\T', (time() + $config['cache_update_time'])), TRUE);
		header('Last-Modified: '.date('D, d M Y H:i:s \G\M\T', $mtime));
		header('Pragma: none', TRUE);
	}

	// Push image to client.
	echo $img_data;
	imagedestroy($img_data);
}

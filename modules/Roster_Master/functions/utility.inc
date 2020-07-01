<?php 
/***********************************************************************
  Roster Master for Dragonfly(TM) CMS
  **********************************************************************
  Copyright (C) 2005-2020 by Dark Grue

  Module utility functions.

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

function var_dump_r($mixed = null) {
	ob_start();
	var_dump($mixed);
	$content = ob_get_contents();
	ob_end_clean();
	return $content;
}

function duration($seconds, $max_periods = 7) {
	if (empty($seconds)) return;

	$periods = array(
		'year' => 31536000,
		'month' => 2419200,
		'week' => 604800,
		'day' => 86400,
		'hour' => 3600,
		'minute' => 60,
		'second' => 1);
	$i = 1;
	foreach($periods as $period => $period_seconds) {
		$period_duration = floor($seconds / $period_seconds);
		$seconds = $seconds % $period_seconds;
		if ($period_duration == 0) { continue; }
		$duration[] = "{$period_duration} {$period}".(($period_duration > 1) ? 's' : '');
		$i++;
		if ($i >  $max_periods) break;
	}
	return implode(' ', $duration);
}

function in_array_regex($needle, $regex_haystack) {
	foreach($regex_haystack as $regex) {
		$match = preg_match($regex, $needle);
		if ($match === 1) { return TRUE; }
	}
	return FALSE;
}

function array_equal($a, $b, $strict = TRUE) {
	// Check size of both arrays.
	if (count($a) !== count($b)) { return FALSE; }

	foreach($b as $key => $bValue) {
		// Check that expected value exists in the array.
		if (!in_array($bValue, $a, $strict)) {
			return FALSE;
		}

		// Check that expected value occurs the same amount of times in both arrays.
		if (count(array_keys($a, $bValue, $strict)) !== count(array_keys($b, $bValue, $strict))) { return FALSE; }
    }
	return TRUE;
}

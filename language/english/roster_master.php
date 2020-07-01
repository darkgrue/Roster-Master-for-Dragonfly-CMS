<?php
/***********************************************************************
  Roster Master for Dragonfly(TM) CMS
  **********************************************************************
  Copyright (C) 2005-2020 by Dark Grue

  EverQuest II guild roster management, quest tracker, and dynamic
  signature generator that integrates with the Dragonfly(TM) Content
  Management System (CMS).

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


// Marker
define('_ROSTERMASTER8', 'English');

// _ACTIVE
// _ADMIN
// _ADMINISTRATION
define('_ADORNING', 'Adorning');
define('_ADORNINGT', 'Adrn');
define('_ADVCLASS', 'Adventurer Class');
define('_ADVCLASST', 'Adv');
define('_ADVLEVEL', 'Adventurer Level');
define('_ADVLEVELT', 'Lvl');
define('_ALTS', 'Alts:');
define('_ARTCLASS', 'Artisan Class');
define('_ARTCLASST', 'Art');
define('_ARTLEVEL', 'Artisan Level');
define('_ARTLEVELT', 'Lvl');
define('_ASC', 'A'); // ascending
define('_ASCENDING', 'Ascending');
define('_AVATAR_RM', 'Avatar');
define('_BBCODE', 'BBCode');
define('_BUILDCOLLECTIONS', 'Build Collections Data');
define('_BUILDCOLLECTIONSDESC', 'Rebuild the $CollectionsData array from the Census.');
define('_BUILDICONS', 'Build Item Icon Cache');
define('_BUILDICONSDESC', 'Download and save to disk all unique collection icons.');
define('_BUILDQUESTS', 'Build Quests Data');
define('_BUILDQUESTSDESC', 'Rebuild the $Timelines arrays from the Census.');
// _BC_DELIM
// _CATEGORY
define('_CENSUS', 'Census');
define('_CHARACTER', 'Character');
define('_CHARACTERID', 'Character ID');
define('_CHARACTERTYPE', 'Character Type');
define('_CHARACTERTYPET', 'Type');
define('_CMSVERSION', 'CMS Version');
define('_CLASS', 'CLASS');
define('_COLLECTABLE', 'Collectable');
define('_COLLECTIONS', 'Collections');
define('_COMPLETED', 'Completed');
define('_COUNT', 'Count');
define('_CURRENTCLAIM', 'Current Claims');
define('_DELCLAIM', 'Delete Claim');
define('_DESC', 'D'); // descending
define('_DESCENDING', 'Descending');
define('_DEVELOPERTOOLS', 'Developer Tools');
define('_DIAGNOSTICS', 'Diagnostics');
define('_DIAGNOSTICSDESC', 'Perform system configuration diagnostics relevant to the Roster Master module.');
define('_DISPALL', 'Display All');
define('_DISPPRI', 'Display Primary');
define('_DISPLAYING', 'Displaying');
define('_DONTHAVE', 'Not Started');
define('_EDITCLAIM', 'Edit Claim');
define('_EDITCHARCLAIM', 'Edit Character Claim');
define('_END', 'End');
define('_EXPANSION', 'Expansion');
define('_FILLSIGCACHE', 'Fill Signature Block Cache');
define('_FILLSIGCACHECOMP', 'Signature block caching operation completed.');
define('_FILLSIGCACHECONF', 'Do you really wish to prepopulate all cached signature block images? This can take an extended amount of time (and exceed the time a script is allowed to run).');
define('_FILLSIGCACHEDESC', 'Fill the signature block cache.');
define('_FORCEUPDATE', 'Force Update');
define('_GROUPED', 'Grouped');
define('_GUILDBANK', 'Guild Bank');
define('_GUILDLEVEL', 'Guild Level');
define('_GUILDNAME', 'Guild Name');
define('_GUILDRANK', 'Guild Rank');
define('_GUILDRANKT', 'Rank');
define('_HAVESTARTER', 'Have Starter');
define('_HIGHESTMAGICHIT', 'Max Magic');
define('_HIGHESTMELEEHIT', 'Max Melee');
define('_HTML', 'HTML');
define('_INPROGRESS', 'In Progress');
define('_JOINEDON', 'Joined');
define('_KVDRATIO', 'KVD Ratio');
define('_LASTNAME', 'Lastname');
define('_LASTONLINE', 'Online');
define('_LASTUPDATE', 'Last Updated');
define('_LEGEND', 'Legend');
define('_LEVEL', 'Level');
define('_LINKTOCACHED', 'Link to Cached Image');
define('_LINKTOCHAR', 'Link Image to Character');
define('_LOG', 'Log');
define('_LOGPRGECOMP', 'Logfile purge operation completed.');
define('_LOOKUPGID', 'Lookup Guild ID');
define('_LOOKUPGIDDESC', 'Find a guild\'s unique Guild ID by searching guild names and servers. (<a href="http://en.wikipedia.org/wiki/Regular_expression" target="_blank">Regular expression</a> syntax allowed.)');
define('_LOOKUPGIDTRUNC', '%d results found - displaying first %d only.<br /><br />If the results you are looking for do not appear in this list, narrow your search by making it more specific.<br /><b>TIP:</b> Anchoring the start of your search string by prefixing it with a "^" to anchor the start of the search and/or "$" to anchor the end will usually produce the desired results.');
define('_MANAGECHAR', 'Manage Characters');
define('_MANAGEROSTER', 'Manage Roster');
define('_MEMBERCOUNT', 'Members');
define('_META', 'META');
define('_NEWCLAIM', 'New Claim');
// _NAME
define('_NAMET', 'Name');
// _NO
// _NONE
define('_OF', 'of');
define('_OPCANCEL', 'Operation CANCELLED by user request.');
define('_OPTIMZETABLES', 'Optimize Tables');
define('_OPTIMIZETABLESDESC', 'Optimize the database tables by removing all widowed and orphaned records from the database.');
define('_OPTIMIZETABLESCONF', 'Do you wish to delete all widowed and orphaned claims? <b>NOTE:</b> This will delete the character claim, all quest status, and dynamic signature configurations for all affected records.');
define('_OPTIMIZETABLESCOMP', 'Table optimization completed.');
define('_ORPHANCLAIM', 'Orphaned Claims');
define('_PHPDIAGNOSTICSDESC', 'Display core PHP configuration for PHP.');
define('_PHPINFO', 'PHP Info');
define('_PHPVERSION', 'PHP Version');
define('_PRIMARY', 'Primary');
define('_PROGRESS', 'Progress');
define('_PURGELOGDESC', 'Empty the log file of all records.');
define('_PURGELOGCONF', 'Do you wish to purge the logfile of all data?');
define('_PURGELOGFILE', 'Purge Logfile');
define('_PURGESIGCACHE', 'Purge Signature Block Cache');
define('_PURGESIGCACHEDESC', 'Purge the signature block cache.');
define('_PURGESIGCACHECOMP', 'Signature block cache purge operation completed.<br /><br />Removed %d signature files from cache.');
define('_PURGESIGCACHECONF', 'Do you really wish to remove all cached signature block images?');
define('_PURGETABLE', 'Purge Roster Table');
define('_PURGETABLECONF', 'Do you wish to purge the Roster Table of all parsed roster data? <b>NOTE:</b> This will <i>not</i> affect Claims. If you need to zero out all Roster Master data, uninstall, then reinstall the module from the <a href="%s">'._MODULESADMIN.'</a> panel.');
define('_PURGETABLECOMP', 'Purge table operation completed.');
define('_PURGETABLEDESC', 'Purge parsed Roster Master data from database tables.');
define('_QUERY', 'Query');
define('_QUERYQS', 'Query Quest Status');
define('_QUEST', 'Quest');
define('_QUESTS', 'Quests');
define('_QUESTST', 'Quests');
define('_QUESTNAME', 'Quest Name');
define('_RACE', 'Race');
define('_RACET', 'Race');
define('_RECLVL', 'Recommended Level');
define('_RECLVLT', 'Rec Lvl');
define('_REQLVL', 'Required Level');
define('_REQLVLT', 'Req Lvl');
define('_REWARD', 'Reward');
define('_ROSTER', 'Roster');
define('_SAVECLAIM', 'Save Claim');
// _SEARCH
define('_SERVER', 'Server');
define('_SIGIMG', 'Signature Block Image');
define('_SIGLINK', 'Signature Block Link');
define('_SIGPREVIEW', 'Signature Block Preview');
define('_SITEROOT', 'Path from site root');
define('_SHOWALL', 'Showing all characters.');
define('_SHOWPRI', 'Showing Primary characters only.');
define('_SHOWCO', 'Show your Claimed characters only');
define('_SHOWCQ', 'Show completed quests');
define('_SHOWIQ', 'Show inactive quests');
define('_SORT', 'Sort');
define('_SORTPRI', 'Primary Sort');
define('_SORTSEC', 'Secondary Sort');
define('_STAGE', 'Stage');
define('_STARTER', 'Starter?');
define('_STATUSPOINTS', 'Status');
define('_STATUSPOINTST', 'Status');
define('_STEP', 'Step');
define('_SYNCCLAIMS', 'Sync Active Claims');
define('_TINKERING', 'Tinkering');
define('_TINKERINGT', 'Tink');
define('_TOTAL', 'Total');
define('_TOTALMEM', 'Total Members');
define('_TRACKERCOLLECT', 'Collection Quest Tracker');
define('_TRACKERHERITAGE', 'Heritage Quest Tracker');
define('_TRACKERKEY', 'Access Quest Tracker');
define('_TRACKERTIMELINE', 'Timeline Quest Tracker');
// _TYPE
define('_UNCLAIMEDCHARS', 'Unclaimed Characters');
define('_UNGROUPED', 'Ungrouped');
define('_UNKNOWN_RM', 'Unknown');
define('_USERID_RM', 'User ID');
define('_USERNAME_RM', 'Username');
define('_USERNAMET', 'User');
define('_VIEWLOG', 'View the Roster Master module log.');
define('_VIEWLOGFILE', 'View Logfile');
define('_VIEWROSTER', 'Guild Roster');
define('_WIDOWCLAIM', 'Widowed Claims');
// _YES

define('_LOG_NOLOGFILE', 'No Logfile Found.');
define('_LOG_NOLOGCONTENT', 'Log is empty.');
define('_LOG_PROCESSCOMPLETE', 'Parsed %d (of %d total) character record(s) into the roster database. Removed %d characters. Deleted %d table rows from removed characters.');
define('_LOG_ROSTERADD', 'Roster change: %s added to local database.');
define('_LOG_ROSTERDEL', 'Roster change: %s removed from local database.');
define('_LOG_ROSTERFIELDCHANGE', '%s: %s was %s, is now %s.');
define('_LOG_SUMMARYCHANGE', 'Guild Summary: %s was %s, is now %s.');
define('_LOG_UPDATE', 'Update');

define('_NTC_GETJSON', 'Time to retrieve raw JSON %s collection (%s): %.4f sec.');
define('_NTC_JSONPARSE', 'Time to parse JSON %s collection: %.4f sec, time to process: %.4f sec, total parser time: %.4f sec.');
define('_NTC_RSTRDISPLAY', 'Time to display roster (%s): %.4f sec.');
define('_NTC_SMCFILL', 'Time to populate signature block cache (%d of %d images): %.4f sec.');
define('_NTC_UPDATEDATA', 'Time to update Guild Summary: %.4f sec total (+%.4f sec for parse, +%.4f for db write, +%.4f sec processing);<br />
Character Collection: %.4f sec total (+%.4f sec for parse, +%.4f for db write [prep statements: %.4f sec, delete tables: %.4f sec, write roster %.4f sec, write collections %.4f sec, write quests %.4f sec], +%.4f sec processing);<br />
total update time: %.4f sec.');

define('_ERR_ACCESSCONT', 'FATAL: Access control error. You are not permitted to complete this operation.');
define('_ERR_CLAIMNONE', 'No characters claimed.');
define('_ERR_CLAIMREADFAIL', 'FATAL: Unable to retrieve claim from database.');
define('_ERR_CLAIMSAVEFAIL', 'FATAL: Unable to add claim to database.');
define('_ERR_CSVFAIL', 'CSV Parse Failure: no matches found.');
define('_ERR_CURLINFO_SIZE_DOWNLOAD', 'cURL downloaded %s (raw), %s (uncompressed); %.2f%% savings due to compression.');
define('_ERR_CURLERR', 'FATAL: cURL error "%s" returned.');
define('_ERR_CURLNOTFOUND', 'WARNING: cURL support not found. Falling back to internal method.');
define('_ERR_DBCE', 'Database consistency error, more than one result returned.');
define('_ERR_FATALCONFIG_RM', 'FATAL: A configuration error has occurred.');
define('_ERR_FILENOTFOUND', 'FATAL: File "%s" not found.');
define('_ERR_FILEHANLEFAIL', 'FAIL: Failed to fopen() file handle for file "%s".');
define('_ERR_GUILDNOTFOUND', 'No guild found!');
define('_ERR_HTTPRESPONSEFAIL', 'FATAL: Recieved HTTP response status code other than success or redirection. Actual code recieved was %s. Error may indicate URL path (%s) is faulty.');
define('_ERR_INSTANTIATION', 'The module installation directory, <nobr>"%s"</nobr>, does not follow the expected instantation syntax of <nobr>"%s[_<i>&lt;integer&gt;</i>]"</nobr>.');
define('_ERR_JSON_FAIL1', 'WARNING: fetch_url() unable to contact the JSON collection URL, Sleeping %s seconds...');
define('_ERR_JSON_FAIL2a', 'FATAL: Gave up trying to contact the JSON collection URL.');
define('_ERR_JSON_FAIL2b', 'Try refreshing this page or visiting %s directly.');
define('_ERR_PARSE', 'FATAL: There was an error during the parse operation.');
define('_ERR_LOGWRITE', 'Unable to open logfile.inc for writing.<br />Make sure that the webserver has write permission set for this file AND the install folder.');
define('_ERR_NOSERVICEID', 'WARNING: Missing Service ID. Rate limited to less than 10 queries per minute. Retrying in 60 seconds...');
define('_ERR_PROCESSFAIL', 'FATAL: %s parser processing failure. Roster database NOT updated.');
define('_ERR_ROSTEREXPIRED', 'Roster data cache expired. Data stale for %s');
define('_ERR_STREAMBLOCKING', 'NOTICE: Unable to set stream to non-blocking mode. (Safe to ignore.)');
define('_ERR_STREAMTIMEOUT', 'WARNING: Stream timed out!');
define('_ERR_URL', 'Fetching URL: ');
define('_ERR_URLREDIRECT', 'REDIRECT to URL: ');

define('_ERR_XML_FAIL1', 'WARNING: fetch_url() unable to contact the XML collection page, Sleeping %s seconds...');
define('_ERR_XML_FAIL2a', 'FATAL: Gave up trying to contact the XML collection page.');
define('_ERR_XML_FAIL2b', 'Try refreshing this page or visiting %s directly.');

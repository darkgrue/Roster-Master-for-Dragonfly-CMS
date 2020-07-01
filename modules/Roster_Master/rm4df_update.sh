#!/bin/sh
#***********************************************************************
# Roster Master for Dragonfly(TM) CMS
# **********************************************************************
# Copyright (C) 2005-2020 by Dark Grue
#
# Shell script to update the roster using an external process.
# Requires the following settings in config.inc file:
#   $config['cache_update_time'] = 0
#   $config['read_file_hack'] = TRUE;
# Requires sed and wget.
# Invoke via crontab using something of the form (example runs every six
# hours):
# 0 0,6,12,18 * * *	/complete-path-to/rm4df_update.sh
#
# License:
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or (at
# your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
# 02111-1307 USA
#**********************************************************************/


# Location of program binaries. Edit as appropriate.
SED=/usr/bin/sed
WGET=/usr/local/bin/wget

# Parse Guild ID from config.inc file in this directory.
GUILD_ID=`$SED -n 's/^$config\['\''guild_id'\''\] = \([0-9][0-9]*\);.*/\1/p' config.inc`

# Get XML guild collection page.
wget --output-document=./guild.html --quiet 'http://census.daybreakgames.com/xml/get/eq2/guild/'$GUILD_ID'/'
# Get XML character collection page.
wget --output-document=./character.xml --quiet 'http://census.daybreakgames.com/xml/get/eq2/character/?guild.id='$GUILD_ID'&c:limit=999&c:show=visible,dbid,last_update,displayname,playedtime,pvp,secondarytradeskills,collections,type,tradeskills,achievements,statistics,locationdata,name,arena,account,guild,quests&c:join=character_misc^on:id^to:id^inject_at:character_misc^show:collection_list\'quest_list\'completed_quest_list';
	
# Trigger Roster Master update (requires client be bound to localhost).
wget --bind-address=127.0.0.1 --output-document=/dev/null --quiet 'http://<yoursitename>/index.php?name=Roster_Master&force_update=1'

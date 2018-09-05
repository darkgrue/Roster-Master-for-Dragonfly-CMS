#
# Table structure for table 'cms_history'
#

DROP TABLE IF EXISTS cms_history;
CREATE TABLE cms_history (
	eid INT(11) NOT NULL AUTO_INCREMENT,
	did TINYINT(4) DEFAULT '0' NOT NULL,
	mid TINYINT(4) DEFAULT '0' NOT NULL,
	yid SMALLINT(6) DEFAULT '0' NOT NULL,
	content MEDIUMTEXT NOT NULL,
	language VARCHAR(30) NOT NULL,
	PRIMARY KEY (eid)
);


#
# Dumping data for table 'cms_history'
#

INSERT INTO cms_history  VALUES ('1', '14', '3', '0', 'International PI day!', 'english');
INSERT INTO cms_history  VALUES ('2', '5', '7', '1687', 'Newton\'s Principia is published. Math classes now more difficult with the introduction of this new fangled calculus stuff.', 'english');
INSERT INTO cms_history  VALUES ('3', '23', '9', '1889', 'Nintento Company founded - to make playing cards.', 'english');
INSERT INTO cms_history  VALUES ('4', '9', '9', '1945', 'First bug found in the ENIAC - a moth.', 'english');
INSERT INTO cms_history  VALUES ('5', '16', '11', '1952', 'Shigeru Miyamoto\'s birthday. As a baby, he enjoys collecting coins.', 'english');
INSERT INTO cms_history  VALUES ('6', '8', '9', '1966', 'First episode of Star Trek airs. The series lasts only three years.', 'english');
INSERT INTO cms_history  VALUES ('7', '22', '5', '1973', 'Ethernet invented. Thus begins the ascendancy of the geek.', 'english');
INSERT INTO cms_history  VALUES ('8', '7', '1', '1980', '\"Space Invaders\" debuts on Atari 2600. Programmer Rick Mauer earns only $11,000 for the game, which grosses over $100 Million.', 'english');
INSERT INTO cms_history  VALUES ('9', '1', '4', '1982', '\"Pac Man\" debuts on Atari 2600. Programmer Tod Frye earns $1 million for the the game.', 'english');
INSERT INTO cms_history  VALUES ('10', '9', '7', '1982', 'Theatrical release of TRON. Always remember arcade culture. Always.', 'english');
INSERT INTO cms_history  VALUES ('11', '3', '1', '1983', 'Computer named 1982 \"Man of the Year\" by Time Magazine.', 'english');
INSERT INTO cms_history  VALUES ('12', '20', '11', '1983', 'Microsoft releases Windows 1.0 for IBM and COMPAQ computers.', 'english');
INSERT INTO cms_history  VALUES ('13', '30', '4', '1993', 'The World Wide Web is born at CERN.', 'english');
INSERT INTO cms_history  VALUES ('14', '19', '9', '1995', 'Talk Like A Pirate Day', 'english');
INSERT INTO cms_history  VALUES ('15', '10', '2', '1996', 'IBM\'s Deep Blue spanks world champion Garry Kasparov in a game of chess.', 'english');
INSERT INTO cms_history  VALUES ('16', '4', '8', '1997', 'Skynet goes online. After starting a nuclear war, sends Terminators into the past.', 'english');
INSERT INTO cms_history  VALUES ('17', '7', '9', '1998', 'Stanford students Larry Page and Sergey Brin found Google.', 'english');
INSERT INTO cms_history  VALUES ('18', '16', '3', '1999', '989 Studios announces that its massive online RPG EverQuest is now available in stores, and the game servers are up and running.', 'english');
INSERT INTO cms_history  VALUES ('19', '26', '3', '1999', 'A lap dancer from Flroida becomes the inspiration for the infamous Melissa worm.', 'english');
INSERT INTO cms_history  VALUES ('20', '10', '3', '2000', 'The Dot Com Bubble bursts. ROFLCOPTER.', 'english');
INSERT INTO cms_history  VALUES ('21', '22', '3', '2233', 'James T. Kirk born in Riverside, Iowa.', 'english');
INSERT INTO cms_history  VALUES ('22', '5', '4', '2063', 'Humans experience first contact with Vulcans in Montana.', 'english');
INSERT INTO cms_history  VALUES ('23', '24', '4', '2000', 'The first EverQuest expansion, The Ruins of Kunark, is released.', 'english');
INSERT INTO cms_history  VALUES ('24', '31', '5', '2000', 'Sony Pictures Entertainment acquires online gaming company Verant Interactive.', 'english');
INSERT INTO cms_history  VALUES ('25', '30', '10', '2000', 'EverQuest signs 300,000th subscriber.', 'english');
INSERT INTO cms_history  VALUES ('26', '5', '12', '2000', 'The second EverQuest expansion, The Scars of Velious, is released in North America.', 'english');
INSERT INTO cms_history  VALUES ('27', '15', '12', '2000', 'The second EverQuest expansion, Scars of Velious, is released in Europe.', 'english');
INSERT INTO cms_history  VALUES ('28', '21', '6', '2001', 'EverQuest signs 400,000th subscriber.', 'english');
INSERT INTO cms_history  VALUES ('29', '17', '9', '2001', 'EverQuest: Trilogy (a collection that offers the original game as well as the first two expansion packs: The Ruins of Kunark and The Scars of Velious) is released.', 'english');
INSERT INTO cms_history  VALUES ('30', '2', '12', '2001', 'The third EverQuest expansion, The Shadows of Luclin, is released in North America.', 'english');
INSERT INTO cms_history  VALUES ('31', '7', '12', '2001', 'The third EverQuest expansion, The Shadows of Luclin, is released in Europe', 'english');
INSERT INTO cms_history  VALUES ('32', '12', '2', '2002', 'EverQuest Legends, a new premium server, launches.', 'english');
INSERT INTO cms_history  VALUES ('33', '25', '4', '2002', 'The subscription rate for EverQuest increased to $12.95 per month.', 'english');
INSERT INTO cms_history  VALUES ('34', '28', '10', '2002', 'The fourth EverQuest expansion, The Planes of Power, is released.', 'english');
INSERT INTO cms_history  VALUES ('35', '21', '11', '2002', 'Localized version of EverQuest launches in Europe (French and German).', 'english');
INSERT INTO cms_history  VALUES ('36', '5', '2', '2003', 'Localized version of EverQuest launches in Japan.', 'english');
INSERT INTO cms_history  VALUES ('37', '9', '2', '2003', 'EverQuest Online Adventures (for the PlayStation 2) is released in North America.', 'english');
INSERT INTO cms_history  VALUES ('38', '4', '3', '2003', 'The fifth EverQuest expansion, The Legacy of Ykesha, is released. It was the first EverQuest content expansion available almost exclusively from Sony Online\'s direct purchase and download service.', 'english');
INSERT INTO cms_history  VALUES ('39', '15', '3', '2003', 'Localized version of EverQuest launches in Taiwan and Hong Kong.', 'english');
INSERT INTO cms_history  VALUES ('40', '15', '4', '2003', 'EverQuest: Hero\'s Call launches on select BREW phones from Verizon Wireless.', 'english');
INSERT INTO cms_history  VALUES ('41', '16', '4', '2003', 'Localized version of EverQuest launches in Korea.', 'english');
INSERT INTO cms_history  VALUES ('42', '28', '4', '2003', 'Localized version of EverQuest launches in Mainland China.', 'english');
INSERT INTO cms_history  VALUES ('43', '24', '6', '2003', 'EverQuest Macintosh Edition for OS X released.', 'english');
INSERT INTO cms_history  VALUES ('44', '2', '7', '2003', 'Everquest: Hero\'s Call is released for QUALCOMM\'s Binary Runtime Environment for Wireless (BREW) phones.', 'english');
INSERT INTO cms_history  VALUES ('45', '27', '8', '2003', 'EverQuest: Evolution (a collection that offers the original game as well as the first five expansion packs: The Ruins of Kunark, The Scars of Velious, The Shadows of Luclin, The Planes of Power and The Legacy of Ykesha) is released.', 'english');
INSERT INTO cms_history  VALUES ('46', '9', '9', '2003', 'The sixth EverQuest expansion, Lost Dungeons of Norrath, is released.', 'english');
INSERT INTO cms_history  VALUES ('47', '24', '10', '2003', 'EverQuest Online Adventures (for the PlayStation 2) is released in Europe.', 'english');
INSERT INTO cms_history  VALUES ('48', '17', '11', '2003', 'EverQuest Online Adventures: Frontiers (for the PlayStation 2) is released in North America.', 'english');
INSERT INTO cms_history  VALUES ('49', '1', '12', '2003', 'Lords of EverQuest is released in North America.', 'english');
INSERT INTO cms_history  VALUES ('50', '5', '12', '2003', 'Lords of EverQuest is released in Europe.', 'english');
INSERT INTO cms_history  VALUES ('51', '9', '2', '2004', 'The seventh EverQuest expansion, Gates of Discord, is released in North America.', 'english');
INSERT INTO cms_history  VALUES ('52', '10', '2', '2004', 'Champions of Norrath (for the PlayStation 2) is released in North America.', 'english');
INSERT INTO cms_history  VALUES ('53', '12', '2', '2004', 'The seventh EverQuest expansion, Gates of Discord, is released in Europe.', 'english');
INSERT INTO cms_history  VALUES ('54', '30', '4', '2004', 'Champions of Norrath (for the Playstation 2) is released in Europe.', 'english');
INSERT INTO cms_history  VALUES ('55', '30', '4', '2004', 'Champions of Norrath: Realms of EverQuest is released in Europe.', 'english');
INSERT INTO cms_history  VALUES ('56', '30', '4', '2004', 'Champions of Norrath: Realms of EverQuest (for the PlayStation 2) is released in Europe.', 'english');
INSERT INTO cms_history  VALUES ('57', '3', '5', '2004', 'Everquest: Hero\'s Call 2 is released for QUALCOMM\'s Binary Runtime Environment for Wireless (BREW) phones.', 'english');
INSERT INTO cms_history  VALUES ('58', '26', '5', '2004', 'Woody Hearn of GU Comics called for all EverQuest gamers to boycott the Omens of War expansion in an effort to force SOE to address existing issues with the game rather than release another \"quick-fire\" expansion. The call to boycott was rescinded after SOE held a summit to address player concerns, improve (internal and external) communication, and correct specific issues within the game.', 'english');
INSERT INTO cms_history  VALUES ('59', '14', '9', '2004', 'The eighth EverQuest expansion, Omens of War, is released in North America.', 'english');
INSERT INTO cms_history  VALUES ('60', '27', '7', '2004', 'EverQuest: Platinum (a collection that offers the original game as well as the first seven expansion packs, The Ruins of Kunark, The Scars of Velious, The Shadows of Luclin, The Planes of Power, The Legacy of Ykesha, Lost Dungeons of Norrath, and Gates of Discord) is released in North America.', 'english');
INSERT INTO cms_history  VALUES ('61', '4', '10', '2004', 'SpaceShipOne wins the Ansari X Prize for private spaceflight. Flying car still not invented.', 'english');
INSERT INTO cms_history  VALUES ('62', '8', '11', '2004', 'EverQuest II ships simultaneously to North American and European retail stores.', 'english');
INSERT INTO cms_history  VALUES ('63', '10', '11', '2004', 'EverQuest II: Collector\'s Edition ships late as a result of \"an unfortunate delay with U.S. Customs in clearing importation of recent shipments of the \"tin\" in which it is packaged\".', 'english');
INSERT INTO cms_history  VALUES ('64', '10', '12', '2004', 'Champions of Norrath (for the Playstation 2) is released in North America.', 'english');
INSERT INTO cms_history  VALUES ('65', '15', '2', '2005', 'The ninth EverQuest expansion, Dragons of Norrath, is released.', 'english');
INSERT INTO cms_history  VALUES ('66', '21', '3', '2005', 'The first EverQuest II Adventure Pack, The Bloodline Chronicles, is released.', 'english');
INSERT INTO cms_history  VALUES ('67', '28', '6', '2005', 'The second EverQuest II Adventure Pack, The Splitpaw Saga, is released.', 'english');
INSERT INTO cms_history  VALUES ('68', '13', '9', '2005', 'The tenth EverQuest expansion, Depths of Darkhollow, is released.', 'english');
INSERT INTO cms_history  VALUES ('69', '14', '9', '2005', 'The first EverQuest II expansion, Desert of Flames, is released. Marking the game\'s first introduction to player-versus-player combat.', 'english');
INSERT INTO cms_history  VALUES ('70', '20', '7', '2005', 'Station Exchange: The Official Secure Marketplace for EverQuest II Players goes live on the servers (Shadowhaven and The Bazaar).', 'english');
INSERT INTO cms_history  VALUES ('71', '21', '2', '2006', 'The eleventh EverQuest expansion, Prophecy of Ro, is released.', 'english');
INSERT INTO cms_history  VALUES ('72', '21', '2', '2006', 'The second EverQuest II expansion, Kingdom of Sky, goes live worldwide.', 'english');
INSERT INTO cms_history  VALUES ('73', '14', '6', '2005', 'The third EverQuest II Adventure Pack, The Fallen Dynasty, is released.', 'english');
INSERT INTO cms_history  VALUES ('74', '19', '9', '2006', 'The twelfth EverQuest expansion, The Serpent\'s Spine, is released.', 'english');
INSERT INTO cms_history  VALUES ('75', '14', '11', '2006', 'The third EverQuest II expansion, Echoes of Faydwer, goes live worldwide.', 'english');
INSERT INTO cms_history  VALUES ('76', '13', '2', '2007', 'The thirteenth EverQuest expansion, The Buried Sea, is released.', 'english');
INSERT INTO cms_history  VALUES ('77', '13', '11', '2007', 'The fourteenth EverQuest expansion, Secrets of Faydwer, is released.', 'english');
INSERT INTO cms_history  VALUES ('78', '13', '11', '2007', 'The fourth EverQuest II expansion, Rise of Kunark, is released.', 'english');
INSERT INTO cms_history  VALUES ('79', '6', '12', '2007', 'The eighth EverQuest II expansion, Age of Discovery, is released.', 'english');
INSERT INTO cms_history  VALUES ('80', '17', '1', '2008', 'The Judge of the 17th Vara Federal da Seção Judiciária do Estado de Minas Gerais forbade the sales of the game in the whole Brazilian territory. The reason was that the game leads the players to a loss of moral virtue and takes them into \"heavy\" psychological conflicts because of the game quests.', 'english');
INSERT INTO cms_history  VALUES ('81', '4', '3', '2008', 'Gary Gygax, \"Father of D&D\" passes away.', 'english');
INSERT INTO cms_history  VALUES ('82', '21', '10', '2008', 'The fifteenth EverQuest expansion, Seeds of Destruction, is released.', 'english');
INSERT INTO cms_history  VALUES ('83', '18', '11', '2008', 'The fifth EverQuest II expansion, The Shadow Odyssey, is released.', 'english');
INSERT INTO cms_history  VALUES ('84', '15', '12', '2009', 'The sixteenth EverQuest expansion, Underfoot, is released.', 'english');
INSERT INTO cms_history  VALUES ('85', '16', '2', '2010', 'The sixth EverQuest II expansion, Sentinel\' Fate, is released.', 'english');
INSERT INTO cms_history  VALUES ('86', '12', '10', '2010', 'The seventeenth EverQuest expansion, House of Thule, is released.', 'english');
INSERT INTO cms_history  VALUES ('87', '22', '2', '2011', 'The seventh EverQuest II expansion, Destiny of Velious, is released.', 'english');
INSERT INTO cms_history  VALUES ('88', '15', '11', '2011', 'The eighteenth EverQuest expansion, Veil of Alaris, is released.', 'english');
INSERT INTO cms_history  VALUES ('89', '27', '4', '2011', 'Sony, the parent company of SOE, released statements regarding an intrusion, on or about April 18, into the PlayStation Network, and the potential theft of up to 77 million subscribers\' personal data.', 'english');
INSERT INTO cms_history  VALUES ('90', '2', '5', '2011', 'SOE completely interrupts their online services, telling players \"We have had to take the SOE service down temporarily. In the course of our investigation into the intrusion into our systems we have discovered an issue that warrants enough concern for us to take the service down effective immediately. We will provide an update later today (Monday).\" Later, SOE disclosed that \"personal information from approximately 24.6 million SOE accounts may have been stolen\", including names, addresses, telephone numbers, email addresses, gender, date of birth, login ID, and hashed passwords.', 'english');
INSERT INTO cms_history  VALUES ('91', '6', '12', '2011', 'EverQuest II switched to a free-to-play model, with optional subscriptions.', 'english');
INSERT INTO cms_history  VALUES ('92', '28', '11', '2012', 'The nineteenth EverQuest expansion, Rain of Fear, is released.', 'english');
INSERT INTO cms_history  VALUES ('93', '13', '11', '2012', 'The ninth EverQuest II expansion, Chains of Eternity, is released.', 'english');
INSERT INTO cms_history  VALUES ('94', '8', '10', '2013', 'The twentieth EverQuest expansion, Call of the Forsaken, is released.', 'english');
INSERT INTO cms_history  VALUES ('95', '12', '11', '2013', 'The tenth EverQuest II expansion, Tears of Veeshan, is released.', 'english');
INSERT INTO cms_history  VALUES ('96', '28', '1', '2014', 'The twenty-first EverQuest expansion, The Darkened Sea, is released.', 'english');
INSERT INTO cms_history  VALUES ('97', '11', '11', '2014', 'The eleventh EverQuest II expansion, Altar of Malice, is released.', 'english');
INSERT INTO cms_history  VALUES ('98', '18', '1', '2015', 'The twenty-second EverQuest expansion, The Broken Mirror, is released.', 'english');
INSERT INTO cms_history  VALUES ('99', '2', '2', '2015', 'Sony announces the sale of Sony Online Entertainment LLC (SOE) to the private equity group Columbus Nova for an undisclosed amount, and that it would be renamed Daybreak Game Company. The studio will operate as an independent company and continue operating SOE\'s existing games.', 'english');
INSERT INTO cms_history  VALUES ('100', '17', '11', '2015', 'The twelfth EverQuest II expansion, Terrors of Thalumbra, is released.', 'english');
INSERT INTO cms_history  VALUES ('101', '16', '1', '2016', 'The twenty-third EverQuest expansion, Empires of Kunark, is released.', 'english');
INSERT INTO cms_history  VALUES ('102', '17', '8', '2016', 'Legends of Norrath, an online digital collectible card game associated with EverQuest and EverQuest II, was discontinued.', 'english');
INSERT INTO cms_history  VALUES ('103', '3', '11', '2016', 'Daybreak discontinues development of EverQuest Next.', 'english');
INSERT INTO cms_history  VALUES ('104', '15', '11', '2016', 'The thirteenth EverQuest II expansion, Kunark Ascending, is released.', 'english');
INSERT INTO cms_history  VALUES ('105', '28', '11', '2017', 'The fourteenth EverQuest II expansion, Planes of Prophecy, is released.', 'english');
INSERT INTO cms_history  VALUES ('106', '12', '12', '2017', 'The twenty-fourth EverQuest expansion, Ring of Scale, is released.', 'english');

CREATE TABLE  `mcpp`.`painting` (
`name` VARCHAR( 13 ) NOT NULL ,
`sizeX` INT NOT NULL ,
`sizeY` INT NOT NULL ,
`itemId` INT NOT NULL ,
`itemData` INT NOT NULL ,
PRIMARY KEY (  `name` )
) ENGINE = INNODB;

INSERT INTO `painting`(`name`,`sizeX`,`sizeY`,`itemId`,`itemData`) VALUES ('Kebab',16,16,321,-1)
    ON DUPLICATE KEY UPDATE `sizeX` = VALUES(`sizeX`),`sizeY` = VALUES(`sizeY`),`itemId` = VALUES(`itemId`),`itemData` = VALUES(`itemData`);
INSERT INTO `painting`(`name`,`sizeX`,`sizeY`,`itemId`,`itemData`) VALUES ('Aztec',16,16,321,-1)
    ON DUPLICATE KEY UPDATE `sizeX` = VALUES(`sizeX`),`sizeY` = VALUES(`sizeY`),`itemId` = VALUES(`itemId`),`itemData` = VALUES(`itemData`);
INSERT INTO `painting`(`name`,`sizeX`,`sizeY`,`itemId`,`itemData`) VALUES ('Alban',16,16,321,-1)
    ON DUPLICATE KEY UPDATE `sizeX` = VALUES(`sizeX`),`sizeY` = VALUES(`sizeY`),`itemId` = VALUES(`itemId`),`itemData` = VALUES(`itemData`);
INSERT INTO `painting`(`name`,`sizeX`,`sizeY`,`itemId`,`itemData`) VALUES ('Aztec2',16,16,321,-1)
    ON DUPLICATE KEY UPDATE `sizeX` = VALUES(`sizeX`),`sizeY` = VALUES(`sizeY`),`itemId` = VALUES(`itemId`),`itemData` = VALUES(`itemData`);
INSERT INTO `painting`(`name`,`sizeX`,`sizeY`,`itemId`,`itemData`) VALUES ('Bomb',16,16,321,-1)
    ON DUPLICATE KEY UPDATE `sizeX` = VALUES(`sizeX`),`sizeY` = VALUES(`sizeY`),`itemId` = VALUES(`itemId`),`itemData` = VALUES(`itemData`);
INSERT INTO `painting`(`name`,`sizeX`,`sizeY`,`itemId`,`itemData`) VALUES ('Plant',16,16,321,-1)
    ON DUPLICATE KEY UPDATE `sizeX` = VALUES(`sizeX`),`sizeY` = VALUES(`sizeY`),`itemId` = VALUES(`itemId`),`itemData` = VALUES(`itemData`);
INSERT INTO `painting`(`name`,`sizeX`,`sizeY`,`itemId`,`itemData`) VALUES ('Wasteland',16,16,321,-1)
    ON DUPLICATE KEY UPDATE `sizeX` = VALUES(`sizeX`),`sizeY` = VALUES(`sizeY`),`itemId` = VALUES(`itemId`),`itemData` = VALUES(`itemData`);
INSERT INTO `painting`(`name`,`sizeX`,`sizeY`,`itemId`,`itemData`) VALUES ('Pool',32,16,321,-1)
    ON DUPLICATE KEY UPDATE `sizeX` = VALUES(`sizeX`),`sizeY` = VALUES(`sizeY`),`itemId` = VALUES(`itemId`),`itemData` = VALUES(`itemData`);
INSERT INTO `painting`(`name`,`sizeX`,`sizeY`,`itemId`,`itemData`) VALUES ('Courbet',32,16,321,-1)
    ON DUPLICATE KEY UPDATE `sizeX` = VALUES(`sizeX`),`sizeY` = VALUES(`sizeY`),`itemId` = VALUES(`itemId`),`itemData` = VALUES(`itemData`);
INSERT INTO `painting`(`name`,`sizeX`,`sizeY`,`itemId`,`itemData`) VALUES ('Sea',32,16,321,-1)
    ON DUPLICATE KEY UPDATE `sizeX` = VALUES(`sizeX`),`sizeY` = VALUES(`sizeY`),`itemId` = VALUES(`itemId`),`itemData` = VALUES(`itemData`);
INSERT INTO `painting`(`name`,`sizeX`,`sizeY`,`itemId`,`itemData`) VALUES ('Sunset',32,16,321,-1)
    ON DUPLICATE KEY UPDATE `sizeX` = VALUES(`sizeX`),`sizeY` = VALUES(`sizeY`),`itemId` = VALUES(`itemId`),`itemData` = VALUES(`itemData`);
INSERT INTO `painting`(`name`,`sizeX`,`sizeY`,`itemId`,`itemData`) VALUES ('Creebet',32,16,321,-1)
    ON DUPLICATE KEY UPDATE `sizeX` = VALUES(`sizeX`),`sizeY` = VALUES(`sizeY`),`itemId` = VALUES(`itemId`),`itemData` = VALUES(`itemData`);
INSERT INTO `painting`(`name`,`sizeX`,`sizeY`,`itemId`,`itemData`) VALUES ('Wanderer',16,32,321,-1)
    ON DUPLICATE KEY UPDATE `sizeX` = VALUES(`sizeX`),`sizeY` = VALUES(`sizeY`),`itemId` = VALUES(`itemId`),`itemData` = VALUES(`itemData`);
INSERT INTO `painting`(`name`,`sizeX`,`sizeY`,`itemId`,`itemData`) VALUES ('Graham',16,32,321,-1)
    ON DUPLICATE KEY UPDATE `sizeX` = VALUES(`sizeX`),`sizeY` = VALUES(`sizeY`),`itemId` = VALUES(`itemId`),`itemData` = VALUES(`itemData`);
INSERT INTO `painting`(`name`,`sizeX`,`sizeY`,`itemId`,`itemData`) VALUES ('Match',32,32,321,-1)
    ON DUPLICATE KEY UPDATE `sizeX` = VALUES(`sizeX`),`sizeY` = VALUES(`sizeY`),`itemId` = VALUES(`itemId`),`itemData` = VALUES(`itemData`);
INSERT INTO `painting`(`name`,`sizeX`,`sizeY`,`itemId`,`itemData`) VALUES ('Bust',32,32,321,-1)
    ON DUPLICATE KEY UPDATE `sizeX` = VALUES(`sizeX`),`sizeY` = VALUES(`sizeY`),`itemId` = VALUES(`itemId`),`itemData` = VALUES(`itemData`);
INSERT INTO `painting`(`name`,`sizeX`,`sizeY`,`itemId`,`itemData`) VALUES ('Stage',32,32,321,-1)
    ON DUPLICATE KEY UPDATE `sizeX` = VALUES(`sizeX`),`sizeY` = VALUES(`sizeY`),`itemId` = VALUES(`itemId`),`itemData` = VALUES(`itemData`);
INSERT INTO `painting`(`name`,`sizeX`,`sizeY`,`itemId`,`itemData`) VALUES ('Void',32,32,321,-1)
    ON DUPLICATE KEY UPDATE `sizeX` = VALUES(`sizeX`),`sizeY` = VALUES(`sizeY`),`itemId` = VALUES(`itemId`),`itemData` = VALUES(`itemData`);
INSERT INTO `painting`(`name`,`sizeX`,`sizeY`,`itemId`,`itemData`) VALUES ('SkullAndRoses',32,32,321,-1)
    ON DUPLICATE KEY UPDATE `sizeX` = VALUES(`sizeX`),`sizeY` = VALUES(`sizeY`),`itemId` = VALUES(`itemId`),`itemData` = VALUES(`itemData`);
INSERT INTO `painting`(`name`,`sizeX`,`sizeY`,`itemId`,`itemData`) VALUES ('Wither',32,32,321,-1)
    ON DUPLICATE KEY UPDATE `sizeX` = VALUES(`sizeX`),`sizeY` = VALUES(`sizeY`),`itemId` = VALUES(`itemId`),`itemData` = VALUES(`itemData`);
INSERT INTO `painting`(`name`,`sizeX`,`sizeY`,`itemId`,`itemData`) VALUES ('Fighters',64,32,321,-1)
    ON DUPLICATE KEY UPDATE `sizeX` = VALUES(`sizeX`),`sizeY` = VALUES(`sizeY`),`itemId` = VALUES(`itemId`),`itemData` = VALUES(`itemData`);
INSERT INTO `painting`(`name`,`sizeX`,`sizeY`,`itemId`,`itemData`) VALUES ('Pointer',64,64,321,-1)
    ON DUPLICATE KEY UPDATE `sizeX` = VALUES(`sizeX`),`sizeY` = VALUES(`sizeY`),`itemId` = VALUES(`itemId`),`itemData` = VALUES(`itemData`);
INSERT INTO `painting`(`name`,`sizeX`,`sizeY`,`itemId`,`itemData`) VALUES ('Pigscene',64,64,321,-1)
    ON DUPLICATE KEY UPDATE `sizeX` = VALUES(`sizeX`),`sizeY` = VALUES(`sizeY`),`itemId` = VALUES(`itemId`),`itemData` = VALUES(`itemData`);
INSERT INTO `painting`(`name`,`sizeX`,`sizeY`,`itemId`,`itemData`) VALUES ('BurningSkull',64,64,321,-1)
    ON DUPLICATE KEY UPDATE `sizeX` = VALUES(`sizeX`),`sizeY` = VALUES(`sizeY`),`itemId` = VALUES(`itemId`),`itemData` = VALUES(`itemData`);
INSERT INTO `painting`(`name`,`sizeX`,`sizeY`,`itemId`,`itemData`) VALUES ('Skeleton',64,48,321,-1)
    ON DUPLICATE KEY UPDATE `sizeX` = VALUES(`sizeX`),`sizeY` = VALUES(`sizeY`),`itemId` = VALUES(`itemId`),`itemData` = VALUES(`itemData`);
INSERT INTO `painting`(`name`,`sizeX`,`sizeY`,`itemId`,`itemData`) VALUES ('DonkeyKong',64,48,321,-1)
    ON DUPLICATE KEY UPDATE `sizeX` = VALUES(`sizeX`),`sizeY` = VALUES(`sizeY`),`itemId` = VALUES(`itemId`),`itemData` = VALUES(`itemData`);

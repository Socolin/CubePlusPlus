INSERT INTO `blocksound`(`soundId`,`name`,`volume`,`modifier`,`stepSound`,`placeSound`,`breakSound`)
    VALUES (1,'UNKNOW',1.0,1.0,'step.stone','dig.stone','dig.stone')
    ON DUPLICATE KEY UPDATE name='UNKNOW',volume=1.0,modifier=1.0,stepSound='step.stone',placeSound='dig.stone',breakSound='dig.stone';
INSERT INTO `material`(`materialId`,`name`,`canBurn`,`replacable`,`translucent`,`requiresNoTool`,`mobilityFlag`)
    VALUES (1,'UNKNOW',false,false,false,true,0)    ON DUPLICATE KEY UPDATE name=UNKNOW,canBurn=false,replacable=false,translucent=false,requiresNoTool=false,mobilityFlag=0;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (1,'tile.stone',255,0,30.0,1.5,false,0.6,true,true,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.stone',lightOpacity=255,lightValue=0,blockResistance=30.0,blockHardness=1.5,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `blocksound`(`soundId`,`name`,`volume`,`modifier`,`stepSound`,`placeSound`,`breakSound`)
    VALUES (2,'UNKNOW',1.0,1.0,'step.grass','dig.grass','dig.grass')
    ON DUPLICATE KEY UPDATE name='UNKNOW',volume=1.0,modifier=1.0,stepSound='step.grass',placeSound='dig.grass',breakSound='dig.grass';
INSERT INTO `material`(`materialId`,`name`,`canBurn`,`replacable`,`translucent`,`requiresNoTool`,`mobilityFlag`)
    VALUES (2,'UNKNOW',false,false,false,false,0)    ON DUPLICATE KEY UPDATE name=UNKNOW,canBurn=false,replacable=false,translucent=false,requiresNoTool=true,mobilityFlag=0;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (2,'tile.grass',255,0,3.0,0.6,true,0.6,true,true,2,'',2,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.grass',lightOpacity=255,lightValue=0,blockResistance=3.0,blockHardness=0.6,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=2,soundId=2,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `blocksound`(`soundId`,`name`,`volume`,`modifier`,`stepSound`,`placeSound`,`breakSound`)
    VALUES (3,'UNKNOW',1.0,1.0,'step.gravel','dig.gravel','dig.gravel')
    ON DUPLICATE KEY UPDATE name='UNKNOW',volume=1.0,modifier=1.0,stepSound='step.gravel',placeSound='dig.gravel',breakSound='dig.gravel';
INSERT INTO `material`(`materialId`,`name`,`canBurn`,`replacable`,`translucent`,`requiresNoTool`,`mobilityFlag`)
    VALUES (3,'UNKNOW',false,false,false,false,0)    ON DUPLICATE KEY UPDATE name=UNKNOW,canBurn=false,replacable=false,translucent=false,requiresNoTool=true,mobilityFlag=0;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (3,'tile.dirt',255,0,2.5,0.5,false,0.6,true,true,3,'',3,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.dirt',lightOpacity=255,lightValue=0,blockResistance=2.5,blockHardness=0.5,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=3,soundId=3,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (4,'tile.stonebrick',255,0,30.0,2.0,false,0.6,true,true,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.stonebrick',lightOpacity=255,lightValue=0,blockResistance=30.0,blockHardness=2.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `blocksound`(`soundId`,`name`,`volume`,`modifier`,`stepSound`,`placeSound`,`breakSound`)
    VALUES (4,'UNKNOW',1.0,1.0,'step.wood','dig.wood','dig.wood')
    ON DUPLICATE KEY UPDATE name='UNKNOW',volume=1.0,modifier=1.0,stepSound='step.wood',placeSound='dig.wood',breakSound='dig.wood';
INSERT INTO `material`(`materialId`,`name`,`canBurn`,`replacable`,`translucent`,`requiresNoTool`,`mobilityFlag`)
    VALUES (4,'UNKNOW',true,false,false,false,0)    ON DUPLICATE KEY UPDATE name=UNKNOW,canBurn=true,replacable=false,translucent=false,requiresNoTool=true,mobilityFlag=0;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (5,'tile.wood',255,0,15.0,2.0,false,0.6,true,true,4,'',4,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.wood',lightOpacity=255,lightValue=0,blockResistance=15.0,blockHardness=2.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=4,soundId=4,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `material`(`materialId`,`name`,`canBurn`,`replacable`,`translucent`,`requiresNoTool`,`mobilityFlag`)
    VALUES (5,'UNKNOW',false,false,true,false,1)    ON DUPLICATE KEY UPDATE name=UNKNOW,canBurn=false,replacable=false,translucent=true,requiresNoTool=true,mobilityFlag=1;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (6,'tile.sapling',0,0,0.0,0.0,true,0.6,true,false,5,'',2,0.09999999403953552,0.0,0.09999999403953552,0.8999999761581421,0.800000011920929,0.8999999761581421,false) 
    ON DUPLICATE KEY UPDATE name='tile.sapling',lightOpacity=0,lightValue=0,blockResistance=0.0,blockHardness=0.0,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=5,soundId=2,minX=0.09999999403953552,minY=0.0,minZ=0.09999999403953552,maxX=0.8999999761581421,maxY=0.800000011920929,maxZ=0.8999999761581421,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (7,'tile.bedrock',255,0,1.8E7,-1.0,false,0.6,true,true,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.bedrock',lightOpacity=255,lightValue=0,blockResistance=1.8E7,blockHardness=-1.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `blocksound`(`soundId`,`name`,`volume`,`modifier`,`stepSound`,`placeSound`,`breakSound`)
    VALUES (5,'UNKNOW',1.0,1.0,'step.stone','dig.stone','dig.stone')
    ON DUPLICATE KEY UPDATE name='UNKNOW',volume=1.0,modifier=1.0,stepSound='step.stone',placeSound='dig.stone',breakSound='dig.stone';
INSERT INTO `material`(`materialId`,`name`,`canBurn`,`replacable`,`translucent`,`requiresNoTool`,`mobilityFlag`)
    VALUES (6,'UNKNOW',false,true,true,false,1)    ON DUPLICATE KEY UPDATE name=UNKNOW,canBurn=false,replacable=true,translucent=true,requiresNoTool=true,mobilityFlag=1;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (8,'tile.water',3,0,500.0,100.0,true,0.6,true,false,6,'',5,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.water',lightOpacity=3,lightValue=0,blockResistance=500.0,blockHardness=100.0,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=6,soundId=5,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (9,'tile.water',3,0,500.0,100.0,false,0.6,true,false,6,'',5,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.water',lightOpacity=3,lightValue=0,blockResistance=500.0,blockHardness=100.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=6,soundId=5,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `material`(`materialId`,`name`,`canBurn`,`replacable`,`translucent`,`requiresNoTool`,`mobilityFlag`)
    VALUES (7,'UNKNOW',false,true,true,false,1)    ON DUPLICATE KEY UPDATE name=UNKNOW,canBurn=false,replacable=true,translucent=true,requiresNoTool=true,mobilityFlag=1;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (10,'tile.lava',0,15,0.0,0.0,true,0.6,true,false,7,'',5,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.lava',lightOpacity=0,lightValue=15,blockResistance=0.0,blockHardness=0.0,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=7,soundId=5,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (11,'tile.lava',0,15,500.0,100.0,true,0.6,true,false,7,'',5,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.lava',lightOpacity=0,lightValue=15,blockResistance=500.0,blockHardness=100.0,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=7,soundId=5,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `blocksound`(`soundId`,`name`,`volume`,`modifier`,`stepSound`,`placeSound`,`breakSound`)
    VALUES (6,'UNKNOW',1.0,1.0,'step.sand','dig.sand','dig.sand')
    ON DUPLICATE KEY UPDATE name='UNKNOW',volume=1.0,modifier=1.0,stepSound='step.sand',placeSound='dig.sand',breakSound='dig.sand';
INSERT INTO `material`(`materialId`,`name`,`canBurn`,`replacable`,`translucent`,`requiresNoTool`,`mobilityFlag`)
    VALUES (8,'UNKNOW',false,false,false,false,0)    ON DUPLICATE KEY UPDATE name=UNKNOW,canBurn=false,replacable=false,translucent=false,requiresNoTool=true,mobilityFlag=0;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (12,'tile.sand',255,0,2.5,0.5,false,0.6,true,true,8,'',6,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.sand',lightOpacity=255,lightValue=0,blockResistance=2.5,blockHardness=0.5,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=8,soundId=6,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (13,'tile.gravel',255,0,3.0,0.6,false,0.6,true,true,8,'',3,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.gravel',lightOpacity=255,lightValue=0,blockResistance=3.0,blockHardness=0.6,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=8,soundId=3,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (14,'tile.oreGold',255,0,15.0,3.0,false,0.6,true,true,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.oreGold',lightOpacity=255,lightValue=0,blockResistance=15.0,blockHardness=3.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (15,'tile.oreIron',255,0,15.0,3.0,false,0.6,true,true,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.oreIron',lightOpacity=255,lightValue=0,blockResistance=15.0,blockHardness=3.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (16,'tile.oreCoal',255,0,15.0,3.0,false,0.6,true,true,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.oreCoal',lightOpacity=255,lightValue=0,blockResistance=15.0,blockHardness=3.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (17,'tile.log',255,0,10.0,2.0,false,0.6,true,true,4,'',4,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.log',lightOpacity=255,lightValue=0,blockResistance=10.0,blockHardness=2.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=4,soundId=4,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `material`(`materialId`,`name`,`canBurn`,`replacable`,`translucent`,`requiresNoTool`,`mobilityFlag`)
    VALUES (9,'UNKNOW',true,false,true,false,1)    ON DUPLICATE KEY UPDATE name=UNKNOW,canBurn=true,replacable=false,translucent=true,requiresNoTool=true,mobilityFlag=1;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (18,'tile.leaves',1,0,1.0,0.2,true,0.6,true,false,9,'',2,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.leaves',lightOpacity=1,lightValue=0,blockResistance=1.0,blockHardness=0.2,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=9,soundId=2,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `material`(`materialId`,`name`,`canBurn`,`replacable`,`translucent`,`requiresNoTool`,`mobilityFlag`)
    VALUES (10,'UNKNOW',false,false,false,false,0)    ON DUPLICATE KEY UPDATE name=UNKNOW,canBurn=false,replacable=false,translucent=false,requiresNoTool=true,mobilityFlag=0;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (19,'tile.sponge',255,0,3.0,0.6,false,0.6,true,true,10,'',2,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.sponge',lightOpacity=255,lightValue=0,blockResistance=3.0,blockHardness=0.6,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=10,soundId=2,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `blocksound`(`soundId`,`name`,`volume`,`modifier`,`stepSound`,`placeSound`,`breakSound`)
    VALUES (7,'UNKNOW',1.0,1.0,'step.stone','step.stone','random.glass')
    ON DUPLICATE KEY UPDATE name='UNKNOW',volume=1.0,modifier=1.0,stepSound='step.stone',placeSound='step.stone',breakSound='random.glass';
INSERT INTO `material`(`materialId`,`name`,`canBurn`,`replacable`,`translucent`,`requiresNoTool`,`mobilityFlag`)
    VALUES (11,'UNKNOW',false,false,true,false,0)    ON DUPLICATE KEY UPDATE name=UNKNOW,canBurn=false,replacable=false,translucent=true,requiresNoTool=true,mobilityFlag=0;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (20,'tile.glass',0,0,1.5,0.3,false,0.6,true,false,11,'',7,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.glass',lightOpacity=0,lightValue=0,blockResistance=1.5,blockHardness=0.3,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=11,soundId=7,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (21,'tile.oreLapis',255,0,15.0,3.0,false,0.6,true,true,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.oreLapis',lightOpacity=255,lightValue=0,blockResistance=15.0,blockHardness=3.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (22,'tile.blockLapis',255,0,15.0,3.0,false,0.6,true,true,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.blockLapis',lightOpacity=255,lightValue=0,blockResistance=15.0,blockHardness=3.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (23,'tile.dispenser',255,0,17.5,3.5,false,0.6,true,true,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.dispenser',lightOpacity=255,lightValue=0,blockResistance=17.5,blockHardness=3.5,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (24,'tile.sandStone',255,0,4.0,0.8,false,0.6,true,true,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.sandStone',lightOpacity=255,lightValue=0,blockResistance=4.0,blockHardness=0.8,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (25,'tile.musicBlock',255,0,4.0,0.8,false,0.6,true,true,4,'',5,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.musicBlock',lightOpacity=255,lightValue=0,blockResistance=4.0,blockHardness=0.8,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=4,soundId=5,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `material`(`materialId`,`name`,`canBurn`,`replacable`,`translucent`,`requiresNoTool`,`mobilityFlag`)
    VALUES (12,'UNKNOW',true,false,false,false,0)    ON DUPLICATE KEY UPDATE name=UNKNOW,canBurn=true,replacable=false,translucent=false,requiresNoTool=true,mobilityFlag=0;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (26,'tile.bed',0,0,1.0,0.2,false,0.6,true,false,12,'',5,0.0,0.0,0.0,1.0,0.5625,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.bed',lightOpacity=0,lightValue=0,blockResistance=1.0,blockHardness=0.2,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=12,soundId=5,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=0.5625,maxZ=1.0,renderAsNormal=false;
INSERT INTO `blocksound`(`soundId`,`name`,`volume`,`modifier`,`stepSound`,`placeSound`,`breakSound`)
    VALUES (8,'UNKNOW',1.0,1.5,'step.stone','dig.stone','dig.stone')
    ON DUPLICATE KEY UPDATE name='UNKNOW',volume=1.0,modifier=1.5,stepSound='step.stone',placeSound='dig.stone',breakSound='dig.stone';
INSERT INTO `material`(`materialId`,`name`,`canBurn`,`replacable`,`translucent`,`requiresNoTool`,`mobilityFlag`)
    VALUES (13,'UNKNOW',false,false,true,false,1)    ON DUPLICATE KEY UPDATE name=UNKNOW,canBurn=false,replacable=false,translucent=true,requiresNoTool=true,mobilityFlag=1;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (27,'tile.goldenRail',0,0,3.5,0.7,false,0.6,true,false,13,'',8,0.0,0.0,0.0,1.0,0.125,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.goldenRail',lightOpacity=0,lightValue=0,blockResistance=3.5,blockHardness=0.7,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=13,soundId=8,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=0.125,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (28,'tile.detectorRail',0,0,3.5,0.7,true,0.6,true,false,13,'',8,0.0,0.0,0.0,1.0,0.125,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.detectorRail',lightOpacity=0,lightValue=0,blockResistance=3.5,blockHardness=0.7,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=13,soundId=8,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=0.125,maxZ=1.0,renderAsNormal=false;
INSERT INTO `material`(`materialId`,`name`,`canBurn`,`replacable`,`translucent`,`requiresNoTool`,`mobilityFlag`)
    VALUES (14,'UNKNOW',false,false,false,false,2)    ON DUPLICATE KEY UPDATE name=UNKNOW,canBurn=false,replacable=false,translucent=false,requiresNoTool=true,mobilityFlag=2;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (29,'tile.pistonStickyBase',0,0,2.5,0.5,false,0.6,true,false,14,'',1,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.pistonStickyBase',lightOpacity=0,lightValue=0,blockResistance=2.5,blockHardness=0.5,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=14,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `material`(`materialId`,`name`,`canBurn`,`replacable`,`translucent`,`requiresNoTool`,`mobilityFlag`)
    VALUES (15,'UNKNOW',false,false,true,true,1)    ON DUPLICATE KEY UPDATE name=UNKNOW,canBurn=false,replacable=false,translucent=true,requiresNoTool=false,mobilityFlag=1;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (30,'tile.web',1,0,20.0,4.0,false,0.6,true,false,15,'',5,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.web',lightOpacity=1,lightValue=0,blockResistance=20.0,blockHardness=4.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=15,soundId=5,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `material`(`materialId`,`name`,`canBurn`,`replacable`,`translucent`,`requiresNoTool`,`mobilityFlag`)
    VALUES (16,'UNKNOW',true,true,true,false,1)    ON DUPLICATE KEY UPDATE name=UNKNOW,canBurn=true,replacable=true,translucent=true,requiresNoTool=true,mobilityFlag=1;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (31,'tile.tallgrass',0,0,0.0,0.0,true,0.6,true,false,16,'',2,0.09999999403953552,0.0,0.09999999403953552,0.8999999761581421,0.800000011920929,0.8999999761581421,false) 
    ON DUPLICATE KEY UPDATE name='tile.tallgrass',lightOpacity=0,lightValue=0,blockResistance=0.0,blockHardness=0.0,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=16,soundId=2,minX=0.09999999403953552,minY=0.0,minZ=0.09999999403953552,maxX=0.8999999761581421,maxY=0.800000011920929,maxZ=0.8999999761581421,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (32,'tile.deadbush',0,0,0.0,0.0,true,0.6,true,false,16,'',2,0.09999999403953552,0.0,0.09999999403953552,0.8999999761581421,0.800000011920929,0.8999999761581421,false) 
    ON DUPLICATE KEY UPDATE name='tile.deadbush',lightOpacity=0,lightValue=0,blockResistance=0.0,blockHardness=0.0,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=16,soundId=2,minX=0.09999999403953552,minY=0.0,minZ=0.09999999403953552,maxX=0.8999999761581421,maxY=0.800000011920929,maxZ=0.8999999761581421,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (33,'tile.pistonBase',0,0,2.5,0.5,false,0.6,true,false,14,'',1,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.pistonBase',lightOpacity=0,lightValue=0,blockResistance=2.5,blockHardness=0.5,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=14,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (34,'tile.null',0,0,2.5,0.5,false,0.6,true,false,14,'',1,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.null',lightOpacity=0,lightValue=0,blockResistance=2.5,blockHardness=0.5,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=14,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `blocksound`(`soundId`,`name`,`volume`,`modifier`,`stepSound`,`placeSound`,`breakSound`)
    VALUES (9,'UNKNOW',1.0,1.0,'step.cloth','dig.cloth','dig.cloth')
    ON DUPLICATE KEY UPDATE name='UNKNOW',volume=1.0,modifier=1.0,stepSound='step.cloth',placeSound='dig.cloth',breakSound='dig.cloth';
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (35,'tile.cloth',255,0,4.0,0.8,false,0.6,true,true,12,'',9,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.cloth',lightOpacity=255,lightValue=0,blockResistance=4.0,blockHardness=0.8,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=12,soundId=9,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (36,'tile.null',0,0,0.0,-1.0,false,0.6,true,false,14,'',5,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.null',lightOpacity=0,lightValue=0,blockResistance=0.0,blockHardness=-1.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=14,soundId=5,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (37,'tile.flower',0,0,0.0,0.0,true,0.6,true,false,5,'',2,0.30000001192092896,0.0,0.30000001192092896,0.699999988079071,0.6000000238418579,0.699999988079071,false) 
    ON DUPLICATE KEY UPDATE name='tile.flower',lightOpacity=0,lightValue=0,blockResistance=0.0,blockHardness=0.0,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=5,soundId=2,minX=0.30000001192092896,minY=0.0,minZ=0.30000001192092896,maxX=0.699999988079071,maxY=0.6000000238418579,maxZ=0.699999988079071,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (38,'tile.rose',0,0,0.0,0.0,true,0.6,true,false,5,'',2,0.30000001192092896,0.0,0.30000001192092896,0.699999988079071,0.6000000238418579,0.699999988079071,false) 
    ON DUPLICATE KEY UPDATE name='tile.rose',lightOpacity=0,lightValue=0,blockResistance=0.0,blockHardness=0.0,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=5,soundId=2,minX=0.30000001192092896,minY=0.0,minZ=0.30000001192092896,maxX=0.699999988079071,maxY=0.6000000238418579,maxZ=0.699999988079071,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (39,'tile.mushroom',0,1,0.0,0.0,true,0.6,true,false,5,'',2,0.30000001192092896,0.0,0.30000001192092896,0.699999988079071,0.4000000059604645,0.699999988079071,false) 
    ON DUPLICATE KEY UPDATE name='tile.mushroom',lightOpacity=0,lightValue=1,blockResistance=0.0,blockHardness=0.0,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=5,soundId=2,minX=0.30000001192092896,minY=0.0,minZ=0.30000001192092896,maxX=0.699999988079071,maxY=0.4000000059604645,maxZ=0.699999988079071,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (40,'tile.mushroom',0,0,0.0,0.0,true,0.6,true,false,5,'',2,0.30000001192092896,0.0,0.30000001192092896,0.699999988079071,0.4000000059604645,0.699999988079071,false) 
    ON DUPLICATE KEY UPDATE name='tile.mushroom',lightOpacity=0,lightValue=0,blockResistance=0.0,blockHardness=0.0,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=5,soundId=2,minX=0.30000001192092896,minY=0.0,minZ=0.30000001192092896,maxX=0.699999988079071,maxY=0.4000000059604645,maxZ=0.699999988079071,renderAsNormal=false;
INSERT INTO `material`(`materialId`,`name`,`canBurn`,`replacable`,`translucent`,`requiresNoTool`,`mobilityFlag`)
    VALUES (17,'UNKNOW',false,false,false,true,0)    ON DUPLICATE KEY UPDATE name=UNKNOW,canBurn=false,replacable=false,translucent=false,requiresNoTool=false,mobilityFlag=0;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (41,'tile.blockGold',255,0,30.0,3.0,false,0.6,true,true,17,'',8,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.blockGold',lightOpacity=255,lightValue=0,blockResistance=30.0,blockHardness=3.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=17,soundId=8,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (42,'tile.blockIron',255,0,30.0,5.0,false,0.6,true,true,17,'',8,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.blockIron',lightOpacity=255,lightValue=0,blockResistance=30.0,blockHardness=5.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=17,soundId=8,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (43,'tile.stoneSlab',255,0,30.0,2.0,false,0.6,true,true,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.stoneSlab',lightOpacity=255,lightValue=0,blockResistance=30.0,blockHardness=2.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (44,'tile.stoneSlab',255,0,30.0,2.0,false,0.6,true,false,1,'',1,0.0,0.0,0.0,1.0,0.5,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.stoneSlab',lightOpacity=255,lightValue=0,blockResistance=30.0,blockHardness=2.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=0.5,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (45,'tile.brick',255,0,30.0,2.0,false,0.6,true,true,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.brick',lightOpacity=255,lightValue=0,blockResistance=30.0,blockHardness=2.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `material`(`materialId`,`name`,`canBurn`,`replacable`,`translucent`,`requiresNoTool`,`mobilityFlag`)
    VALUES (18,'UNKNOW',true,false,true,false,0)    ON DUPLICATE KEY UPDATE name=UNKNOW,canBurn=true,replacable=false,translucent=true,requiresNoTool=true,mobilityFlag=0;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (46,'tile.tnt',255,0,0.0,0.0,false,0.6,true,true,18,'',2,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.tnt',lightOpacity=255,lightValue=0,blockResistance=0.0,blockHardness=0.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=18,soundId=2,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (47,'tile.bookshelf',255,0,7.5,1.5,false,0.6,true,true,4,'',4,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.bookshelf',lightOpacity=255,lightValue=0,blockResistance=7.5,blockHardness=1.5,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=4,soundId=4,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (48,'tile.stoneMoss',255,0,30.0,2.0,false,0.6,true,true,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.stoneMoss',lightOpacity=255,lightValue=0,blockResistance=30.0,blockHardness=2.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (49,'tile.obsidian',255,0,6000.0,50.0,false,0.6,true,true,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.obsidian',lightOpacity=255,lightValue=0,blockResistance=6000.0,blockHardness=50.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (50,'tile.torch',0,14,0.0,0.0,true,0.6,true,false,13,'',4,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.torch',lightOpacity=0,lightValue=14,blockResistance=0.0,blockHardness=0.0,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=13,soundId=4,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `material`(`materialId`,`name`,`canBurn`,`replacable`,`translucent`,`requiresNoTool`,`mobilityFlag`)
    VALUES (19,'UNKNOW',false,true,true,false,1)    ON DUPLICATE KEY UPDATE name=UNKNOW,canBurn=false,replacable=true,translucent=true,requiresNoTool=true,mobilityFlag=1;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (51,'tile.fire',0,15,0.0,0.0,true,0.6,false,false,19,'',4,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.fire',lightOpacity=0,lightValue=15,blockResistance=0.0,blockHardness=0.0,needsRandomTick=true,slipperiness=0.6,isCollidable=false,isOpaqueCube=false,material=19,soundId=4,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (52,'tile.mobSpawner',0,0,25.0,5.0,false,0.6,true,false,1,'',8,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.mobSpawner',lightOpacity=0,lightValue=0,blockResistance=25.0,blockHardness=5.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=1,soundId=8,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (53,'tile.stairsWood',255,0,15.0,2.0,false,0.6,true,false,4,'',4,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.stairsWood',lightOpacity=255,lightValue=0,blockResistance=15.0,blockHardness=2.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=4,soundId=4,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (54,'tile.chest',0,0,12.5,2.5,false,0.6,true,false,4,'',4,0.0625,0.0,0.0625,0.9375,0.875,0.9375,false) 
    ON DUPLICATE KEY UPDATE name='tile.chest',lightOpacity=0,lightValue=0,blockResistance=12.5,blockHardness=2.5,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=4,soundId=4,minX=0.0625,minY=0.0,minZ=0.0625,maxX=0.9375,maxY=0.875,maxZ=0.9375,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (55,'tile.redstoneDust',0,0,0.0,0.0,false,0.6,true,false,13,'',5,0.0,0.0,0.0,1.0,0.0625,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.redstoneDust',lightOpacity=0,lightValue=0,blockResistance=0.0,blockHardness=0.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=13,soundId=5,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=0.0625,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (56,'tile.oreDiamond',255,0,15.0,3.0,false,0.6,true,true,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.oreDiamond',lightOpacity=255,lightValue=0,blockResistance=15.0,blockHardness=3.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (57,'tile.blockDiamond',255,0,30.0,5.0,false,0.6,true,true,17,'',8,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.blockDiamond',lightOpacity=255,lightValue=0,blockResistance=30.0,blockHardness=5.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=17,soundId=8,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (58,'tile.workbench',255,0,12.5,2.5,false,0.6,true,true,4,'',4,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.workbench',lightOpacity=255,lightValue=0,blockResistance=12.5,blockHardness=2.5,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=4,soundId=4,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (59,'tile.crops',0,0,0.0,0.0,true,0.6,true,false,5,'',2,0.0,0.0,0.0,1.0,0.25,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.crops',lightOpacity=0,lightValue=0,blockResistance=0.0,blockHardness=0.0,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=5,soundId=2,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=0.25,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (60,'tile.farmland',255,0,3.0,0.6,true,0.6,true,false,3,'',3,0.0,0.0,0.0,1.0,0.9375,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.farmland',lightOpacity=255,lightValue=0,blockResistance=3.0,blockHardness=0.6,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=3,soundId=3,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=0.9375,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (61,'tile.furnace',255,0,17.5,3.5,false,0.6,true,true,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.furnace',lightOpacity=255,lightValue=0,blockResistance=17.5,blockHardness=3.5,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (62,'tile.furnace',255,13,17.5,3.5,false,0.6,true,true,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.furnace',lightOpacity=255,lightValue=13,blockResistance=17.5,blockHardness=3.5,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (63,'tile.sign',0,0,5.0,1.0,false,0.6,true,false,4,'',4,0.25,0.0,0.25,0.75,1.0,0.75,false) 
    ON DUPLICATE KEY UPDATE name='tile.sign',lightOpacity=0,lightValue=0,blockResistance=5.0,blockHardness=1.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=4,soundId=4,minX=0.25,minY=0.0,minZ=0.25,maxX=0.75,maxY=1.0,maxZ=0.75,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (64,'tile.doorWood',0,0,15.0,3.0,false,0.6,true,false,4,'',4,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.doorWood',lightOpacity=0,lightValue=0,blockResistance=15.0,blockHardness=3.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=4,soundId=4,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `blocksound`(`soundId`,`name`,`volume`,`modifier`,`stepSound`,`placeSound`,`breakSound`)
    VALUES (10,'UNKNOW',1.0,1.0,'step.ladder','dig.wood','dig.wood')
    ON DUPLICATE KEY UPDATE name='UNKNOW',volume=1.0,modifier=1.0,stepSound='step.ladder',placeSound='dig.wood',breakSound='dig.wood';
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (65,'tile.ladder',0,0,2.0,0.4,false,0.6,true,false,13,'',10,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.ladder',lightOpacity=0,lightValue=0,blockResistance=2.0,blockHardness=0.4,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=13,soundId=10,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (66,'tile.rail',0,0,3.5,0.7,false,0.6,true,false,13,'',8,0.0,0.0,0.0,1.0,0.125,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.rail',lightOpacity=0,lightValue=0,blockResistance=3.5,blockHardness=0.7,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=13,soundId=8,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=0.125,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (67,'tile.stairsStone',255,0,30.0,2.0,false,0.6,true,false,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.stairsStone',lightOpacity=255,lightValue=0,blockResistance=30.0,blockHardness=2.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (68,'tile.sign',0,0,5.0,1.0,false,0.6,true,false,4,'',4,0.25,0.0,0.25,0.75,1.0,0.75,false) 
    ON DUPLICATE KEY UPDATE name='tile.sign',lightOpacity=0,lightValue=0,blockResistance=5.0,blockHardness=1.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=4,soundId=4,minX=0.25,minY=0.0,minZ=0.25,maxX=0.75,maxY=1.0,maxZ=0.75,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (69,'tile.lever',0,0,2.5,0.5,false,0.6,true,false,13,'',4,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.lever',lightOpacity=0,lightValue=0,blockResistance=2.5,blockHardness=0.5,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=13,soundId=4,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (70,'tile.pressurePlate',0,0,2.5,0.5,true,0.6,true,false,1,'',1,0.0625,0.0,0.0625,0.9375,0.03125,0.9375,false) 
    ON DUPLICATE KEY UPDATE name='tile.pressurePlate',lightOpacity=0,lightValue=0,blockResistance=2.5,blockHardness=0.5,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=1,soundId=1,minX=0.0625,minY=0.0,minZ=0.0625,maxX=0.9375,maxY=0.03125,maxZ=0.9375,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (71,'tile.doorIron',0,0,25.0,5.0,false,0.6,true,false,17,'',8,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.doorIron',lightOpacity=0,lightValue=0,blockResistance=25.0,blockHardness=5.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=17,soundId=8,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (72,'tile.pressurePlate',0,0,2.5,0.5,true,0.6,true,false,4,'',4,0.0625,0.0,0.0625,0.9375,0.03125,0.9375,false) 
    ON DUPLICATE KEY UPDATE name='tile.pressurePlate',lightOpacity=0,lightValue=0,blockResistance=2.5,blockHardness=0.5,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=4,soundId=4,minX=0.0625,minY=0.0,minZ=0.0625,maxX=0.9375,maxY=0.03125,maxZ=0.9375,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (73,'tile.oreRedstone',255,0,15.0,3.0,false,0.6,true,true,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.oreRedstone',lightOpacity=255,lightValue=0,blockResistance=15.0,blockHardness=3.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (74,'tile.oreRedstone',255,9,15.0,3.0,true,0.6,true,true,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.oreRedstone',lightOpacity=255,lightValue=9,blockResistance=15.0,blockHardness=3.0,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (75,'tile.notGate',0,0,0.0,0.0,true,0.6,true,false,13,'',4,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.notGate',lightOpacity=0,lightValue=0,blockResistance=0.0,blockHardness=0.0,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=13,soundId=4,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (76,'tile.notGate',0,7,0.0,0.0,true,0.6,true,false,13,'',4,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.notGate',lightOpacity=0,lightValue=7,blockResistance=0.0,blockHardness=0.0,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=13,soundId=4,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (77,'tile.button',0,0,2.5,0.5,true,0.6,true,false,13,'',1,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.button',lightOpacity=0,lightValue=0,blockResistance=2.5,blockHardness=0.5,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=13,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `blocksound`(`soundId`,`name`,`volume`,`modifier`,`stepSound`,`placeSound`,`breakSound`)
    VALUES (11,'UNKNOW',1.0,1.0,'step.snow','dig.snow','dig.snow')
    ON DUPLICATE KEY UPDATE name='UNKNOW',volume=1.0,modifier=1.0,stepSound='step.snow',placeSound='dig.snow',breakSound='dig.snow';
INSERT INTO `material`(`materialId`,`name`,`canBurn`,`replacable`,`translucent`,`requiresNoTool`,`mobilityFlag`)
    VALUES (20,'UNKNOW',false,true,true,true,1)    ON DUPLICATE KEY UPDATE name=UNKNOW,canBurn=false,replacable=true,translucent=true,requiresNoTool=false,mobilityFlag=1;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (78,'tile.snow',0,0,0.5,0.1,true,0.6,true,false,20,'',11,0.0,0.0,0.0,1.0,0.125,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.snow',lightOpacity=0,lightValue=0,blockResistance=0.5,blockHardness=0.1,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=20,soundId=11,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=0.125,maxZ=1.0,renderAsNormal=false;
INSERT INTO `material`(`materialId`,`name`,`canBurn`,`replacable`,`translucent`,`requiresNoTool`,`mobilityFlag`)
    VALUES (21,'UNKNOW',false,false,true,false,0)    ON DUPLICATE KEY UPDATE name=UNKNOW,canBurn=false,replacable=false,translucent=true,requiresNoTool=true,mobilityFlag=0;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (79,'tile.ice',3,0,2.5,0.5,true,0.98,true,false,21,'',7,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.ice',lightOpacity=3,lightValue=0,blockResistance=2.5,blockHardness=0.5,needsRandomTick=true,slipperiness=0.98,isCollidable=true,isOpaqueCube=false,material=21,soundId=7,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `material`(`materialId`,`name`,`canBurn`,`replacable`,`translucent`,`requiresNoTool`,`mobilityFlag`)
    VALUES (22,'UNKNOW',false,false,false,true,0)    ON DUPLICATE KEY UPDATE name=UNKNOW,canBurn=false,replacable=false,translucent=false,requiresNoTool=false,mobilityFlag=0;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (80,'tile.snow',255,0,1.0,0.2,true,0.6,true,true,22,'',11,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.snow',lightOpacity=255,lightValue=0,blockResistance=1.0,blockHardness=0.2,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=22,soundId=11,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `material`(`materialId`,`name`,`canBurn`,`replacable`,`translucent`,`requiresNoTool`,`mobilityFlag`)
    VALUES (23,'UNKNOW',false,false,true,false,1)    ON DUPLICATE KEY UPDATE name=UNKNOW,canBurn=false,replacable=false,translucent=true,requiresNoTool=true,mobilityFlag=1;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (81,'tile.cactus',0,0,2.0,0.4,true,0.6,true,false,23,'',9,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.cactus',lightOpacity=0,lightValue=0,blockResistance=2.0,blockHardness=0.4,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=23,soundId=9,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `material`(`materialId`,`name`,`canBurn`,`replacable`,`translucent`,`requiresNoTool`,`mobilityFlag`)
    VALUES (24,'UNKNOW',false,false,false,false,0)    ON DUPLICATE KEY UPDATE name=UNKNOW,canBurn=false,replacable=false,translucent=false,requiresNoTool=true,mobilityFlag=0;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (82,'tile.clay',255,0,3.0,0.6,false,0.6,true,true,24,'',3,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.clay',lightOpacity=255,lightValue=0,blockResistance=3.0,blockHardness=0.6,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=24,soundId=3,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (83,'tile.reeds',0,0,0.0,0.0,true,0.6,true,false,5,'',2,0.125,0.0,0.125,0.875,1.0,0.875,false) 
    ON DUPLICATE KEY UPDATE name='tile.reeds',lightOpacity=0,lightValue=0,blockResistance=0.0,blockHardness=0.0,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=5,soundId=2,minX=0.125,minY=0.0,minZ=0.125,maxX=0.875,maxY=1.0,maxZ=0.875,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (84,'tile.jukebox',255,0,30.0,2.0,false,0.6,true,true,4,'',1,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.jukebox',lightOpacity=255,lightValue=0,blockResistance=30.0,blockHardness=2.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=4,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (85,'tile.fence',0,0,15.0,2.0,false,0.6,true,false,4,'',4,0.375,0.0,0.0,0.625,1.0,0.625,false) 
    ON DUPLICATE KEY UPDATE name='tile.fence',lightOpacity=0,lightValue=0,blockResistance=15.0,blockHardness=2.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=4,soundId=4,minX=0.375,minY=0.0,minZ=0.0,maxX=0.625,maxY=1.0,maxZ=0.625,renderAsNormal=false;
INSERT INTO `material`(`materialId`,`name`,`canBurn`,`replacable`,`translucent`,`requiresNoTool`,`mobilityFlag`)
    VALUES (25,'UNKNOW',false,false,false,false,1)    ON DUPLICATE KEY UPDATE name=UNKNOW,canBurn=false,replacable=false,translucent=false,requiresNoTool=true,mobilityFlag=1;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (86,'tile.pumpkin',255,0,5.0,1.0,true,0.6,true,true,25,'',4,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.pumpkin',lightOpacity=255,lightValue=0,blockResistance=5.0,blockHardness=1.0,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=25,soundId=4,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (87,'tile.hellrock',255,0,2.0,0.4,false,0.6,true,true,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.hellrock',lightOpacity=255,lightValue=0,blockResistance=2.0,blockHardness=0.4,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (88,'tile.hellsand',255,0,2.5,0.5,false,0.6,true,true,8,'',6,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.hellsand',lightOpacity=255,lightValue=0,blockResistance=2.5,blockHardness=0.5,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=8,soundId=6,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (89,'tile.lightgem',255,15,1.5,0.3,false,0.6,true,true,11,'',7,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.lightgem',lightOpacity=255,lightValue=15,blockResistance=1.5,blockHardness=0.3,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=11,soundId=7,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `material`(`materialId`,`name`,`canBurn`,`replacable`,`translucent`,`requiresNoTool`,`mobilityFlag`)
    VALUES (26,'UNKNOW',false,false,true,false,2)    ON DUPLICATE KEY UPDATE name=UNKNOW,canBurn=false,replacable=false,translucent=true,requiresNoTool=true,mobilityFlag=2;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (90,'tile.portal',0,11,0.0,-1.0,true,0.6,true,false,26,'',7,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.portal',lightOpacity=0,lightValue=11,blockResistance=0.0,blockHardness=-1.0,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=26,soundId=7,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (91,'tile.litpumpkin',255,15,5.0,1.0,true,0.6,true,true,25,'',4,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.litpumpkin',lightOpacity=255,lightValue=15,blockResistance=5.0,blockHardness=1.0,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=25,soundId=4,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `material`(`materialId`,`name`,`canBurn`,`replacable`,`translucent`,`requiresNoTool`,`mobilityFlag`)
    VALUES (27,'UNKNOW',false,false,false,false,1)    ON DUPLICATE KEY UPDATE name=UNKNOW,canBurn=false,replacable=false,translucent=false,requiresNoTool=true,mobilityFlag=1;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (92,'tile.cake',0,0,2.5,0.5,true,0.6,true,false,27,'',9,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.cake',lightOpacity=0,lightValue=0,blockResistance=2.5,blockHardness=0.5,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=27,soundId=9,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (93,'tile.diode',0,0,0.0,0.0,false,0.6,true,false,13,'',4,0.0,0.0,0.0,1.0,0.125,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.diode',lightOpacity=0,lightValue=0,blockResistance=0.0,blockHardness=0.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=13,soundId=4,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=0.125,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (94,'tile.diode',0,9,0.0,0.0,false,0.6,true,false,13,'',4,0.0,0.0,0.0,1.0,0.125,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.diode',lightOpacity=0,lightValue=9,blockResistance=0.0,blockHardness=0.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=13,soundId=4,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=0.125,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (95,'tile.lockedchest',255,15,0.0,0.0,true,0.6,true,true,4,'',4,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.lockedchest',lightOpacity=255,lightValue=15,blockResistance=0.0,blockHardness=0.0,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=4,soundId=4,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (96,'tile.trapdoor',0,0,15.0,3.0,false,0.6,true,false,4,'',4,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.trapdoor',lightOpacity=0,lightValue=0,blockResistance=15.0,blockHardness=3.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=4,soundId=4,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (97,'tile.monsterStoneEgg',255,0,3.75,0.75,false,0.6,true,true,24,'',5,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.monsterStoneEgg',lightOpacity=255,lightValue=0,blockResistance=3.75,blockHardness=0.75,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=24,soundId=5,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (98,'tile.stonebricksmooth',255,0,30.0,1.5,false,0.6,true,true,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.stonebricksmooth',lightOpacity=255,lightValue=0,blockResistance=30.0,blockHardness=1.5,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (99,'tile.mushroom',255,0,1.0,0.2,false,0.6,true,true,4,'',4,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.mushroom',lightOpacity=255,lightValue=0,blockResistance=1.0,blockHardness=0.2,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=4,soundId=4,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (100,'tile.mushroom',255,0,1.0,0.2,false,0.6,true,true,4,'',4,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.mushroom',lightOpacity=255,lightValue=0,blockResistance=1.0,blockHardness=0.2,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=4,soundId=4,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (101,'tile.fenceIron',0,0,30.0,5.0,false,0.6,true,false,17,'',8,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.fenceIron',lightOpacity=0,lightValue=0,blockResistance=30.0,blockHardness=5.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=17,soundId=8,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (102,'tile.thinGlass',0,0,1.5,0.3,false,0.6,true,false,11,'',7,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.thinGlass',lightOpacity=0,lightValue=0,blockResistance=1.5,blockHardness=0.3,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=11,soundId=7,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (103,'tile.melon',255,0,5.0,1.0,false,0.6,true,true,25,'',4,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.melon',lightOpacity=255,lightValue=0,blockResistance=5.0,blockHardness=1.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=25,soundId=4,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (104,'tile.pumpkinStem',0,0,0.0,0.0,true,0.6,true,false,5,'',4,0.375,0.0,0.375,0.625,0.25,0.625,false) 
    ON DUPLICATE KEY UPDATE name='tile.pumpkinStem',lightOpacity=0,lightValue=0,blockResistance=0.0,blockHardness=0.0,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=5,soundId=4,minX=0.375,minY=0.0,minZ=0.375,maxX=0.625,maxY=0.25,maxZ=0.625,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (105,'tile.pumpkinStem',0,0,0.0,0.0,true,0.6,true,false,5,'',4,0.375,0.0,0.375,0.625,0.25,0.625,false) 
    ON DUPLICATE KEY UPDATE name='tile.pumpkinStem',lightOpacity=0,lightValue=0,blockResistance=0.0,blockHardness=0.0,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=5,soundId=4,minX=0.375,minY=0.0,minZ=0.375,maxX=0.625,maxY=0.25,maxZ=0.625,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (106,'tile.vine',0,0,1.0,0.2,true,0.6,true,false,16,'',2,0.0,0.0,0.0,0.0625,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.vine',lightOpacity=0,lightValue=0,blockResistance=1.0,blockHardness=0.2,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=16,soundId=2,minX=0.0,minY=0.0,minZ=0.0,maxX=0.0625,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (107,'tile.fenceGate',0,0,15.0,2.0,false,0.6,true,false,4,'',4,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.fenceGate',lightOpacity=0,lightValue=0,blockResistance=15.0,blockHardness=2.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=4,soundId=4,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (108,'tile.stairsBrick',255,0,30.0,2.0,false,0.6,true,false,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.stairsBrick',lightOpacity=255,lightValue=0,blockResistance=30.0,blockHardness=2.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (109,'tile.stairsStoneBrickSmooth',255,0,30.0,1.5,false,0.6,true,false,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.stairsStoneBrickSmooth',lightOpacity=255,lightValue=0,blockResistance=30.0,blockHardness=1.5,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (110,'tile.mycel',255,0,3.0,0.6,true,0.6,true,true,2,'',2,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.mycel',lightOpacity=255,lightValue=0,blockResistance=3.0,blockHardness=0.6,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=2,soundId=2,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (111,'tile.waterlily',0,0,0.0,0.0,true,0.6,true,false,5,'',2,0.0,0.0,0.0,1.0,0.015625,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.waterlily',lightOpacity=0,lightValue=0,blockResistance=0.0,blockHardness=0.0,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=5,soundId=2,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=0.015625,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (112,'tile.netherBrick',255,0,30.0,2.0,false,0.6,true,true,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.netherBrick',lightOpacity=255,lightValue=0,blockResistance=30.0,blockHardness=2.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (113,'tile.netherFence',0,0,30.0,2.0,false,0.6,true,false,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.netherFence',lightOpacity=0,lightValue=0,blockResistance=30.0,blockHardness=2.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (114,'tile.stairsNetherBrick',255,0,30.0,2.0,false,0.6,true,false,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.stairsNetherBrick',lightOpacity=255,lightValue=0,blockResistance=30.0,blockHardness=2.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (115,'tile.netherStalk',0,0,0.0,0.0,true,0.6,true,false,5,'',5,0.0,0.0,0.0,1.0,0.25,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.netherStalk',lightOpacity=0,lightValue=0,blockResistance=0.0,blockHardness=0.0,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=5,soundId=5,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=0.25,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (116,'tile.enchantmentTable',0,0,6000.0,5.0,false,0.6,true,false,1,'',5,0.0,0.0,0.0,1.0,0.75,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.enchantmentTable',lightOpacity=0,lightValue=0,blockResistance=6000.0,blockHardness=5.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=1,soundId=5,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=0.75,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (117,'tile.brewingStand',0,1,2.5,0.5,false,0.6,true,false,17,'',5,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.brewingStand',lightOpacity=0,lightValue=1,blockResistance=2.5,blockHardness=0.5,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=17,soundId=5,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (118,'tile.cauldron',0,0,10.0,2.0,false,0.6,true,false,17,'',5,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.cauldron',lightOpacity=0,lightValue=0,blockResistance=10.0,blockHardness=2.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=17,soundId=5,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (119,'tile.null',0,15,1.8E7,-1.0,false,0.6,true,false,26,'',5,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.null',lightOpacity=0,lightValue=15,blockResistance=1.8E7,blockHardness=-1.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=26,soundId=5,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (120,'tile.endPortalFrame',0,1,1.8E7,-1.0,false,0.6,true,false,1,'',7,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.endPortalFrame',lightOpacity=0,lightValue=1,blockResistance=1.8E7,blockHardness=-1.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=1,soundId=7,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (121,'tile.whiteStone',255,0,45.0,3.0,false,0.6,true,true,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.whiteStone',lightOpacity=255,lightValue=0,blockResistance=45.0,blockHardness=3.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `material`(`materialId`,`name`,`canBurn`,`replacable`,`translucent`,`requiresNoTool`,`mobilityFlag`)
    VALUES (28,'UNKNOW',false,false,false,false,1)    ON DUPLICATE KEY UPDATE name=UNKNOW,canBurn=false,replacable=false,translucent=false,requiresNoTool=true,mobilityFlag=1;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (122,'tile.dragonEgg',0,1,45.0,3.0,false,0.6,true,false,28,'',1,0.0625,0.0,0.0625,0.9375,1.0,0.9375,false) 
    ON DUPLICATE KEY UPDATE name='tile.dragonEgg',lightOpacity=0,lightValue=1,blockResistance=45.0,blockHardness=3.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=28,soundId=1,minX=0.0625,minY=0.0,minZ=0.0625,maxX=0.9375,maxY=1.0,maxZ=0.9375,renderAsNormal=false;
INSERT INTO `material`(`materialId`,`name`,`canBurn`,`replacable`,`translucent`,`requiresNoTool`,`mobilityFlag`)
    VALUES (29,'UNKNOW',false,false,false,false,0)    ON DUPLICATE KEY UPDATE name=UNKNOW,canBurn=false,replacable=false,translucent=false,requiresNoTool=true,mobilityFlag=0;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (123,'tile.redstoneLight',255,0,1.5,0.3,false,0.6,true,true,29,'',7,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.redstoneLight',lightOpacity=255,lightValue=0,blockResistance=1.5,blockHardness=0.3,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=29,soundId=7,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (124,'tile.redstoneLight',255,15,1.5,0.3,false,0.6,true,true,29,'',7,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.redstoneLight',lightOpacity=255,lightValue=15,blockResistance=1.5,blockHardness=0.3,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=29,soundId=7,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (125,'tile.woodSlab',255,0,15.0,2.0,false,0.6,true,true,4,'',4,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.woodSlab',lightOpacity=255,lightValue=0,blockResistance=15.0,blockHardness=2.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=4,soundId=4,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (126,'tile.woodSlab',255,0,15.0,2.0,false,0.6,true,false,4,'',4,0.0,0.0,0.0,1.0,0.5,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.woodSlab',lightOpacity=255,lightValue=0,blockResistance=15.0,blockHardness=2.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=4,soundId=4,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=0.5,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (127,'tile.cocoa',0,0,15.0,0.2,true,0.6,true,false,5,'',4,0.25,0.1875,0.0625,0.75,0.75,0.5625,false) 
    ON DUPLICATE KEY UPDATE name='tile.cocoa',lightOpacity=0,lightValue=0,blockResistance=15.0,blockHardness=0.2,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=5,soundId=4,minX=0.25,minY=0.1875,minZ=0.0625,maxX=0.75,maxY=0.75,maxZ=0.5625,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (128,'tile.stairsSandStone',255,0,4.0,0.8,false,0.6,true,false,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.stairsSandStone',lightOpacity=255,lightValue=0,blockResistance=4.0,blockHardness=0.8,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (129,'tile.oreEmerald',255,0,15.0,3.0,false,0.6,true,true,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.oreEmerald',lightOpacity=255,lightValue=0,blockResistance=15.0,blockHardness=3.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (130,'tile.enderChest',0,7,3000.0,22.5,false,0.6,true,false,1,'',1,0.0625,0.0,0.0625,0.9375,0.875,0.9375,false) 
    ON DUPLICATE KEY UPDATE name='tile.enderChest',lightOpacity=0,lightValue=7,blockResistance=3000.0,blockHardness=22.5,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=1,soundId=1,minX=0.0625,minY=0.0,minZ=0.0625,maxX=0.9375,maxY=0.875,maxZ=0.9375,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (131,'tile.tripWireSource',0,0,0.0,0.0,true,0.6,true,false,13,'',5,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.tripWireSource',lightOpacity=0,lightValue=0,blockResistance=0.0,blockHardness=0.0,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=13,soundId=5,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (132,'tile.tripWire',0,0,0.0,0.0,true,0.6,true,false,13,'',5,0.0,0.0,0.0,1.0,0.15625,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.tripWire',lightOpacity=0,lightValue=0,blockResistance=0.0,blockHardness=0.0,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=13,soundId=5,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=0.15625,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (133,'tile.blockEmerald',255,0,30.0,5.0,false,0.6,true,true,17,'',8,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.blockEmerald',lightOpacity=255,lightValue=0,blockResistance=30.0,blockHardness=5.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=17,soundId=8,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (134,'tile.stairsWoodSpruce',255,0,15.0,2.0,false,0.6,true,false,4,'',4,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.stairsWoodSpruce',lightOpacity=255,lightValue=0,blockResistance=15.0,blockHardness=2.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=4,soundId=4,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (135,'tile.stairsWoodBirch',255,0,15.0,2.0,false,0.6,true,false,4,'',4,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.stairsWoodBirch',lightOpacity=255,lightValue=0,blockResistance=15.0,blockHardness=2.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=4,soundId=4,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (136,'tile.stairsWoodJungle',255,0,15.0,2.0,false,0.6,true,false,4,'',4,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.stairsWoodJungle',lightOpacity=255,lightValue=0,blockResistance=15.0,blockHardness=2.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=4,soundId=4,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (137,'tile.commandBlock',255,0,0.0,0.0,false,0.6,true,true,17,'',5,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.commandBlock',lightOpacity=255,lightValue=0,blockResistance=0.0,blockHardness=0.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=17,soundId=5,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (138,'tile.beacon',0,15,15.0,3.0,false,0.6,true,false,11,'',5,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.beacon',lightOpacity=0,lightValue=15,blockResistance=15.0,blockHardness=3.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=11,soundId=5,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (139,'tile.cobbleWall',0,0,30.0,2.0,false,0.6,true,false,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.cobbleWall',lightOpacity=0,lightValue=0,blockResistance=30.0,blockHardness=2.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (140,'tile.flowerPot',0,0,0.0,0.0,false,0.6,true,false,13,'',5,0.3125,0.0,0.3125,0.6875,0.375,0.6875,false) 
    ON DUPLICATE KEY UPDATE name='tile.flowerPot',lightOpacity=0,lightValue=0,blockResistance=0.0,blockHardness=0.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=13,soundId=5,minX=0.3125,minY=0.0,minZ=0.3125,maxX=0.6875,maxY=0.375,maxZ=0.6875,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (141,'tile.carrots',0,0,0.0,0.0,true,0.6,true,false,5,'',2,0.0,0.0,0.0,1.0,0.25,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.carrots',lightOpacity=0,lightValue=0,blockResistance=0.0,blockHardness=0.0,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=5,soundId=2,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=0.25,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (142,'tile.potatoes',0,0,0.0,0.0,true,0.6,true,false,5,'',2,0.0,0.0,0.0,1.0,0.25,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.potatoes',lightOpacity=0,lightValue=0,blockResistance=0.0,blockHardness=0.0,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=5,soundId=2,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=0.25,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (143,'tile.button',0,0,2.5,0.5,true,0.6,true,false,13,'',4,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.button',lightOpacity=0,lightValue=0,blockResistance=2.5,blockHardness=0.5,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=13,soundId=4,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (144,'tile.skull',0,0,5.0,1.0,false,0.6,true,false,13,'',1,0.25,0.0,0.25,0.75,0.5,0.75,false) 
    ON DUPLICATE KEY UPDATE name='tile.skull',lightOpacity=0,lightValue=0,blockResistance=5.0,blockHardness=1.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=13,soundId=1,minX=0.25,minY=0.0,minZ=0.25,maxX=0.75,maxY=0.5,maxZ=0.75,renderAsNormal=false;
INSERT INTO `blocksound`(`soundId`,`name`,`volume`,`modifier`,`stepSound`,`placeSound`,`breakSound`)
    VALUES (12,'UNKNOW',0.3,1.0,'step.anvil','random.anvil_land','dig.stone')
    ON DUPLICATE KEY UPDATE name='UNKNOW',volume=0.3,modifier=1.0,stepSound='step.anvil',placeSound='random.anvil_land',breakSound='dig.stone';
INSERT INTO `material`(`materialId`,`name`,`canBurn`,`replacable`,`translucent`,`requiresNoTool`,`mobilityFlag`)
    VALUES (30,'UNKNOW',false,false,false,true,2)    ON DUPLICATE KEY UPDATE name=UNKNOW,canBurn=false,replacable=false,translucent=false,requiresNoTool=false,mobilityFlag=2;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (145,'tile.anvil',0,0,6000.0,5.0,false,0.6,true,false,30,'',12,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.anvil',lightOpacity=0,lightValue=0,blockResistance=6000.0,blockHardness=5.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=30,soundId=12,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (146,'tile.chestTrap',0,0,12.5,2.5,false,0.6,true,false,4,'',4,0.0625,0.0,0.0625,0.9375,0.875,0.9375,false) 
    ON DUPLICATE KEY UPDATE name='tile.chestTrap',lightOpacity=0,lightValue=0,blockResistance=12.5,blockHardness=2.5,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=4,soundId=4,minX=0.0625,minY=0.0,minZ=0.0625,maxX=0.9375,maxY=0.875,maxZ=0.9375,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (147,'tile.weightedPlate_light',0,0,2.5,0.5,true,0.6,true,false,17,'',4,0.0625,0.0,0.0625,0.9375,0.03125,0.9375,false) 
    ON DUPLICATE KEY UPDATE name='tile.weightedPlate_light',lightOpacity=0,lightValue=0,blockResistance=2.5,blockHardness=0.5,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=17,soundId=4,minX=0.0625,minY=0.0,minZ=0.0625,maxX=0.9375,maxY=0.03125,maxZ=0.9375,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (148,'tile.weightedPlate_heavy',0,0,2.5,0.5,true,0.6,true,false,17,'',4,0.0625,0.0,0.0625,0.9375,0.03125,0.9375,false) 
    ON DUPLICATE KEY UPDATE name='tile.weightedPlate_heavy',lightOpacity=0,lightValue=0,blockResistance=2.5,blockHardness=0.5,needsRandomTick=true,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=17,soundId=4,minX=0.0625,minY=0.0,minZ=0.0625,maxX=0.9375,maxY=0.03125,maxZ=0.9375,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (149,'tile.comparator',0,0,0.0,0.0,false,0.6,true,false,13,'',4,0.0,0.0,0.0,1.0,0.125,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.comparator',lightOpacity=0,lightValue=0,blockResistance=0.0,blockHardness=0.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=13,soundId=4,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=0.125,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (150,'tile.comparator',0,9,0.0,0.0,false,0.6,true,false,13,'',4,0.0,0.0,0.0,1.0,0.125,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.comparator',lightOpacity=0,lightValue=9,blockResistance=0.0,blockHardness=0.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=13,soundId=4,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=0.125,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (151,'tile.daylightDetector',0,0,1.0,0.2,false,0.6,true,false,4,'',4,0.0,0.0,0.0,1.0,0.375,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.daylightDetector',lightOpacity=0,lightValue=0,blockResistance=1.0,blockHardness=0.2,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=4,soundId=4,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=0.375,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (152,'tile.blockRedstone',255,0,30.0,5.0,false,0.6,true,true,17,'',8,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.blockRedstone',lightOpacity=255,lightValue=0,blockResistance=30.0,blockHardness=5.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=17,soundId=8,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (153,'tile.netherquartz',255,0,15.0,3.0,false,0.6,true,true,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.netherquartz',lightOpacity=255,lightValue=0,blockResistance=15.0,blockHardness=3.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (154,'tile.hopper',0,0,24.0,3.0,false,0.6,true,false,17,'',4,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.hopper',lightOpacity=0,lightValue=0,blockResistance=24.0,blockHardness=3.0,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=17,soundId=4,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (155,'tile.quartzBlock',255,0,4.0,0.8,false,0.6,true,true,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.quartzBlock',lightOpacity=255,lightValue=0,blockResistance=4.0,blockHardness=0.8,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (156,'tile.stairsQuartz',255,0,4.0,0.8,false,0.6,true,false,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.stairsQuartz',lightOpacity=255,lightValue=0,blockResistance=4.0,blockHardness=0.8,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (157,'tile.activatorRail',0,0,3.5,0.7,false,0.6,true,false,13,'',8,0.0,0.0,0.0,1.0,0.125,1.0,false) 
    ON DUPLICATE KEY UPDATE name='tile.activatorRail',lightOpacity=0,lightValue=0,blockResistance=3.5,blockHardness=0.7,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=false,material=13,soundId=8,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=0.125,maxZ=1.0,renderAsNormal=false;
INSERT INTO `block`(`blockId`,`name`,`lightOpacity`,`lightValue`,`blockResistance`,`blockHardness`,`needsRandomTick`,`slipperiness`,`isCollidable`,`isOpaqueCube`,`material`,`scriptId`,`soundId`,`minX`,`minY`,`minZ`,`maxX`,`maxY`,`maxZ`,`renderAsNormal`) 
    VALUES (158,'tile.dropper',255,0,17.5,3.5,false,0.6,true,true,1,'',1,0.0,0.0,0.0,1.0,1.0,1.0,true) 
    ON DUPLICATE KEY UPDATE name='tile.dropper',lightOpacity=255,lightValue=0,blockResistance=17.5,blockHardness=3.5,needsRandomTick=false,slipperiness=0.6,isCollidable=true,isOpaqueCube=true,material=1,soundId=1,minX=0.0,minY=0.0,minZ=0.0,maxX=1.0,maxY=1.0,maxZ=1.0,renderAsNormal=true;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (256,'item.shovelIron',1,250,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.shovelIron',maxStackSize=1,maxDamage=250,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (257,'item.pickaxeIron',1,250,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.pickaxeIron',maxStackSize=1,maxDamage=250,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (258,'item.hatchetIron',1,250,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.hatchetIron',maxStackSize=1,maxDamage=250,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (259,'item.flintAndSteel',1,64,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.flintAndSteel',maxStackSize=1,maxDamage=64,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (260,'item.apple',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.apple',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (261,'item.bow',1,384,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.bow',maxStackSize=1,maxDamage=384,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (262,'item.arrow',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.arrow',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (263,'item.coal',64,0,true,0,0)
    ON DUPLICATE KEY UPDATE name='item.coal',maxStackSize=64,maxDamage=0,hasSubType=true,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (264,'item.diamond',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.diamond',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (265,'item.ingotIron',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.ingotIron',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (266,'item.ingotGold',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.ingotGold',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (267,'item.swordIron',1,250,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.swordIron',maxStackSize=1,maxDamage=250,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (268,'item.swordWood',1,59,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.swordWood',maxStackSize=1,maxDamage=59,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (269,'item.shovelWood',1,59,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.shovelWood',maxStackSize=1,maxDamage=59,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (270,'item.pickaxeWood',1,59,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.pickaxeWood',maxStackSize=1,maxDamage=59,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (271,'item.hatchetWood',1,59,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.hatchetWood',maxStackSize=1,maxDamage=59,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (272,'item.swordStone',1,131,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.swordStone',maxStackSize=1,maxDamage=131,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (273,'item.shovelStone',1,131,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.shovelStone',maxStackSize=1,maxDamage=131,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (274,'item.pickaxeStone',1,131,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.pickaxeStone',maxStackSize=1,maxDamage=131,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (275,'item.hatchetStone',1,131,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.hatchetStone',maxStackSize=1,maxDamage=131,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (276,'item.swordDiamond',1,1561,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.swordDiamond',maxStackSize=1,maxDamage=1561,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (277,'item.shovelDiamond',1,1561,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.shovelDiamond',maxStackSize=1,maxDamage=1561,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (278,'item.pickaxeDiamond',1,1561,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.pickaxeDiamond',maxStackSize=1,maxDamage=1561,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (279,'item.hatchetDiamond',1,1561,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.hatchetDiamond',maxStackSize=1,maxDamage=1561,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (280,'item.stick',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.stick',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (281,'item.bowl',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.bowl',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (282,'item.mushroomStew',1,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.mushroomStew',maxStackSize=1,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (283,'item.swordGold',1,32,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.swordGold',maxStackSize=1,maxDamage=32,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (284,'item.shovelGold',1,32,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.shovelGold',maxStackSize=1,maxDamage=32,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (285,'item.pickaxeGold',1,32,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.pickaxeGold',maxStackSize=1,maxDamage=32,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (286,'item.hatchetGold',1,32,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.hatchetGold',maxStackSize=1,maxDamage=32,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (287,'item.string',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.string',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (288,'item.feather',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.feather',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (289,'item.sulphur',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.sulphur',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (290,'item.hoeWood',1,59,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.hoeWood',maxStackSize=1,maxDamage=59,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (291,'item.hoeStone',1,131,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.hoeStone',maxStackSize=1,maxDamage=131,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (292,'item.hoeIron',1,250,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.hoeIron',maxStackSize=1,maxDamage=250,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (293,'item.hoeDiamond',1,1561,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.hoeDiamond',maxStackSize=1,maxDamage=1561,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (294,'item.hoeGold',1,32,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.hoeGold',maxStackSize=1,maxDamage=32,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (295,'item.seeds',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.seeds',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (296,'item.wheat',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.wheat',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (297,'item.bread',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.bread',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (298,'item.helmetCloth',1,55,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.helmetCloth',maxStackSize=1,maxDamage=55,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (299,'item.chestplateCloth',1,80,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.chestplateCloth',maxStackSize=1,maxDamage=80,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (300,'item.leggingsCloth',1,75,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.leggingsCloth',maxStackSize=1,maxDamage=75,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (301,'item.bootsCloth',1,65,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.bootsCloth',maxStackSize=1,maxDamage=65,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (302,'item.helmetChain',1,165,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.helmetChain',maxStackSize=1,maxDamage=165,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (303,'item.chestplateChain',1,240,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.chestplateChain',maxStackSize=1,maxDamage=240,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (304,'item.leggingsChain',1,225,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.leggingsChain',maxStackSize=1,maxDamage=225,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (305,'item.bootsChain',1,195,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.bootsChain',maxStackSize=1,maxDamage=195,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (306,'item.helmetIron',1,165,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.helmetIron',maxStackSize=1,maxDamage=165,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (307,'item.chestplateIron',1,240,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.chestplateIron',maxStackSize=1,maxDamage=240,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (308,'item.leggingsIron',1,225,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.leggingsIron',maxStackSize=1,maxDamage=225,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (309,'item.bootsIron',1,195,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.bootsIron',maxStackSize=1,maxDamage=195,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (310,'item.helmetDiamond',1,363,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.helmetDiamond',maxStackSize=1,maxDamage=363,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (311,'item.chestplateDiamond',1,528,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.chestplateDiamond',maxStackSize=1,maxDamage=528,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (312,'item.leggingsDiamond',1,495,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.leggingsDiamond',maxStackSize=1,maxDamage=495,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (313,'item.bootsDiamond',1,429,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.bootsDiamond',maxStackSize=1,maxDamage=429,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (314,'item.helmetGold',1,77,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.helmetGold',maxStackSize=1,maxDamage=77,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (315,'item.chestplateGold',1,112,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.chestplateGold',maxStackSize=1,maxDamage=112,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (316,'item.leggingsGold',1,105,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.leggingsGold',maxStackSize=1,maxDamage=105,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (317,'item.bootsGold',1,91,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.bootsGold',maxStackSize=1,maxDamage=91,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (318,'item.flint',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.flint',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (319,'item.porkchopRaw',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.porkchopRaw',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (320,'item.porkchopCooked',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.porkchopCooked',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (321,'item.painting',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.painting',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (322,'item.appleGold',64,0,true,0,0)
    ON DUPLICATE KEY UPDATE name='item.appleGold',maxStackSize=64,maxDamage=0,hasSubType=true,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (323,'item.sign',16,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.sign',maxStackSize=16,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (324,'item.doorWood',1,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.doorWood',maxStackSize=1,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (325,'item.bucket',16,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.bucket',maxStackSize=16,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (326,'item.bucketWater',1,0,false,325,0)
    ON DUPLICATE KEY UPDATE name='item.bucketWater',maxStackSize=1,maxDamage=0,hasSubType=false,containerId=325;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (327,'item.bucketLava',1,0,false,325,0)
    ON DUPLICATE KEY UPDATE name='item.bucketLava',maxStackSize=1,maxDamage=0,hasSubType=false,containerId=325;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (328,'item.minecart',1,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.minecart',maxStackSize=1,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (329,'item.saddle',1,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.saddle',maxStackSize=1,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (330,'item.doorIron',1,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.doorIron',maxStackSize=1,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (331,'item.redstone',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.redstone',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (332,'item.snowball',16,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.snowball',maxStackSize=16,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (333,'item.boat',1,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.boat',maxStackSize=1,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (334,'item.leather',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.leather',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (335,'item.milk',1,0,false,325,0)
    ON DUPLICATE KEY UPDATE name='item.milk',maxStackSize=1,maxDamage=0,hasSubType=false,containerId=325;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (336,'item.brick',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.brick',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (337,'item.clay',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.clay',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (338,'item.reeds',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.reeds',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (339,'item.paper',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.paper',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (340,'item.book',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.book',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (341,'item.slimeball',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.slimeball',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (342,'item.minecartChest',1,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.minecartChest',maxStackSize=1,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (343,'item.minecartFurnace',1,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.minecartFurnace',maxStackSize=1,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (344,'item.egg',16,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.egg',maxStackSize=16,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (345,'item.compass',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.compass',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (346,'item.fishingRod',1,64,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.fishingRod',maxStackSize=1,maxDamage=64,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (347,'item.clock',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.clock',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (348,'item.yellowDust',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.yellowDust',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (349,'item.fishRaw',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.fishRaw',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (350,'item.fishCooked',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.fishCooked',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (351,'item.dyePowder',64,0,true,0,0)
    ON DUPLICATE KEY UPDATE name='item.dyePowder',maxStackSize=64,maxDamage=0,hasSubType=true,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (352,'item.bone',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.bone',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (353,'item.sugar',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.sugar',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (354,'item.cake',1,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.cake',maxStackSize=1,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (355,'item.bed',1,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.bed',maxStackSize=1,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (356,'item.diode',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.diode',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (357,'item.cookie',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.cookie',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (358,'item.map',64,0,true,0,0)
    ON DUPLICATE KEY UPDATE name='item.map',maxStackSize=64,maxDamage=0,hasSubType=true,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (359,'item.shears',1,238,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.shears',maxStackSize=1,maxDamage=238,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (360,'item.melon',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.melon',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (361,'item.seeds_pumpkin',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.seeds_pumpkin',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (362,'item.seeds_melon',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.seeds_melon',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (363,'item.beefRaw',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.beefRaw',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (364,'item.beefCooked',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.beefCooked',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (365,'item.chickenRaw',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.chickenRaw',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (366,'item.chickenCooked',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.chickenCooked',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (367,'item.rottenFlesh',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.rottenFlesh',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (368,'item.enderPearl',16,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.enderPearl',maxStackSize=16,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (369,'item.blazeRod',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.blazeRod',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (370,'item.ghastTear',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.ghastTear',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (371,'item.goldNugget',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.goldNugget',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (372,'item.netherStalkSeeds',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.netherStalkSeeds',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (373,'item.potion',1,0,true,0,0)
    ON DUPLICATE KEY UPDATE name='item.potion',maxStackSize=1,maxDamage=0,hasSubType=true,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (374,'item.glassBottle',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.glassBottle',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (375,'item.spiderEye',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.spiderEye',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (376,'item.fermentedSpiderEye',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.fermentedSpiderEye',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (377,'item.blazePowder',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.blazePowder',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (378,'item.magmaCream',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.magmaCream',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (379,'item.brewingStand',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.brewingStand',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (380,'item.cauldron',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.cauldron',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (381,'item.eyeOfEnder',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.eyeOfEnder',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (382,'item.speckledMelon',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.speckledMelon',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (383,'item.monsterPlacer',64,0,true,0,0)
    ON DUPLICATE KEY UPDATE name='item.monsterPlacer',maxStackSize=64,maxDamage=0,hasSubType=true,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (384,'item.expBottle',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.expBottle',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (385,'item.fireball',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.fireball',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (386,'item.writingBook',1,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.writingBook',maxStackSize=1,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (387,'item.writtenBook',1,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.writtenBook',maxStackSize=1,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (388,'item.emerald',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.emerald',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (389,'item.frame',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.frame',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (390,'item.flowerPot',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.flowerPot',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (391,'item.carrots',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.carrots',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (392,'item.potato',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.potato',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (393,'item.potatoBaked',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.potatoBaked',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (394,'item.potatoPoisonous',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.potatoPoisonous',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (395,'item.emptyMap',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.emptyMap',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (396,'item.carrotGolden',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.carrotGolden',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (397,'item.skull',64,0,true,0,0)
    ON DUPLICATE KEY UPDATE name='item.skull',maxStackSize=64,maxDamage=0,hasSubType=true,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (398,'item.carrotOnAStick',1,25,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.carrotOnAStick',maxStackSize=1,maxDamage=25,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (399,'item.netherStar',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.netherStar',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (400,'item.pumpkinPie',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.pumpkinPie',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (401,'item.fireworks',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.fireworks',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (402,'item.fireworksCharge',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.fireworksCharge',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (403,'item.enchantedBook',1,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.enchantedBook',maxStackSize=1,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (404,'item.comparator',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.comparator',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (405,'item.netherbrick',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.netherbrick',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (406,'item.netherquartz',64,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.netherquartz',maxStackSize=64,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (407,'item.minecartTnt',1,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.minecartTnt',maxStackSize=1,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (408,'item.minecartHopper',1,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.minecartHopper',maxStackSize=1,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (2256,'item.record',1,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.record',maxStackSize=1,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (2257,'item.record',1,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.record',maxStackSize=1,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (2258,'item.record',1,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.record',maxStackSize=1,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (2259,'item.record',1,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.record',maxStackSize=1,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (2260,'item.record',1,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.record',maxStackSize=1,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (2261,'item.record',1,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.record',maxStackSize=1,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (2262,'item.record',1,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.record',maxStackSize=1,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (2263,'item.record',1,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.record',maxStackSize=1,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (2264,'item.record',1,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.record',maxStackSize=1,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (2265,'item.record',1,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.record',maxStackSize=1,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (2266,'item.record',1,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.record',maxStackSize=1,maxDamage=0,hasSubType=false,containerId=0;
INSERT INTO `items`(`itemId`,`name`,`maxStackSize`,`maxDamage`,`hasSubType`,`containerId`,`scriptId`) VALUES (2267,'item.record',1,0,false,0,0)
    ON DUPLICATE KEY UPDATE name='item.record',maxStackSize=1,maxDamage=0,hasSubType=false,containerId=0;

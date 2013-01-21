CREATE DATABASE  IF NOT EXISTS `pyjob` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `pyjob`;
-- MySQL dump 10.13  Distrib 5.1.63, for debian-linux-gnu (x86_64)
--
-- Host: localhost    Database: pyjob
-- ------------------------------------------------------
-- Server version	5.1.63-0ubuntu0.10.04.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `task`
--

DROP TABLE IF EXISTS `task`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `task` (
  `tid` int(11) NOT NULL AUTO_INCREMENT,
  `tname` varchar(45) DEFAULT NULL,
  `tbegintime` datetime DEFAULT NULL,
  `texpiredtime` datetime DEFAULT NULL,
  `tinterval` int(11) DEFAULT NULL,
  `tdata` varchar(1024) DEFAULT NULL,
  `tnextruntime` datetime DEFAULT NULL,
  PRIMARY KEY (`tid`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `task`
--

LOCK TABLES `task` WRITE;
/*!40000 ALTER TABLE `task` DISABLE KEYS */;
INSERT INTO `task` VALUES (1,'test 1+2','2011-08-02 00:00:00','2013-01-01 00:00:00',1,'print(1+20)','2012-09-09 21:05:14'),(2,'test 1+2','2011-08-02 00:00:00','2013-01-01 00:00:00',2,'print(1+2*3)','2012-09-09 21:05:16'),(3,'test 1+2','2011-08-12 00:00:00','2013-01-01 00:00:00',1,'print(1+2)','2012-09-09 21:05:15'),(4,'test 1+2','2011-08-12 00:00:00','2013-01-01 00:00:00',3,'print(1+2)','2012-09-09 21:05:15'),(5,'test 1+2','2011-08-12 00:00:00','2013-01-01 00:00:00',5,'print(1+2)','2012-09-09 21:05:15'),(6,'test 1+2','2011-08-12 00:00:00','2013-01-01 00:00:00',10,'print(1+2)','2012-09-09 21:05:22'),(7,'test 1+2','2011-08-12 00:00:00','2013-01-01 00:00:00',30,'print(1+2)','2012-09-09 21:05:36');
/*!40000 ALTER TABLE `task` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tskresp`
--

DROP TABLE IF EXISTS `tskresp`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tskresp` (
  `tryid` int(11) NOT NULL AUTO_INCREMENT,
  `tid` int(11) NOT NULL,
  `result` int(11) DEFAULT NULL,
  `response` varchar(1024) DEFAULT NULL,
  PRIMARY KEY (`tryid`)
) ENGINE=InnoDB AUTO_INCREMENT=436 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tskresp`
--

LOCK TABLES `tskresp` WRITE;
/*!40000 ALTER TABLE `tskresp` DISABLE KEYS */;
INSERT INTO `tskresp` VALUES (357,1,0,'3\n'),(358,3,0,'\0\03\n'),(359,2,0,'\0\0\0\03\n'),(360,4,0,'\0\0\0\0\0\03\n'),(361,6,0,'\0\0\0\0\0\0\0\03\n'),(362,1,0,'3\n'),(363,2,0,'\0\03\n'),(364,3,0,'\0\0\0\03\n'),(365,4,0,'\0\0\0\0\0\03\n'),(366,6,0,'\0\0\0\0\0\0\0\03\n'),(367,5,0,'3\n'),(368,7,0,'\0\03\n'),(369,1,0,'\0\0\0\03\n'),(370,3,0,'\0\0\0\0\0\03\n'),(371,2,0,'\0\0\0\0\0\0\0\03\n'),(372,1,0,'3\n'),(373,3,0,'\0\03\n'),(374,4,0,'\0\0\0\03\n'),(375,2,0,'\0\0\0\0\0\03\n'),(376,5,0,'\0\0\0\0\0\0\0\03\n'),(377,1,0,'3\n'),(378,3,0,'\0\03\n'),(379,2,0,'\0\0\0\03\n'),(380,4,0,'\0\0\0\0\0\03\n'),(381,5,0,'\0\0\0\0\0\0\0\03\n'),(382,1,0,'3\n'),(383,3,0,'\0\03\n'),(384,2,0,'\0\0\0\03\n'),(385,4,0,'\0\0\0\0\0\03\n'),(386,1,0,'21\n'),(387,3,0,'\0\0\03\n'),(388,2,0,'\0\0\0\0\07\n'),(389,6,0,'\0\0\0\0\0\0\03\n'),(390,5,0,'\0\0\0\0\0\0\0\0\03\n'),(391,4,0,'3\n'),(392,7,0,'\0\03\n'),(393,1,0,'\0\0\0\021\n'),(394,3,0,'\0\0\0\0\0\0\03\n'),(395,2,0,'\0\0\0\0\0\0\0\0\07\n'),(396,1,0,'21\n'),(397,3,0,'\0\0\03\n'),(398,2,0,'\0\0\0\0\07\n'),(399,4,0,'\0\0\0\0\0\0\03\n'),(400,5,0,'\0\0\0\0\0\0\0\0\03\n'),(401,1,0,'21\n'),(402,3,0,'\0\0\03\n'),(403,2,0,'\0\0\0\0\07\n'),(404,4,0,'\0\0\0\0\0\0\03\n'),(405,5,0,'\0\0\0\0\0\0\0\0\03\n'),(406,1,0,'21\n'),(407,3,0,'\0\0\03\n'),(408,2,0,'7\n'),(409,1,0,'\0\021\n'),(410,3,0,'\0\0\0\0\03\n'),(411,4,0,'\0\0\0\0\0\0\03\n'),(412,6,0,'\0\0\0\0\0\0\0\0\03\n'),(413,5,0,'3\n'),(414,7,0,'\0\03\n'),(415,1,0,'\0\0\0\021\n'),(416,2,0,'\0\0\0\0\0\0\07\n'),(417,3,0,'\0\0\0\0\0\0\0\0\03\n'),(418,1,0,'21\n'),(419,3,0,'\0\0\03\n'),(420,4,0,'\0\0\0\0\03\n'),(421,2,0,'\0\0\0\0\0\0\07\n'),(422,5,0,'\0\0\0\0\0\0\0\0\03\n'),(423,1,0,'21\n'),(424,3,0,'\0\0\03\n'),(425,2,0,'\0\0\0\0\07\n'),(426,4,0,'\0\0\0\0\0\0\03\n'),(427,5,0,'\0\0\0\0\0\0\0\0\03\n'),(428,1,0,'21\n'),(429,3,0,'\0\0\03\n'),(430,2,0,'\0\0\0\0\07\n'),(431,4,0,'\0\0\0\0\0\0\03\n'),(432,6,0,'\0\0\0\0\0\0\0\0\03\n'),(433,1,0,'21\n'),(434,3,0,'\0\0\03\n'),(435,2,0,'\0\0\0\0\07\n');
/*!40000 ALTER TABLE `tskresp` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping routines for database 'pyjob'
--
/*!50003 DROP PROCEDURE IF EXISTS `fetchtask` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`dbuser`@`%`*/ /*!50003 PROCEDURE `fetchtask`()
begin

declare curTime datetime;

set curTime = now();

select tid,tdata
from task
where tbegintime < curTime and texpiredtime > curTime
order by tnextruntime
limit 0,5;



end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `insertresult` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`dbuser`@`%`*/ /*!50003 PROCEDURE `insertresult`(IN tskid int,IN retcode int, resp varchar(1024))
begin

insert into tskresp
(
tid
,result
,response
)
values
(
tskid
,retcode
,resp
);


update task
set tnextruntime = now() +tinterval
where tid = tskid;

select last_insert_id() as tryid;

end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `inserttask` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`dbuser`@`%`*/ /*!50003 PROCEDURE `inserttask`(IN tskname varchar(45)
								,IN tskbegintime datetime
								,IN tskexpiredtime datetime
								,IN tskinterval int
								,IN tskData varchar(1024))
begin

insert into task
(
	tname
	,tbegintime
	,texpiredtime
	,tinterval
	,tdata
)
value
(
	tskname
	,tskbegintime
	,tskexpiredtime
	,tskinterval
	,tskData
);

select last_insert_id() as tskid;


end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `querytaskresult` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`dbuser`@`%`*/ /*!50003 PROCEDURE `querytaskresult`(IN tskid int,IN trid int)
begin

select tryid,result,response
from tskresp
where tid = tskid and tryid > trid
order by tryid;


end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2012-10-29 20:55:04

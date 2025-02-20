-- MySQL dump 10.13  Distrib 8.2.0, for Win64 (x86_64)
--
-- Host: 127.0.0.1    Database: bankautomat
-- ------------------------------------------------------
-- Server version	8.2.0

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `account`
--

DROP TABLE IF EXISTS `account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `account` (
  `idaccount` int NOT NULL AUTO_INCREMENT,
  `idcustomer` int NOT NULL,
  `balance` decimal(10,2) DEFAULT NULL,
  `creditlimit` decimal(10,2) DEFAULT NULL,
  `accountnumber` int DEFAULT NULL,
  `accounttype` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`idaccount`,`idcustomer`),
  KEY `customerid_idx` (`idcustomer`),
  CONSTRAINT `fk_customerid` FOREIGN KEY (`idcustomer`) REFERENCES `customer` (`idcustomer`) ON DELETE RESTRICT ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=21 DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `account`
--

LOCK TABLES `account` WRITE;
/*!40000 ALTER TABLE `account` DISABLE KEYS */;
INSERT INTO `account` VALUES (16,1,49660.00,0.00,123456789,'debit'),(17,2,0.00,5000.00,555444999,'credit'),(19,4,0.00,2000.00,111444999,'credit'),(20,4,50000.00,0.00,222444999,'debit');
/*!40000 ALTER TABLE `account` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `accountcard`
--

DROP TABLE IF EXISTS `accountcard`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `accountcard` (
  `idcard` int NOT NULL,
  `idaccount` int NOT NULL,
  PRIMARY KEY (`idcard`,`idaccount`),
  KEY `accountid_idx` (`idaccount`),
  CONSTRAINT `fk_account_id` FOREIGN KEY (`idaccount`) REFERENCES `account` (`idaccount`) ON DELETE RESTRICT ON UPDATE CASCADE,
  CONSTRAINT `fk_accountcard` FOREIGN KEY (`idcard`) REFERENCES `card` (`idcard`) ON DELETE RESTRICT ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `accountcard`
--

LOCK TABLES `accountcard` WRITE;
/*!40000 ALTER TABLE `accountcard` DISABLE KEYS */;
INSERT INTO `accountcard` VALUES (17,16),(16,17),(20,19),(20,20);
/*!40000 ALTER TABLE `accountcard` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `card`
--

DROP TABLE IF EXISTS `card`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `card` (
  `idcard` int NOT NULL AUTO_INCREMENT,
  `cardnumber` int DEFAULT NULL,
  `pin` varchar(255) DEFAULT NULL,
  `active` int DEFAULT NULL,
  PRIMARY KEY (`idcard`),
  UNIQUE KEY `cardnumber_UNIQUE` (`cardnumber`)
) ENGINE=InnoDB AUTO_INCREMENT=24 DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `card`
--

LOCK TABLES `card` WRITE;
/*!40000 ALTER TABLE `card` DISABLE KEYS */;
INSERT INTO `card` VALUES (16,45605555,'$2a$10$dtG7mNyJptXB2q//l/u2MOOTBYiRfxO5jxyUUzaKlUx7DB4aZgXcu',1),(17,45706666,'$2a$10$gObFslfvon/pVlWmQh3fz.uE2X3JtoptFhO4LIVG5FkPoBYcHOS.a',1),(20,45707777,'$2a$10$tL7xhGEXL71ALETER9sFM.DAzbJkvX3gJy8PVDdHTUQ7rA3C5z64C',1),(21,5555555,'$2a$10$lvrWlNzQqX9N1FoP.WQh.uPXHI7cXzFDEQUxVoiwoK0Kd5a./zk1a',1);
/*!40000 ALTER TABLE `card` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `customer`
--

DROP TABLE IF EXISTS `customer`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `customer` (
  `idcustomer` int NOT NULL AUTO_INCREMENT,
  `fname` varchar(45) DEFAULT NULL,
  `lname` varchar(45) DEFAULT NULL,
  `address` varchar(45) DEFAULT NULL,
  `phone` varchar(15) DEFAULT NULL,
  PRIMARY KEY (`idcustomer`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `customer`
--

LOCK TABLES `customer` WRITE;
/*!40000 ALTER TABLE `customer` DISABLE KEYS */;
INSERT INTO `customer` VALUES (1,'Teppo','Testaaja','Koulukatu 5','0554441110'),(2,'Tauno','Taavetti','Hallituskatu 5','0554445555'),(3,'Aatu','Ahvenlahtti','Kalatie 5','0554443333'),(4,'Asko','Ahvenlahtti','Kalatie 65','0554443213');
/*!40000 ALTER TABLE `customer` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `transactions`
--

DROP TABLE IF EXISTS `transactions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `transactions` (
  `idtransactions` int NOT NULL AUTO_INCREMENT,
  `idaccount` int NOT NULL,
  `datetime` timestamp NULL DEFAULT NULL,
  `transaction` varchar(45) DEFAULT NULL,
  `amount` decimal(10,2) DEFAULT NULL,
  PRIMARY KEY (`idtransactions`,`idaccount`),
  KEY `accountid_idx` (`idaccount`),
  CONSTRAINT `fk_accountid` FOREIGN KEY (`idaccount`) REFERENCES `account` (`idaccount`) ON DELETE RESTRICT ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=23 DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `transactions`
--

LOCK TABLES `transactions` WRITE;
/*!40000 ALTER TABLE `transactions` DISABLE KEYS */;
INSERT INTO `transactions` VALUES (1,17,'2025-02-15 14:04:48','Nosto',-250.00),(2,17,'2025-02-15 14:04:49','Nosto',-250.00),(3,17,'2025-02-15 14:04:50','Nosto',-250.00),(4,17,'2025-02-15 14:04:50','Nosto',-250.00),(5,17,'2025-02-15 14:04:57','Pano',500.00),(6,17,'2025-02-15 14:04:57','Pano',500.00),(7,17,'2025-02-15 14:04:58','Pano',500.00),(8,17,'2025-02-15 14:05:00','Pano',500.00),(9,17,'2025-02-15 14:05:00','Pano',500.00),(10,17,'2025-02-15 14:05:01','Pano',500.00),(11,17,'2025-02-15 14:05:02','Pano',500.00),(12,17,'2025-02-15 14:05:03','Pano',500.00),(13,17,'2025-02-15 14:05:03','Pano',500.00),(14,17,'2025-02-15 14:05:04','Pano',500.00),(15,17,'2025-02-15 14:05:05','Pano',500.00),(16,17,'2025-02-15 14:05:05','Pano',500.00),(17,16,'2025-02-20 07:01:11','withdraw',20.00),(18,16,'2025-02-20 07:03:27','withdraw',80.00),(19,16,'2025-02-20 07:03:45','withdraw',20.00),(20,16,'2025-02-20 07:03:56','withdraw',100.00),(21,16,'2025-02-20 07:04:36','withdraw',100.00),(22,16,'2025-02-20 08:07:48','withdraw',20.00);
/*!40000 ALTER TABLE `transactions` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2025-02-20 10:10:31

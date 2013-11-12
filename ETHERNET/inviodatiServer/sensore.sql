-- phpMyAdmin SQL Dump
-- version 3.4.7.1
-- http://www.phpmyadmin.net
--
-- Host: 62.149.150.149
-- Generato il: Giu 25, 2012 alle 00:34
-- Versione del server: 5.5.24
-- Versione PHP: 5.3.8

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `Sql531406_5`
--

-- --------------------------------------------------------

--
-- Struttura della tabella `sensore`
--

CREATE TABLE IF NOT EXISTS `sensore` (
  `id_sensore` int(30) NOT NULL AUTO_INCREMENT,
  `nome` varchar(30) NOT NULL,
  `grandezza` varchar(30) NOT NULL,
  `valoreMax` float NOT NULL,
  `valoreMin` float NOT NULL,
  PRIMARY KEY (`id_sensore`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=3 ;

--
-- Dump dei dati per la tabella `sensore`
--

INSERT INTO `sensore` (`id_sensore`, `nome`, `grandezza`, `valoreMax`, `valoreMin`) VALUES
(1, 'fotoresistenza', 'lux', 0, 1000),
(2, 'wda', 'wfewf', 4, 44);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;

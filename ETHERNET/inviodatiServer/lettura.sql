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
-- Struttura della tabella `lettura`
--

CREATE TABLE IF NOT EXISTS `lettura` (
  `id_lettura` int(20) NOT NULL AUTO_INCREMENT,
  `id_sensore` int(20) NOT NULL,
  `lettura` double NOT NULL,
  `data` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id_lettura`),
  UNIQUE KEY `id_sensore` (`id_sensore`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=3 ;

--
-- Dump dei dati per la tabella `lettura`
--

INSERT INTO `lettura` (`id_lettura`, `id_sensore`, `lettura`, `data`) VALUES
(1, 1, 500, '2012-06-22 13:52:44'),
(2, 2, 500, '2012-06-22 14:06:20');

--
-- Limiti per le tabelle scaricate
--

--
-- Limiti per la tabella `lettura`
--
ALTER TABLE `lettura`
  ADD CONSTRAINT `lettura_ibfk_1` FOREIGN KEY (`id_sensore`) REFERENCES `sensore` (`id_sensore`);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;

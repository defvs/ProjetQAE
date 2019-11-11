-- phpMyAdmin SQL Dump
-- version 4.8.5
-- https://www.phpmyadmin.net/
--
-- Hôte : 127.0.0.1:3306
-- Généré le :  sam. 09 nov. 2019 à 15:53
-- Version du serveur :  5.7.26
-- Version de PHP :  7.2.18

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de données :  `qae`
--

-- --------------------------------------------------------

--
-- Structure de la table `data_analog`
--

DROP TABLE IF EXISTS `data_analog`;
CREATE TABLE IF NOT EXISTS `data_analog` (
  `index` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Index',
  `name` tinytext COLLATE utf8_unicode_ci COMMENT 'Nom du champ',
  `value` float NOT NULL DEFAULT '0' COMMENT 'Valeur',
  `last_update` timestamp NULL DEFAULT NULL COMMENT 'Temps de dernière modification',
  PRIMARY KEY (`index`)
) ENGINE=MyISAM AUTO_INCREMENT=8 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Déchargement des données de la table `data_analog`
--

INSERT INTO `data_analog` (`index`, `name`, `value`, `last_update`) VALUES
(1, 'HCHO', 1, '2019-11-08 09:41:46'),
(2, 'MQ2', 10, '2019-11-08 09:41:46');

-- --------------------------------------------------------

--
-- Structure de la table `data_numeric`
--

DROP TABLE IF EXISTS `data_numeric`;
CREATE TABLE IF NOT EXISTS `data_numeric` (
  `index` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Index',
  `name` tinytext COLLATE utf8_unicode_ci COMMENT 'Nom du champ',
  `value` float NOT NULL DEFAULT '0' COMMENT 'Valeur',
  `last_update` timestamp NULL DEFAULT NULL COMMENT 'Temps de dernière modification',
  PRIMARY KEY (`index`)
) ENGINE=MyISAM AUTO_INCREMENT=15 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Déchargement des données de la table `data_numeric`
--

INSERT INTO `data_numeric` (`index`, `name`, `value`, `last_update`) VALUES
(1, 'NH3', 34, '2019-11-08 08:46:59'),
(2, 'CO', 105, '2019-11-08 09:41:46'),
(3, 'NO2', 104, '2019-11-08 09:41:46'),
(4, 'C3H8', 103, '2019-11-08 09:41:46'),
(5, 'C4H10', 102, '2019-11-08 09:41:46'),
(6, 'CH4', 101, '2019-11-08 09:41:46'),
(7, 'H2', 100, '2019-11-08 09:41:46');
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;

-- phpMyAdmin SQL Dump
-- version 4.6.6deb5
-- https://www.phpmyadmin.net/
--
-- Client :  localhost:3306
-- Généré le :  Mar 26 Novembre 2019 à 09:22
-- Version du serveur :  10.3.17-MariaDB-0+deb10u1
-- Version de PHP :  7.3.11-1~deb10u1

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de données :  `qae2`
--

-- --------------------------------------------------------

--
-- Structure de la table `data_analog`
--

CREATE TABLE `data_analog` (
  `id` int(11) NOT NULL,
  `HCHO` int(11) NOT NULL,
  `MQ2` int(11) NOT NULL,
  `time` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Structure de la table `data_numeric`
--

CREATE TABLE `data_numeric` (
  `id` int(11) NOT NULL,
  `NH3` int(11) NOT NULL,
  `CO` int(11) NOT NULL,
  `NO2` int(11) NOT NULL,
  `C3H8` int(11) NOT NULL,
  `C4H10` int(11) NOT NULL,
  `CH4` int(11) NOT NULL,
  `H2` int(11) NOT NULL,
  `time` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Index pour les tables exportées
--

--
-- Index pour la table `data_analog`
--
ALTER TABLE `data_analog`
  ADD PRIMARY KEY (`id`);

--
-- Index pour la table `data_numeric`
--
ALTER TABLE `data_numeric`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `id` (`id`);

--
-- AUTO_INCREMENT pour les tables exportées
--

--
-- AUTO_INCREMENT pour la table `data_analog`
--
ALTER TABLE `data_analog`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT pour la table `data_numeric`
--
ALTER TABLE `data_numeric`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;

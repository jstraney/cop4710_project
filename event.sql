-- phpMyAdmin SQL Dump
-- version 4.7.0
-- https://www.phpmyadmin.net/
--
-- Host: localhost
-- Generation Time: May 31, 2017 at 05:26 PM
-- Server version: 10.1.21-MariaDB
-- PHP Version: 5.6.30

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `event`
--

-- --------------------------------------------------------

--
-- Table structure for table `comments`
--

CREATE TABLE `comments` (
  `user_id` int(11) UNSIGNED NOT NULL,
  `comment` varchar(140) NOT NULL,
  `event_id` int(11) UNSIGNED NOT NULL,
  `comment_id` int(10) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `events`
--

CREATE TABLE `events` (
  `event_name` varchar(30) NOT NULL,
  `event_street` varchar(30) NOT NULL,
  `event_time` time NOT NULL,
  `rso_id` int(11) UNSIGNED NOT NULL,
  `event_long` double DEFAULT NULL,
  `event_lat` double DEFAULT NULL,
  `event_loc_name` varchar(30) NOT NULL,
  `event_city` varchar(30) NOT NULL,
  `event_zip` int(5) NOT NULL,
  `event_duration` time NOT NULL,
  `event_id` int(10) UNSIGNED NOT NULL,
  `state` char(2) NOT NULL,
  `uni_id` int(11) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `RSOs`
--

CREATE TABLE `RSOs` (
  `rso_name` varchar(30) NOT NULL,
  `rso_admin_id` int(11) UNSIGNED NOT NULL,
  `member_count` int(10) UNSIGNED NOT NULL,
  `rso_id` int(11) UNSIGNED NOT NULL,
  `uni_id` int(11) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `universities`
--

CREATE TABLE `universities` (
  `uni_name` varchar(30) NOT NULL,
  `uni_street` varchar(60) NOT NULL,
  `uni_state` char(2) NOT NULL,
  `uni_zip` int(5) NOT NULL,
  `uni_num_students` int(11) UNSIGNED NOT NULL,
  `uni_id` int(11) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `users`
--

CREATE TABLE `users` (
  `first_name` varchar(30) NOT NULL,
  `last_name` varchar(30) NOT NULL,
  `email` varchar(30) NOT NULL,
  `user_id` int(11) UNSIGNED NOT NULL,
  `uni_id` int(11) UNSIGNED NOT NULL,
  `hash` varchar(60) NOT NULL,
  `user_name` varchar(30) NOT NULL DEFAULT 'NOT NULL',
  `role` enum('STU','ADM','SA','') NOT NULL DEFAULT 'STU',
  `rso_id` int(11) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `comments`
--
ALTER TABLE `comments`
  ADD PRIMARY KEY (`comment_id`),
  ADD KEY `event_id` (`event_id`),
  ADD KEY `user_id` (`user_id`);

--
-- Indexes for table `events`
--
ALTER TABLE `events`
  ADD PRIMARY KEY (`event_id`),
  ADD KEY `rso_id` (`rso_id`);

--
-- Indexes for table `RSOs`
--
ALTER TABLE `RSOs`
  ADD PRIMARY KEY (`rso_id`),
  ADD KEY `rso_admin_id` (`rso_admin_id`),
  ADD KEY `uni_id` (`uni_id`);

--
-- Indexes for table `universities`
--
ALTER TABLE `universities`
  ADD PRIMARY KEY (`uni_id`);

--
-- Indexes for table `users`
--
ALTER TABLE `users`
  ADD PRIMARY KEY (`user_id`),
  ADD KEY `uni_id` (`uni_id`),
  ADD KEY `rso_id` (`rso_id`) USING BTREE;

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `comments`
--
ALTER TABLE `comments`
  MODIFY `comment_id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `events`
--
ALTER TABLE `events`
  MODIFY `event_id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `RSOs`
--
ALTER TABLE `RSOs`
  MODIFY `rso_id` int(11) UNSIGNED NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `universities`
--
ALTER TABLE `universities`
  MODIFY `uni_id` int(11) UNSIGNED NOT NULL AUTO_INCREMENT;
--
-- Constraints for dumped tables
--

--
-- Constraints for table `comments`
--
ALTER TABLE `comments`
  ADD CONSTRAINT `event_id` FOREIGN KEY (`event_id`) REFERENCES `events` (`event_id`),
  ADD CONSTRAINT `user_id` FOREIGN KEY (`user_id`) REFERENCES `users` (`user_id`);

--
-- Constraints for table `events`
--
ALTER TABLE `events`
  ADD CONSTRAINT `rso_id` FOREIGN KEY (`rso_id`) REFERENCES `RSOs` (`rso_id`);

--
-- Constraints for table `RSOs`
--
ALTER TABLE `RSOs`
  ADD CONSTRAINT `rso_admin_id` FOREIGN KEY (`rso_admin_id`) REFERENCES `users` (`user_id`),
  ADD CONSTRAINT `uni_id` FOREIGN KEY (`uni_id`) REFERENCES `universities` (`uni_id`);

--
-- Constraints for table `users`
--
ALTER TABLE `users`
  ADD CONSTRAINT `users_ibfk_1` FOREIGN KEY (`uni_id`) REFERENCES `universities` (`uni_id`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;

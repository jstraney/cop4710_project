-- phpMyAdmin SQL Dump
-- version 4.5.2
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Jun 14, 2017 at 07:36 PM
-- Server version: 10.1.13-MariaDB
-- PHP Version: 7.0.5

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `event`
--

DELIMITER $$
--
-- Procedures
--
CREATE DEFINER=`root`@`localhost` PROCEDURE `create_user` (IN `_user_name` VARCHAR(30), IN `_first_name` VARCHAR(60), IN `_last_name` VARCHAR(60), IN `_email` VARCHAR(60), IN `_role` ENUM("SA","ADM","STU"), IN `_hash` VARCHAR(60), IN `_uni_id` INT(11))  BEGIN

IF _role = "STU" AND (check_uni_emails(_email, _uni_id) < 1) THEN
  SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'e-mail must match university selected.';
ELSEIF _role = "STU" THEN
  BEGIN
  INSERT INTO users (user_name, first_name, last_name, email, role, hash)
  VALUES (_user_name, _first_name, _last_name, _email, _role, _hash);
  INSERT INTO attending (user_id, uni_id)
  VALUES (LAST_INSERT_ID(), _uni_id);
  END;
ELSEIF _role = "ADM" OR _role = "SA" THEN
  INSERT INTO users (user_name, first_name, last_name, email, role, hash)
  VALUES (_user_name, _first_name, _last_name, _email, role, _hash);
END IF;
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `select_students` ()  BEGIN
  SELECT u.user_id, u.user_name, u.email, u.first_name, u.last_name
  FROM users u
  WHERE u.role = "STU";
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `update_user` (IN `_user_id` INT(11), IN `_user_name` VARCHAR(30), IN `_first_name` VARCHAR(60), IN `_last_name` VARCHAR(60), IN `_email` VARCHAR(60), IN `_role` ENUM("SA","ADM","STU"), IN `_hash` VARCHAR(60), IN `_uni_id` INT(11))  BEGIN
IF _role = "STU" AND (check_uni_emails(_email, _uni_id) < 1) THEN
  SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'e-mail must match university selected.';
ELSEIF _role = "STU" THEN
  BEGIN
    UPDATE users SET
    user_name = _user_name,
    first_name = _first_name,
    last_name = _last_name,
    email = _email,
    role = _role,
    hash = _hash
    WHERE user_id = _user_id;
    UPDATE attending SET
    uni_id = _uni_id
    WHERE user_id = _user_id;
  END;
ELSEIF _role = "ADM" OR _role = "SA" THEN
  UPDATE users SET
  user_name = _user_name,
  first_name = _first_name,
  last_name = _last_name,
  email = _email,
  role = _role,
  hash = _hash
  WHERE user_id = _user_id;
END IF;
END$$

--
-- Functions
--
CREATE DEFINER=`root`@`localhost` FUNCTION `check_uni_emails` (`_email` VARCHAR(60), `_uni_id` INT(11)) RETURNS INT(11) RETURN (
  SELECT COUNT(n.uni_id) 
  FROM universities n
  WHERE _uni_id = n.uni_id AND _email LIKE CONCAT("%", n.email_domain))$$

DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `administrates`
--

CREATE TABLE `administrates` (
  `user_id` int(11) UNSIGNED NOT NULL,
  `rso_id` int(11) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `attending`
--

CREATE TABLE `attending` (
  `user_id` int(11) UNSIGNED NOT NULL,
  `uni_id` int(11) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `commented_on`
--

CREATE TABLE `commented_on` (
  `comment_id` int(11) UNSIGNED NOT NULL,
  `date_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `content` tinytext NOT NULL,
  `user_id` int(11) UNSIGNED NOT NULL,
  `event_id` int(11) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `created`
--

CREATE TABLE `created` (
  `rso_id` int(11) UNSIGNED NOT NULL,
  `event_id` int(11) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `events`
--

CREATE TABLE `events` (
  `event_id` int(11) UNSIGNED NOT NULL,
  `name` varchar(60) NOT NULL,
  `start_time` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `end_time` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `description` tinytext NOT NULL,
  `location` varchar(60) NOT NULL,
  `street` varchar(60) NOT NULL,
  `city` varchar(60) NOT NULL,
  `state` tinytext NOT NULL,
  `zip` tinytext NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `has`
--

CREATE TABLE `has` (
  `uni_id` int(11) UNSIGNED NOT NULL,
  `rso_id` int(11) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `hosting`
--

CREATE TABLE `hosting` (
  `uni_id` int(11) UNSIGNED NOT NULL,
  `event_id` int(11) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `is_member`
--

CREATE TABLE `is_member` (
  `user_id` int(11) UNSIGNED NOT NULL,
  `rso_id` int(11) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `participating`
--

CREATE TABLE `participating` (
  `user_id` int(11) UNSIGNED NOT NULL,
  `event_id` int(11) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `private_events`
--

CREATE TABLE `private_events` (
  `event_id` int(11) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `public_events`
--

CREATE TABLE `public_events` (
  `event_id` int(11) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `rsos`
--

CREATE TABLE `rsos` (
  `rso_id` int(11) UNSIGNED NOT NULL,
  `name` varchar(60) NOT NULL,
  `description` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `rso_events`
--

CREATE TABLE `rso_events` (
  `event_id` int(11) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `universities`
--

CREATE TABLE `universities` (
  `uni_id` int(11) UNSIGNED NOT NULL,
  `name` varchar(60) NOT NULL,
  `description` text NOT NULL,
  `street` varchar(60) NOT NULL,
  `city` varchar(60) NOT NULL,
  `state` tinytext NOT NULL,
  `zip` tinytext NOT NULL,
  `email_domain` varchar(30) NOT NULL,
  `website_url` varchar(60) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `users`
--

CREATE TABLE `users` (
  `user_id` int(11) UNSIGNED NOT NULL,
  `first_name` varchar(60) NOT NULL,
  `last_name` varchar(60) NOT NULL,
  `user_name` varchar(30) NOT NULL,
  `email` varchar(60) NOT NULL,
  `role` enum('SA','ADM','STU','') NOT NULL DEFAULT 'STU',
  `hash` varchar(60) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `attending`
--
ALTER TABLE `attending`
  ADD KEY `user_id` (`user_id`),
  ADD KEY `uni_id` (`uni_id`);

--
-- Indexes for table `commented_on`
--
ALTER TABLE `commented_on`
  ADD PRIMARY KEY (`comment_id`);

--
-- Indexes for table `events`
--
ALTER TABLE `events`
  ADD PRIMARY KEY (`event_id`);

--
-- Indexes for table `has`
--
ALTER TABLE `has`
  ADD KEY `uni_id` (`uni_id`),
  ADD KEY `rso_id` (`rso_id`);

--
-- Indexes for table `hosting`
--
ALTER TABLE `hosting`
  ADD KEY `uni_id` (`uni_id`),
  ADD KEY `event_id` (`event_id`);

--
-- Indexes for table `rsos`
--
ALTER TABLE `rsos`
  ADD PRIMARY KEY (`rso_id`);

--
-- Indexes for table `universities`
--
ALTER TABLE `universities`
  ADD PRIMARY KEY (`uni_id`),
  ADD UNIQUE KEY `name` (`name`);

--
-- Indexes for table `users`
--
ALTER TABLE `users`
  ADD PRIMARY KEY (`user_id`),
  ADD UNIQUE KEY `user_name` (`user_name`),
  ADD UNIQUE KEY `email` (`email`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `commented_on`
--
ALTER TABLE `commented_on`
  MODIFY `comment_id` int(11) UNSIGNED NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `events`
--
ALTER TABLE `events`
  MODIFY `event_id` int(11) UNSIGNED NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `rsos`
--
ALTER TABLE `rsos`
  MODIFY `rso_id` int(11) UNSIGNED NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `universities`
--
ALTER TABLE `universities`
  MODIFY `uni_id` int(11) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7;
--
-- AUTO_INCREMENT for table `users`
--
ALTER TABLE `users`
  MODIFY `user_id` int(11) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=17;
--
-- Constraints for dumped tables
--

--
-- Constraints for table `attending`
--
ALTER TABLE `attending`
  ADD CONSTRAINT `attending_uni_id` FOREIGN KEY (`uni_id`) REFERENCES `universities` (`uni_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `attending_user_id` FOREIGN KEY (`user_id`) REFERENCES `users` (`user_id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `has`
--
ALTER TABLE `has`
  ADD CONSTRAINT `has_rso_id` FOREIGN KEY (`rso_id`) REFERENCES `rsos` (`rso_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `has_uni_id` FOREIGN KEY (`uni_id`) REFERENCES `universities` (`uni_id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `hosting`
--
ALTER TABLE `hosting`
  ADD CONSTRAINT `hosting_event_id` FOREIGN KEY (`event_id`) REFERENCES `events` (`event_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `hosting_uni_id` FOREIGN KEY (`uni_id`) REFERENCES `universities` (`uni_id`) ON DELETE CASCADE ON UPDATE CASCADE;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;

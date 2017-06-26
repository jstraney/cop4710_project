-- phpMyAdmin SQL Dump
-- version 4.5.2
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Jun 22, 2017 at 03:34 PM
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
CREATE DEFINER=`root`@`localhost` PROCEDURE `attend_event` (IN `_event_id` INT(11), IN `_user_id` INT(11), IN `_uni_id` INT(11))  BEGIN

  DECLARE _accessibility ENUM('PUB','PRI','RSO');
  DECLARE _not_participating INT(1);

  SET _accessibility = (SELECT e.accessibility FROM events e WHERE e.event_id = _event_id);

  
  IF EXISTS (
    SELECT p.user_id
    FROM participating p, events e
    WHERE _user_id = p.user_id
    AND p.event_id = _event_id) THEN

    SET _not_participating = 0;

  ELSE

    SET _not_participating = 1;

  END IF;

  
  IF _accessibility = "PUB" AND _not_participating THEN
    INSERT INTO participating (user_id, event_id)
    VALUES (_user_id, _event_id);

  ELSEIF _accessibility = "PRI" AND _not_participating THEN
    INSERT INTO participating (user_id, event_id)
    SELECT u.user_id
    FROM users u, attending a, universities n, private_events pe
    WHERE _user_id = u.user_id
    AND u.user_id = a.user_id
    AND a.uni_id = n.uni_id
    AND n.uni_id = pe.uni_id
    AND pe.event_id = _event_id LIMIT 1;

  ELSEIF _accessibility = "RSO" AND _not_participating THEN
    INSERT INTO participating (user_id, event_id)
    SELECT u.user_id, _event_id
    FROM users u, is_member m, rso_events re
    WHERE _user_id = u.user_id
    AND u.user_id = m.user_id
    AND m.rso_id = re.rso_id
    AND re.event_id = _event_id LIMIT 1;

  END IF;

END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `change_event_access` (IN `_event_id` INT(11), IN `_uni_id` INT(11), IN `_rso_id` INT(11), IN `_accessibility` ENUM('PUB','PRI','RSO'), IN `old_type` ENUM('PUB','PRI','RSO'))  BEGIN
  
  

  
  
  DELETE FROM public_events WHERE event_id = _event_id;
  DELETE FROM private_events WHERE event_id = _event_id;
  DELETE FROM rso_events WHERE event_id = _event_id;

  
  IF _accessibility = 'RSO' THEN
    BEGIN
      
      IF NOT EXISTS (SELECT rso_id FROM rsos WHERE rso_id = _rso_id) THEN
        
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "The rso specified cannot be found.";
      ELSE
        INSERT INTO rso_events (event_id, rso_id)
        VALUES (_event_id, _rso_id);
        
      END IF;
    END;
  ELSEIF _accessibility = 'PRI' THEN
    BEGIN
      
      IF NOT EXISTS (SELECT uni_id FROM universities WHERE uni_id = _uni_id) THEN
        
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "The university specified cannot be found.";
      ELSE
        INSERT INTO private_events (event_id, uni_id)
        VALUES (_event_id, _uni_id);
      END IF;
    END;
  ELSE 
    BEGIN
        INSERT INTO public_events (event_id)
        VALUES (_event_id);
    END;
  END IF;
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `comment_on_event` (IN `_content` TINYTEXT, IN `_event_id` INT(11), IN `_user_id` INT(11), IN `_uni_id` INT(11))  BEGIN

  DECLARE _accessibility ENUM ('PUB','PRI','RSO');

  SET _accessibility = (SELECT accessibility FROM events WHERE event_id = _event_id);

  
  IF _accessibility = "PUB" OR
    
    ( _accessibility = "PRI" AND EXISTS(
    SELECT a.user_id
    FROM attending a, private_events pe
    WHERE _user_id = a.user_id
    AND a.uni_id = pe.uni_id 
    AND pe.event_id = _event_id)) OR
    
    ( _accessibility = "RSO" AND EXISTS(
    SELECT m.user_id
    FROM is_member m, rso_events re 
    WHERE _user_id = m.user_id
    AND m.rso_id = re.rso_id
    AND re.event_id = _event_id)) THEN

    
    INSERT INTO commented_on (date_posted, content, user_id, event_id)
    VALUES (NOW(), _content, _user_id, _event_id);

    SELECT comment_id FROM commented_on WHERE comment_id = LAST_INSERT_ID(); 

  END IF;
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `create_event` (IN `_name` VARCHAR(60), IN `_start_time` TIMESTAMP, IN `_end_time` TIMESTAMP, IN `_description` TINYTEXT, IN `_location` VARCHAR(60), IN `_street` VARCHAR(60), IN `_city` VARCHAR(60), IN `_state` TINYTEXT, IN `_zip` TINYTEXT, IN `_lat` DECIMAL(9,6), IN `_lon` DECIMAL(9,6), IN `_accessibility` ENUM('PUB','PRI','RSO'), IN `_categories` VARCHAR(60), IN `_rso_id` INT(11), IN `_uni_id` INT(11))  BEGIN

  
  DECLARE _event_id INT(11);

  DECLARE _status ENUM('PND','ACT');

  
  IF EXISTS(

    SELECT e.event_id FROM events e 
    
    WHERE e.location = _location
    
    AND ((_end_time >= e.start_time AND _end_time <= e.end_time) OR
      
        (_start_time >= e.start_time AND _start_time <= e.end_time))) THEN

    
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "Your event is occuring at the same time and location as another event";

  END IF;

  
  
  INSERT INTO events (name, start_time, end_time, description, location,
  street, city, state, zip, lat, lon, accessibility)
  VALUES (_name, _start_time, _end_time, _description, _location,
  _street, _city, _state, _zip, _lat, _lon, _accessibility);

  
  SET _event_id = LAST_INSERT_ID();

  
  INSERT INTO hosting (uni_id, event_id)
  VALUES (_uni_id, _event_id);

  
  
  
  IF _rso_id > 0 THEN
    
    SET _status = 'ACT';
    
    INSERT INTO r_created_e (rso_id, event_id)
    VALUES (_rso_id, _event_id);
  
  ELSE
    
    SET _status = 'PND';
    
    INSERT INTO u_created_e (user_id, event_id)
    VALUES (_user_id, _rso_id);

  END IF;

  
  UPDATE events SET status = _status WHERE event_id = _event_id;

  
  IF _accessibility = 'RSO' THEN
    BEGIN
      
      IF NOT EXISTS (SELECT r.rso_id FROM rsos r WHERE r.rso_id = _rso_id) THEN
        
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "The rso specified cannot be found.";
      ELSE
        
        INSERT INTO rso_events (event_id, rso_id)
        VALUES (_event_id, _rso_id);
      END IF;
    END;
  ELSEIF _accessibility = 'PRI' THEN
    BEGIN
      
      IF NOT EXISTS (SELECT uni_id FROM universities WHERE uni_id = _uni_id) THEN
        
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "The university specified cannot be found.";
      ELSE
        
        INSERT INTO private_events (event_id, uni_id)
        VALUES (_event_id, _uni_id);
      END IF;
    END;
  ELSE
    BEGIN
        INSERT INTO public_events (event_id)
        VALUES (_event_id);
    END;
  END IF;

  
  SELECT _event_id AS event_id FROM events LIMIT 1;

END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `create_rso` (IN `_role` ENUM('SA','ADM','STU'), IN `_user_id` INT(11), IN `_name` VARCHAR(60), IN `_description` TEXT(500), IN `_members` VARCHAR(300), IN `_rso_admin_id` INT(11), IN `_uni_id` INT(11))  BEGIN

  DECLARE _rso_id INT(11);

  
  CREATE TEMPORARY TABLE temp_members (user_id INT(11) UNSIGNED);
  INSERT INTO temp_members (user_id)
  SELECT user_id FROM users WHERE FIND_IN_SET(user_id, _members);

  
  IF _role = "STU" AND (SELECT COUNT(user_id) FROM temp_members) < 5 THEN
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "You must specify at least 5 members";
  END IF;

  
  IF _role = "STU" AND NOT EXISTS (
    SELECT u.user_id FROM
    users u, attending a, universities n
    WHERE _user_id = u.user_id
    AND u.user_id = a.user_id 
    AND a.uni_id = n.uni_id
    AND n.uni_id = _uni_id) THEN
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "You may only create an RSO at a university you attend";
  
  ELSEIF _role = "ADM" AND NOT EXISTS (
    SELECT u.user_id FROM
    users u, affiliated_with a, universities n
    WHERE _user_id = u.user_id
    AND u.user_id = a.user_id 
    AND a.uni_id = n.uni_id
    AND n.uni_id = _uni_id) THEN
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "You may only create an RSO at a university you're affiliated with";
  END IF;

  
  INSERT INTO rsos (name, description)
  VALUES (_name, _description);

  
  SET _rso_id = LAST_INSERT_ID();

  
  INSERT INTO has (uni_id, rso_id)
  VALUES (_uni_id, _rso_id); 

  
  INSERT INTO administrates (user_id, rso_id)
  VALUES (_rso_admin_id, _rso_id);

  
  INSERT INTO is_member (user_id, rso_id) (SELECT user_id, _rso_id FROM temp_members);
  DROP TABLE temp_members;

  
  
  IF _role = "SA" OR _role = "ADM" THEN
    BEGIN
    
    END;
  
  
  ELSEIF _role = "STU" THEN
    BEGIN
    
    
    END;
  END IF;

  
  SELECT _rso_id AS rso_id FROM rsos LIMIT 1; 

END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `create_user` (IN `_user_name` VARCHAR(30), IN `_first_name` VARCHAR(60), IN `_last_name` VARCHAR(60), IN `_email` VARCHAR(60), IN `_role` ENUM("SA","ADM","STU"), IN `_hash` VARCHAR(60), IN `_uni_id` INT(11))  BEGIN

IF (_role = "STU" OR _role = "ADM") AND (check_uni_emails(_email, _uni_id) < 1) THEN
  SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'e-mail must match university selected.';
ELSEIF _role = "STU" THEN
  BEGIN
    INSERT INTO users (user_name, first_name, last_name, email, role, hash)
    VALUES (_user_name, _first_name, _last_name, _email, _role, _hash);
    
    INSERT INTO attending (user_id, uni_id)
    VALUES (LAST_INSERT_ID(), _uni_id);
  END;

ELSEIF _role = "ADM" THEN
  BEGIN
    INSERT INTO users (user_name, first_name, last_name, email, role, hash)
    VALUES (_user_name, _first_name, _last_name, _email, _role, _hash);
    
    INSERT INTO affiliated_with (user_id, uni_id)
    VALUES (LAST_INSERT_ID(), _uni_id);
  END;

ELSEIF _role = "SA"  THEN
  INSERT INTO users (user_name, first_name, last_name, email, role, hash)
  VALUES (_user_name, _first_name, _last_name, _email, role, _hash);
END IF;
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `get_current_user` (IN `_user_id` INT(11))  BEGIN

  DECLARE _role ENUM('SA','ADM','STU');

  SET _role = (SELECT u.role FROM users u WHERE u.user_id = _user_id LIMIT 1);

  
  IF _role = "STU" THEN
    SELECT u.user_name, u.email, u.first_name, u.last_name, u.role, a.uni_id
    FROM users u, attending a 
    WHERE _user_id = u.user_id
    AND u.user_id = a.user_id
    LIMIT 1;
  ELSEIF _role = "ADM" THEN
    SELECT u.user_name, u.email, u.first_name, u.last_name, u.role, a.uni_id
    FROM users u, affiliated_with a 
    WHERE _user_id = u.user_id
    AND u.user_id = a.user_id
    LIMIT 1;
  ELSEIF _role = "SA" THEN
    SELECT u.user_name, u.email, u.first_name, u.last_name, u.role
    FROM users u, attending a 
    WHERE _user_id = u.user_id
    LIMIT 1;
  END IF;

END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `rate_event` (IN `_user_id` INT(11), IN `_event_id` INT(11), IN `_rating` INT(1))  BEGIN

  
  IF NOT EXISTS(
    SELECT r.user_id
    FROM rating r
    WHERE r.user_id = _user_id
    AND r.event_id = _event_id) THEN
    
    INSERT INTO rating (user_id, event_id, rating)
    VALUES (_user_id, _event_id, _rating);
  ELSE
    
    UPDATE rating SET
    rating = _rating
    WHERE user_id = _user_id
    AND event_id = _event_id;
  END IF;

  
  UPDATE events SET
  rating = (
    SELECT AVG(r.rating)
    FROM rating r
    WHERE r.event_id = _event_id)
  WHERE event_id = _event_id;

  
  SELECT r.rating, e.rating AS total_rating
  FROM rating r, events e
  WHERE r.user_id = _user_id
  AND r.event_id = _event_id
  AND e.event_id = _event_id;

END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `search_events` (IN `_user_id` INT(11), IN `_uni_id` INT(11), IN `_scope` TINYTEXT, IN `_sort_by` TINYTEXT, IN `_accessibility` ENUM('PUB','PRI','RSO'))  BEGIN
  DECLARE _is_owner INT(1);
  DECLARE _is_participating INT(1);

  SET _is_owner = get_is_owner(_user_id, _event_id);
  SET _is_participating = get_is_participating(_user_id, _event_id);

  
  IF _scope = "other-uni" THEN
    SELECT e.name, e.start_time, e.end_time, e.description, e.location, e.street,
    e.city, e.state, e.zip, e.lat, e.lon, e.accessibility, e.status, _is_owner AS is_owner,
    _is_participating AS is_participating, e.rating
    FROM events e, hosting h
    WHERE _uni_id = h.uni_id
    AND h.event_id = e.event_id
    AND e.accessibility = "PUB";

  
  ELSEIF _accessibility = "PUB" THEN
    BEGIN
      SELECT e.name, e.start_time, e.end_time, e.description, e.location, e.street,
      e.city, e.state, e.zip, e.lat, e.lon, e.accessibility, e.status, _is_owner AS is_owner,
      _is_participating AS is_participating, e.rating
      FROM events e, hosting h
      WHERE h.uni_id = _uni_id 
      AND h.event_id = e.event_id;
    END;

  
  ELSEIF _accessibility = "PRI" THEN
    BEGIN
      SELECT e.name, e.start_time, e.end_time, e.description, e.location, e.street,
      e.city, e.state, e.zip, e.lat, e.lon, e.accessibility, e.status, _is_owner AS is_owner,
      _is_participating AS is_participating, e.rating
      FROM events e, private_events p, attending at, affiliated_with aw
      WHERE p.uni_id = _uni_id 
      AND p.event_id = e.event_id;
    END;

  
  ELSEIF _accessibility = "RSO" THEN
    BEGIN
      SELECT e.name, e.start_time, e.end_time, e.description, e.location, e.street,
      e.city, e.state, e.zip, e.lat, e.lon, e.accessibility, e.status, _is_owner AS is_owner,
      _is_participating AS is_participating, e.rating
      FROM events e, rso_events re
      WHERE e.event_id = _event_id
      AND re.event_id = e.event_id
      AND re.rso_id = ANY(
        SELECT r.rso_id 
        FROM rsos r, is_member m
        WHERE _user_id = m.user_id AND m.rso_id = r.rso_id);
    END;
  END IF;
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `select_students` ()  BEGIN
  SELECT u.user_id, u.user_name, u.email, u.first_name, u.last_name
  FROM users u
  WHERE u.role = "STU";
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `update_event` (IN `_event_id` INT(11), IN `_name` VARCHAR(60), IN `_start_time` TIMESTAMP, IN `_end_time` TIMESTAMP, IN `_description` TINYTEXT, IN `_location` VARCHAR(60), IN `_street` VARCHAR(60), IN `_city` VARCHAR(60), IN `_state` TINYTEXT, IN `_zip` TINYTEXT, IN `_lat` DECIMAL(9,6), IN `_lon` DECIMAL(9,6), IN `_accessibility` ENUM('PUB','PRI','RSO'), IN `_categories` VARCHAR(60), IN `_user_id` INT(11), IN `_rso_id` INT(11), IN `_uni_id` INT(11))  BEGIN

  
  
  DECLARE _old_type ENUM('PUB','PRI','RSO');

  
  IF NOT EXISTS(
    SELECT e.event_id
    FROM events e, r_created_e c, rsos r, administrates a
    WHERE _user_id = a.user_id
    AND a.rso_id = r.rso_id
    AND r.rso_id = c.rso_id
    AND c.event_id = _event_id) AND 
  
  NOT EXISTS (
    SELECT e.event_id 
    FROM events e, u_created_e u 
    WHERE e.event_id = _event_id
    AND e.event_id = u.event_id
    AND u.user_id = _user_id) THEN

    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "You are not authorized to change this event.";

  END IF;


  SET _old_type = (SELECT e.accessibility FROM events e WHERE e.event_id = _event_id LIMIT 1);
   
  
  
  UPDATE events SET
  name = _name,
  start_time = _start_time,
  end_time = _end_time,
  description = _description,
  location = _location,
  street = _street,
  city = _city,
  state = _state,
  zip = _state,
  lat = _lat,
  lon = _lon
  WHERE events.event_id = _event_id;

  IF _old_type <> _accessibility THEN
    BEGIN
      
      CALL change_event_access(_event_id, _uni_id, _rso_id, _accessibility, _old_type);
    END;
  END IF;

  
  SELECT _event_id AS event_id FROM events LIMIT 1;

END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `update_rso` (IN `_rso_id` INT(11), IN `_role` ENUM('SA','ADM','STU'), IN `_user_id` INT(11), IN `_name` VARCHAR(60), IN `_description` TEXT(500), IN `_rso_admin_id` INT(11), IN `_uni_id` INT(11))  BEGIN
  
  
  

  
  IF _user_id <> (SELECT a.user_id FROM administrates a WHERE a.rso_id = _rso_id) THEN
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "You are not athorized to change this RSO";
  END IF; 

  
  IF _role = "STU" AND NOT EXISTS (
    SELECT u.user_id FROM
    users u, attending a, universities n
    WHERE _user_id = u.user_id
    AND u.user_id = a.user_id 
    AND a.uni_id = n.uni_id
    AND n.uni_id = _uni_id) THEN
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "You may only create an RSO at a university you attend";
  
  ELSEIF _role = "ADM" AND NOT EXISTS (
    SELECT u.user_id FROM
    users u, affiliated_with a, universities n
    WHERE _user_id = u.user_id
    AND u.user_id = a.user_id 
    AND a.uni_id = n.uni_id
    AND n.uni_id = _uni_id) THEN
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "You may only create an RSO at a university you're affiliated with";
  END IF;

  
  UPDATE rsos SET
  name = _name,
  description = _description
  WHERE rso_id = _rso_id; 

  
  IF _role = "SA" OR _role = "ADM" THEN
    BEGIN
    
    END;
  END IF;

  
  SELECT _rso_id AS rso_id FROM rsos LIMIT 1;

END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `update_user` (IN `_user_id` INT(11), IN `_user_name` VARCHAR(30), IN `_first_name` VARCHAR(60), IN `_last_name` VARCHAR(60), IN `_email` VARCHAR(60), IN `_role` ENUM("SA","ADM","STU"), IN `_hash` VARCHAR(60), IN `_uni_id` INT(11))  BEGIN

  IF _role = "STU" AND (check_uni_emails(_email, _uni_id) < 1) THEN
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'e-mail must match university selected.';
  END IF;

  UPDATE users SET
  user_name = _user_name,
  first_name = _first_name,
  last_name = _last_name,
  email = _email,
  role = _role,
  hash = _hash
  WHERE user_id = _user_id;

  IF _role = "STU" THEN
    UPDATE attending SET
    uni_id = _uni_id
    WHERE user_id = _user_id;
  ELSEIF _role = "ADM" OR _role = "SA" THEN
    UPDATE affiliated_with SET
    uni_id = _uni_id
    WHERE user_id = _user_id;
  END IF;

END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `view_event` (IN `_event_id` INT(11), IN `_user_id` INT(11), IN `_role` ENUM('SA','ADM','STU'), IN `_uni_id` INT(11))  BEGIN
  
  DECLARE _accessibility ENUM ('PUB','PRI','RSO');
  DECLARE _is_owner INT(1);
  DECLARE _is_participating INT(1);

  
  SET _accessibility = (SELECT accessibility FROM events WHERE event_id = _event_id);

  
  
  SET _is_owner = get_is_owner(_user_id, _event_id);
  SET _is_participating = get_is_participating(_user_id, _event_id);

  
  IF _accessibility = "PUB" THEN
    BEGIN
      SELECT e.name, e.start_time, e.end_time, e.description, e.location, e.street,
      e.city, e.state, e.zip, e.lat, e.lon, e.accessibility, e.status, _is_owner AS is_owner,
      _is_participating AS is_participating, e.rating
      FROM events e
      WHERE e.event_id = _event_id LIMIT 1;
    END;

  
  ELSEIF _accessibility = "PRI" THEN
    BEGIN
      SELECT e.name, e.start_time, e.end_time, e.description, e.location, e.street,
      e.city, e.state, e.zip, e.lat, e.lon, e.accessibility, e.status, _is_owner AS is_owner,
      _is_participating AS is_participating, e.rating
      FROM events e, private_events p
      WHERE e.event_id = _event_id
      AND e.event_id = p.event_id
      AND p.uni_id = _uni_id
      OR _role = "SA" LIMIT 1;
    END;

  
  ELSEIF _accessibility = "RSO" THEN
    BEGIN
      SELECT e.name, e.start_time, e.end_time, e.description, e.location, e.street,
      e.city, e.state, e.zip, e.lat, e.lon, e.accessibility, e.status, _is_owner AS is_owner,
      _is_participating AS is_participating, e.rating
      FROM events e, rso_events re
      WHERE e.event_id = _event_id
      AND re.event_id = e.event_id
      AND re.rso_id = ANY(
        SELECT r.rso_id 
        FROM rsos r, is_member m
        WHERE _user_id = m.user_id AND m.rso_id = r.rso_id)
      OR _role = "SA" LIMIT 1;
    END;

  END IF;

END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `view_rso` (IN `_rso_id` INT(11))  BEGIN
  SELECT r.rso_id, r.name, r.description, a.user_id AS rso_admin
  FROM  administrates a, rsos r
  WHERE r.rso_id = _rso_id
  AND a.rso_id = _rso_id;
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `view_user` (IN `_user_id` INT(11))  BEGIN

  DECLARE _role ENUM('SA','ADM','STU');

  SET _role = (SELECT role FROM users WHERE user_id = _user_id);

  IF _role = "STU" THEN
    SELECT u.user_name, u.first_name, u.last_name, n.uni_id, u.email, u.role, n.name AS uni_name
    FROM users u, attending a, universities n
    WHERE u.user_id = _user_id AND _user_id = a.user_id AND a.uni_id = n.uni_id
    LIMIT 1;
  ELSEIF _role = "ADM" THEN
    SELECT u.user_name, u.first_name, u.last_name, n.uni_id, u.email, u.role, n.name AS uni_name
    FROM users u, affiliated_with a, universities n
    WHERE u.user_id = _user_id AND _user_id = a.user_id AND a.uni_id = n.uni_id
    LIMIT 1;
  ELSEIF _role = "SA" THEN
    SELECT u.user_name, u.first_name, u.last_name, u.email, u.role
    FROM users u
    WHERE u.user_id = _user_id LIMIT 1;
  END IF;
END$$

--
-- Functions
--
CREATE DEFINER=`root`@`localhost` FUNCTION `check_uni_emails` (`_email` VARCHAR(60), `_uni_id` INT(11)) RETURNS INT(11) RETURN (
  SELECT COUNT(n.uni_id) 
  FROM universities n
  WHERE _uni_id = n.uni_id AND _email LIKE CONCAT("%", n.email_domain))$$

CREATE DEFINER=`root`@`localhost` FUNCTION `get_is_owner` (`_user_id` INT(11), `_event_id` INT(11)) RETURNS INT(1) BEGIN

  DECLARE _is_owner INT(1);

  IF EXISTS(
    SELECT u.user_id
    FROM u_created_e u, events e
    WHERE u.user_id = _user_id
    AND u.event_id = _event_id) THEN
    
    SET _is_owner = 1;

  
  ELSEIF EXISTS(
    SELECT a.user_id
    FROM administrates a, rsos r, r_created_e re
    WHERE a.user_id = _user_id
    AND a.rso_id = r.rso_id
    AND r.rso_id = re.rso_id
    AND re.event_id = _event_id) THEN

    
    SET _is_owner = 1;

  ELSE

    
    SET _is_owner = 0;

  END IF;

  RETURN _is_owner;

END$$

CREATE DEFINER=`root`@`localhost` FUNCTION `get_is_participating` (`_user_id` INT(11), `_event_id` INT(11)) RETURNS INT(1) BEGIN

  DECLARE _is_participating INT(1);

  
  IF EXISTS(
    SELECT p.user_id
    FROM participating p
    WHERE _user_id = p.user_id
    AND p.event_id = _event_id ) THEN
    
    SET _is_participating = 1;

  ELSE


    SET _is_participating = 0;

  END IF;

  RETURN _is_participating;

END$$

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
-- Table structure for table `affiliated_with`
--

CREATE TABLE `affiliated_with` (
  `user_id` int(10) UNSIGNED NOT NULL,
  `uni_id` int(10) UNSIGNED NOT NULL
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
  `date_posted` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `content` tinytext NOT NULL,
  `user_id` int(11) UNSIGNED NOT NULL,
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
  `zip` tinytext NOT NULL,
  `lat` decimal(9,6) NOT NULL,
  `lon` decimal(9,6) NOT NULL,
  `accessibility` enum('PUB','PRI','RSO') NOT NULL DEFAULT 'PUB',
  `status` enum('PND','ACT') NOT NULL DEFAULT 'PND',
  `rating` decimal(9,2) NOT NULL
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
  `event_id` int(11) UNSIGNED NOT NULL,
  `uni_id` int(11) NOT NULL
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
-- Table structure for table `rating`
--

CREATE TABLE `rating` (
  `event_id` int(10) UNSIGNED NOT NULL,
  `rating` tinyint(1) NOT NULL,
  `last_rated` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `user_id` int(10) UNSIGNED NOT NULL
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
  `event_id` int(11) UNSIGNED NOT NULL,
  `rso_id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `r_created_e`
--

CREATE TABLE `r_created_e` (
  `rso_id` int(11) UNSIGNED NOT NULL,
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
  `lat` decimal(9,6) NOT NULL,
  `lon` decimal(9,6) NOT NULL,
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

-- --------------------------------------------------------

--
-- Table structure for table `u_created_e`
--

CREATE TABLE `u_created_e` (
  `user_id` int(10) UNSIGNED NOT NULL,
  `event_id` int(10) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `administrates`
--
ALTER TABLE `administrates`
  ADD KEY `user_id` (`user_id`),
  ADD KEY `rso_id` (`rso_id`);

--
-- Indexes for table `affiliated_with`
--
ALTER TABLE `affiliated_with`
  ADD KEY `user_id` (`user_id`),
  ADD KEY `uni_id` (`uni_id`);

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
-- Indexes for table `is_member`
--
ALTER TABLE `is_member`
  ADD KEY `user_id` (`user_id`),
  ADD KEY `rso_id` (`rso_id`);

--
-- Indexes for table `rating`
--
ALTER TABLE `rating`
  ADD KEY `event_id` (`event_id`),
  ADD KEY `user_id` (`user_id`);

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
-- Indexes for table `u_created_e`
--
ALTER TABLE `u_created_e`
  ADD KEY `user_id` (`user_id`),
  ADD KEY `event_id` (`event_id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `commented_on`
--
ALTER TABLE `commented_on`
  MODIFY `comment_id` int(11) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=9;
--
-- AUTO_INCREMENT for table `events`
--
ALTER TABLE `events`
  MODIFY `event_id` int(11) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;
--
-- AUTO_INCREMENT for table `rsos`
--
ALTER TABLE `rsos`
  MODIFY `rso_id` int(11) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=20;
--
-- AUTO_INCREMENT for table `universities`
--
ALTER TABLE `universities`
  MODIFY `uni_id` int(11) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7;
--
-- AUTO_INCREMENT for table `users`
--
ALTER TABLE `users`
  MODIFY `user_id` int(11) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=11;
--
-- Constraints for dumped tables
--

--
-- Constraints for table `administrates`
--
ALTER TABLE `administrates`
  ADD CONSTRAINT `administrates_uni_id` FOREIGN KEY (`rso_id`) REFERENCES `rsos` (`rso_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `administrates_user_id` FOREIGN KEY (`user_id`) REFERENCES `users` (`user_id`) ON DELETE CASCADE ON UPDATE CASCADE;

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

--
-- Constraints for table `is_member`
--
ALTER TABLE `is_member`
  ADD CONSTRAINT `is_member_rso_id` FOREIGN KEY (`rso_id`) REFERENCES `rsos` (`rso_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `is_member_user_id` FOREIGN KEY (`user_id`) REFERENCES `users` (`user_id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `rating`
--
ALTER TABLE `rating`
  ADD CONSTRAINT `rating_event_id` FOREIGN KEY (`event_id`) REFERENCES `events` (`event_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `rating_user_id` FOREIGN KEY (`user_id`) REFERENCES `users` (`user_id`) ON DELETE CASCADE ON UPDATE CASCADE;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;

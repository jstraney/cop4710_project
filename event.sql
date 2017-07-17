-- phpMyAdmin SQL Dump
-- version 4.5.2
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Jul 18, 2017 at 12:14 AM
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
  DECLARE _status ENUM('PND','ACT');

  DECLARE EXIT HANDLER FOR SQLEXCEPTION 
  BEGIN
    DECLARE _c_num INT;
    DECLARE _err_msg TEXT;
    ROLLBACK;
    GET DIAGNOSTICS _c_num = NUMBER;
    GET DIAGNOSTICS CONDITION _c_num _err_msg = MESSAGE_TEXT;
    SELECT _err_msg;
  END;

  START TRANSACTION;

    SELECT e.accessibility, e.status INTO _accessibility, _status
    FROM events e
    WHERE e.event_id = _event_id;

    
    IF EXISTS (
      SELECT p.user_id
      FROM participating p, events e
      WHERE _user_id = p.user_id
      AND p.event_id = _event_id) THEN

      SET _not_participating = 0;

    ELSE

      SET _not_participating = 1;

    END IF;

    
    IF _status = "ACT" AND _accessibility = "PUB" AND _not_participating THEN
      INSERT INTO participating (user_id, event_id)
      VALUES (_user_id, _event_id);

    ELSEIF _status = "ACT" AND _accessibility = "PRI" AND _not_participating THEN
      INSERT INTO participating (user_id, event_id)
      SELECT u.user_id, _event_id
      FROM users u, attending a, universities n, private_events pe
      WHERE _user_id = u.user_id
      AND u.user_id = a.user_id
      AND a.uni_id = n.uni_id
      AND n.uni_id = pe.uni_id
      AND pe.event_id = _event_id LIMIT 1;

    ELSEIF _status = "ACT" AND _accessibility = "RSO" AND _not_participating THEN
      INSERT INTO participating (user_id, event_id)
      SELECT u.user_id, _event_id
      FROM users u, is_member m, rso_events re
      WHERE _user_id = u.user_id
      AND u.user_id = m.user_id
      AND m.rso_id = re.rso_id
      AND re.event_id = _event_id LIMIT 1;

    END IF;

  COMMIT;

END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `change_event_access` (IN `_event_id` INT(11), IN `_uni_id` INT(11), IN `_rso_id` INT(11), IN `_accessibility` ENUM('PUB','PRI','RSO'), IN `_old_type` ENUM('PUB','PRI','RSO'))  BEGIN

  
  DECLARE EXIT HANDLER FOR SQLEXCEPTION 
  BEGIN
    DECLARE _c_num INT;
    DECLARE _err_msg TEXT;
    ROLLBACK;
    GET DIAGNOSTICS _c_num = NUMBER;
    GET DIAGNOSTICS CONDITION _c_num _err_msg = MESSAGE_TEXT;
    SELECT _err_msg;
  END;

  START TRANSACTION;
  
  

  
  
  IF _old_type = 'PUB' THEN 
    DELETE FROM public_events WHERE event_id = _event_id;

  ELSEIF _old_type = 'PRI' THEN 
    DELETE FROM private_events WHERE event_id = _event_id;

  ELSEIF _old_type = 'RSO' THEN 
    DELETE FROM rso_events WHERE event_id = _event_id;
    DELETE FROM r_created_e WHERE event_id = _event_id;

  END IF;

  
  IF _accessibility = 'RSO' THEN

    BEGIN
      

      IF NOT EXISTS (SELECT rso_id FROM rsos WHERE rso_id = _rso_id) THEN
        
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "The rso specified cannot be found.";

      ELSE
        INSERT INTO rso_events (event_id, rso_id, uni_id)
        VALUES (_event_id, _rso_id, _uni_id);
        
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
        INSERT INTO public_events (event_id, uni_id)
        VALUES (_event_id, _uni_id);
    END;
  END IF;

  
  UPDATE events SET
  accessibility = _accessibility
  WHERE event_id = _event_id;

END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `comment_on_event` (IN `_content` TINYTEXT, IN `_event_id` INT(11), IN `_user_id` INT(11), IN `_uni_id` INT(11))  BEGIN

  DECLARE _accessibility ENUM ('PUB','PRI','RSO');

  DECLARE EXIT HANDLER FOR SQLEXCEPTION 
  BEGIN
    DECLARE _c_num INT;
    DECLARE _err_msg TEXT;
    ROLLBACK;
    GET DIAGNOSTICS _c_num = NUMBER;
    GET DIAGNOSTICS CONDITION _c_num _err_msg = MESSAGE_TEXT;
    SELECT _err_msg;
  END;

  START TRANSACTION;

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

  COMMIT;

END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `create_event` (IN `_name` VARCHAR(60), IN `_start_time` TIMESTAMP, IN `_end_time` TIMESTAMP, IN `_telephone` VARCHAR(12), IN `_email` VARCHAR(60), IN `_description` TINYTEXT, IN `_location` VARCHAR(60), IN `_lat` DECIMAL(9,6), IN `_lon` DECIMAL(9,6), IN `_accessibility` ENUM('PUB','PRI','RSO'), IN `_user_id` INT(11), IN `_rso_id` INT(11), IN `_uni_id` INT(11))  BEGIN

  
  DECLARE _event_id INT(11);

  DECLARE _status ENUM('PND','ACT');

  DECLARE EXIT HANDLER FOR SQLEXCEPTION 

  BEGIN
    DECLARE _c_num INT;
    DECLARE _err_msg TEXT;
    ROLLBACK;
    GET DIAGNOSTICS _c_num = NUMBER;
    GET DIAGNOSTICS CONDITION _c_num _err_msg = MESSAGE_TEXT;
    SELECT _err_msg;
  END;

  START TRANSACTION;

    
    IF _name IS NULL OR _start_time IS NULL OR _end_time IS NULL OR _description IS NULL OR
    _location IS NULL OR _lat IS NULL OR _lon IS NULL OR _accessibility IS NULL THEN

      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "One or more field was missing from your event";

    END IF;

    
    IF EXISTS(

      SELECT e.event_id FROM events e 
      
      WHERE e.location = _location
      
      AND ((_end_time >= e.start_time AND _end_time <= e.end_time) OR
        
          (_start_time >= e.start_time AND _start_time <= e.end_time))) THEN

      
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "Your event is occuring at the same time and location as another event";

    END IF;

    
    
    INSERT INTO events (name, start_time, end_time, telephone, email, description, location, lat, lon, accessibility)
    VALUES (_name, _start_time, _end_time, _telephone, _email, _description, _location, _lat, _lon, _accessibility);

    
    SET _event_id = LAST_INSERT_ID();

    
    INSERT INTO hosting (uni_id, event_id)
    VALUES (_uni_id, _event_id);

    
    
    
    IF _rso_id > 0 THEN

      
      SET _status = 'ACT';

      IF NOT EXISTS (
        SELECT a.user_id
        FROM administrates a
        WHERE _rso_id = a.rso_id
        AND a.user_id = _user_id) THEN

        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "You must administrate the RSO selected";

      END IF;

      
      INSERT INTO r_created_e (rso_id, event_id)
      VALUES (_rso_id, _event_id);

    
    ELSE

      
      SET _status = 'PND';

    END IF;
        
    
    
    INSERT INTO u_created_e (user_id, event_id)
    VALUES (_user_id, _event_id);

    
    UPDATE events SET status = _status WHERE event_id = _event_id;

    
    IF _accessibility = 'RSO' THEN

      BEGIN
        
        IF NOT EXISTS (
          SELECT r.rso_id
          FROM rsos r 
          WHERE r.rso_id = _rso_id
          AND r.status = 'ACT') THEN

          
          SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "The rso specified cannot be found.";

        ELSE
          
          INSERT INTO rso_events (event_id, rso_id, uni_id)
          VALUES (_event_id, _rso_id, _uni_id);

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
          INSERT INTO public_events (event_id, uni_id)
          VALUES (_event_id, _uni_id);
      END;
    END IF;

    
    SELECT _event_id AS event_id FROM events LIMIT 1;

  COMMIT;

END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `create_rso` (IN `_role` ENUM('SA','ADM','STU'), IN `_user_id` INT(11), IN `_name` VARCHAR(60), IN `_description` TEXT(500), IN `_members` VARCHAR(300), IN `_rso_admin_id` INT(11), IN `_uni_id` INT(11))  BEGIN

  DECLARE _rso_id INT(11);

  DECLARE EXIT HANDLER FOR SQLEXCEPTION
  BEGIN
    DECLARE _c_num INT;
    DECLARE _err_msg TEXT;
    ROLLBACK;
    GET DIAGNOSTICS _c_num = NUMBER;
    GET DIAGNOSTICS CONDITION _c_num _err_msg = MESSAGE_TEXT;
    SELECT _err_msg;
  END;

  START TRANSACTION;

    
    CREATE TEMPORARY TABLE temp_members (user_id INT(11) UNSIGNED);
    INSERT INTO temp_members (user_id)
    SELECT user_id FROM users WHERE FIND_IN_SET(user_id, _members);

    IF _name IS NULL OR _description IS NULL THEN 

      DROP TABLE IF EXISTS temp_members;
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "One or more field was missing from your RSO.";

    END IF;

    
    IF _role = "STU" AND (SELECT COUNT(user_id) FROM temp_members) < 5 THEN
      DROP TABLE IF EXISTS temp_members;
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "You must specify at least 5 members";
    END IF;

    
    IF _role = "STU" AND NOT EXISTS (
      SELECT u.user_id FROM
      users u, attending a, universities n
      WHERE _user_id = u.user_id
      AND u.user_id = a.user_id 
      AND a.uni_id = n.uni_id
      AND n.uni_id = _uni_id) THEN
      DROP TABLE IF EXISTS temp_members;
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "You may only create an RSO at a university you attend";
    
    ELSEIF _role = "ADM" AND NOT EXISTS (
      SELECT u.user_id FROM
      users u, affiliated_with a, universities n
      WHERE _user_id = u.user_id
      AND u.user_id = a.user_id 
      AND a.uni_id = n.uni_id
      AND n.uni_id = _uni_id) THEN
      DROP TABLE IF EXISTS temp_members;
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "You may only create an RSO at a university you're affiliated with";

    END IF;

    
    INSERT INTO rsos (name, description, status)
    VALUES (_name, _description, 'ACT');

    
    SET _rso_id = LAST_INSERT_ID();

    
    INSERT INTO has (uni_id, rso_id)
    VALUES (_uni_id, _rso_id); 

    
    INSERT INTO administrates (user_id, rso_id)
    VALUES (_rso_admin_id, _rso_id);

    
    INSERT INTO is_member (user_id, rso_id) (SELECT user_id, _rso_id FROM temp_members);
    DROP TABLE temp_members;

    
    SELECT _rso_id AS rso_id FROM rsos LIMIT 1; 

  COMMIT;

END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `create_user` (IN `_user_name` VARCHAR(30), IN `_first_name` VARCHAR(60), IN `_last_name` VARCHAR(60), IN `_email` VARCHAR(60), IN `_role` ENUM("SA","ADM","STU"), IN `_hash` VARCHAR(60), IN `_uni_id` INT(11))  BEGIN

  DECLARE _user_id INT(11);

  DECLARE EXIT HANDLER FOR SQLEXCEPTION 
  BEGIN
    DECLARE _c_num INT;
    DECLARE _err_msg TEXT;
    ROLLBACK;
    GET DIAGNOSTICS _c_num = NUMBER;
    GET DIAGNOSTICS CONDITION _c_num _err_msg = MESSAGE_TEXT;
    SELECT _err_msg;
  END;

  START TRANSACTION;

    
    IF (_role = "STU" OR _role = "ADM") AND (check_uni_emails(_email, _uni_id) < 1) THEN
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'e-mail must match university selected.';
    ELSEIF _role = "STU" THEN

      
      IF _user_name IS NULL OR _first_name IS NULL OR _last_name IS NULL OR
      _email IS NULL OR _role IS NULL OR _hash IS NULL OR _uni_id IS NULL THEN

        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "One or more field was missing when creating your account";

      END IF;

      BEGIN
        INSERT INTO users (user_name, first_name, last_name, email, role, hash)
        VALUES (_user_name, _first_name, _last_name, _email, _role, _hash);

        SET _user_id = LAST_INSERT_ID();

        
        INSERT INTO attending (user_id, uni_id)
        VALUES (_user_id, _uni_id);
      END;
    
    ELSEIF _role = "ADM" THEN
      BEGIN
        
        IF _user_name IS NULL OR _first_name IS NULL OR _last_name IS NULL OR
        _email IS NULL OR _role IS NULL OR _hash IS NULL OR _uni_id IS NULL THEN

          SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "One or more field was missing when creating your account";

        END IF;

        INSERT INTO users (user_name, first_name, last_name, email, role, hash)
        VALUES (_user_name, _first_name, _last_name, _email, _role, _hash);

        SET _user_id = LAST_INSERT_ID();

        
        INSERT INTO affiliated_with (user_id, uni_id)
        VALUES (_user_id, _uni_id);
      END;
    
    ELSEIF _role = "SA"  THEN

        IF _user_name IS NULL OR _first_name IS NULL OR _last_name IS NULL OR
        _email IS NULL OR _role IS NULL OR _hash IS NULL THEN

          SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "One or more field was missing when creating your account";

        END IF;
      INSERT INTO users (user_name, first_name, last_name, email, role, hash)
      VALUES (_user_name, _first_name, _last_name, _email, _role, _hash);

      SET _user_id = LAST_INSERT_ID();

    END IF;

    
    SELECT _user_id AS user_id;

  COMMIT;

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
    FROM users u 
    WHERE _user_id = u.user_id
    LIMIT 1;
  END IF;

END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `join_rso` (IN `_user_id` INT(11), IN `_rso_id` INT(11), IN `_uni_id` INT(11))  BEGIN

  DECLARE EXIT HANDLER FOR SQLEXCEPTION 
  BEGIN
    DECLARE _c_num INT;
    DECLARE _err_msg TEXT;
    ROLLBACK;
    GET DIAGNOSTICS _c_num = NUMBER;
    GET DIAGNOSTICS CONDITION _c_num _err_msg = MESSAGE_TEXT;
    SELECT _err_msg;
  END;

  START TRANSACTION;

  
    IF NOT EXISTS (
      SELECT a.uni_id
      FROM attending a
      WHERE a.user_id = _user_id
      AND a.uni_id = _uni_id)
    AND NOT EXISTS (
      SELECT a.uni_id
      FROM affiliated_with a 
      WHERE a.user_id = _user_id
      AND a.uni_id = _uni_id) THEN

      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'You must belong to this Universities RSO.';

    
    ELSEIF EXISTS (
      SELECT m.user_id
      FROM is_member m
      WHERE _rso_id = m.rso_id
      AND m.user_id = _user_id) THEN

      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'You are already a member of this RSO';

    ELSE
      
      INSERT INTO is_member (user_id, rso_id)
      VALUES (_user_id, _rso_id);

      IF (SELECT COUNT(m.user_id) FROM is_member m WHERE m.rso_id = _rso_id) >= 5 THEN
        UPDATE rsos SET
        status = 'ACT'
        WHERE rso_id = _rso_id;
      END IF;

      SELECT _user_id AS user_id;

    END IF;


  COMMIT;

END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `leave_rso` (IN `_user_id` INT(11), IN `_rso_id` INT(11))  BEGIN

  DECLARE _rso_administrator INT(11);

  DECLARE EXIT HANDLER FOR SQLEXCEPTION 
  BEGIN
    DECLARE _c_num INT;
    DECLARE _err_msg TEXT;
    ROLLBACK;
    GET DIAGNOSTICS _c_num = NUMBER;
    GET DIAGNOSTICS CONDITION _c_num _err_msg = MESSAGE_TEXT;
    SELECT _err_msg;
  END;

  START TRANSACTION;

    SELECT user_id INTO _rso_administrator
    FROM administrates
    WHERE user_id = _user_id
    AND rso_id = _rso_id;

    
    IF _rso_administrator = _user_id THEN

      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "You must mark someone else as the administrator before leaving";

    END IF;

    DELETE FROM is_member 
    WHERE user_id = _user_id
    AND rso_id = _rso_id;

    
    IF (SELECT COUNT(m.user_id) FROM is_member m WHERE m.rso_id = _rso_id) < 5 THEN
      UPDATE rsos SET
      status = 'PND'
      WHERE rso_id = _rso_id;
    END IF;

    SELECT _user_id AS user_id;

  COMMIT;

END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `rate_event` (IN `_user_id` INT(11), IN `_event_id` INT(11), IN `_rating` INT(1))  BEGIN

  DECLARE EXIT HANDLER FOR SQLEXCEPTION 
  BEGIN
    DECLARE _c_num INT;
    DECLARE _err_msg TEXT;
    ROLLBACK;
    GET DIAGNOSTICS _c_num = NUMBER;
    GET DIAGNOSTICS CONDITION _c_num _err_msg = MESSAGE_TEXT;
    SELECT _err_msg;
  END;

  START TRANSACTION;

    
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

  COMMIT;

END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `search_events` (IN `_user_id` INT(11), IN `_uni_id` INT(11), IN `_scope` TINYTEXT, IN `_sort_by` TINYTEXT, IN `_accessibility` ENUM('PUB','PRI','RSO'), IN `_categories` VARCHAR(500))  BEGIN

  DECLARE _is_owner INT(1);
  DECLARE _is_participating INT(1);

  
  DECLARE _uni_lat DECIMAL(9, 6);
  DECLARE _uni_lon DECIMAL(9, 6);

  SELECT n.lat, n.lon
  INTO _uni_lat, _uni_lon
  FROM universities n
  WHERE n.uni_id = _uni_id;

  
  
  

  IF _scope = "other-uni" THEN
    SELECT DISTINCT e.event_id, e.name, e.start_time, e.end_time, e.description, e.location,
    e.lat, e.lon, e.accessibility, e.status, _is_owner AS is_owner,
    _is_participating AS is_participating, e.rating, h.uni_id AS uni_id, n.name AS uni_name,
    manhattan_distance(_uni_lat, _uni_lon, e.lat, e.lon) AS distance
    FROM public_events pe, hosting h, universities n, events e
    LEFT JOIN categorized_as c ON e.event_id = c.event_id
    WHERE _uni_id = n.uni_id
    AND n.uni_id = h.uni_id
    AND h.event_id = e.event_id
    
    AND e.accessibility = "PUB"
    AND (_categories = "" OR FIND_IN_SET(c.label, _categories))
    ORDER BY
      CASE _sort_by WHEN "date" THEN e.start_time
      WHEN "location" THEN distance 
      END;

  
  ELSEIF _accessibility = "PUB" THEN
    BEGIN
      SELECT DISTINCT e.event_id, e.name, e.start_time, e.end_time, e.description, e.location,
      e.lat, e.lon, e.accessibility, e.status, _is_owner AS is_owner,
      _is_participating AS is_participating, e.rating, h.uni_id AS uni_id,
      n.name AS uni_name, manhattan_distance(_uni_lat, _uni_lon, e.lat, e.lon) AS distance
      FROM  public_events pe, hosting h, universities n, events e
      LEFT JOIN categorized_as c ON e.event_id = c.event_id
      WHERE n.uni_id = _uni_id 
      AND n.uni_id = h.uni_id
      AND h.event_id = e.event_id
      AND e.event_id = pe.event_id
      
      
      AND (_categories = "" OR FIND_IN_SET(c.label, _categories))
      ORDER BY
        CASE _sort_by WHEN "date" THEN e.start_time 
        WHEN "location" THEN distance 
        END ASC; 
    END;

  
  ELSEIF _accessibility = "PRI" THEN
    BEGIN
      SELECT DISTINCT e.event_id, e.name, e.start_time, e.end_time, e.description, e.location,
      e.lat, e.lon, e.accessibility, e.status, _is_owner AS is_owner,
      _is_participating AS is_participating, e.rating, h.uni_id AS uni_id,
      n.name AS uni_name, manhattan_distance(_uni_lat, _uni_lon, e.lat, e.lon) AS distance
      FROM private_events p, attending at, affiliated_with aw, hosting h, universities n, events e
      LEFT JOIN categorized_as c ON e.event_id = c.event_id
      WHERE _uni_id = n.uni_id
      AND n.uni_id = h.uni_id
      AND h.uni_id = p.uni_id 
      AND p.event_id = e.event_id
      
      AND (_categories = "" OR FIND_IN_SET(c.label, _categories))
      ORDER BY
        CASE _sort_by WHEN "date" THEN e.start_time 
        WHEN "location" THEN distance 
        END ASC; 
    END;

  
  ELSEIF _accessibility = "RSO" THEN
    BEGIN
      SELECT DISTINCT e.event_id, e.name, e.start_time, e.end_time, e.description, e.location,
      e.lat, e.lon, e.accessibility, e.status, _is_owner AS is_owner,
      _is_participating AS is_participating, e.rating, h.uni_id AS uni_id,
      n.name AS uni_name, manhattan_distance(_uni_lat, _uni_lon, e.lat, e.lon) AS distance
      FROM rso_events re, hosting h, universities n, events e
      LEFT JOIN categorized_as c ON e.event_id = c.event_id
      WHERE e.event_id = re.event_id
      AND _uni_id = h.uni_id
      AND h.uni_id = n.uni_id
      
      AND re.rso_id = ANY(
        SELECT r.rso_id 
        FROM rsos r, is_member m
        WHERE _user_id = m.user_id AND m.rso_id = r.rso_id)
      AND (_categories = "" OR FIND_IN_SET(c.label, _categories))
      ORDER BY
        CASE _sort_by WHEN "date" THEN e.start_time 
        WHEN "location" THEN distance 
        END ASC; 
    END;

  END IF;

END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `select_students` ()  BEGIN
  SELECT u.user_id, u.user_name, u.email, u.first_name, u.last_name
  FROM users u
  WHERE u.role = "STU";
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `update_event` (IN `_event_id` INT(11), IN `_name` VARCHAR(60), IN `_start_time` TIMESTAMP, IN `_end_time` TIMESTAMP, IN `_telephone` VARCHAR(12), IN `_email` VARCHAR(60), IN `_description` TINYTEXT, IN `_location` VARCHAR(60), IN `_lat` DECIMAL(9,6), IN `_lon` DECIMAL(9,6), IN `_accessibility` ENUM('PUB','PRI','RSO'), IN `_user_id` INT(11), IN `_rso_id` INT(11), IN `_status` ENUM('ACT','PND'))  BEGIN

  
  
  DECLARE _old_type ENUM('PUB','PRI','RSO');
  
  
  DECLARE _current_rso_id INT(11);

  DECLARE _role ENUM('SA','ADM','STU');

  DECLARE _uni_id INT(11);

  DECLARE EXIT HANDLER FOR SQLEXCEPTION 
  BEGIN
    DECLARE _c_num INT;
    DECLARE _err_msg TEXT;
    ROLLBACK;
    GET DIAGNOSTICS _c_num = NUMBER;
    GET DIAGNOSTICS CONDITION _c_num _err_msg = MESSAGE_TEXT;
    SELECT _err_msg;
  END;

  START TRANSACTION;

    
    IF _name IS NULL OR _start_time IS NULL OR _end_time IS NULL OR _description IS NULL OR
    _location IS NULL OR _lat IS NULL OR _lon IS NULL OR _accessibility IS NULL THEN

      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "One or more field was missing from your event";

    END IF;

    
    IF EXISTS(

      SELECT e.event_id FROM events e 
      
      WHERE e.event_id <> _event_id AND e.location = _location
      
      AND ((_end_time >= e.start_time AND _end_time <= e.end_time) OR
        
          (_start_time >= e.start_time AND _start_time <= e.end_time))) THEN

      
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "Your event is occuring at the same time and location as another event";

    END IF;
  
    
    SELECT h.uni_id INTO _uni_id
    FROM hosting h
    WHERE h.event_id = _event_id;

    
    IF _role <> "SA" AND NOT EXISTS(
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

    
    SET _current_rso_id = (
      SELECT r.rso_id
      FROM rsos r, r_created_e re
      WHERE r.rso_id = re.rso_id
      AND re.event_id = _event_id);

    
    SET _role = (SELECT role FROM users WHERE user_id = _user_id);

    
    
    IF _current_rso_id > 0 AND _rso_id = 0 AND _role <> 'SA' THEN
      
      UPDATE events SET
      status = 'PND' 
      WHERE event_id = _event_id;

      DELETE FROM r_created_e
      WHERE event_id = _event_id;

      
      IF _accessibility = "RSO" THEN

        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "An event must be RSO created to have RSO only access";

      END IF;

    
    ELSEIF _current_rso_id IS NULL AND _rso_id > 0 THEN

      
      IF NOT EXISTS(
        SELECT r.rso_id
        FROM rsos
        WHERE r.rso_id = _rso_id
        AND r.status = 'ACT') THEN

        
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "The RSO specified cannot be found";

      END IF;

      
      INSERT INTO r_created_e (event_id, rso_id)
      VALUES (_event_id ,_rso_id);

      UPDATE events SET
      status = 'ACT' 
      WHERE event_id = _event_id;

    END IF;

    
    IF _role = "SA" THEN

      UPDATE events SET
      status = _status
      WHERE event_id = _event_id;
      
    END IF;
     
    
    
    UPDATE events SET
    name = _name,
    start_time = _start_time,
    end_time = _end_time,
    telephone = _telephone,
    email = _email,
    description = _description,
    location = _location,
    lat = _lat,
    lon = _lon

    WHERE events.event_id = _event_id;

    IF _old_type <> _accessibility THEN

      BEGIN

        
        CALL change_event_access(_event_id, _uni_id, _rso_id, _accessibility, _old_type);

      END;

    END IF;

    
    SELECT _event_id AS event_id FROM events LIMIT 1;

  COMMIT;

END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `update_rso` (IN `_rso_id` INT(11), IN `_role` ENUM('SA','ADM','STU'), IN `_user_id` INT(11), IN `_name` VARCHAR(60), IN `_description` TEXT(500), IN `_members` VARCHAR(300), IN `_rso_admin_id` INT(11), IN `_uni_id` INT(11))  BEGIN

  DECLARE EXIT HANDLER FOR SQLEXCEPTION 
  BEGIN
    DECLARE _c_num INT;
    DECLARE _err_msg TEXT;
    ROLLBACK;
    GET DIAGNOSTICS _c_num = NUMBER;
    GET DIAGNOSTICS CONDITION _c_num _err_msg = MESSAGE_TEXT;
    SELECT _err_msg;
  END;

  START TRANSACTION;

    IF _name IS NULL OR _description IS NULL THEN 

      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "One or more field was missing from your RSO.";

    END IF;

    
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

    
    CREATE TEMPORARY TABLE new_members (user_id INT(11) UNSIGNED);
    CREATE TEMPORARY TABLE gone_members (user_id INT(11) UNSIGNED);

    
    INSERT INTO new_members (user_id)
    SELECT DISTINCT u.user_id
    FROM users u, is_member m, attending a
    
    WHERE FIND_IN_SET(u.user_id, _members)
    AND u.user_id = a.user_id
    AND a.uni_id = _uni_id
    AND u.user_id <> ALL(SELECT m.user_id FROM is_member m WHERE m.rso_id = _rso_id);

    
    INSERT INTO gone_members (user_id)
    SELECT m.user_id
    FROM is_member m
    WHERE m.rso_id = _rso_id
    AND NOT FIND_IN_SET(user_id, _members);

    

    
    UPDATE rsos SET
    name = _name,
    description = _description
    WHERE rso_id = _rso_id; 

    
    UPDATE administrates SET 
    user_id = _rso_admin_id 
    WHERE rso_id = _rso_id;

    
    IF _role = "SA" OR _role = "ADM" THEN
      BEGIN
      
      END;
    END IF;

    
    INSERT INTO is_member (user_id, rso_id) (SELECT user_id, _rso_id FROM new_members);
    DROP TABLE new_members;

    
    DELETE FROM is_member WHERE user_id IN (SELECT user_id FROM gone_members);
    DROP TABLE gone_members;

    
    IF (
      SELECT COUNT(m.user_id) 
      FROM is_member m 
      WHERE m.rso_id = _rso_id) < 5 THEN
        UPDATE rsos SET
        status = 'PND'
        WHERE rso_id = _rso_id;

    
    ELSE
        UPDATE rsos SET
        status = 'ACT'
        WHERE rso_id = _rso_id;

    END IF;

    
    SELECT _rso_id AS rso_id FROM rsos LIMIT 1;

  COMMIT;

END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `update_user` (IN `_user_id` INT(11), IN `_user_name` VARCHAR(30), IN `_first_name` VARCHAR(60), IN `_last_name` VARCHAR(60), IN `_email` VARCHAR(60), IN `_role` ENUM("SA","ADM","STU"), IN `_uni_id` INT(11))  BEGIN

  DECLARE EXIT HANDLER FOR SQLEXCEPTION 
  BEGIN
    DECLARE _c_num INT;
    DECLARE _err_msg TEXT;
    ROLLBACK;
    GET DIAGNOSTICS _c_num = NUMBER;
    GET DIAGNOSTICS CONDITION _c_num _err_msg = MESSAGE_TEXT;
    SELECT _err_msg;
  END;

  START TRANSACTION;

    
    IF _user_name IS NULL OR _first_name IS NULL OR _last_name IS NULL OR
    _email IS NULL OR _role IS NULL OR _uni_id IS NULL THEN

      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "One or more field was missing when updating your account";

    END IF;
    
    IF _role = "STU" AND (check_uni_emails(_email, _uni_id) < 1) THEN
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'e-mail must match university selected.';
    END IF;

    UPDATE users SET
    user_name = _user_name,
    first_name = _first_name,
    last_name = _last_name,
    email = _email,
    role = _role
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

  COMMIT;

END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `view_event` (IN `_event_id` INT(11), IN `_user_id` INT(11), IN `_role` ENUM('SA','ADM','STU'), IN `_uni_id` INT(11))  BEGIN

  
  DECLARE _accessibility ENUM ('PUB','PRI','RSO');
  DECLARE _is_owner INT(1);
  DECLARE _is_participating INT(1);

  
  SET _accessibility = (SELECT accessibility FROM events WHERE event_id = _event_id);

  
  
  SET _is_owner = get_is_owner(_user_id, _event_id);

  SET _is_participating = get_is_participating(_user_id, _event_id);

  
  IF _role = "SA" OR _accessibility = "PUB" THEN
    BEGIN
      SELECT DISTINCT e.name, e.start_time, e.end_time, e.description, e.location,
      e.telephone, e.email, e.lat, e.lon, e.accessibility, e.status, _is_owner AS is_owner,
      _is_participating AS is_participating, e.rating, r.rso_id, r.name as rso_name,
      n.name AS uni_name, n.uni_id
      FROM public_events pe, universities n, events e
      LEFT OUTER JOIN r_created_e re ON re.event_id = e.event_id
      LEFT OUTER JOIN rsos r ON r.rso_id = re.rso_id
      WHERE e.event_id = _event_id
      AND e.event_id = pe.event_id
      AND pe.uni_id = n.uni_id
      LIMIT 1;
    END;

  
  ELSEIF _accessibility = "PRI" THEN
    BEGIN
      SELECT DISTINCT e.name, e.start_time, e.end_time, e.description, e.location,
      e.telephone, e.email, e.lat, e.lon, e.accessibility, e.status, _is_owner AS is_owner,
      _is_participating AS is_participating, e.rating, e.event_id, re.rso_id, r.name AS rso_name,
      n.name AS uni_name, n.uni_id
      FROM events e, universities n, private_events p
      LEFT OUTER JOIN r_created_e re ON re.event_id = p.event_id
      LEFT OUTER JOIN rsos r ON r.rso_id = re.rso_id
      WHERE e.event_id = _event_id
      AND e.event_id = p.event_id
      AND p.uni_id = n.uni_id
      AND n.uni_id = _uni_id LIMIT 1;
    END;

  
  ELSEIF _accessibility = "RSO" THEN
    BEGIN
      SELECT DISTINCT e.name, e.start_time, e.end_time, e.description, e.location,
      e.telephone, e.email, e.lat, e.lon, e.accessibility, e.status, _is_owner AS is_owner,
      _is_participating AS is_participating, e.rating, r.rso_id, r.name AS rso_name,
      n.name AS uni_name, n.uni_id
      FROM events e, universities n, rso_events re, rsos r
      WHERE e.event_id = _event_id
      AND re.event_id = e.event_id
      AND re.rso_id = r.rso_id
      AND re.rso_id = ANY(
        SELECT r.rso_id 
        FROM rsos r, is_member m
        WHERE _user_id = m.user_id AND m.rso_id = r.rso_id)
      AND re.uni_id = n.uni_id;
    END;

  END IF;

END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `view_rso` (IN `_rso_id` INT(11), IN `_user_id` INT(11), IN `_role` ENUM('SA','ADM','STU'))  BEGIN

  
  DECLARE _uni_id INT(11);

  

  
  IF _role = "STU" THEN
    SELECT a.uni_id INTO _uni_id
    FROM attending a
    WHERE a.user_id = _user_id;

  
  ELSEIF _role = "ADM" THEN
    SELECT a.uni_id INTO _uni_id
    FROM affiliated_with a
    WHERE a.user_id = _user_id;
  END IF;
  
  IF _role = "SA" THEN

    
    SELECT r.rso_id, r.name, r.description, r.status, a.user_id AS rso_administrator
    FROM rsos r, administrates a, has h
    WHERE r.rso_id = _rso_id
    AND r.rso_id = a.rso_id;

  
  
  ELSE
    SELECT r.rso_id, r.name, r.description, r.status, a.user_id AS rso_administrator
    FROM rsos r, administrates a, has h
    WHERE r.rso_id = _rso_id
    AND r.rso_id = a.rso_id
    AND h.rso_id = r.rso_id
    AND h.uni_id = _uni_id;
  END IF;

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
  DECLARE _role ENUM('SA','ADM','STU');

  SELECT u.role INTO _role FROM users u WHERE u.user_id = _user_id;

  
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

  ELSEIF _role = "SA" THEN

    
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

CREATE DEFINER=`root`@`localhost` FUNCTION `manhattan_distance` (`_lat1` DECIMAL(9,6), `_lon1` DECIMAL(9,6), `_lat2` DECIMAL(9,6), `_lon2` DECIMAL(9,6)) RETURNS DECIMAL(9,6) BEGIN
  RETURN (ABS(_lat1 - _lat2) + ABS(_lon1 - _lon2));
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

--
-- Dumping data for table `administrates`
--

INSERT INTO `administrates` (`user_id`, `rso_id`) VALUES
(2, 1),
(13, 2),
(11, 3),
(12, 4),
(31, 5);

-- --------------------------------------------------------

--
-- Table structure for table `affiliated_with`
--

CREATE TABLE `affiliated_with` (
  `user_id` int(11) UNSIGNED NOT NULL,
  `uni_id` int(11) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `affiliated_with`
--

INSERT INTO `affiliated_with` (`user_id`, `uni_id`) VALUES
(12, 4),
(13, 1),
(29, 6),
(35, 2);

-- --------------------------------------------------------

--
-- Table structure for table `attending`
--

CREATE TABLE `attending` (
  `user_id` int(11) UNSIGNED NOT NULL,
  `uni_id` int(11) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `attending`
--

INSERT INTO `attending` (`user_id`, `uni_id`) VALUES
(2, 1),
(3, 1),
(4, 1),
(5, 1),
(6, 1),
(7, 1),
(8, 6),
(9, 2),
(10, 3),
(11, 4),
(14, 4),
(15, 4),
(16, 4),
(17, 4),
(18, 4),
(19, 3),
(20, 3),
(21, 3),
(22, 3),
(23, 3),
(24, 6),
(25, 6),
(26, 6),
(27, 6),
(28, 6),
(30, 2),
(31, 2),
(32, 2),
(33, 2),
(34, 2);

-- --------------------------------------------------------

--
-- Table structure for table `categorized_as`
--

CREATE TABLE `categorized_as` (
  `event_id` int(11) UNSIGNED NOT NULL,
  `label` varchar(60) NOT NULL
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
  `telephone` varchar(12) NOT NULL,
  `email` varchar(60) NOT NULL,
  `description` tinytext NOT NULL,
  `location` varchar(60) NOT NULL,
  `lat` decimal(9,6) NOT NULL,
  `lon` decimal(9,6) NOT NULL,
  `accessibility` enum('PUB','PRI','RSO') NOT NULL DEFAULT 'PUB',
  `status` enum('PND','ACT') NOT NULL DEFAULT 'PND',
  `rating` decimal(9,2) NOT NULL DEFAULT '0.00'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `events`
--

INSERT INTO `events` (`event_id`, `name`, `start_time`, `end_time`, `telephone`, `email`, `description`, `location`, `lat`, `lon`, `accessibility`, `status`, `rating`) VALUES
(1, 'Pizza Party', '2017-07-11 15:00:00', '2017-07-11 17:00:00', '123-456-7890', 'billabog@knights.ucf.edu', 'Lets get pizza and play pong. Bring your favorite paddle and your appetite. No entry costs.', '1151 University Blvd, Orlando, FL 32817, USA', '28.598338', '-81.219712', 'PUB', 'PND', '0.00'),
(2, 'First Debate', '2017-07-11 15:00:00', '2017-07-11 17:00:00', '123-456-7890', 'billabog@knights.ucf.edu', 'Lets have our first debate. This will be on the topic of foreign policy. How should the United States handle its external affairs, ya know?', 'Health & Public Affairs II, Orlando, FL 32816, USA', '28.603014', '-81.198650', 'PUB', 'ACT', '0.00'),
(3, 'Second Debate', '2017-07-18 15:00:00', '2017-07-18 17:00:00', '123-456-7890', 'billabog@knights.ucf.edu', 'Lets have our second debate. This will be on the topic of ethics in science. Would it be a bad idea to clone humans, ya know?', 'Health & Public Affairs II, Orlando, FL 32816, USA', '28.603014', '-81.198650', 'PUB', 'ACT', '0.00'),
(4, 'Surprise Stakeout', '2017-07-11 16:00:00', '2017-07-11 18:00:00', '123-456-7890', 'sara@knights.ucf.edu', 'Today, we will be hiding by the shrubs of the student union. Bring your maximum stealth.', 'Apollo Cir, Orlando, FL 32816, USA', '28.605335', '-81.198346', 'RSO', 'ACT', '0.00'),
(5, 'First Doctor Meeting', '2017-07-11 15:00:00', '2017-07-11 17:00:00', '123-456-7890', 'ttarmach@mich.ppl.edu', 'Let''s meet at the chipotle''s near campus. Bring your club dues! We will discuss this years plans and how we can best prepare!', '235 S State St, Ann Arbor, MI 48104, USA', '42.279316', '-83.740442', 'PUB', 'ACT', '0.00'),
(6, 'Second Doctors Meeting', '2017-07-18 15:00:00', '2017-07-18 17:00:00', '123-456-7890', 'ttarmach@mich.ppl.edu', 'So we meet again!!! Bring a notebook!', '235 S State St, Ann Arbor, MI 48104, USA', '42.279316', '-83.740442', 'PRI', 'ACT', '0.00'),
(7, 'FSU Event 0', '2017-07-12 19:00:00', '2017-07-12 22:00:00', '123-456-7890', 'pmappleleaf@noles.flsu.edu', 'Sweet roll cupcake marzipan oat cake macaroon bear claw halvah soufflé macaroon. Gummi bears I love gingerbread. Biscuit I love I love cupcake topping. Icing macaroon bear claw caramels sesame snaps. Tart chupa chups gummi bears sesame snaps ice cream juj', '918 Way, Tallahassee, FL 32306, USA', '30.441878', '-84.298489', 'PUB', 'ACT', '0.00'),
(8, 'FSU Event 1', '2017-07-12 19:00:00', '2017-07-12 22:00:00', '123-456-7890', 'pmappleleaf@noles.flsu.edu', 'I love I love soufflé toffee pie I love soufflé. Cake danish brownie. Gummies jelly beans I love. Lemon drops carrot cake candy marzipan. Ice cream lemon drops chocolate bar jelly beans lemon drops jujubes. Dessert jelly caramels soufflé cookie sugar plum', '3300 Capital Cir SW, Tallahassee, FL 32310, USA', '30.395407', '-84.345052', 'PRI', 'ACT', '0.00'),
(9, 'FSU Event 2', '2017-07-19 19:00:00', '2017-07-19 22:00:00', '123-456-7890', 'pmappleleaf@noles.flsu.edu', 'Gummi bears I love carrot cake chocolate bar jujubes toffee jelly beans sesame snaps tart. Fruitcake fruitcake marshmallow. Soufflé jujubes wafer macaroon. Jelly beans ice cream caramels. Candy jujubes candy canes sugar plum jelly liquorice halvah I love.', 'Woodward Ave. Garage, Tallahassee, FL 32304, USA', '30.444573', '-84.298888', 'PUB', 'PND', '0.00'),
(10, 'FSU Event 3', '2017-07-19 23:00:00', '2017-07-20 00:00:00', '123-456-7890', 'pmappleleaf@noles.flsu.edu', 'Chocolate pudding macaroon tart caramels I love bonbon I love tiramisu. Brownie candy canes lemon drops I love pastry cake tiramisu jelly beans gummies. Bear claw cupcake fruitcake gummi bears. Powder biscuit jelly pastry gummies I love chocolate cake car', '3945 Museum Rd, Tallahassee, FL 32310, USA', '30.410471', '-84.344054', 'RSO', 'ACT', '0.00'),
(11, 'FSU Event 4', '2017-07-20 19:00:00', '2017-07-20 22:00:00', '123-456-7890', 'pmappleleaf@noles.flsu.edu', 'Sweet pie ice cream brownie. Jujubes brownie macaroon jelly beans brownie. Bonbon macaroon tiramisu I love dragée caramels fruitcake jelly-o. Gummies cookie danish jelly soufflé chupa chups. Cheesecake pudding oat cake toffee. Toffee danish macaroon marsh', '2225 N Monroe St, Tallahassee, FL 32303, USA', '30.470386', '-84.287964', 'PRI', 'PND', '0.00'),
(12, 'FSU Event 5', '2017-07-22 14:00:00', '2017-07-22 15:00:00', '123-456-7890', 'pmappleleaf@noles.flsu.edu', 'Candy cookie candy cheesecake jelly-o. Pastry jelly tiramisu sugar plum jelly bear claw candy candy canes dragée. Marzipan topping caramels croissant. Muffin oat cake lollipop croissant pudding I love I love I love. Ice cream gummi bears cookie topping ca', '600 W College Ave, Tallahassee, FL 32306, USA', '30.441878', '-84.298489', 'PUB', 'PND', '0.00');

-- --------------------------------------------------------

--
-- Table structure for table `has`
--

CREATE TABLE `has` (
  `uni_id` int(11) UNSIGNED NOT NULL,
  `rso_id` int(11) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `has`
--

INSERT INTO `has` (`uni_id`, `rso_id`) VALUES
(1, 1),
(1, 2),
(4, 3),
(4, 4),
(2, 5);

-- --------------------------------------------------------

--
-- Table structure for table `hosting`
--

CREATE TABLE `hosting` (
  `uni_id` int(11) UNSIGNED NOT NULL,
  `event_id` int(11) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `hosting`
--

INSERT INTO `hosting` (`uni_id`, `event_id`) VALUES
(1, 1),
(1, 2),
(1, 3),
(1, 4),
(4, 5),
(4, 6),
(2, 7),
(2, 8),
(2, 9),
(2, 10),
(2, 11),
(2, 12);

-- --------------------------------------------------------

--
-- Table structure for table `is_member`
--

CREATE TABLE `is_member` (
  `user_id` int(11) UNSIGNED NOT NULL,
  `rso_id` int(11) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `is_member`
--

INSERT INTO `is_member` (`user_id`, `rso_id`) VALUES
(2, 1),
(6, 1),
(3, 1),
(4, 1),
(5, 1),
(2, 2),
(6, 2),
(4, 2),
(5, 2),
(13, 2),
(11, 3),
(15, 3),
(14, 3),
(16, 3),
(12, 3),
(11, 4),
(12, 4),
(31, 5),
(34, 5),
(35, 5),
(32, 5),
(33, 5);

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
  `uni_id` int(11) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `private_events`
--

INSERT INTO `private_events` (`event_id`, `uni_id`) VALUES
(6, 4),
(8, 2),
(11, 2);

-- --------------------------------------------------------

--
-- Table structure for table `public_events`
--

CREATE TABLE `public_events` (
  `event_id` int(11) UNSIGNED NOT NULL,
  `uni_id` int(11) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `public_events`
--

INSERT INTO `public_events` (`event_id`, `uni_id`) VALUES
(1, 1),
(2, 1),
(3, 1),
(5, 4),
(7, 2),
(9, 2),
(12, 2);

-- --------------------------------------------------------

--
-- Table structure for table `rating`
--

CREATE TABLE `rating` (
  `event_id` int(11) UNSIGNED NOT NULL,
  `rating` tinyint(1) NOT NULL,
  `last_rated` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `user_id` int(11) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `rsos`
--

CREATE TABLE `rsos` (
  `rso_id` int(11) UNSIGNED NOT NULL,
  `name` varchar(60) NOT NULL,
  `description` text NOT NULL,
  `status` enum('PND','ACT') NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `rsos`
--

INSERT INTO `rsos` (`rso_id`, `name`, `description`, `status`) VALUES
(1, 'Surprise Society', 'So, our thing is to hide in innocuous places and startle passerby''s. This may seem like a harmless passtime, but is highly effective at unravling the fabric of civilization as we know it. We meet every third Wednesday of the month... when you''d least expect it.', 'ACT'),
(2, 'Debate Club', 'Do you like to fight people? That might be a problem. But theres good news! Now there''s a club where you can cultivate your contrarian attitudes. At debate club, you''ll learn how to assert your views and make friends at the same time. Travel abroad, enter contests, practice public speaking. Club dues are $5.00 and should be paid upon submitting a request to join', 'ACT'),
(3, 'Michigan Kids Unite', 'Are you in Michigan, and a kid? Look no further! Our club curates the coolest things to do. This club is free to join and plans trips twice a year. No one will be turned away! You are all invited. We love you.', 'ACT'),
(4, 'Mich Doctors club', 'Earning your doctorates? Passionate about your field? We are too. Let''s talk about what fascinates us and make some breakthroughs. Non-doctorate program students are welcome to sit in and participate', 'ACT'),
(5, 'FSU RSO', 'Tootsie roll jelly beans cheesecake cheesecake pie marshmallow sesame snaps ice cream. Muffin cake dessert I love I love sweet roll liquorice ice cream sugar plum. Marshmallow ice cream bear claw bear claw wafer tart lollipop jelly brownie. Marshmallow apple pie jelly beans. Candy canes dragée ice cream sesame snaps dessert jelly beans dessert apple pie liquorice. Danish cheesecake donut gingerbread. Cupcake jelly-o biscuit. Pastry dragée I love candy soufflé gummi bears tootsie roll I love chocolate cake.', 'ACT');

-- --------------------------------------------------------

--
-- Table structure for table `rso_events`
--

CREATE TABLE `rso_events` (
  `event_id` int(11) UNSIGNED NOT NULL,
  `rso_id` int(11) UNSIGNED NOT NULL,
  `uni_id` int(11) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `rso_events`
--

INSERT INTO `rso_events` (`event_id`, `rso_id`, `uni_id`) VALUES
(4, 1, 1),
(10, 5, 2);

-- --------------------------------------------------------

--
-- Table structure for table `r_created_e`
--

CREATE TABLE `r_created_e` (
  `rso_id` int(11) UNSIGNED NOT NULL,
  `event_id` int(11) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `r_created_e`
--

INSERT INTO `r_created_e` (`rso_id`, `event_id`) VALUES
(2, 2),
(2, 3),
(1, 4),
(4, 5),
(4, 6),
(5, 7),
(5, 8),
(5, 10);

-- --------------------------------------------------------

--
-- Table structure for table `universities`
--

CREATE TABLE `universities` (
  `uni_id` int(11) UNSIGNED NOT NULL,
  `name` varchar(60) NOT NULL,
  `description` text NOT NULL,
  `location` varchar(60) NOT NULL,
  `lat` decimal(9,6) NOT NULL,
  `lon` decimal(9,6) NOT NULL,
  `email_domain` varchar(30) NOT NULL,
  `website_url` varchar(60) NOT NULL,
  `population` int(10) UNSIGNED NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `universities`
--

INSERT INTO `universities` (`uni_id`, `name`, `description`, `location`, `lat`, `lon`, `email_domain`, `website_url`, `population`) VALUES
(1, 'University of Central Florida', 'With more than 64,000 students, UCF is one of the largest universities in the U.S. In addition to its impressive size and strength, UCF is ranked as a best-value university by The Princeton Review and Kiplinger''s, as well as one of the nation''s most affordable colleges by Forbes.', '4000 Central Florida Blvd, Orlando, FL 32816', '28.602427', '-81.200060', '@knights.ucf.edu', 'http://www.ucf.edu', 64318),
(2, 'Florida State University', 'Florida State University (commonly referred to as Florida State or FSU) is an American public space-grant and sea-grant research university. ... It is a senior member of the State University System of Florida. Founded in 1851, it is located on the oldest continuous site of higher education in the state of Florida.', '600 W College Ave, Tallahassee, FL 32306', '30.441878', '-84.298489', '@noles.fsu.edu', 'http://www.fsu.edu', 41867),
(3, 'University of Kansas', 'The University of Kansas, often referred to as KU or Kansas, is a public research university in the U.S. state of Kansas. The main campus in Lawrence, one of the largest college towns in Kansas, is on Mount Oread, the highest elevation in Lawrence.', '1450 Jayhawk Blvd, Lawrence, KS 66045', '38.954344', '-95.255796', '@kansas.ppl.edu', 'http://www.kans.edu', 28401),
(4, 'University of Michigan', 'From research to sports to music to medicine: the University of Michigan has a 200-year-old tradition of excellence. ', '500 S State St, Ann Arbor, MI 48109', '42.278044', '-83.738224', '@mich.ppl.edu', 'http://www.mich.edu', 44718),
(5, 'Cornell University', 'Cornell is a privately endowed research university and a partner of the State University of New York. As the federal land-grant institution in New York State, we have a responsibility—unique within the Ivy League—to make contributions in all fields of knowledge in a manner that prioritizes public engagement to help improve the quality of life in our state, the nation, the world.', 'Ithaca, NY 14850', '42.453449', '-76.473503', '@corn.ppl.edu', 'http://www.corn.edu', 21904),
(6, 'Yale University', 'Yale’s reach is both local and international. It partners with its hometown of New Haven, Connecticut to strengthen the city’s community and economy. And it engages with people and institutions across the globe in the quest to promote cultural understanding, improve the human condition, delve more deeply into the secrets of the universe, and train the next generation of world leaders.', 'New Haven, CT 06520', '41.316324', '-72.922343', '@yale.ppl.edu', 'http://www.yale.edu', 12385);

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
-- Dumping data for table `users`
--

INSERT INTO `users` (`user_id`, `first_name`, `last_name`, `user_name`, `email`, `role`, `hash`) VALUES
(1, 'jeff', 'straney', 'jstraney', 'jeff@gmail.com', 'SA', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(2, 'joe', 'bilabog', 'jbilabog', 'bilabog@knights.ucf.edu', 'STU', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(3, 'robert', 'doorstopper', 'rdoorstopper', 'doorstopper@knights.ucf.edu', 'STU', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(4, 'raymond', 'faust', 'rfaust', 'faust@knights.ucf.edu', 'STU', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(5, 'ross', 'olson', 'rolson', 'rolson@knights.ucf.edu', 'STU', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(6, 'rafael', 'caprese', 'rafaelthepainter', 'rafael@knights.ucf.edu', 'STU', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(7, 'rachel', 'primrose', 'primrose', 'rprimrose@knights.ucf.edu', 'STU', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(8, 'robert', 'sandcastle', 'rsandcastle', 'sandcastle@yale.ppl.edu', 'STU', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(9, 'josh', 'doorstopper', 'jdoorstopper', 'doorstopper@noles.fsu.edu', 'STU', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(10, 'jonas', 'borg', 'jonasborg', 'jborg@kansas.ppl.edu', 'STU', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(11, 'jenifer', 'bosch', 'jbosch', 'jbosch@mich.ppl.edu', 'STU', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(12, 'travis', 'tarmach', 'ttarmach', 'travis@mich.ppl.edu', 'ADM', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(13, 'sara', 'mackley', 'smackley', 'sara@knights.ucf.edu', 'ADM', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(14, 'jacob', 'marshall', 'jsmarshall', 'jmarshall@mich.ppl.edu', 'STU', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(15, 'james', 'march', 'jmarch', 'jmarch@mich.ppl.edu', 'STU', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(16, 'jake', 'tally', 'jtally', 'jtally@mich.ppl.edu', 'STU', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(17, 'joshua', 'porche', 'jporche', 'jporche@mich.ppl.edu', 'STU', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(18, 'jacob', 'bailey', 'jbailey', 'jbailey@mich.ppl.edu', 'STU', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(19, 'marc', 'reilly', 'mreilly', 'mreilly@kansas.ppl.edu', 'STU', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(20, 'maxine', 'tosh', 'mtosh', 'mtosh@kansas.ppl.edu', 'STU', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(21, 'marco', 'poland', 'mpoland', 'mpoland@kansas.ppl.edu', 'STU', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(22, 'marchall', 'slater', 'mslater', 'mslater@kansas.ppl.edu', 'STU', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(23, 'michelle', 'travertine', 'mtravertine', 'mtravertine@kansas.ppl.edu', 'STU', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(24, 'freddy', 'chris', 'fchris', 'fchris@yale.ppl.edu', 'STU', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(25, 'fabio', 'olson', 'folson', 'folson@yale.ppl.edu', 'STU', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(26, 'falcon', 'rembrant', 'frembrant', 'frembrant@yale.ppl.edu', 'STU', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(27, 'faust', 'leonardo', 'fleonardo', 'fleonardo@yale.ppl.edu', 'STU', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(28, 'fehey', 'flemming', 'fflemming', 'fflemming@yale.ppl.edu', 'STU', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(29, 'frank', 'lozenges', 'flozenges', 'flozenges@yale.ppl.edu', 'ADM', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(30, 'pierre', 'alton', 'palton', 'palton@noles.fsu.edu', 'STU', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(31, 'perry', 'mappleleaf', 'pmappleleaf', 'pmappleleaf@noles.fsu.edu', 'STU', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(32, 'picard', 'ramtha', 'pramtha', 'pramtha@noles.fsu.edu', 'STU', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(33, 'penny', 'solomon', 'psolomon', 'psolomon@noles.fsu.edu', 'STU', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(34, 'pete', 'marine', 'pmarine', 'pmarine@noles.fsu.edu', 'STU', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq'),
(35, 'perry', 'plymouth', 'pplymouth', 'pplymouth@noles.fsu.edu', 'ADM', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq');

-- --------------------------------------------------------

--
-- Table structure for table `u_created_e`
--

CREATE TABLE `u_created_e` (
  `user_id` int(11) UNSIGNED NOT NULL,
  `event_id` int(11) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `u_created_e`
--

INSERT INTO `u_created_e` (`user_id`, `event_id`) VALUES
(2, 1),
(13, 2),
(13, 3),
(2, 4),
(12, 5),
(12, 6),
(31, 7),
(31, 8),
(31, 9),
(31, 10),
(31, 11),
(31, 12);

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
-- Indexes for table `categorized_as`
--
ALTER TABLE `categorized_as`
  ADD KEY `event_id` (`event_id`),
  ADD KEY `label` (`label`) USING HASH;

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
-- Indexes for table `participating`
--
ALTER TABLE `participating`
  ADD KEY `user_id` (`user_id`),
  ADD KEY `event_id` (`event_id`);

--
-- Indexes for table `private_events`
--
ALTER TABLE `private_events`
  ADD KEY `event_id` (`event_id`),
  ADD KEY `uni_id` (`uni_id`);

--
-- Indexes for table `public_events`
--
ALTER TABLE `public_events`
  ADD KEY `event_id` (`event_id`),
  ADD KEY `uni_id` (`uni_id`);

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
-- Indexes for table `rso_events`
--
ALTER TABLE `rso_events`
  ADD KEY `event_id` (`event_id`),
  ADD KEY `rso_id` (`rso_id`),
  ADD KEY `uni_id` (`uni_id`);

--
-- Indexes for table `r_created_e`
--
ALTER TABLE `r_created_e`
  ADD KEY `rso_id` (`rso_id`),
  ADD KEY `event_id` (`event_id`);

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
  MODIFY `comment_id` int(11) UNSIGNED NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `events`
--
ALTER TABLE `events`
  MODIFY `event_id` int(11) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=13;
--
-- AUTO_INCREMENT for table `rsos`
--
ALTER TABLE `rsos`
  MODIFY `rso_id` int(11) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;
--
-- AUTO_INCREMENT for table `universities`
--
ALTER TABLE `universities`
  MODIFY `uni_id` int(11) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7;
--
-- AUTO_INCREMENT for table `users`
--
ALTER TABLE `users`
  MODIFY `user_id` int(11) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=36;
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
-- Constraints for table `affiliated_with`
--
ALTER TABLE `affiliated_with`
  ADD CONSTRAINT `affiliated_with_uni_id` FOREIGN KEY (`uni_id`) REFERENCES `universities` (`uni_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `affiliated_with_user_id` FOREIGN KEY (`user_id`) REFERENCES `users` (`user_id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `attending`
--
ALTER TABLE `attending`
  ADD CONSTRAINT `attending_uni_id` FOREIGN KEY (`uni_id`) REFERENCES `universities` (`uni_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `attending_user_id` FOREIGN KEY (`user_id`) REFERENCES `users` (`user_id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `categorized_as`
--
ALTER TABLE `categorized_as`
  ADD CONSTRAINT `categorized_as_event_id` FOREIGN KEY (`event_id`) REFERENCES `events` (`event_id`) ON DELETE CASCADE ON UPDATE CASCADE;

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
-- Constraints for table `participating`
--
ALTER TABLE `participating`
  ADD CONSTRAINT `participating_event_id` FOREIGN KEY (`event_id`) REFERENCES `events` (`event_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `participating_user_id` FOREIGN KEY (`user_id`) REFERENCES `users` (`user_id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `private_events`
--
ALTER TABLE `private_events`
  ADD CONSTRAINT `private_event_id` FOREIGN KEY (`event_id`) REFERENCES `events` (`event_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `private_event_uni_id` FOREIGN KEY (`uni_id`) REFERENCES `universities` (`uni_id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `public_events`
--
ALTER TABLE `public_events`
  ADD CONSTRAINT `public_event_id` FOREIGN KEY (`event_id`) REFERENCES `events` (`event_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `public_event_uni_id` FOREIGN KEY (`uni_id`) REFERENCES `universities` (`uni_id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `rating`
--
ALTER TABLE `rating`
  ADD CONSTRAINT `rating_event_id` FOREIGN KEY (`event_id`) REFERENCES `events` (`event_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `rating_user_id` FOREIGN KEY (`user_id`) REFERENCES `users` (`user_id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `rso_events`
--
ALTER TABLE `rso_events`
  ADD CONSTRAINT `rso_event_id` FOREIGN KEY (`event_id`) REFERENCES `events` (`event_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `rso_event_rso_id` FOREIGN KEY (`rso_id`) REFERENCES `rsos` (`rso_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `rso_event_uni_id` FOREIGN KEY (`uni_id`) REFERENCES `universities` (`uni_id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `r_created_e`
--
ALTER TABLE `r_created_e`
  ADD CONSTRAINT `r_created_e_event_id` FOREIGN KEY (`event_id`) REFERENCES `events` (`event_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `r_created_e_rso_id` FOREIGN KEY (`rso_id`) REFERENCES `rsos` (`rso_id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `u_created_e`
--
ALTER TABLE `u_created_e`
  ADD CONSTRAINT `u_created_e_event_id` FOREIGN KEY (`event_id`) REFERENCES `events` (`event_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `u_created_e_user_id` FOREIGN KEY (`user_id`) REFERENCES `users` (`user_id`) ON DELETE CASCADE ON UPDATE CASCADE;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;

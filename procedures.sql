-- Procedures are used to update or modify the database (implies side effects)
-- use functions simply to select or return an output.
-- a procedure should be used for transactions which require multiple inserts
-- or for transactions with complex logic

-- A couple of conventions that used in the file:
-- variables start with an underscore, this includes
-- variables passed to procedures, as well as locally defined variables
-- as you add procedures of your own, be sure to include a DROP IF EXISTS
-- statement at the top to allow effective creation of procedures in this file.
DROP FUNCTION IF EXISTS `event`.check_uni_emails;
DROP PROCEDURE IF EXISTS `event`.get_current_user;
DROP PROCEDURE IF EXISTS `event`.create_user;
DROP PROCEDURE IF EXISTS `event`.view_user;
DROP PROCEDURE IF EXISTS `event`.update_user;

DROP FUNCTION IF EXISTS `event`.get_is_owner;
DROP FUNCTION IF EXISTS `event`.get_is_participating;

DROP PROCEDURE IF EXISTS `event`.create_event;
DROP PROCEDURE IF EXISTS `event`.update_event;
DROP PROCEDURE IF EXISTS `event`.change_event_access;
DROP PROCEDURE IF EXISTS `event`.view_event;
DROP PROCEDURE IF EXISTS `event`.view_all_events;
DROP PROCEDURE IF EXISTS `event`.attend_event;
DROP PROCEDURE IF EXISTS `event`.unattend_event;
DROP PROCEDURE IF EXISTS `event`.comment_on_event;
DROP PROCEDURE IF EXISTS `event`.rate_event;
DROP PROCEDURE IF EXISTS `event`.search_events;

DROP FUNCTION IF EXISTS `event`.manhattan_distance;

DROP PROCEDURE IF EXISTS `event`.create_rso;
DROP PROCEDURE IF EXISTS `event`.update_rso;
DROP PROCEDURE IF EXISTS `event`.view_rso;
DROP PROCEDURE IF EXISTS `event`.join_rso;
DROP PROCEDURE IF EXISTS `event`.leave_rso;

-- use the applications database
USE `event`;

DELIMITER //


    

-- these are procedures attached to the database which are then callable by our PDO
-- procedures are being used for the following reasons 

-- 1. Mysql does not support the use of CREATE CONSTRAINT and CREATE CHECK. most sources indicated
-- that the work around was to use triggers, which sounds messy for some logic

-- 2. the application uses PDO prepared statements using named keys. Applying named keys to the
-- query string directly requires unique names for keys even if they are an identical value (
-- using a user id in the query twice, requires using two separate keys in the parameters).
-- using prepared statements on a procedural call allows us to bind the parameter only once.

CREATE FUNCTION check_uni_emails (
  _email varchar(60),
  _uni_id INT(11))
  RETURNS INT 
  RETURN (
  SELECT COUNT(n.uni_id) 
  FROM universities n
  WHERE _uni_id = n.uni_id AND _email LIKE CONCAT("%", n.email_domain))//

-- procedure inserts a new user, with conditions based on role
CREATE PROCEDURE create_user (
  IN _user_name varchar(30),
  IN _first_name varchar(60),
  IN _last_name varchar(60),
  IN _email varchar(60),
  IN _role enum("SA", "ADM", "STU"),
  IN _hash varchar(60),
  IN _uni_id INT(11))
BEGIN

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

    -- check for manditory fields
    IF _user_name IS NULL OR _first_name IS NULL OR _last_name IS NULL OR
    _email IS NULL OR _role IS NULL OR _hash IS NULL OR _uni_id IS NULL THEN

      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "One or more field was missing when creating your account";

    END IF;

    -- only perform insert if the email matches the universities email domain 
    IF (_role = "STU" OR _role = "ADM") AND (check_uni_emails(_email, _uni_id) < 1) THEN
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'e-mail must match university selected.';
    ELSEIF _role = "STU" THEN
      BEGIN
        INSERT INTO users (user_name, first_name, last_name, email, role, hash)
        VALUES (_user_name, _first_name, _last_name, _email, _role, _hash);

        SET _user_id = LAST_INSERT_ID();

        -- add relationship to user_attending_university table
        INSERT INTO attending (user_id, uni_id)
        VALUES (_user_id, _uni_id);
      END;
    -- admin is affiliated with a university
    ELSEIF _role = "ADM" THEN
      BEGIN
        INSERT INTO users (user_name, first_name, last_name, email, role, hash)
        VALUES (_user_name, _first_name, _last_name, _email, _role, _hash);

        SET _user_id = LAST_INSERT_ID();

        -- add affiliated_with relationship
        INSERT INTO affiliated_with (user_id, uni_id)
        VALUES (_user_id, _uni_id);
      END;
    -- super admin has no affiliation with university
    ELSEIF _role = "SA"  THEN
      INSERT INTO users (user_name, first_name, last_name, email, role, hash)
      VALUES (_user_name, _first_name, _last_name, _email, _role, _hash);

      SET _user_id = LAST_INSERT_ID();

    END IF;

    -- return the user id for redirect/login/picture creation purposes.
    SELECT _user_id AS user_id;

  COMMIT;

END//

CREATE PROCEDURE update_user (
  IN _user_id INT(11),
  IN _user_name varchar(30),
  IN _first_name varchar(60),
  IN _last_name varchar(60),
  IN _email varchar(60),
  IN _role enum("SA", "ADM", "STU"),
  IN _uni_id INT(11)) 
BEGIN

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

    -- check fields
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

END//

CREATE PROCEDURE view_user (
  IN _user_id INT(11)
  )
BEGIN

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
END//

CREATE PROCEDURE get_current_user (
  IN _user_id INT(11))
BEGIN
  
  DECLARE _role ENUM('SA','ADM','STU');

  SET _role = (SELECT u.role FROM users u WHERE u.user_id = _user_id LIMIT 1);

  -- both the student and admin have an associativity with a university
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

END//


-- calls one of three sub-procedures
-- (create_private_event, create_public_event, create_rso_event)
-- all events have certain things in common (name, start, end, category)
-- which get stored in the 'events' table. All additional fields are placed
-- into the inheriting table (pending status for non rso events)
CREATE PROCEDURE create_event (
  IN _name varchar(60),
  IN _start_time TIMESTAMP,
  IN _end_time TIMESTAMP,
  IN _telephone varchar(12),
  IN _email varchar(60),
  IN _description TINYTEXT,
  IN _location varchar(60),
  IN _lat DECIMAL(9, 6),
  IN _lon DECIMAL(9, 6),
  -- determines the ISA relationship
  IN _accessibility ENUM('PUB','PRI','RSO'),
  -- user creating event
  IN _user_id INT(11),
  -- id of rso if this is an RSO accessible event
  IN _rso_id INT(11),
  -- id of hosting university if this is a privately accessible event
  IN _uni_id INT(11))
BEGIN

  -- ID of new event once it's been inserted
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

    -- check fields
    IF _name IS NULL OR _start_time IS NULL OR _end_time IS NULL OR _description IS NULL OR
    _location IS NULL OR _lat IS NULL OR _lon IS NULL OR _accessibility IS NULL THEN

      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "One or more field was missing from your event";

    END IF;

    -- no event should be happening at the same time
    IF EXISTS(

      SELECT e.event_id FROM events e 
      -- location is the same.
      WHERE e.location = _location
      -- the end time is between the start and end time of this event 
      AND ((_end_time >= e.start_time AND _end_time <= e.end_time) OR
        -- or the start time is between the start and end time of this event.
          (_start_time >= e.start_time AND _start_time <= e.end_time))) THEN

      -- throw an error.
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "Your event is occuring at the same time and location as another event";

    END IF;

    -- all events have many of the the same columns. Lookup from the application layer is performed
    -- on the event sub-type tables, thus limiting the scope of what is looked up based on user id.
    INSERT INTO events (name, start_time, end_time, telephone, email, description, location, lat, lon, accessibility)
    VALUES (_name, _start_time, _end_time, _telephone, _email, _description, _location, _lat, _lon, _accessibility);

    -- get event id from last insert operation
    SET _event_id = LAST_INSERT_ID();

    -- create relationship between this event and the host university
    INSERT INTO hosting (uni_id, event_id)
    VALUES (_uni_id, _event_id);

    -- create the 'created' relationship. 
    -- if a a real rso id was used in creating the event. mark the event 
    -- as being made by an rso
    IF _rso_id > 0 THEN

      -- set status to active. No super admin approval needed
      SET _status = 'ACT';

      IF NOT EXISTS (
        SELECT a.user_id
        FROM administrates a
        WHERE _rso_id = a.rso_id
        AND a.user_id = _user_id) THEN

        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "You must administrate the RSO selected";

      END IF;

      -- create relationship between the event and the RSO 
      INSERT INTO r_created_e (rso_id, event_id)
      VALUES (_rso_id, _event_id);

    -- if there isn't an RSO, the status should be PND for pending, even for Admins
    ELSE

      -- status is pending. needs Super admin approval
      SET _status = 'PND';

    END IF;
        
    -- always create a relationship between the user and the event created.
    -- regardless of whether an RSO is used or not.
    INSERT INTO u_created_e (user_id, event_id)
    VALUES (_user_id, _event_id);

    -- regardless of whether it's RSO created or User created, update the status.
    UPDATE events SET status = _status WHERE event_id = _event_id;

    -- set accessibilty for viewing and commenting
    IF _accessibility = 'RSO' THEN

      BEGIN
        -- check if rso does not exist. notice that it must be active to be found.
        IF NOT EXISTS (
          SELECT r.rso_id
          FROM rsos r 
          WHERE r.rso_id = _rso_id
          AND r.status = 'ACT') THEN

          -- throw error
          SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "The rso specified cannot be found.";

        ELSE
          -- add to rso_events
          INSERT INTO rso_events (event_id, rso_id, uni_id)
          VALUES (_event_id, _rso_id, _uni_id);

        END IF;
      END;
    ELSEIF _accessibility = 'PRI' THEN
      BEGIN

        -- check if university does not exist
        IF NOT EXISTS (SELECT uni_id FROM universities WHERE uni_id = _uni_id) THEN
          -- throw error
          SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "The university specified cannot be found.";

        ELSE
          -- inser into private events.
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

    -- Select the id of the event just created. used for redirect
    SELECT _event_id AS event_id FROM events LIMIT 1;

  COMMIT;

END//

CREATE PROCEDURE update_event (
  IN _event_id INT(11),
  IN _name varchar(60),
  IN _start_time TIMESTAMP,
  IN _end_time TIMESTAMP,
  IN _telephone varchar(12),
  IN _email varchar(60),
  IN _description TINYTEXT,
  IN _location varchar(60),
  IN _lat DECIMAL(9, 6),
  IN _lon DECIMAL(9, 6),
  -- determines the ISA relationship
  IN _accessibility ENUM('PUB','PRI','RSO'),
  -- id of user. checks if is the administrator of the RSO 
  IN _user_id INT(11),
  -- id of rso if this is an RSO accessible event
  IN _rso_id INT(11),
  -- status. only submitable by SA roles
  IN _status ENUM('ACT','PND'))

BEGIN

  -- now the tricky part. If someone wants to change the event type, we need to see if
  -- the type has changed
  DECLARE _old_type ENUM('PUB','PRI','RSO');
  -- id of the current RSO. If this is set from an existing RSO id, to 0 (no rso)
  -- then the event status must be set to Pending Super Admin approval.
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

    -- check fields
    IF _name IS NULL OR _start_time IS NULL OR _end_time IS NULL OR _description IS NULL OR
    _location IS NULL OR _lat IS NULL OR _lon IS NULL OR _accessibility IS NULL THEN

      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "One or more field was missing from your event";

    END IF;

    -- no event should be happening at the same time
    IF EXISTS(

      SELECT e.event_id FROM events e 
      -- location is the same.
      WHERE e.location = _location
      -- the end time is between the start and end time of this event 
      AND ((_end_time >= e.start_time AND _end_time <= e.end_time) OR
        -- or the start time is between the start and end time of this event.
          (_start_time >= e.start_time AND _start_time <= e.end_time))) THEN

      -- throw an error.
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "Your event is occuring at the same time and location as another event";

    END IF;
  
    -- the uni ID should be the id of the hosting university.
    SELECT h.uni_id INTO _uni_id
    FROM hosting h
    WHERE h.event_id = _event_id;

    -- check if the user is SuperAdmin or the administrator of the RSO that made the event.
    IF _role <> "SA" AND NOT EXISTS(
      SELECT e.event_id
      FROM events e, r_created_e c, rsos r, administrates a
      WHERE _user_id = a.user_id
      AND a.rso_id = r.rso_id
      AND r.rso_id = c.rso_id
      AND c.event_id = _event_id) AND 
    -- or if the user is the creator of the event
    NOT EXISTS (
      SELECT e.event_id 
      FROM events e, u_created_e u 
      WHERE e.event_id = _event_id
      AND e.event_id = u.event_id
      AND u.user_id = _user_id) THEN

      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "You are not authorized to change this event.";

    END IF;

    -- set the events old type of pub, pri, or rso
    SET _old_type = (SELECT e.accessibility FROM events e WHERE e.event_id = _event_id LIMIT 1);

    -- set the old rso id
    SET _current_rso_id = (
      SELECT r.rso_id
      FROM rsos r, r_created_e re
      WHERE r.rso_id = re.rso_id
      AND re.event_id = _event_id);

    -- set the role of the current user.
    SET _role = (SELECT role FROM users WHERE user_id = _user_id);

    -- check if we are upating an RSO created event to have No RSO.
    -- note that this condition fails when it is already PND and SA makes adjustment.
    IF _current_rso_id > 0 AND _rso_id = 0 AND _role <> 'SA' THEN
      -- set this event to pending for super admin.
      UPDATE events SET
      status = 'PND' 
      WHERE event_id = _event_id;

      DELETE FROM r_created_e
      WHERE event_id = _event_id;

      -- since the rso has been removed, do not allow this event to have rso only access
      IF _accessibility = "RSO" THEN

        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "An event must be RSO created to have RSO only access";

      END IF;

    -- check if the event used to have no RSO, but has just added one.
    ELSEIF _current_rso_id IS NULL AND _rso_id > 0 THEN

      -- If the rso is not active
      IF NOT EXISTS(
        SELECT r.rso_id
        FROM rsos
        WHERE r.rso_id = _rso_id
        AND r.status = 'ACT') THEN

        -- throw an error.
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "The RSO specified cannot be found";

      END IF;

      -- indicated that this is an RSO created event. Allow joins when viewing events.
      INSERT INTO r_created_e (event_id, rso_id)
      VALUES (_event_id ,_rso_id);

      UPDATE events SET
      status = 'ACT' 
      WHERE event_id = _event_id;

    END IF;

    -- only super admin can directly allow the status to change.
    IF _role = "SA" THEN

      UPDATE events SET
      status = _status
      WHERE event_id = _event_id;
      
    END IF;
     
    -- all events have many of the the same columns. Lookup from the application layer is performed
    -- on the event sub-type tables, thus limiting the scope of what is looked up based on user id.
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

        -- call the change_event_access procedure
        CALL change_event_access(_event_id, _uni_id, _rso_id, _accessibility, _old_type);

      END;

    END IF;

    -- Select the id of the event just created. used for redirect
    SELECT _event_id AS event_id FROM events LIMIT 1;

  COMMIT;

END//

CREATE PROCEDURE attend_event (
  -- event to attend
  IN _event_id INT(11),
  -- users id
  IN _user_id INT(11),
  -- university id associated with user
  IN _uni_id INT(11)
  )
BEGIN

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

    -- check if user is already participating.
    IF EXISTS (
      SELECT p.user_id
      FROM participating p, events e
      WHERE _user_id = p.user_id
      AND p.event_id = _event_id) THEN

      SET _not_participating = 0;

    ELSE

      SET _not_participating = 1;

    END IF;

    -- if public do a simple attend
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

END//

CREATE PROCEDURE comment_on_event (
  IN _content TINYTEXT,
  IN _event_id INT(11),
  IN _user_id INT(11),
  IN _uni_id INT(11)
  )
BEGIN

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

    -- to comment on the event, the event must be public 
    IF _accessibility = "PUB" OR
      -- or private and the user attends the university
      ( _accessibility = "PRI" AND EXISTS(
      SELECT a.user_id
      FROM attending a, private_events pe
      WHERE _user_id = a.user_id
      AND a.uni_id = pe.uni_id 
      AND pe.event_id = _event_id)) OR
      -- or an rso event and the user is a member of the rso
      ( _accessibility = "RSO" AND EXISTS(
      SELECT m.user_id
      FROM is_member m, rso_events re 
      WHERE _user_id = m.user_id
      AND m.rso_id = re.rso_id
      AND re.event_id = _event_id)) THEN

      -- insert the comment
      INSERT INTO commented_on (date_posted, content, user_id, event_id)
      VALUES (NOW(), _content, _user_id, _event_id);

      SELECT comment_id FROM commented_on WHERE comment_id = LAST_INSERT_ID(); 

    END IF;

  COMMIT;

END//

CREATE PROCEDURE rate_event (
  IN _user_id INT(11),
  IN _event_id INT(11),
  IN _rating INT(1)
  )
BEGIN

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

    -- if  rating exists, update it. otherwise, insert a new one.
    IF NOT EXISTS(
      SELECT r.user_id
      FROM rating r
      WHERE r.user_id = _user_id
      AND r.event_id = _event_id) THEN
      -- insert new rating for unique user rating event pair.
      INSERT INTO rating (user_id, event_id, rating)
      VALUES (_user_id, _event_id, _rating);
    ELSE
      -- update the record with matching user id and event id.
      UPDATE rating SET
      rating = _rating
      WHERE user_id = _user_id
      AND event_id = _event_id;
    END IF;

    -- update the events average rating
    UPDATE events SET
    rating = (
      SELECT AVG(r.rating)
      FROM rating r
      WHERE r.event_id = _event_id)
    WHERE event_id = _event_id;

    -- bring back the users rating and the events rating to update the page
    SELECT r.rating, e.rating AS total_rating
    FROM rating r, events e
    WHERE r.user_id = _user_id
    AND r.event_id = _event_id
    AND e.event_id = _event_id;

  COMMIT;

END//

CREATE PROCEDURE change_event_access (
  IN _event_id INT(11),
  -- university id for private events
  IN _uni_id INT(11),
  -- rso id for rso events
  IN _rso_id INT(11),
  IN _accessibility ENUM('PUB','PRI','RSO'),
  IN _old_type ENUM('PUB','PRI','RSO'))
BEGIN

  -- roll back if there is a user defined error
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
  -- since the access of the event is being changed, we know that the event
  -- is getting removed from one of the child tables. 

  -- in all three tables, event_id is a foreign key to events primary key.
  -- we are guaranteed to remove only the event being updated.
  IF _old_type = 'PUB' THEN 
    DELETE FROM public_events WHERE event_id = _event_id;

  ELSEIF _old_type = 'PRI' THEN 
    DELETE FROM private_events WHERE event_id = _event_id;

  ELSEIF _old_type = 'RSO' THEN 
    DELETE FROM rso_events WHERE event_id = _event_id;
    DELETE FROM r_created_e WHERE event_id = _event_id;

  END IF;

  -- now perform an insert on the proper table
  IF _accessibility = 'RSO' THEN

    BEGIN
      -- check if rso does not exist 

      IF NOT EXISTS (SELECT rso_id FROM rsos WHERE rso_id = _rso_id) THEN
        -- throw error
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "The rso specified cannot be found.";

      ELSE
        INSERT INTO rso_events (event_id, rso_id, uni_id)
        VALUES (_event_id, _rso_id, _uni_id);
        -- erase any public or private event that has the same event id
      END IF;
    END;
  ELSEIF _accessibility = 'PRI' THEN
    BEGIN
      -- check if university does not exist
      IF NOT EXISTS (SELECT uni_id FROM universities WHERE uni_id = _uni_id) THEN
        -- throw error
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

  -- always update the 'accessibility' column on 'events'
  UPDATE events SET
  accessibility = _accessibility
  WHERE event_id = _event_id;

END//

-- function to check if a user owns an event (created it as user, or admin of RSO)
CREATE FUNCTION get_is_owner (
  _user_id INT(11),
  _event_id INT(11)
  )
  RETURNS INT(1)
BEGIN

  DECLARE _is_owner INT(1);
  DECLARE _role ENUM('SA','ADM','STU');

  SELECT u.role INTO _role FROM users u WHERE u.user_id = _user_id;

  -- check if user created the event
  IF EXISTS(
    SELECT u.user_id
    FROM u_created_e u, events e
    WHERE u.user_id = _user_id
    AND u.event_id = _event_id) THEN
    -- this user is the owner
    SET _is_owner = 1;

  -- else if the user is adminstrator to the RSO that created the event.
  ELSEIF EXISTS(
    SELECT a.user_id
    FROM administrates a, rsos r, r_created_e re
    WHERE a.user_id = _user_id
    AND a.rso_id = r.rso_id
    AND r.rso_id = re.rso_id
    AND re.event_id = _event_id) THEN

    -- this user is the owner
    SET _is_owner = 1;

  ELSEIF _role = "SA" THEN

    -- Super Admin can modify any event to mark it 'Pending'. Can also Delete them. 
    SET _is_owner = 1;


  ELSE

    -- this user is not the owner 
    SET _is_owner = 0;

  END IF;

  RETURN _is_owner;

END//
  
CREATE FUNCTION get_is_participating (
  _user_id INT(11),
  _event_id INT(11)
  )
  RETURNS INT(1)
BEGIN

  DECLARE _is_participating INT(1);

  -- check if the current user is attending the event.
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

END//

CREATE PROCEDURE view_event (
  IN _event_id INT(11),
  -- user attempting access, as well as role and their uni_id
  IN _user_id INT(11),
  IN _role ENUM ('SA', 'ADM', 'STU'),
  IN _uni_id INT(11)
  )
BEGIN

  -- events accessibility
  DECLARE _accessibility ENUM ('PUB','PRI','RSO');
  DECLARE _is_owner INT(1);
  DECLARE _is_participating INT(1);

  -- get the accessibility from event
  SET _accessibility = (SELECT accessibility FROM events WHERE event_id = _event_id);

  -- see if the user attempting access is the owner, or if they are admin to the RSO that
  -- made the event
  SET _is_owner = get_is_owner(_user_id, _event_id);

  SET _is_participating = get_is_participating(_user_id, _event_id);

  -- no check necessary for public events
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

  -- return event only if user attends/affiliates-with hosting university 
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

  -- return event only if user is_member of RSO. implicitly made by an RSO, so no join is necessary
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

END//

-- uses the manhattan heuristic to obtain a rough estimate of distance
-- from point a to point b. adds abs y distance to abs x distance
-- plus side is it's fast to calculate, downside is it's not really accurate
CREATE FUNCTION manhattan_distance (
  _lat1 DECIMAL(9, 6),
  _lon1 DECIMAL(9, 6),
  _lat2 DECIMAL(9, 6),
  _lon2 DECIMAL(9, 6)
  )
  RETURNS DECIMAL(9, 6) 
BEGIN
  RETURN (ABS(_lat1 - _lat2) + ABS(_lon1 - _lon2));
END//

CREATE PROCEDURE search_events (
  IN _user_id INT(11),
  IN _uni_id INT(11),
  IN _scope TINYTEXT,
  IN _sort_by TINYTEXT, 
  IN _accessibility ENUM ('PUB','PRI','RSO') ,
  IN _categories varchar(500) 
  )
BEGIN

  DECLARE _is_owner INT(1);
  DECLARE _is_participating INT(1);

  -- user latitude and longitude. set by university location.
  DECLARE _uni_lat DECIMAL(9, 6);
  DECLARE _uni_lon DECIMAL(9, 6);

  SELECT n.lat, n.lon
  INTO _uni_lat, _uni_lon
  FROM universities n
  WHERE n.uni_id = _uni_id;

  -- to calculate distance, get the coordinates of university and use manhattan
  -- heuristic between event points. (ABS(x1 - x2) + ABS(y1 - y1))
  -- if the scope is for another university, then the events viewed can only be public

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
    -- a student can only see public events at another university
    AND e.accessibility = "PUB"
    AND (_categories = "" OR FIND_IN_SET(c.label, _categories))
    ORDER BY
      CASE _sort_by WHEN "date" THEN e.start_time
      WHEN "location" THEN distance 
      END;

  -- if the scope is "my-uni" (the only other scope) Allow a range of event types to be selected
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
      -- find category in the categories search string
      -- or return full set if string is blank
      AND (_categories = "" OR FIND_IN_SET(c.label, _categories))
      ORDER BY
        CASE _sort_by WHEN "date" THEN e.start_time 
        WHEN "location" THEN distance 
        END ASC; 
    END;

  -- return event only if user attends/affiliates-with hosting university 
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
      -- if no categories provided, return all events in the scope
      AND (_categories = "" OR FIND_IN_SET(c.label, _categories))
      ORDER BY
        CASE _sort_by WHEN "date" THEN e.start_time 
        WHEN "location" THEN distance 
        END ASC; 
    END;

  -- return event only if user is_member of RSO
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
      -- make sure the events are in the users RSO's
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

END//

CREATE PROCEDURE create_rso (
  -- role and user id of user making rso
  IN _role ENUM ('SA','ADM','STU'),
  IN _user_id INT(11),
  -- name and description of rso
  IN _name varchar(60),
  IN _description TEXT(500),
  -- comma separated list of members' ids
  -- sadly, this was the best approach I could concieve
  -- using PDO's and stored procedures
  IN _members varchar(300),
  -- id of user chosen to administrate the rso
  -- will be the user with admin role if making the rso
  -- or a student chosen by another student.
  IN _rso_admin_id INT(11),
  -- id of the university to which the rso belongs
  IN _uni_id INT(11))

BEGIN

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

    -- create a temporary table for members using the comma separated list
    CREATE TEMPORARY TABLE temp_members (user_id INT(11) UNSIGNED);
    INSERT INTO temp_members (user_id)
    SELECT user_id FROM users WHERE FIND_IN_SET(user_id, _members);

    IF _name IS NULL OR _description IS NULL THEN 

      DROP TABLE IF EXISTS temp_members;
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "One or more field was missing from your RSO.";

    END IF;

    -- check if the members list has 5 records (or more). waive for SA and ADM
    IF _role = "STU" AND (SELECT COUNT(user_id) FROM temp_members) < 5 THEN
      DROP TABLE IF EXISTS temp_members;
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "You must specify at least 5 members";
    END IF;

    -- check if the RSO is posted to the univserity currently attended by the user
    IF _role = "STU" AND NOT EXISTS (
      SELECT u.user_id FROM
      users u, attending a, universities n
      WHERE _user_id = u.user_id
      AND u.user_id = a.user_id 
      AND a.uni_id = n.uni_id
      AND n.uni_id = _uni_id) THEN
      DROP TABLE IF EXISTS temp_members;
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "You may only create an RSO at a university you attend";
    -- similarly, an admin should not be able to make RSO's for other universities.
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

    -- insert the rso into the base table. 5 members are confirmed, as an error is thrown above.
    INSERT INTO rsos (name, description, status)
    VALUES (_name, _description, 'ACT');

    -- get the id from the rso just inserted
    SET _rso_id = LAST_INSERT_ID();

    -- now set the has relationship between the university and the rso
    INSERT INTO has (uni_id, rso_id)
    VALUES (_uni_id, _rso_id); 

    -- now set the has administrates rel. between the user and the rso
    INSERT INTO administrates (user_id, rso_id)
    VALUES (_rso_admin_id, _rso_id);

    -- insert any members specified
    INSERT INTO is_member (user_id, rso_id) (SELECT user_id, _rso_id FROM temp_members);
    DROP TABLE temp_members;

    -- return the id of RSO created for redirect purposes
    SELECT _rso_id AS rso_id FROM rsos LIMIT 1; 

  COMMIT;

END//

CREATE PROCEDURE update_rso (
  -- id of rso to update
  IN _rso_id INT(11),
  -- role and user id of user making rso
  IN _role ENUM ('SA','ADM','STU'),
  IN _user_id INT(11),
  -- name and description of rso
  IN _name varchar(60),
  IN _description TEXT(500),
  -- comma separated list of members' ids
  -- sadly, this was the best approach I could concieve
  -- using PDO's and stored procedures
  IN _members varchar(300),
  -- id of user chosen to administrate the rso
  -- will be the user with admin role if making the rso
  -- or a student chosen by another student.
  IN _rso_admin_id INT(11),

  -- id of the university to which the rso belongs
  IN _uni_id INT(11))

BEGIN

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

    -- check if the user is the rso_administrator 
    IF _user_id <> (SELECT a.user_id FROM administrates a WHERE a.rso_id = _rso_id) THEN
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "You are not athorized to change this RSO";
    END IF; 

    -- check if user is student the the rso's host university
    IF _role = "STU" AND NOT EXISTS (
      SELECT u.user_id FROM
      users u, attending a, universities n
      WHERE _user_id = u.user_id
      AND u.user_id = a.user_id 
      AND a.uni_id = n.uni_id
      AND n.uni_id = _uni_id) THEN
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "You may only create an RSO at a university you attend";

    -- or check if the admin user is affiliated with the university
    ELSEIF _role = "ADM" AND NOT EXISTS (
      SELECT u.user_id FROM
      users u, affiliated_with a, universities n
      WHERE _user_id = u.user_id
      AND u.user_id = a.user_id 
      AND a.uni_id = n.uni_id
      AND n.uni_id = _uni_id) THEN
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "You may only create an RSO at a university you're affiliated with";
    END IF;

    -- create a temporary table for members using the comma separated list
    CREATE TEMPORARY TABLE new_members (user_id INT(11) UNSIGNED);
    CREATE TEMPORARY TABLE gone_members (user_id INT(11) UNSIGNED);

    -- members to be added
    INSERT INTO new_members (user_id)
    SELECT DISTINCT u.user_id
    FROM users u, is_member m, attending a
    -- user must not be a member
    WHERE FIND_IN_SET(u.user_id, _members)
    AND u.user_id = a.user_id
    AND a.uni_id = _uni_id
    AND u.user_id <> ALL(SELECT m.user_id FROM is_member m WHERE m.rso_id = _rso_id);

    -- members to be deleted
    INSERT INTO gone_members (user_id)
    SELECT m.user_id
    FROM is_member m
    WHERE m.rso_id = _rso_id
    AND NOT FIND_IN_SET(user_id, _members);

    -- otherwise, the member is still a member. do nothing.

    -- insert the rso into the base table
    UPDATE rsos SET
    name = _name,
    description = _description
    WHERE rso_id = _rso_id; 

    -- change the administrator
    UPDATE administrates SET 
    user_id = _rso_admin_id 
    WHERE rso_id = _rso_id;

    -- if user is super admin or admin, allow changing the 'status' of the RSO
    IF _role = "SA" OR _role = "ADM" THEN
      BEGIN
      -- set rso status to 'ACT' for active or 'PND' for pending
      END;
    END IF;

    -- insert any new members specified
    INSERT INTO is_member (user_id, rso_id) (SELECT user_id, _rso_id FROM new_members);
    DROP TABLE new_members;

    -- delete members that are gone.
    DELETE FROM is_member WHERE user_id IN (SELECT user_id FROM gone_members);
    DROP TABLE gone_members;

    -- if there are fewer than 5 members, make the RSO inactive 
    IF (
      SELECT COUNT(m.user_id) 
      FROM is_member m 
      WHERE m.rso_id = _rso_id) < 5 THEN
        UPDATE rsos SET
        status = 'PND'
        WHERE rso_id = _rso_id;

    -- make the rso active
    ELSE
        UPDATE rsos SET
        status = 'ACT'
        WHERE rso_id = _rso_id;

    END IF;

    -- return the id of RSO created for redirect purposes
    SELECT _rso_id AS rso_id FROM rsos LIMIT 1;

  COMMIT;

END//

CREATE PROCEDURE view_rso (
  IN _rso_id INT(11),
  IN _user_id INT(11),
  IN _role ENUM('SA','ADM','STU')
  )
BEGIN

  -- id of attending/affiliated with university
  DECLARE _uni_id INT(11);

  -- check the role

  -- if user is student, make sure they attend the university
  IF _role = "STU" THEN
    SELECT a.uni_id INTO _uni_id
    FROM attending a
    WHERE a.user_id = _user_id;

  -- if user is administrator, make sure they attend the university
  ELSEIF _role = "ADM" THEN
    SELECT a.uni_id INTO _uni_id
    FROM affiliated_with a
    WHERE a.user_id = _user_id;
  END IF;
  -- now check if super admin. if super admin, just view the RSO
  IF _role = "SA" THEN

    -- select the university. Do not bother to check affiliation
    SELECT r.rso_id, r.name, r.description, r.status, a.user_id AS rso_administrator
    FROM rsos r, administrates a, has h
    WHERE r.rso_id = _rso_id
    AND r.rso_id = a.rso_id;

  -- if not SA (stu, or adm), apply the condition that the user must
  -- attend/affiliate with the University
  ELSE
    SELECT r.rso_id, r.name, r.description, r.status, a.user_id AS rso_administrator
    FROM rsos r, administrates a, has h
    WHERE r.rso_id = _rso_id
    AND r.rso_id = a.rso_id
    AND h.rso_id = r.rso_id
    AND h.uni_id = _uni_id;
  END IF;

END//

-- procedure that allows a user to join the RSO
CREATE PROCEDURE join_rso (
  IN _user_id INT(11),
  IN _rso_id INT(11),
  IN _uni_id INT(11)
)
BEGIN

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

  -- check if user is member of the university
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

    -- also check if this user is a member
    ELSEIF EXISTS (
      SELECT m.user_id
      FROM is_member m
      WHERE _rso_id = m.rso_id
      AND m.user_id = _user_id) THEN

      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'You are already a member of this RSO';

    ELSE
      -- add new member
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

END;

-- procedure that allows a user to leave the RSO
CREATE PROCEDURE leave_rso (
  IN _user_id INT(11),
  IN _rso_id INT(11)
)
BEGIN

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

    -- prevent a complicated situation
    IF _rso_administrator = _user_id THEN

      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "You must mark someone else as the administrator before leaving";

    END IF;

    DELETE FROM is_member 
    WHERE user_id = _user_id
    AND rso_id = _rso_id;

    -- if the new count of users is less than 5, make RSO pending, or unable to post events.
    IF (SELECT COUNT(m.user_id) FROM is_member m WHERE m.rso_id = _rso_id) < 5 THEN
      UPDATE rsos SET
      status = 'PND'
      WHERE rso_id = _rso_id;
    END IF;

    SELECT _user_id AS user_id;

  COMMIT;

END//

DELIMITER ;

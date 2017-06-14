-- Procedures are used to update or modify the database (implies side effects)
-- use functions simply to select or return an output.
-- a procedure should be used for transactions which require multiple inserts
-- or for transactions with complex logic
DROP FUNCTION IF EXISTS `event`.check_uni_emails;
DROP PROCEDURE IF EXISTS `event`.select_students;
DROP PROCEDURE IF EXISTS `event`.create_user;
DROP PROCEDURE IF EXISTS `event`.update_user;

USE `event`;

DELIMITER //
-- these are procedures attached to the database which are then callable by our PDO
-- procedures are being used for the following reasons (put reasons here)
CREATE PROCEDURE select_students()
BEGIN
  SELECT u.user_id, u.user_name, u.email, u.first_name, u.last_name
  FROM users u
  WHERE u.role = "STU";
END//

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
-- only perform insert if the email matches the universities email domain 
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
END//

CREATE PROCEDURE update_user (
  IN _user_id INT(11),
  IN _user_name varchar(30),
  IN _first_name varchar(60),
  IN _last_name varchar(60),
  IN _email varchar(60),
  IN _role enum("SA", "ADM", "STU"),
  IN _hash varchar(60),
  IN _uni_id INT(11)) 
BEGIN

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
END//
DELIMITER ;

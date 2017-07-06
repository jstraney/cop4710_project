<?php
// These queries are 'included' into the method/action called
// on the User Controller. When they are included, they are set
// to the objects private fields. These, along with any other partials
// for the user entity are in separate files, to allow for cleaner
// readability as well as better separation of logic.
//
// heredoc syntax uses <<<EOT to begin a multi line string
// the ending EOT must be left justified (no spaces or tabs proceding it.)
//
// two create queries are needed because PDO's can only do one insert at
// a time. queries are transactionalized by pdo->beginTransaction and
// pdo->commit
//
// create_user and update_user procedures are stored in /procedures.sql.
// because they contain a bit of logic and multiple inserts (users, attending)
$create_query = <<<EOT
CALL create_user(:user_name, :first_name, :last_name, :email, :role, :hash, :uni_id);
EOT;

// updates the fields on a profile when someone submits the user/edit form.
$update_query = <<<EOT
CALL update_user(:user_id, :user_name, :first_name, :last_name, :email, :role, :uni_id);
EOT;

// destroys a user using their :uid
$destroy_query = <<<EOT
DELETE FROM users
WHERE user_id = :uid;
EOT;

// returns all the fields used on a users profile page.
$view_query = <<<EOT
CALL view_user(:user_id);
EOT;

// gets student by university id.
$user_university_query = <<<EOT
SELECT u.user_name, u.first_name, u.last_name, a.uni_id
FROM users u, attending a
WHERE u.user_id = a.user_id AND a.uni_id = :uni_id
LIMIT :start, :end;
EOT;

// gets users at a university by matching username
$get_students_like_query = <<<EOT
SELECT u.user_id AS data, u.user_name AS value 
FROM users u, attending a
WHERE u.role = "STU"
AND u.user_id = a.user_id
AND a.uni_id = :uni_id
AND u.user_name LIKE CONCAT(:name, "%");
EOT;

$user_participating_query = <<<EOT
SELECT e.event_id, e.name
FROM participating p, events e
WHERE p.user_id = :user_id
AND p.event_id = e.event_id
LIMIT :start, :end;
EOT;

$user_membership_query = <<<EOT
SELECT r.rso_id, r.name 
FROM is_member m, rsos r 
WHERE m.user_id = :user_id
AND m.rso_id = r.rso_id
LIMIT :start, :end;
EOT;

// gets credentials for user trying to edit a profile.
// checks if the user is super admin or is the user.
$credentials_id_query = <<<EOT
SELECT user_id, hash
FROM users
WHERE user_id = :user_id;
EOT;

// gets the credentials for the user trying to log in.
// this is to check against what was submitted by the user
// attempting to log in.
$credentials_email_query = <<<EOT
SELECT user_id, hash
FROM users
WHERE email = :email;
EOT;

// get the current user logged into the website. uses $_SESSION['uid']
// uses a procedure which will return a different object depending on role
// SA for example has no association with attending a school
$current_user_query = <<<EOT
CALL get_current_user(:uid);
EOT;
/*
SELECT u.user_name, u.first_name, u.last_name, u.email, u.role, a.uni_id
FROM users u, attending a
WHERE user_id = :uid;
*/
?>

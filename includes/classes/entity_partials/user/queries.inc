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
CALL update_user(:user_id, :user_name, :first_name, :last_name, :email, :role, :hash, :uni_id);
EOT;

// destroys a user using their :uid
$destroy_query = <<<EOT
DELETE FROM users
WHERE user_id = :uid;
EOT;

// returns all the fields used on a users profile page.
$view_query = <<<EOT
SELECT u.user_name, u.first_name, u.last_name, n.uni_id, u.email, n.name AS uni_name
FROM users u, attending a, universities n
WHERE u.user_id = :uid AND (:uid1 = a.user_id AND a.uni_id = n.uni_id)
LIMIT 1;
EOT;

// gets student by university id.
$user_university_query = <<<EOT
SELECT u.user_name, u.first_name, u.last_name, a.uni_id
FROM users u, attending a
WHERE u.user_id = a.user_id AND a.uni_id = :uni_id;
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
// to get the current user from database.
$current_user_query = <<<EOT
SELECT user_name, first_name, last_name, email, role
FROM users
WHERE user_id = :uid;
EOT;
?>

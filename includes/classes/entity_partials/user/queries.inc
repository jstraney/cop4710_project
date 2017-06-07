<?php
// These queries are 'included' into the method/action called
// on the User Controller. When they are included, they are set
// to the objects private fields. These, along with any other partials
// for the user entity are in separate files, to allow for cleaner
// readability as well as better separation of logic.
//
// heredoc syntax uses <<<EOT to begin a multi line string
// the ending EOT must be left justified (no spaces or tabs proceding it.)
$create_query = <<<EOT
INSERT INTO users (user_name, first_name, last_name, email, uni_id, hash)
VALUES (:user_name, :first_name, :last_name, :email, :uni_id, :hash);
EOT;

// updates the fields on a profile when someone submits the user/edit form.
$update_query = <<<EOT
UPDATE users
SET user_name = :user_name, first_name = :first_name,
last_name = :last_name, email = :email, uni_id = :uni_id, hash = :hash
WHERE user_id = :uid;
EOT;

// destroys a user using their :uid
$destroy_query = <<<EOT
DELETE FROM users
WHERE user_id = :uid;
EOT;

// returns all the fields used on a users profile page.
$view_query = <<<EOT
SELECT u.user_name, u.first_name, u.last_name, u.uni_id, u.email, n.uni_name
FROM users u, universities n
WHERE u.user_id = :uid AND n.uni_id = u.uni_id;
EOT;

// gets student by university id.
$user_university_query = <<<EOT
SELECT u.user_name, u.first_name, u.last_name, u.uni_id
FROM users u
WHERE u.uni_id = :uni_id;
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
SELECT user_name, first_name, last_name, email, role, uni_id
FROM users
WHERE user_id = :uid;
EOT;
?>

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
INSERT INTO users (user_name, first_name, last_name, email, university, hash)
VALUES (:user_name, :first_name, :last_name, :email, :university, :hash);
EOT;

// updates the fields on a profile when someone submits the user/edit form.
$update_query = <<<EOT
UPDATE users
SET user_name = :user_name, first_name = :first_name,
 last_name = :last_name, email = :email, university = :university, hash = :hash;
EOT;

// destroys a user using their :uid
$destroy_query = <<<EOT
DELETE FROM users
WHERE user_id = :uid;
EOT;

// returns all the fields used on a users profile page.
$view_query = <<<EOT
SELECT user_name, first_name, last_name, university, email 
FROM users
WHERE user_id = :uid;
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
SELECT user_name, first_name, last_name, university, email, role 
FROM users
WHERE user_id = :uid;
EOT;
?>

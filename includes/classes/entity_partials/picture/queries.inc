<?php
$owns_user_query = <<<EOT
SELECT COUNT(u.user_id) AS is_owner
FROM users u
-- people can edit their own account
WHERE u.user_id = :user_id
AND u.user_id = :entity_id;
EOT;

$owns_rso_query = <<<EOT
-- only an administrator can change the RSO's picture
SELECT COUNT(a.user_id) AS is_owner
FROM administrates a
WHERE a.user_id = :user_id
AND a.rso_id = :entity_id;
EOT;

// a procedure is actually in place for event ownership. Right now it's set
// so that if a user creates the event they can edit it, but also, if its an
// RSO event, and that user administrates the RSO, they can edit it as well.
$owns_event_query = <<<EOT
CALL get_is_owner(:user_id, :entity_id);
EOT;

$owns_university_query = <<<EOT
-- only the super admin can edit a universities picture.
SELECT COUNT(u.user_id)
FROM users u
WHERE u.user_id = :user_id
AND u.role = "SA"
-- have to include this or pdo throws error. all params must be used.
AND :entity_id = :entity_id;
EOT;
?>

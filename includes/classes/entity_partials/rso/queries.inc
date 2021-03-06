<?php
$create_query = <<<EOT
CALL create_rso(:role, :user_id, :name, :description, :members, :rso_administrator, :uni_id );
EOT;

$update_query = <<<EOT
CALL update_rso(:rso_id, :role, :user_id, :name, :description, :members, :rso_administrator, :uni_id);
EOT;

$view_query = <<<EOT
CALL view_rso(:rso_id, :user_id, :role);
EOT;

$view_all_query = <<<EOT
EOT;

$destroy_query = <<<EOT
DELETE FROM rsos WHERE rso_id = :rso_id;
EOT;

$administrates_rso_query = <<<EOT
SELECT r.rso_id AS data, r.name AS value 
FROM users u, administrates a, rsos r
WHERE u.user_id = :user_id
AND u.user_id = a.user_id
AND a.rso_id = r.rso_id
AND r.status = 'ACT'
AND r.name LIKE CONCAT(:name, "%");
EOT;

$rso_members_query = <<<EOT
SELECT u.user_id, u.user_name
FROM users u, is_member m, rsos r
WHERE r.rso_id = :rso_id
AND m.rso_id = r.rso_id
AND u.user_id = m.user_id;
EOT;

$rso_university_query = <<<EOT
SELECT r.name, r.description, r.rso_id, COUNT(m.user_id) AS total_members
FROM has h, rsos r
LEFT JOIN is_member m ON r.rso_id = m.rso_id
WHERE :uni_id = h.uni_id
AND h.rso_id = r.rso_id 
GROUP BY r.rso_id
LIMIT :start, :end;
EOT;

$join_rso_query = <<<EOT
CALL join_rso (:user_id, :rso_id, :uni_id);
EOT;

$leave_rso_query = <<<EOT
CALL leave_rso (:user_id, :rso_id);
EOT;

$view_all_query = <<<EOT
SELECT n.name AS uni_name, n.uni_id, r.name AS rso_name, r.rso_id
FROM universities n, has h, rsos r
WHERE :uni_id = n.uni_id
AND n.uni_id = h.uni_id
AND h.rso_id = r.rso_id;
EOT;
?>

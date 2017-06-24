<?php
$create_query = <<<EOT
CALL create_event (
  :name, :start_time, :end_time, :telephone, :email, :description,
  :location, :lat, :lon, :accessibility, :categories, :user_id, :rso_id, :uni_id);
EOT;

$update_query = <<<EOT
CALL update_event ( 
  :event_id, :name, :start_time, :end_time, :end_time, :telephone, :description,
  :location, :lat, :lon, :accessibility, :categories, :user_id, :rso_id, :uni_id);
EOT;

$delete_query = <<<EOT
DELETE events
WHERE event_id = :event_id;
EOT;

$view_query = <<<EOT
CALL view_event(:event_id, :user_id, :role, :uni_id);
EOT;

$destroy_query = <<<EOT
DELETE events
WHERE event_id = :event_id;
EOT;

$attend_event_query = <<<EOT
CALL attend_event(:event_id, :user_id, :uni_id);
EOT;

$unattend_event_query = <<<EOT
DELETE FROM participating
WHERE user_id = :user_id
AND event_id = :event_id;
EOT;

$rate_event_query = <<<EOT
CALL rate_event(:user_id, :event_id, :rating);
EOT;

// todo: add a 'date_declared' field for better sorting.
$event_participants_query = <<<EOT
SELECT u.user_id, u.user_name
FROM users u, participating p
WHERE u.user_id = p.user_id
AND p.event_id = :event_id
ORDER BY u.user_id;
EOT;

$search_events_query = <<<EOT
CALL search_events(:user_id, :uni_id, :scope, :sort_by, :accessibility);
EOT;

$get_rso_events_query = <<<EOT
SELECT e.event_id, e.name, e.description, e.start_time, e.end_time, e.location
FROM events e, r_created_e rce 
WHERE rce.rso_id = :rso_id
AND rce.event_id = e.event_id
ORDER BY e.start_time;
EOT;

?>

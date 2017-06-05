<?php
$create_query = <<<EOT
INSERT INTO events (event_name, event_street, event_time, event_org, event_long, event_lat,
event_loc_name, event_city, event_zip, event_duration)
VALUES (:event_name, :event_street, :event_time, :event_org, :event_long, :event_lat,
:event_loc_name, :event_city, :event_zip, :event_duration);
EOT;

$update_query = <<<EOT
UPDATE events SET event_name = :event_name
WHERE event_id = :event_id;
EOT;

$delete_query = <<<EOT
DELETE events
WHERE event_id = :event_id;
EOT;

$view_query = <<<EOT
SELECT event_name, event_street, event_time, event_org,
event_long, event_lat, event_loc_name, event_city, event_zip, event_duration,
event_id, university_id, state FROM events WHERE event_id = :event_id LIMIT 1; 
EOT;

$view_all_query = <<<EOT
SELECT event_name, event_street, event_time, event_org,
event_long, event_lat, event_loc_name, event_city, event_zip, event_duration,
event_id, university_id, state FROM events WHERE event_id = :event_id LIMIT 10; 
EOT;

$destroy_query = <<<EOT
DELETE events
WHERE event_id = :event_id;
EOT;

?>

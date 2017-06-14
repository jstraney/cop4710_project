<?php
$create_query = <<<EOT
INSERT INTO events (name, street, start_time, end_time, long, lat,
location, city, zip, state)
VALUES (:name, :street, :start_time, :end_time, :long, :lat,
:loc_name, :city, :zip, :state);
EOT;

$update_query = <<<EOT
UPDATE events SET name = :name
WHERE event_id = :event_id;
EOT;

$delete_query = <<<EOT
DELETE events
WHERE event_id = :event_id;
EOT;

$view_query = <<<EOT
SELECT name, street, time,
long, lat, loc_name, city, zip, duration,
id, uni_id, state FROM events WHERE id = :id LIMIT 1; 
EOT;

$view_all_query = <<<EOT
SELECT name, street, time,
long, lat, loc_name, city, zip, 
event_id, uni_id, state FROM events LIMIT :start, :end; 
EOT;

$destroy_query = <<<EOT
DELETE events
WHERE event_id = :event_id;
EOT;

?>

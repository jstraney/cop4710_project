<?php

$create_query = <<<EOT
INSERT INTO universities (name, description, location, lat, lon, email_domain, website_url, population)
VALUES (:name, :description, :location, :lat, :lon, :email_domain, :website_url, :population);
EOT;

$update_query = <<<EOT
UPDATE universities
SET name = :name, description = :description, location = :location, lat = :lat, lon = :lon, email_domain = :email_domain, website_url = :website_url, population = :population
EOT;

$destroy_query = <<<EOT
DELETE FROM universities u
WHERE u.uni_id = :uni_id
EOT;

$view_query = <<<EOT
SELECT n.uni_id, n.name, n.description, n.location, n.lat, n.lon, n.website_url, n.population
FROM universities n
WHERE n.uni_id = :uni_id
EOT;

$university_rsos_query = <<<EOT
SELECT h.rso_id, r.name, r.description
FROM has h, rsos r
WHERE h.uni_id = :uni_id AND h.rso_id = r.rso_id
EOT;

$university_events_query = <<<EOT
SELECT h.event_id, e.name, e.description
FROM hosting h, events e
WHERE h.uni_id = :uni_id AND h.event_id = e.event_id
EOT;

$view_all_query = <<<EOT
SELECT uni_id, name, description, location, lat, lon, website_url
FROM universities LIMIT :start, :end;
EOT;

// gets a list of all universities. used when building select elements for forms
// like in the 'new user' form.
$select_all_query = <<<EOT
SELECT uni_id, name FROM universities;
EOT;

$match_name_query= <<<EOT
SELECT name AS 'value', uni_id AS 'data' FROM universities WHERE name LIKE :name;
EOT;
?>

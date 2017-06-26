<?php

$create_query = <<<EOT

EOT;

$update_query = <<<EOT

EOT;

$destroy_query = <<<EOT

EOT;

$view_query = <<<EOT
SELECT n.uni_id, n.name, n.description, n.location, n.lat, n.lon, n.website_url, n.population
FROM universities n
WHERE n.uni_id = :uni_id
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

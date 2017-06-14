<?php

$create_query = <<<EOT

EOT;

$update_query = <<<EOT

EOT;

$destroy_query = <<<EOT

EOT;

$view_query = <<<EOT
SELECT n.uni_id, n.name, n.street, n.state, n.zip, count(a.user_id)
FROM universities n, attending a
WHERE n.uni_id = :uni_id AND a.uni_id = n.uni_id;
EOT;

$view_all_query = <<<EOT
SELECT uni_id, name, street, state, zip 
FROM universities LIMIT :start, :end;
EOT;

// gets a list of all universities. used when building select elements for forms
// like in the 'new user' form.
$select_all_query = <<<EOT
SELECT uni_id, name FROM universities;
EOT;

$match_name_query= <<<EOT
SELECT name AS 'label', uni_id AS 'value' FROM universities WHERE name LIKE :name;
EOT;
?>

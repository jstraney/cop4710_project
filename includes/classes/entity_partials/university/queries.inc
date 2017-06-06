<?php

$create_query = <<<EOT

EOT;

$update_query = <<<EOT

EOT;

$destroy_query = <<<EOT

EOT;

$view_query = <<<EOT
SELECT uni_id, uni_name, uni_street, uni_state, uni_zip, uni_num_students
FROM universities
WHERE uni_id = :uni_id;
EOT;

$view_all_query = <<<EOT
SELECT uni_id, uni_name, uni_street, uni_state, uni_zip, uni_num_students
FROM universities LIMIT :start, :end;
EOT;

// gets a list of all universities. used when building select elements for forms
// like in the 'new user' form.
$select_all_query = <<<EOT
SELECT uni_id, uni_name FROM universities;
EOT;
?>

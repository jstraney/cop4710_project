<div class="entity-record">

<?php

include "includes/classes/{$resource_name}_controller.inc";

console_log($partial);

$controller = new $controller_class();

$content = $controller->view_all(array());

echo $content;

?>

</div>

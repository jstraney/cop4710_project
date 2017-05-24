<div class="entity-record-list">

<?php

include "includes/classes/{$resource_name}_controller.inc";

$controller = new $controller_class();

$content = $controller->view_all(array());

echo $content;

?>

</div>

<?php
// todo: find a way to pass a parameter here to choose
// how to aggregate the users (e.g. by university, by rso)
$template = <<<EOT
<div class="event-list">
EOT;
// events queried by event controller. passed into this template
// by the context of the 'view_all' method.
foreach ($events as $event) {

}

$template .= <<<EOT
</div>
EOT;
?>

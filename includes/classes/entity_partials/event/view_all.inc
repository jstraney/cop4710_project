<?php
// todo: find a way to pass a parameter here to choose
// how to aggregate the users (e.g. by university, by rso)
$template = <<<EOT
<div class="entity-list events">
<h2>Events</h2>
EOT;
// events queried by event controller. passed into this template
// by the context of the 'view_all' method.
foreach ($events as $event) {

  $template .= '<div class="entity-record">';
  $template .= '<a href="'.$site_root.'/event/'.$event['event_id'].'">';
  $template .= '<h3>'.$event['event_name'].'</h3>';
  $template .= '</a>';
  $template .= '<div>'.$event['event_loc_name'].'</div>';
  $template .= '<div>time:'.$event['event_time'].'</div>';
  $template .= '<div>duration:'.$event['event_duration'].'</div>';
  $template .= '<div>hosted by:'.$event['event_org'].'</div>';
  $template .= '</div>';

}

$template .= <<<EOT
</div>
EOT;
?>

<?php
$template = <<<EOT
<div><a href="{$site_root}/events">&laquo; Back to events</a></div>

<h2>{$event_name}</h2>

<div>Location: {$event_loc_name}. {$event_city}, {$state}. {$event_zip}</div>
<input type="hidden" id="location" value="{$event_loc_name}"></input>
<div id="map"></div>
<script type="text/javascript" src="https://maps.google.com/maps/api/js?key=AIzaSyBGfGPKeqJZRoQSInY5LXqvZt7lZwihCPA&libraries=places"></script>
<script src="{$site_root}/js/lib/jquery-locationpicker-plugin/locationpicker.jquery.js"></script>

<div>Start time: {$event_time}</div>
<div>Duration: {$event_duration}</div>

<div>Hosted by: {$event_org}</div>

EOT;

?>

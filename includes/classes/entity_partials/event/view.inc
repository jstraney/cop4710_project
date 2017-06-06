<?php

// calculates the start and end on an id.
$back_range = get_pagination_from_id($event_id, 10);
$start = $back_range[0];
$end = $back_range[1];

$template = <<<EOT
<div><a href="{$site_root}/events/{$start}-{$end}">&laquo; Back to events</a></div>

<h2>{$event_name}</h2>

<div>Location: {$event_loc_name}. {$event_city}, {$state}. {$event_zip}</div>
<input type="hidden" id="location" value="{$event_loc_name}"></input>
<input type="hidden" id="lon" value="{$event_long}"></input>
<input type="hidden" id="lat" value="{$event_lat}"></input>
<div id="map"></div>
<script type="text/javascript" src="https://maps.google.com/maps/api/js?key=AIzaSyBGfGPKeqJZRoQSInY5LXqvZt7lZwihCPA&libraries=places"></script>
<script src="{$site_root}/js/lib/jquery-locationpicker-plugin/locationpicker.jquery.js"></script>
<script>app.makeMapPresentation()</script>
<div>Start time: {$event_time}</div>
<div>Duration: {$event_duration}</div>

<div>Hosted by: {$event_org}</div>

EOT;

?>

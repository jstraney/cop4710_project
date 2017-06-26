<?php

$template = <<<EOT
<div id="inner-content">
<script>
(function(a) {
  a.scope.event_id = {$event_id};
  a.scope.status = "{$status}";
  a.scope.current_user_id = {$user_id};
})(app);
</script>

<script src="{$site_root}/js/view_event.js"></script>

<div><a href="{$site_root}/events">&laquo; Back to events</a></div>

EOT;

// if the current user is the owner, show the link to edit.
if ($is_owner) {

  $template .= "<div><a href=\"{$site_root}/event/{$event_id}/edit\">&laquo; Edit Event</a></div>";
  $template .= "<div><a href=\"{$site_root}/event/{$event_id}/delete\">&laquo; Delete Event</a></div>";

}

$template .= <<<EOT
<h2>{$name}</h2>

EOT;
if ($rso_id != NULL) {

  $template .= "<h5>Hosted by ";
  $template .= "<a href=\"{$site_root}/rso/{$rso_id}\">{$rso_name}</a>";
  $template .= "</h5>";

}

if ($status == "PND") {
  $template .= '<p class="notice">This event is still pending the super admin\'s approval</p>';
}

$template .= <<<EOT
<div>Location: {$location}</div>
<input type="hidden" id="location" value="{$location}"></input>
<input type="hidden" id="lon" value="{$lon}"></input>
<input type="hidden" id="lat" value="{$lat}"></input>
<div id="map"></div>
<script type="text/javascript" src="https://maps.google.com/maps/api/js?key=AIzaSyBGfGPKeqJZRoQSInY5LXqvZt7lZwihCPA&libraries=places"></script>
<script src="{$site_root}/js/lib/jquery-locationpicker-plugin/locationpicker.jquery.js"></script>
<script>app.makeMapPresentation()</script>
<div>Start time: {$start_time}</div>
<div>End Time: {$end_time}</div>
<div>Description: {$description}</div>

<div class="event-page aggregate events">
<h4>People Attending this Event:</h4>

</div>

<div class="event-page aggregate participants">
<!-- populate with ajax -->
</div>

<!-- particpate form. populated with JS if user is not attending. -->
<div class="participate">
</div>

<h4>Average Rating: <span id="total-rating">{$rating}</span>/5.00</h4>

<h4>Rate this event!</h4>
<div id="rating">

</div>

<h4>Comment Section</h4>
<div class="event-view aggregate comment">
<!-- comments loaded with AJAX -->
</div>

<div>
<!-- form will be positioned below div using absolute positioning, top:100% -->
<form id="new-comment" class="lefty" method="POST" action="{$site_root}/comment/create">
  <textarea id="comment-body" name="content">
  </textarea>
  <input class="btn btn-large" type="submit" value="Post"/>
</form>
</div>
</div>
EOT;

?>

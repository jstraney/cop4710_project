<?php

$template = <<<EOT
<div id="inner-content">
<script>
(function(a) {
  a.scope.event_id = {$event_id};
  a.scope.status = "{$status}";
  a.scope.current_user_id = {$user_id};
  a.scope.has_pic = "{$has_pic}";
})(app);
</script>

<script src="{$site_root}/js/view_event.js"></script>

<div><a href="{$site_root}/events">&laquo; Back to events</a></div>

EOT;

$template .= "<h2>{$name}</h2>";

$template .= '<div class="event access">'.$access_msg.'</div>';

if ($status == "PND") {
  $status_msg = 'This event is still pending the super admin\'s approval';
}
else if ($status == "ACT") {
  $status_msg = 'This event is active';
}
else {
  $status_msg = "";
}

$template .= '<p class="notice '.strtolower($status).'">'.$status_msg.'</p>';
// if the current user is the owner, show the link to edit.
if ($is_owner) {

  $template .= "<div><a href=\"{$site_root}/event/{$event_id}/edit\">&laquo; Edit Event</a></div>";
  $template .= "<div><a href=\"{$site_root}/event/{$event_id}/delete\">&laquo; Delete Event</a></div>";
  $template .= <<<EOT
  <div class="view-event widget picture"></div>
  <script>
  (function (a) {
    var configs = {type : "events", id: $event_id, style: "medium"};
    a.util.pictureWidget($('div.widget.picture'), configs);
  })(app);
  </script>
EOT;

}
else {
  $template .= load_entity_pic('events', $event_id, 'view-event medium');
}


if ($rso_id != NULL) {

  $template .= '<div class="view-event pane rso">';
  $template .= '<div class="info">Hosted by ';
  $template .= "<strong>{$rso_name}</strong>";
  $template .= " at <strong>{$uni_name}</strong>";
  $template .= "</div>";
  $template .= "<a href=\"{$site_root}/rso/{$rso_id}\">";
  $template .= load_entity_pic('rsos', $rso_id, 'thumbnail');
  $template .= "</a>";
  $template .= "<a href=\"{$site_root}/university/{$uni_id}\">";
  $template .= load_entity_pic('universities', $uni_id, 'thumbnail');
  $template .= "</a>";
  $template .= "</div>";

}


$template .= <<<EOT
<div class="clearfix">Location: {$location}</div>
<input type="hidden" id="location" value="{$location}"></input>
<input type="hidden" id="lon" value="{$lon}"></input>
<input type="hidden" id="lat" value="{$lat}"></input>
<div id="map"></div>
<script type="text/javascript" src="https://maps.google.com/maps/api/js?key=AIzaSyBGfGPKeqJZRoQSInY5LXqvZt7lZwihCPA&libraries=places"></script>
<script src="{$site_root}/js/lib/jquery-locationpicker-plugin/locationpicker.jquery.js"></script>
<script>app.makeMapPresentation()</script>
<h5>Event Type</h5>
<div class="aggregate categories"></div>
<span class="event time">Start time: {$start_time}</span>
<span class="event time">End Time: {$end_time}</span>
<div>
  <h5>Description:</h5>
  {$description}
</div>

<div class="event-page aggregate events">
<h4>People Attending this Event:</h4>
<!-- populated with AJAX -->
</div>

<div class="event-page aggregate participants">
<!-- populated with AJAX -->

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

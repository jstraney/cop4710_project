<?php

$template = <<<EOT
<form method="POST" action="{$site_root}/event/create">
  <script src="{$site_root}/js/lib/jquery-datetimepicker/jquery.datetimepicker.full.js"></script>
  <link rel="stylesheet" href="{$site_root}/js/lib/jquery-datetimepicker/jquery.datetimepicker.min.css"/>
  <h3>Create a New Event</h3>

  <label>Title</label>
  <input name="title" type="text"></input>

  <label>Location</label>
  <input id="location" name="loc-name" type="text"></input>

  <input style="display:none;" id="lon" name="lon" type="text" val=""></input>
  <input style="display:none;" id="lat" name="lat" type="text" val=""></input>
  <div id="map">
  </div>
  <script type="text/javascript" src="https://maps.google.com/maps/api/js?key=AIzaSyBGfGPKeqJZRoQSInY5LXqvZt7lZwihCPA&libraries=places"></script>
  <script src="{$site_root}/js/lib/jquery-locationpicker-plugin/locationpicker.jquery.js"></script>
  <script>app.makePicker();</script>


  <label>City</label>
  <input name="city" type="text"></input>

  <label>State</label>
  <input name="state" type="text"></input>

  <label>Street Address</label>
  <input name="street" type="text"></input>

  <label>Zipcode</label>
  <input name="zip" type="text"></input>

  <label>Start Time</label>
  <input id="start-time" name="start-time" type="text"></input>
  <script>$("#start-time").datetimepicker()</script>

  <label>End Time</label>
  <input id="end-time" name="end-time" type="text"></input>
  <script>$("#end-time").datetimepicker()</script>

  <!-- todo: check if user is administrator any RSO's. if so, include the fields below -->
  <!-- if the event is not hosted by an RSO, it is put into a 'pending' state, which is changed by-->
  <!-- the super admin -->
  <label>Is the organization an RSO (recognized student organization)?</label>
  <div>
    No <input id="is-rso" type="radio" value="0" name="rso-hosted"></input> 
    Yes <input id="not-rso" type="radio" value="1" name="rso-hosted"></input> 
  </div>

  <!-- if the event is an RSO, put the following fields below -->
  <div style="display: none;" class="selection rso">
    <label>Which RSO is it?</label>
    <!-- todo: populate with RSO's administrated by the current user. -->
    <input type="text" name="organization" placeholder="Name of RSO" value="">
  </div>

  <!-- If the event is not an RSO, allow the following.-->
  <div style="display:none;" class="selection organization">
    <label>Who is hosting the event?</label>
    <input name="organization" placeholder="organization" type="text"></input>
  </div>

  <input type="submit" value="Create Event"></input>

  <script src="{$site_root}/js/event_form.js"></script>

</form>
EOT;

?>

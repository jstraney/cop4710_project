<?php

$template = <<<EOT
<form method="POST" action="{$site_root}/event/new">

  <h3>Create a New Event</h3>

  <label>Title</label>
  <input name="title" type="text"></input>

  <label>Location</label>
  <input id="location" name="loc-name" type="text"></input>

  <div id="map">
  </div>
  <script type="text/javascript" src="https://maps.google.com/maps/api/js?key=AIzaSyBGfGPKeqJZRoQSInY5LXqvZt7lZwihCPA&libraries=places"></script>
  <script src="{$site_root}/js/lib/jquery-locationpicker-plugin/locationpicker.jquery.js"></script>
  <script>app.makePicker();</script>

  <input name="lon" type="hidden"></input>
  <input name="lat" type="hidden"></input>

  <label>City</label>
  <input name="city" type="text"></input>

  <label>State</label>
  <input name="state" type="text"></input>

  <label>Street Address</label>
  <input name="street" type="text"></input>

  <label>Zipcode</label>
  <input name="zip" type="text"></input>

  <label>Who is hosting the event?</label>
  <input name="organization" placeholder="organization" type="text"></input>

  <!-- todo: check if user is administrator any RSO's. if so, include the fields below -->
  <!-- if the event is not hosted by an RSO, it is put into a 'pending' state, which is changed by-->
  <!-- the super admin -->
  <label>Is the organization an RSO (recognized student organization)?</label>
  <div>
  No <input type="radio" value="0" name="rso-hosted"></input> 
  Yes <input type="radio" value="1" name="rso-hosted"></input> 
  </div>

  <!-- If the event is not an RSO, allow a plain text field.-->
  <label>Who is hosting the event?</label>
  <input name="organization" placeholder="organization" type="text"></input>

  <!-- if the event is an RSO, put the following fields below -->
  <label>Which RSO is it?</label>
  <select>
    <!-- todo: populate with RSO's administrated by the current user. -->
    {$user_rsos}
  </select>

  <input type="submit" value="Create Event"></input>

</form>
EOT;

?>

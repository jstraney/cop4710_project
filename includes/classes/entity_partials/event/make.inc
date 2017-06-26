<?php

$template = <<<EOT
<div id="inner-content">
<form method="POST" action="{$site_root}/event/create">
  <script src="{$site_root}/js/lib/jquery-datetimepicker/jquery.datetimepicker.full.js"></script>
  <link rel="stylesheet" href="{$site_root}/js/lib/jquery-datetimepicker/jquery.datetimepicker.min.css"/>
  <h3>Create a New Event</h3>

  <label>Title</label>
  <input name="name" type="text"></input>

  <label>Location</label>
  <input id="location" name="location" type="text"></input>

  <input style="display:none;" id="lon" name="lon" type="text" val=""></input>
  <input style="display:none;" id="lat" name="lat" type="text" val=""></input>

  <div id="map">
  </div>
  <script type="text/javascript" src="https://maps.google.com/maps/api/js?key=AIzaSyBGfGPKeqJZRoQSInY5LXqvZt7lZwihCPA&libraries=places"></script>
  <script src="{$site_root}/js/lib/jquery-locationpicker-plugin/locationpicker.jquery.js"></script>
  <script>app.makePicker({defaultLocation: "{$location}", defaultLat: "{$lat}", defaultLon:"{$lon}"});</script>

  <label>Description</label>
  <textarea id="description" name="description" placeholder="Enter a description"></textarea>

  <div class="event-make contact">
    <h4>Contact Info</h4>
    <label>phone number</label>
    <input name="telephone" type="tel" pattern="^[0-9]{3}-[0-9]{3}-[0-9]{4}" title="format: 123-444-5555"></input>
    <label>e-mail</label>
    <input name="email" type="email" value="{$user->email}"></input>
  </div>


  <label>Start Time</label>
  <input id="start-time" name="start-time" type="text"></input>
  <div class="start-time-picker"></div>

  <label>End Time</label>
  <input id="end-time" name="end-time" type="text"></input>
  <div class="end-time-picker"></div>
  <script>
  $("#start-time, #end-time").datetimepicker({

  });
  </script>

  <!-- todo: check if user is administrator any RSO's. if so, include the fields below -->
  <!-- if the event is not hosted by an RSO, it is put into a 'pending' state, which is changed by-->
  <!-- the super admin -->
  <label>Is the organization an RSO (recognized student organization)?</label>
  <div>
    <input id="not-rso" type="radio" value="0" name="rso-hosted"/> 
    <label for="not-rso">No</label>
    <input id="is-rso" type="radio" value="1" name="rso-hosted"/> 
    <label for="is-rso">Yes</label>
  </div>

  <!-- if the event is an RSO, put the following fields below -->
  <div style="display: none;" class="selection rso">
    <label>Which RSO is it?</label>
    <!-- todo: populate with RSO's administrated by the current user. -->
    <input type="text" id="rso-input" name="rso-input" placeholder="Name of RSO" value="">
    <input type="hidden" id="rso-id" name="rso-id" value="">
  </div>

  <div class="selection">
    <label>Who can see this event?</label>
    <select id="accessibility" name="accessibility">
      <option id="access-pub"value="PUB">Public</option>
      <option id="access-pri"value="PRI">Private</option>
      <option id="access-rso" value="RSO">RSO</option>
    </select>
    <p class="field-prompt">
    Events marked as public can be seen by anyone. Events marked as private can only be
    seen by members of the same university. RSO events can only be seen by the RSO specified
    (If any).
    </p>
  </div>

  <div class="selection">
    <label>Pick a Category</label>
    <input id="category" name="category" type="text"></input>
  </div>

  <input class="btn btn-large" type="submit" value="Create Event"></input>

  <script src="{$site_root}/js/event_form.js"></script>

</form>
</div>
EOT;

?>

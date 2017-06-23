<?php

$template = <<<EOT
<form method="POST" action="{$site_root}/event/{$event_id}/update">

EOT;

if ($role === "SA" ) {
  $template .= <<<EOT
  <div>
    <p>
    Marking the event as active will allow the event to be viewed and attended.
    </p>
    <select>
      <option value="PND">Pending</option>
      <option value="ACT">Active</option>
    </select>
  </div> 
EOT;

}

$template .= <<<EOT
  <script src="{$site_root}/js/event_form.js"></script>
  <script src="{$site_root}/js/lib/jquery-datetimepicker/jquery.datetimepicker.full.js"></script>
  <link rel="stylesheet" href="{$site_root}/js/lib/jquery-datetimepicker/jquery.datetimepicker.min.css"/>
  <h3>Edit Event</h3>

  <label>Title</label>
  <input name="name" type="text" value="{$name}"></input>

  <label>Location</label>
  <input id="location" name="location" type="text" value="{$location}"></input>

  <input style="display:none" id="lon" name="lon" type="text" value="{$lon}"></input>
  <input style="display:none" id="lat" name="lat" type="text" value="{$lat}"></input>

  <div id="map">
  </div>

  <script type="text/javascript" src="https://maps.google.com/maps/api/js?key=AIzaSyBGfGPKeqJZRoQSInY5LXqvZt7lZwihCPA&libraries=places"></script>

  <script src="{$site_root}/js/lib/jquery-locationpicker-plugin/locationpicker.jquery.js"></script>
  <script>app.makePicker();</script>

  <label>Description</label>
  <textarea id="description" name="description">{$description}</textarea>

  <label>Start Time</label>
  <input id="start-time" name="start-time" type="text" value="{$start_time}"></input>
  <script>$("#start-time").datetimepicker()</script>

  <label>End Time</label>
  <input id="end-time" name="end-time" type="text" value="{$end_time}" ></input>
  <script>$("#end-time").datetimepicker()</script>

  <div>
  <h4>Contact Info</h4>
  <label>phone number</label>
  <input name="phone-number" type="tel" pattern="^[0-9]{3}-[0-9]{3}-[0-9]{4}"
    title="format: 123-444-5555" value="{$telephone}"></input>
  <label>e-mail</label>
  <input name="e-mail" type="email" value="{$email}"></input>
  </div>

  <!-- if the event is not hosted by an RSO, it is put into a 'pending' state, which is changed by-->
  <!-- the super admin -->
  <label>Is the organization an RSO (recognized student organization)?</label>
  <div>
    No <input id="not-rso" type="radio" value="0" name="rso-hosted"></input> 
    Yes <input id="is-rso" type="radio" value="1" name="rso-hosted"></input> 
  </div>

  <!-- if the event is an RSO, put the following fields below -->
  <div style="display:none" class="selection rso">
    <label>Which RSO is it?</label>
    <!-- todo: populate with RSO's administrated by the current user. -->
    <input type="text" id="rso-input"></input>
    <input type="hidden" id="rso-id"></input>
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
  <div class="selection">

  <input type="submit" value="Update Event"></input>

</form>
EOT;

?>

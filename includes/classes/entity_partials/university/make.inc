<?php

$template = <<<EOT
<form method="POST" action="{$site_root}/university/create">

  <input type="text" name="name"/>
  <label for="name">University Name</label>

  <textarea name="description"></textarea>
  <label for="description">Description of your university</label>
  
  <input type="text" name="location" id="location"/>
  <label for="location">Location</label>
  <div id="map"></div>

  <input type="hidden" name="lat" id="lat"/>
  <input type="hidden" name="lon" id="lon"/>

  <script type="text/javascript" src="https://maps.google.com/maps/api/js?key=AIzaSyBGfGPKeqJZRoQSInY5LXqvZt7lZwihCPA&libraries=places"></script>
  <script src="{$site_root}/js/lib/jquery-locationpicker-plugin/locationpicker.jquery.js"></script>
  <script>
  (function (a) {

    a.makePicker({defaultLocation: "New York, NY, USA", defaultLat: 40.741895, defaultLon: -73.989308});
  
  })(app);
  </script>
  
  <div id="input-field">
    <input type="text" name="email_domain"/>
    <label for="email_domain">Email domain associated with your university</label>
  </div>
  
  <input type="text" name="website_url"/>
  <label for="website_url">Website URL</label>

  <input type="text" name="population"/>
  <label for="population">Population</label>

  <input class="btn btn-large" type="submit" value="Create University"/>
</form>
EOT;

?>

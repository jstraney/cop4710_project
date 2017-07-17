<?php

$university_pic = load_entity_pic('universities', $id, $style = "medium");

$template = <<<EOT
<div id="inner-content">

  <h2>{$name}</h2>

  {$university_pic}

  <p class="university description">
  {$description}
  </p>

  <div class="university related-resource-box address">Address: {$location}</div>

  <div id="map" class="university map" ></div>

  <input id="lat" name="lat" type="hidden" value="{$lat}"/>
  <input id="lon" name="lon" type="hidden" value="{$lon}"/>

  <script type="text/javascript" 
    src="https://maps.google.com/maps/api/js?key=AIzaSyBGfGPKeqJZRoQSInY5LXqvZt7lZwihCPA&libraries=places">
  </script>

  <script src="{$site_root}/js/lib/jquery-locationpicker-plugin/locationpicker.jquery.js"></script>

  <script>
    (function (a) {
      a.makeMapPresentation();
    })(app);
  </script>

  <h5>Students at {$name}</h5>

  <div class="university related-resource-box population">{$num_students}</div>

  <h3>RSOs at {$name}</h3>

  <div class="university-view aggregate rso">
EOT;

// list associated rsos
foreach($associated_rsos as $rso) {
  // assign the rso id for interpolation below
  $rso_id = $rso['rso_id'];
  $rso_name = $rso['name'];
  $rso_pic = load_entity_pic('rsos', $rso_id);
  
  // link to the listed rso
  $template .= <<<EOT
    <div class="record rso">
      <a href="{$site_root}/rso/{$rso_id}">
      {$rso_name}
      {$rso_pic}
      </a>
    </div>  
EOT;
  }

$template .= <<<EOT
  </div>


  <h3>Events at {$name}</h3>

  <div class="university-view aggregate events">
EOT;

// list associated rsos
foreach($associated_events as $event) {
  // assign the event id for interpolation below
  $event_id = $event['event_id'];
  $event_name = $event['name'];
  $event_pic = load_entity_pic('events', $event_id);

  // link to the listed event
  $template .= <<<EOT
    <div class="record event">
      <a href="{$site_root}/event/{$event_id}">
        {$event_pic}
        {$event_name}
      </a>
    </div> 
EOT;

}

$template .= <<<EOT
  </div>

</div>
EOT;

?>

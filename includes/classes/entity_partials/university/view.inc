<?php

$university_pic = load_entity_pic('universities', $id, $style = "medium");

$template = <<<EOT
<div id="inner-content">

<h2>{$name}</h2>
{$university_pic}

<h3>Students at {$name}</h3>
<div class="related-resource-box">{$num_students}</div>

<h3>RSOs at {$name}</h3>
<div class="related-resource-box">
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

</div>
EOT;
}

$template .= <<<EOT
<div>Address: {$location}</div>

</div>
EOT;

?>

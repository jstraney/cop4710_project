<?php

$template = <<<EOT
<div id="inner-content">

<h2>{$name}</h2>

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
  
  // link to the listed rso
  $template .= <<<EOT
    <div class="record rso">
      <a href="{$site_root}/rso/{$rso_id}">{$rso_name}</a>
    </div>  
EOT;
}

$template .= <<<EOT
</div>

<h3>Events at {$name}</h3>
<div class="university-view aggregate events"></div>

<div>Address: {$location}</div>

</div>
EOT;

?>

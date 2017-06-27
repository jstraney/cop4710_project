<?php

$template = <<<EOT
<div id="inner-content">

<h2>{$name}</h2>

<h3>Students at {$name}</h3>
<div class="related-resource-box">{$num_students}</div>

<h3>RSOs at {$name}</h3>
<div class="related-resource-box"></div>

<h3>Events at {$name}</h3>
<div class="university-view aggregate events"></div>

<div>Address: {$location}</div>

</div>
EOT;

?>

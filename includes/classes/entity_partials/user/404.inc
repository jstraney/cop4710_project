<?php
// we can include custom templates for 404, or even 403 responses
// that vary depending on entity type.
$template = <<<EOT
<div id="inner-content">
  <h2>Whoops!</h2>
  <p>
    No user was found with that id!
  </p>
</div>
EOT;
?>

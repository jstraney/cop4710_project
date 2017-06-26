<?php

$template = <<<EOT
<div id="inner-content">

<form method="POST" action="{$site_root}/event/{$id}/destroy">

  <h2>Delete Event</h2>

  <p>
  Deleting events is permanent. By clicking the submit button
  you agree that you understand that all data associated with this
  event will be lost.
  </p>

  <input class="btn btn-large" type="submit" value="Delete Event" ></input>

</form>

</div>
EOT;

?>

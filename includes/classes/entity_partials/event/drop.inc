<?php

$template = <<<EOT

<form method="POST" action="{$site_root}/event/{$id}/destroy">

  <h2>Delete Event</h2>

  <p>
  Deleting events is permanent. By clicking the submit button
  you agree that you understand that all data associated with this
  event will be lost.
  </p>

  <input type="submit" value="Delete Event" ></input>

</form>

EOT;

?>

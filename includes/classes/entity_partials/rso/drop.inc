<?php
$template = <<<EOT
<div id="inner-content">
<div>
  <a href="{$site_root}/rso/{$rso_id}">Back to {$name} &raquo;</a>
</div>

<form method="POST" action="{$site_root}/rso/{$rso_id}/destroy">

  <h2>Delete {$name}</h2>

  <p>
    Deleting an RSO is permanent. All events, user membership, and other
    data associated with the RSO will also be lost. To confirm, enter
    your password
  </p>

  <input type="password"></input>

  <input class="btn btn-large" type="submit" value="Delete RSO"></input>

</form>
</div>
EOT;
?>

<?php
$template = <<<EOT
<div id="inner-content">
<form method="POST" action="{$site_root}/user/{$id}/destroy">

  <h3>Delete Account</h3>

  <p>
    Deleting accounts is a permanent action.
    enter and confirm your password to delete this account.
  </p>

  <input id="password" type="password" name="password" value=""></input>

  <input class="btn btn-large" id="submit" type="submit" name="submit" value="Yes, delete this account"></input>

</form>
</div>
EOT;
?>

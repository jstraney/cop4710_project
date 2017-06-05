<?php
$template = <<<EOT
<form method="POST" action="session/create">
<h3>Sign In</h3>
<label>E-mail</label>
<input id="e-mail" type="email" name="e-mail" value=""></input>
<label>Password</label>
<input id="password" type="password" name="password" value=""></input>
<input id="submit" type="submit" name="submit" value="Sign In"></input>
</form>
EOT;
?>

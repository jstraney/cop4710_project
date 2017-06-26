<?php
$template = <<<EOT
<br><br>
<div class="row">
  <form method="POST" action="{$site_root}/session/create" class="col s12">
    <div class="row">
      <div class="col s4 offset-s2">
        <h5>Sign In</h5>
      </div>
	  </div>
	  <div class="row">
      <div class="input-field col s4 offset-s2">
        <input id="e-mail" name="e-mail" type="text" class="validate">
        <label for="e-mail">E-mail</label>
      </div>
	  </div>
	  <div class="row">
      <div class="input-field col s4 offset-s2">
        <input id="password" type="password" name="password" value=""></input>
        <label for="password">Password</label>
      </div>
	  </div>
	  <div class="row">
	  	<div class="col s4 offset-s2">
			  <button class="btn waves-effect waves-light" type="submit" name="action">Login
			  </button>
		</div>
	  </div>
  </form>
</div>
  
EOT;
?>

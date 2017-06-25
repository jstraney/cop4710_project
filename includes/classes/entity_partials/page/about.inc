<?php
global $configs;

$site_root = $configs['site_root'];
$template = <<<EOT
<br><br>
<div class="row">
<div class="col s4 offset-s2 m6">
  <div class="card amber darken-1">
	<div class="card-content white-text">
	  <span class="card-title">About Us</span>
	  <p>SUP (the student university page), is a unified discovery layer, aimed
	  to help students organize, and come together for the causes they are
	  passionate about.</p>
	</div>
	<div class="card-action white">
	  <a href="{$site_root}/events">Find Events</a>
	  <a href="{$site_root}/universities">Find Your University</a>
	</div>
  </div>
</div>
</div>
<div class="row">
<div class="col s10 offset-s2 m6">
  <div class="card amber darken-1">
	<div class="card-content white-text">
	  <span class="card-title">Join an RSO</span>
	  <p>Using SUP, joining a registered student organization is easier than ever.
		Simply register using your e-mail, and request to join RSO's
	  </p>
	</div>
	<div class="card-action white">
	  <a href="{$site_root}/user/new">Sign Up</a>
	  <a href="{$site_root}/RSO">Find an RSO</a>
	</div>
  </div>
</div>
</div>
<div class="row">
<div class="col s10 offset-s2 m6">
  <div class="card amber darken-1">
	<div class="card-content white-text">
	  <span class="card-title">Attend Events</span>
	  <p>
		Discover events that are being hosted on your campus, and find out how
		you can participate. Using SUP, it is possible to create your own
		RSO's and your own events.
	  </p>
	</div>
	<div class="card-action white">
	  <a href="{$site_root}/user/new">Sign Up</a>
	  <a href="{$site_root}/RSO">Find an RSO</a>
	</div>
  </div>
</div>
</div>


EOT;
?>

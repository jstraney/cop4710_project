<?php
// global configs associative array
global $configs;

// use the site root to construct the links in the header below
// if we moved our site out of our local apache setup, the url would
// change and so we have the base url in a configuration
$site_root = $configs['site_root'];

?>
<nav class="amber darken-4" role="navigation">
    <div class="nav-wrapper container">
      <a id="logo-container" href="<?php echo $site_root; ?>" class="brand-logo">
      <img class="logo" src="<?php echo $site_root; ?>/res/icons/top_logo.png"></img>
      </a>
      <ul class="right hide-on-med-and-down">
		<?php
    // check if user is anonymous, or 0
			if (!isset($user)) {
			  // echo the login and create account links if the user is anonymous
			  echo '<li><a href="'.$site_root.'/login">Login</a></li>';
			  echo '<li><a class="waves-effect waves-light btn" href="'.$site_root.'/user/new">New Account</a></li>';
			} else {
			  $user_name = $user->user_name;
			  // echo links available to the user
			  echo '<li>Hi, <a href="'.$site_root.'/user/'.$uid.'">'.$user_name.'</a></li>';
			  echo '<li><a href="'.$site_root.'/logout">Logout</a></li>';
			}
		?>
      </ul>
      <ul id="nav-mobile" class="side-nav right ">
		<?php
    // check if user is anonymous, or 0
			if (!isset($user)) {
			  // echo the login and create account links if the user is anonymous
			  echo '<li><a href="'.$site_root.'/login">Login</a></li>';
			  echo '<li><a class="waves-effect waves-light btn" href="'.$site_root.'/user/new">New Account</a></li>';
			} else {
			  $user_name = $user->user_name;
			  // echo links available to the user
			  echo '<li>Hi, <a href="'.$site_root.'/user/'.$uid.'">'.$user_name.'</a></li>';
			  echo '<li><a href="'.$site_root.'/logout">Logout</a></li>';
			}
		?>
	  </ul>
      <a href="#" data-activates="nav-mobile" class="button-collapse right"><i class="material-icons">menu</i></a>
    </div>
</nav>

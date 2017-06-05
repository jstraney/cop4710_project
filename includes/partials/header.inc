<?php
// global configs associative array
global $configs;

// use the site root to construct the links in the header below
// if we moved our site out of our local apache setup, the url would
// change and so we have the base url in a configuration
$site_root = $configs['site_root'];

?>
<div id="header">

  <a href="<?php echo $site_root; ?>/">
    <img class="logo" src="<?php echo $site_root; ?>/res/icons/top_logo.png"></img>
  </a>

  <!-- These will login/logout actions -->
  <ul class="menu user">
    <?php
    // check if user is anonymous, or 0
    if (!isset($user)) {
      // echo the login and create account links if the user is anonymous
      echo '<li><a href="'.$site_root.'/login">Sign In</a></li>';
      echo '<li><a href="'.$site_root.'/user/new">New Account</a></li>';
    } else {

      $user_name = $user->user_name;

      // echo links available to the user
      echo '<li>Hi, <a href="'.$site_root.'/user/'.$uid.'">'.$user_name.'</a></li>';
      echo '<li><a href="'.$site_root.'/logout">Logout</a></li>';
    }
    ?>
  </ul>
</div>

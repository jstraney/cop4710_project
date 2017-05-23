<?php
// global configs associative array
global $configs;

// use the site root to construct the links in the header below
// if we moved our site out of our local apache setup, the url would
// change and so we have the base url in a configuration
$site_root = $configs['site_root'];
?>
<div id="header">
  <!-- These will login/logout actions -->
  <ul class="menu user">
    <li><a href="<?php echo $site_root; ?>/login">Login</a></li>
    <li><a href="<?php echo $site_root; ?>/account/create">New Account</a></li>
    <li><a href="<?php echo $site_root; ?>/logout">Logout</a></li>
  </ul>
</div>

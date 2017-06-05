<?php
global $configs;

$site_root = $configs['site_root'];

?>
<div id="footer">
  <!-- put in some links? -->
  <ul class="menu">
    <h4>Top</h4>
    <li><a href="<?php echo $site_root; ?>/">Home</a></li>
    <li><a href="<?php echo $site_root; ?>/about">About</a></li>
    <?php
    if (isset($user)) {
      echo '<li><a href="'.$site_root.'/logout">Logout</a></li>';
    }
    else {
      echo '<li><a href="'.$site_root.'/login">Login</a></li>';
      echo '<li><a href="'.$site_root.'/user/create">New Account</a></li>';
    }
    ?>
  </ul>
  <ul class="menu">
    <h4>View</h4>
    <li><a href="<?php echo $site_root; ?>/university">Universities</a></li>
    <li><a href="<?php echo $site_root; ?>/rso">RSO's</a></li>
    <li><a href="<?php echo $site_root; ?>/event">Events</a></li>
    <li><a href="<?php echo $site_root; ?>/student">Students</a></li>
  </ul>
  <ul class="menu">
    <h4>Technical</h4>
    <li><a href="<?php echo $site_root; ?>/help">Help</a></li>
    <li><a href="<?php echo $site_root; ?>/terms">Terms</a></li>
    <li><a href="<?php echo $site_root; ?>/privacy">Privacy</a></li>
  </ul>
</div>

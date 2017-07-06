<?php
global $configs;

$site_root = $configs['site_root'];

?>
<footer class="page-footer amber darken-4">
    <div class="container">
      <div class="row center">
        <div class="col s4 ">
          <h5 class="white-text">Top</h5>
          <ul>
    		<li><a class="white-text" href="<?php echo $site_root; ?>/">Home</a></li>
    		<li><a class="white-text" href="<?php echo $site_root; ?>/about">About</a></li>
			<?php
			if (isset($user)) {
			  echo '<li><a class="white-text" href="'.$site_root.'/logout">Logout</a></li>';
			}
			else {
			  echo '<li><a class="white-text" href="'.$site_root.'/login">Login</a></li>';
			  echo '<li><a class="white-text" href="'.$site_root.'/user/new">New Account</a></li>';
			}
			?>
          </ul>
        </div>
        <div class="col s4 ">
          <h5 class="white-text">View</h5>
          <ul>
			<li><a class="white-text" href="<?php echo $site_root; ?>/universities/0-10">Universities</a></li>
			<li><a class="white-text" href="<?php echo $site_root; ?>/rsos/">RSO's</a></li>
			<li><a class="white-text" href="<?php echo $site_root; ?>/events/">Events</a></li>
			<li><a class="white-text" href="<?php echo $site_root; ?>/users/">Students</a></li>
          </ul>
        </div>
        <div class="col s4 ">
          <h5 class="white-text">Technical</h5>
          <ul>
			<li><a class="white-text" href="<?php echo $site_root; ?>/help">Help</a></li>
			<li><a class="white-text" href="<?php echo $site_root; ?>/terms">Terms</a></li>
			<li><a class="white-text" href="<?php echo $site_root; ?>/privacy">Privacy</a></li>
          </ul>
        </div>
      </div>
    </div>
    <div class="footer-copyright">
      <div class="container">
      </div>
    </div>
</footer>

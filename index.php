<?php

// TODOS:
// get router class
// use it to determine the route
// decipher the route and perform an action. 
// end TODOS

include "settings.inc";
include "includes/helpers.inc";

$site_root = $configs['site_root'];

?>
<!DOCTYPE html>
<html>
  <?php include "includes/partials/head.inc"; ?>
  <body>
  <?php include "includes/partials/header.inc"; ?>

  <!-- conditionally call in content -->
  <!-- there are four main. create, view, view_all, edit, delete -->
  <!-- these are in the includes/partials directory -->
  <div id="main-view">
  <?php 
    // partials will be included here.
    console_log($_GET);
  ?>
  </div>
  <?php include "includes/partials/footer.inc"; ?>
  </body>
</html>

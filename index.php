<?php
// to use a session, you must call session start at the beginning of a script.
session_start();

// include the apps settings
include "settings.inc";
// include helper functions 
include "includes/helpers.inc";

// user object 
global $user;

// check if the uid is set in the session
if (isset($_SESSION['uid'])) {

  $uid = $_SESSION['uid'];

  $user_controller = new UserController();
  // if it is, get the current user.
  $user = $user_controller->get_current_user($uid);

}

// create a new router
$router = new Router();

// get site root. this is in configuration because if an app is ever deployed,
// we can change it in just one place
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

  // echo the site message if it exists.
  $message = get_message_html();

  echo $message;

  $route = $_GET['q'];

  // the main view will be included here based on the routers interpretation
  // of the url. I've actually 'cut' off the part of the url we need and put
  // it in the script as GET parameters using the .htaccess file
  $router->decode_requested_route($route);

  // when matches_route is called, the first match in the router determines what
  // controller we want to use, what method (action) to call on the controller
  // and splits the url into arguments for that method
  if ($router->matches_route()) {

    // view, view_all, get_create_form, get_update_form ...
    $action = $router->get_action();

    // UserController, EventController, UniversityController
    $controller_class = $router->get_controller_class();

    // '/user/12' = array('user', '12'), '/universities' = array('universities'). Note there is some redundancy here
    // but I think it makes sense to use non-associative arrays here (order is implied)
    $arguments = $router->get_arguments();

    // instantiate the controller class using a variable name
    $controller = new $controller_class();

    // echo the result of that classes method
    echo $controller->{$action}($arguments);

  }

  ?>
  </div>
  <?php include "includes/partials/footer.inc"; ?>
  </body>
</html>

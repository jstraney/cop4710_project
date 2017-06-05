<?php
// This is the script that all the forms submit to. each form defines
// a hidden 'type' and 'action' input. to tell this script the entity
// controller and the method for it to call 
session_start();

// get global configurations
include "settings.inc";

// get helper functions
include "includes/helpers.inc";

global $user;

if (isset($_SESSION['uid'])) {

  $uid = $_SESSION['uid'];
  $user_controller = new UserController();
  $user = $user_controller->get_current_user($uid);
}

$site_root = $configs['site_root'];

// check the referer to make sure the request was made from this domain
// This is to prevent CSRF attacks.
$referer = $_SERVER['HTTP_REFERER'];

// check if the request was from this domain. If it wasn't, we redirect
// to the home page. ideally, we'd also check what page they are coming
// from, but I'll pass on this for now.
$from_domain = strpos($referer, $site_root) === 0;

// check the request method to make sure it was a POST request
$method = $_SERVER['REQUEST_METHOD'];

// redirect if the domain is incorrect, or if the method is GET
if ($method !== "POST" || !$from_domain) {

  // go to homepage
  header("location: /");

}

$router = new Router();

// because it's a post request, I cannot get the url arguments from
// the query string like in the GET. I'll have to break up the url
// myself looking at the request URI
$route = str_replace( "/".basename(getcwd())."/", "", $_SERVER['REQUEST_URI']);

$router->decode_requested_route($route);

$action = $router->get_action();

$controller_class = $router->get_controller_class();

// get the $_POST parameters
$params = $router->get_arguments();

$params += $_POST;

$controller = new $controller_class();

$controller->{$action}($params);

?>

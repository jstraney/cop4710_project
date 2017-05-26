<?php
session_start();
include "settings.inc";
include "includes/helpers.inc";

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

  header("location: /");

}

// get the $_POST parameters
$params = array();

$params += $_POST;

// we need a type and an action to perform (e.g. user->create, event->delete)
if (!isset($params['type']) || !isset($params['action'])) {

  // redirect
  header("location: $site_root ");

}

// name of the resource (e.g. user, event, rso)
$resource_name = $params['type'];

// name of the action or entity method (e.g. create, update, delete)
$action = $params['action'];

$controller_class = ucfirst($resource_name) . "Controller";

// include the entity controller class
include "includes/classes/{$resource_name}_controller.inc";

$controller = new $controller_class();

// call the proper method on the object 
$controller->{$action}($params);

// do a redirect to somewhere...
// header("location: $site_root ");

?>

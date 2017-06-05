<?php
// create a class that performs routing

class Router {

  // lists of routes, implicitly white-listed.
  private $routes = array(
    'GET' => array(), 
    'POST' => array(), 
  ); 

  // true if there is a match
  private $found_match;

  // these are our resources. used to auto-generate routes (new, edit, delete, view, view_all)
  // implicitely white listed.
  private $resources = array(
    'user',
    'event',
    'university',
    'rso',
  );

  // name of the resource to use. chosen from the routes
  private $resource_name;

  // the router maps a route to an action, or method, on a controller class.
  private $action;

  // the controller which will call this action method.
  private $controller;

  // simply the GET request split at the /. "user/12" is array("user", "12");
  private $arguments;

  // helper method to generate resourceful routes.  makes 5 routes for the resource
  // new, view, view_all, delete, edit
  private function create_resourceful_get_routes ($resource_name) {

    $class = ucfirst($resource_name);

    return array(
      "/$resource_name\/new/" => array('make', "{$class}Controller", $resource_name),
      "/$resource_name\/([0-9]+)\/edit/" => array('edit', "{$class}Controller", $resource_name),
      "/$resource_name\/([0-9]+)\/delete/" => array('drop', "{$class}Controller", $resource_name),
      "/$resource_name\/([0-9]+)/" => array('view', "{$class}Controller", $resource_name)
    );

  }

  // creates the resourceful POST routes which are create, update, destroy
  private function create_resourceful_post_routes ($resource_name) {

    $class = ucfirst($resource_name);

    return array(
      "/$resource_name\/create/" => array('create', "{$class}Controller", $resource_name),
      "/$resource_name\/([0-9]+)\/update/" => array('update', "{$class}Controller", $resource_name),
      "/$resource_name\/([0-9]+)\/destroy/" => array('destroy', "{$class}Controller", $resource_name),
    );

  }


  public function __construct() {

    $this->found_match = FALSE;

    // auto generate resourceful routes from the resource names
    foreach ($this->resources as $resource_name) {

      $this->routes['GET'] += $this->create_resourceful_get_routes($resource_name);
      $this->routes['POST'] += $this->create_resourceful_post_routes($resource_name);

    }

    // route to verify users
    $this->routes['GET']['/user\/verify\/[0-9a-f]{60}/'] = array('verify', 'UserController', 'user');

    // session controller isn't exactly resourceful (but it could be!)
    $this->routes['GET']['/login/'] = array('make', 'SessionController', 'session');
    $this->routes['GET']['/logout/'] = array('drop', 'SessionController', 'session');

    $this->routes['POST']['/session\/create/'] = array('create', 'SessionController', 'session');
    $this->routes['POST']['/session\/destroy/'] = array('destroy', 'SessionController', 'session');

    // page routes. due to time constraints. I don't want to make CRUD routes for pages.
    $this->routes['GET']['/privacy/'] = array('privacy', 'PageController', 'page');
    $this->routes['GET']['/terms/'] = array('terms', 'PageController', 'page');
    $this->routes['GET']['/about/'] = array('about', 'PageController', 'page');
    $this->routes['GET']['/help/'] = array('help', 'PageController', 'page');
    $this->routes['GET']['/.*/'] = array('home', 'PageController', 'page');

  }

  // break url into tokens. determine if request is GET or POST
  public function decode_requested_route($route) {

    $method = $_SERVER['REQUEST_METHOD'];

    if (!isset($this->routes[$method])) {

      // handle error. request is neither POST or GET 
      return;

    }

    $possible_routes = $this->routes[$method];

    // look for first match.
    foreach($possible_routes as $exp => $args) {

      $is_match = preg_match($exp, $route);

      if ($is_match === 1) {

        $this->found_match = TRUE;

        $this->action = $args[0];

        $this->controller = $args[1];

        $this->resource_name = $args[2];

        $this->arguments = explode('/', $route);

        break;

      }
      
    }

    return;

  }

  function matches_route () {

    return $this->found_match;

  }

  // gets the action or method of the controller that is mapped to the route
  // for example (view, view_all, get_edit_form). This is usually something
  // fullfilled by a GET request. For POSTing data, the controller will return
  // a form through this action.
  function get_action () {

    return $this->action; 

  }

  function get_resource_name () {

    return $this->resource_name;

  }

  function get_controller_class () {

    return $this->controller;

  }


  function get_arguments() {

    return $this->arguments;

  }

}

?>

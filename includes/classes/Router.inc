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
      "/$resource_name\/([0-9]+)/" => array('view', "{$class}Controller", $resource_name),
      "/{$resource_name}s/" => array('view_all', "{$class}Controller", $resource_name)
    );

  }

  // creates the resourceful POST routes which are create, update, destroy
  private function create_resourceful_post_routes ($resource_name) {

    $class = ucfirst($resource_name);

    return array(
      "/^$resource_name\/create$/" => array('create', "{$class}Controller", $resource_name),
      "/^$resource_name\/([0-9]+)\/update$/" => array('update', "{$class}Controller", $resource_name),
      "/^$resource_name\/([0-9]+)\/destroy$/" => array('destroy', "{$class}Controller", $resource_name),
    );

  }


  public function __construct() {

    $this->found_match = FALSE;

    // auto generate resourceful routes from the resource names
    foreach ($this->resources as $resource_name) {

      $this->routes['GET'] += $this->create_resourceful_get_routes($resource_name);
      $this->routes['POST'] += $this->create_resourceful_post_routes($resource_name);

    }

    // session controller isn't exactly resourceful (but it could be!)
    $this->routes['GET']['/^login$/'] = array('make', 'SessionController');
    $this->routes['GET']['/^logout$/'] = array('drop', 'SessionController');

    // whoops autogenerating the routes doesn't always work for unusual pluralizations.
    // here I make an exception for universities
    unset($this->routes['GET']["/^`universitys\/([0-9]+)-([0-9]+)$/"]);
    $this->routes['GET']["/^universities\/([0-9]+)-([0-9]+)$/"] = array('view_all', 'UniversityController');

    $this->routes['POST']['/^session\/create$/'] = array('create', 'SessionController');
    $this->routes['POST']['/^session\/destroy$/'] = array('destroy', 'SessionController');

    // page routes. due to time constraints. I don't want to make CRUD routes for pages.
    $this->routes['GET']['/^privacy$/'] = array('privacy', 'PageController');
    $this->routes['GET']['/^terms$/'] = array('terms', 'PageController');
    $this->routes['GET']['/^about$/'] = array('about', 'PageController');
    $this->routes['GET']['/^help$/'] = array('help', 'PageController');
    $this->routes['GET']['/.*/'] = array('home', 'PageController');

    // API endpoints. gives a response in JSON. will be useful for creating a sleek front end.
    // for example. we can use this create autocomplete widgets and allow a 'real time' feel.

    // get all students from a university by uni_id.
    // user endpoints
    $this->routes['POST']['/^users\/university=([0-9]+)\/json$/'] = array('get_students_json', 'UserController');

    // gets users at the same university based on matching name. checks users uni_id within controller.
    $this->routes['POST']['/^user\/peer\/like\/json$/'] = array('get_peers_json', 'UserController');
    // get the events attended by a specific user.
    $this->routes['POST']['/^user\/participating\/json$/'] = array('user_participating_json', 'UserController');
    $this->routes['POST']['/^user\/membership\/json$/'] = array('user_membership_json', 'UserController');

    // RSO endpoints
    $this->routes['POST']['/^rsos\/json$/'] = array('get_list_json', 'RsoController');
    $this->routes['POST']['/^rso\/members\/json$/'] = array('rso_members_json', 'RsoController');
    // gets rsos managed by the current user.
    $this->routes['POST']['/^rso\/administrated\/json$/'] = array('rso_administrator_json', 'RsoController');
    $this->routes['POST']['/^rsos\/university\/json$/'] = array('rso_university_json', 'RsoController');

    $this->routes['POST']['/^rso\/join$/'] = array('join_rso', 'RsoController');
    $this->routes['POST']['/^rso\/leave$/'] = array('leave_rso', 'RsoController');

    // comments endpoints
    // note that to give the app a 'real world' feel, all routes for comments will be done with post requests.
    // reasoning: you wouldn't want to have to go to a separate page just to 'make' a comment, or 'edit' it.
    // for that reason, we'll just have a form on an events page which allow comments to be created. We'll have
    // a timeout in javascript which polls the database for new comments and updates the page asynchronously.
    $this->routes['POST']['/^comment\/create$/'] = array('create', 'CommentController');
    $this->routes['POST']['/^comment\/destroy$/'] = array('destroy', 'CommentController');
    $this->routes['POST']['/^comment\/update$/'] = array('update', 'CommentController');

    // get comments on a specific event. based on id. this url will poll the database for new events and update the page.
    $this->routes['POST']['/^comments\/event\/json$/'] = array('get_event_comments_json', 'CommentController');

    // events endpoints
    $this->routes['POST']['/^events\/json$/'] = array('get_events_json', 'EventController');
    $this->routes['POST']['/^event\/attend$/'] = array('attend_event', 'EventController');
    $this->routes['POST']['/^event\/unattend$/'] = array('unattend_event', 'EventController');
    $this->routes['POST']['/^event\/rate$/'] = array('rate_event', 'EventController');
    // list of users attending event.
    $this->routes['POST']['/^event\/participants\/json$/'] = array('event_participants_json', 'EventController');
    $this->routes['POST']['/^event\/categories\/json$/'] = array('get_event_categories_json', 'EventController');
    $this->routes['POST']['/^event\/categories\/like\/json$/'] = array('get_event_categories_like_json', 'EventController');

    // all events hosted by an rso
    $this->routes['POST']['/^rso\/events\/json$/'] = array('get_rso_events_json', 'EventController');

    // all events hosted at a university 
    $this->routes['POST']['/^events\/university=([0-9]+)\/json$/'] = array('get_list_json', 'EventController');

    // university endpoints
    $this->routes['POST']['/^universities\/json$/'] = array('get_list_json', 'UniversityController');
    $this->routes['POST']['/^universities\/like\/json$/'] = array('get_autocomplete_json', 'UniversityController');

    // route for event service
    $this->routes['POST']['/^ucf\/import$/'] = array('import_ucf_events', 'EventService');

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

  function get_controller_class () {

    return $this->controller;

  }

  function get_arguments() {

    if (!isset($this->arguments)) {

      return array();

    }

    return $this->arguments;

  }

}

?>

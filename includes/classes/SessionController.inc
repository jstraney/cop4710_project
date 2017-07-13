<?php

class SessionController extends EntityController {

  protected $pdo;

  // uses the user controller to do a lookup by e-mail
  private $user_controller;

  // sesssions do not have queries because they are not stored in DB
  // instead, the session controller queries the user table and puts
  // the user id in the session super global upon successful login.
  public function __construct() {

    $this->user_controller = new UserController();

  }

  public function create($params) {

    if (!isset($params['e-mail'])) {

      return;

    }
    if (!isset($params['password'])) {

      return;

    }

    $email = $params['e-mail'];

    $password = $params['password'];

    $controller = $this->user_controller;

    // get the user id and password by e-mail.
    // todo: waiting on the database to have a 'user' table
    // instead of 'student'. once this is done the queries should
    // work.

    $credentials = $controller->get_credentials_by_email($email);


    if (empty($credentials)) {

      // no user by that name. set error message and go home
      // todo: set error message through helper function

      // redirect to home
      go_home(); 

    }

    $credentials = $credentials[0];

    $hash = $credentials["hash"];

    $user_id = $credentials["user_id"];

    // decrypt hash and check against the hash using helper function
    if (check_password($password, $hash)) {

      // put the user id in the session variable
      $_SESSION['uid'] = $user_id;

      // on success, go to the account.
      redirect_entity_view("user", $user_id);
      die();

    }
    // not a match, set error message and go home. 
    else {

      set_message("The credentials are incorrect.", "error");

      redirect_to_url("login");

      die();

    }

  }

  public function read($params) { 

    // sessions are not renderable so they cannot be viewed.
  
  }

  public function view_all($params) {

    // sessions are not renderable so they cannot be viewed.

  }

  public function update($params) {

    // updating sessions aren't really handled here.
    // also, no route exists.

  }

  public function destroy ($params) {

    // remove the user id from this users session
    unset($_SESSION['uid']);

    go_home(); 

  }

  public function make ($params) {

    global $configs;

    $site_root = $configs['site_root'];

    include get_entity_partial('session', 'make');

    return $template;

  }

  public function edit ($params) {

    // session doesn't really provide edit form.

  }

  public function drop ($params) {

    include get_entity_partial('session', 'drop');

    return $template;

  }

}

?>

<?php

include "entity_controller.inc";
include_once "user_controller.inc";

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
    console_log('This will be the plain text password hashed');
    console_log(encrypt_password($password)); 
    // $credentials = $controller->get_auth_by_email($email);

    // decrypt password and check against the plain-text one
     
    // put the user id in the session variable

  }

  public function view($params) { 

    // sessions are not renderable so they cannot be viewed.
  
  }

  public function view_all($params) {

    // sessions are not renderable so they cannot be viewed.

  }

  public function update($params) {

    // updating sessions aren't really handled here.
    // also, no route exists.

  }

  public function delete($params) {

    var_dump($params);

    // remove the user id from this users session
    unset($_SESSION['user_id']);

  }

  public function get_create_form ($params) {

    $form = '<form method="POST" action="action.php">';
    $form .= '<h3>Sign In</h3>';
    $form .= '<label>E-mail</label>';
    $form .= '<input id="e-mail" type="email" name="e-mail" value=""></input>';
    $form .= '<label>Password</label>';
    $form .= '<input id="password" type="password" name="password" value=""></input>';
    $form .= '<input id="type" type="hidden" name="type" value="session"></input>';
    $form .= '<input type="hidden" name="action" value="create"></input>';
    $form .= '<input id="submit" type="submit" name="submit" value="Sign In"></input>';
    $form .= '</form>';

    return $form;

  }

  public function get_edit_form ($params) {

    // session doesn't really provide edit form.

  }

  public function get_delete_form ($params) {

    // session doesn't really provide delete form.
    $form = '<form method="POST" action="action.php">';
    $form .= '<input id="type" type="hidden" name="type" value="session"></input>';
    $form .= '<input type="hidden" name="submit" onload="this.form.submit()"></input>';
    $form .= '<input type="hidden" name="action" value="delete"></input>';
    $form .= '</form>';

  }

}

?>

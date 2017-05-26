<?php

include_once "entity_controller.inc";

class UserController extends EntityController {

  private $create_query = "";
  private $view_query = "";
  private $view_all_query = "";
  private $update_query = "";
  private $delete_query = "";

  private $auth_query = "SELECT u.id, u.hash FROM users u WHERE e-mail = ? LIMIT 1;";

  public function __construct() {

    $this->pdo = new SupPDO();

  }

  public function create($params) {


  }

  public function view($params) {

  }

  public function view_all($params) {

  }

  public function update($params) {

  }

  public function delete($params) {

  }

  public function get_create_form () {

    $form = '<h3>Create Account</h3>';
    $form .= '<label>First Name</label>';
    $form .= '<input id="first-name" type="text" name="first-name" value=""></input>';
    $form .= '<label>Last Name</label>';
    $form .= '<input id="last-name" type="text" name="last-name" value=""></input>';
    $form .= '<label>E-mail</label>';
    $form .= '<input id="e-mail" type="email" name="e-mail" value=""></input>';
    $form .= '<label>What University Do You Attend?<label>';
    $form .= '<input id="university" type="text" name="university" value=""></input>';
    $form .= '<label>Password<label>';
    $form .= '<input id="password" type="password" name="password" value=""></input>';
    $form .= '<label>Confirm Password<label>';
    $form .= '<input id="confirm" type="password" name="confirm" value=""></input>';
    $form .= '<input id="submit" type="submit" name="submit" value="Sign Up"></input>';

    return $form;
  }

  public function get_edit_form () {

    $form = '<h3>Edit Account</h3>';
    $form .= '<label>First Name</label>';
    $form .= '<input id="first-name" type="text" name="first-name" value=""></input>';
    $form .= '<label>Last Name</label>';
    $form .= '<input id="last-name" type="text" name="last-name" value=""></input>';
    $form .= '<label>E-mail</label>';
    $form .= '<input id="e-mail" type="email" name="e-mail" value=""></input>';
    $form .= '<label>Change University<label>';
    $form .= '<input id="university" type="text" name="university" value=""></input>';
    $form .= '<label>Password<label>';
    $form .= '<input id="password" type="password" name="password" value=""></input>';
    $form .= '<label>Confirm Password<label>';
    $form .= '<input id="confirm" type="password" name="confirm" value=""></input>';
    $form .= '<input id="submit" type="submit" name="submit" value="Change Info"></input>';

    return $form;

  }

  public function get_delete_form () {

    $form = "<h3>Delete Account</h3>";
    $form .= "<p>Deleting accounts is a permanent action.";
    $form .= "enter and confirm your password to delete your account?</p>";
    $form .= '<input id="password" type="password" name="password" value=""></input>';
    $form .= '<label>Confirm Password<label>';
    $form .= '<input id="confirm" type="password" name="confirm" value=""></input>';
    $form .= '<input id="submit" type="submit" name="submit" value="Yes, delete my account"></input>';

    return $form;

  }

  // this has its own method to get a users id and hash by email
  public function get_auth_by_email($email) {

    $query = $this->auth_query;

    return $this->pdo->prepared_query($query, array(":email" => $email));

  }

}

?>

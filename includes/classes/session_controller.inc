<?php

include "entity_controller.inc";

class SessionController extends EntityController {

  protected $pdo;
  protected $type = "user";

  // sesssions do not have queries because they are not stored in DB
  // instead, the session controller queries the user table and puts
  // the user id in the session super global upon successful login.

  public function create($params) {

    if (!isset($params['username'])) {

      return;

    }
    if (!isset($params['password'])) {

      return;

    }

    // get the user id and password by user name.
    
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

    // remove the user id from this users session
    unset($_SESSION['user_id']);

  }

}

?>

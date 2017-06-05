<?php

class EventController extends EntityController {

  private $create_query ;
  private $view_query;
  private $view_all_query;
  private $update_query;
  private $destroy_query;

  public function __construct() {

    $this->db = new Database();

    include get_entity_partial('event', 'queries');

    $this->create_query = $create_query;
    $this->view_query = $view_query;
    $this->update_query = $update_query;
    $this->destroy_query = $destroy_query;

  }

  public function create($params) {


  }

  public function view($params) {

    global $configs;

    include get_entity_partial('event', 'view');

    return $template;

  }

  public function view_all($params) {

    // todo create a partial for view_all.
    // include it here and return the template
    
  }

  public function update($params) {

    // user who is signed in.
    global $user;

    $uid = $user->id;

    $query = $this->update_query;

    // perform the query
    // $this->db->prepared_query($query, array());
         
    // redirect to event page.


  }

  public function destroy ($params) {

    // user who is signed in.
    global $user;

    $uid = $user->id;

    $query = $this->destroy_query;

    // $this->db->prepared_query($query, array());
    
    // redirect to page of events, or user page. something...
     
  }

  public function new ($params) {

    global $configs;

    // current user logged in.
    global $user;

    if (!isset($user)) {



    }

    //$role = $user->role;

    // if the user is not an admin.
    /*
    if ($role !== "ADM") {

      // just get home page.
      go_home();

      return;

    }
    */
    $user_rsos = "";

    $id = $params[1];

    $site_root = $configs['site_root'];

    // check if the user deleting this is has the permissions. 
    
    include get_entity_partial('event', 'new');

    return $template;

  }

  public function edit ($params) {

    global $configs;

    // current user
    global $user;

    // assume user can't edit
    $can_edit = FALSE;
    // if the user is not an admin.
    /*
    // if they're an admin they can, though.
    $can_edit = $can_edit || $role === "ADM";

    // if they're an admin they can, though.
    $can_edit = $can_edit || $role === "ADM";
    
    */

    $id = $params[1];

    $site_root = $configs['site_root'];

    // check if the user editing this is has the permissions. 
    
    include get_entity_partial("event", "edit");

    return $template;

  }

  public function delete ($params) {

    global $configs;

    $id = $params[1];

    $site_root = $configs['site_root'];

    // check if the user deleting this is has the permissions. 

    include get_entity_partial("event", "delete");

    return $template;

  }

}

?>

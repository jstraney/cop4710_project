<?php

class RSOController extends EntityController {

  private $create_query ;

  private $view_query;
  private $view_all_query;
  private $update_query;
  private $delete_query;

  public function __construct() {

    $this->db = new Database();

    include get_entity_partial('rso', 'queries');

    $this->create_query = $create_query;
    $this->view_query = $view_query;
    $this->update_query = $update_query;
    $this->destroy_query = $destroy_query;

  }

  public function create($params) {

    $result = $this->db->prepared_query($this->create_query, array());

    if ($result === FALSE) {

      go_home();

    } 

    go_home();

  }

  public function view($params) {

    global $configs;

    $site_root = $configs['site_root'];

    // view query for RSO
    $query = $this->view_query;

    // id of RSO
    $id = $params[1];

    // result of querying the rso
    $result = $this->db->prepared_query($query, array(":id" => $uid));

    if (empty($result)) {

      include get_entity_partial('rso', '404');

      return $template;

    }

    include get_entity_partial('rso', 'view');

    return $template;

  }

  public function view_all($params) {
    
    $txt = "";

    return $txt;

  }

  public function update($params) {

  }

  public function destroy ($params) {

    // user who is signed in.
    global $user;

    $uid = $user->id;

    $role = $user->role;
     
  }

  public function make ($params) {

    global $configs;

    $site_root = $configs['site_root'];

    // user who is signed in.
    global $user;

    $uid = $user->user_id;

    $role = $user->role;
     

    include get_entity_partial('rso', 'make');

    return $template;

  }

  public function edit ($params) {

    // user who is signed in.
    global $user;

    $uid = $user->id;

    $role = $user->role;
     

    include get_entity_partial("rso", "edit");

    return $form;

  }

  public function drop ($params) {

    include get_entity_partial("rso", "drop");

    return $template;

  }

}

?>

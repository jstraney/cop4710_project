<?php

class UniversityController extends EntityController {

  private $create_query ;

  private $view_query;
  private $view_all_query;
  private $update_query;
  private $delete_query;

  public function __construct() {

    $this->db = new Database();

    include get_entity_partial('university', 'queries');

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

    // view query for university 
    $query = $this->view_query;

    // id of university 
    $id = $params[1];

    // result of querying the university 
    $result = $this->db->prepared_query($query, array(":id" => $uid));

    if (empty($result)) {

      include get_entity_partial('university', '404');

      return $template;

    }

    include get_entity_partial('university', 'view');

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

  public function new ($params) {

    global $configs;

    $site_root = $configs['site_root'];

    // user who is signed in.
    global $user;

    $uid = $user->id;

    $role = $user->role;
     

    include get_entity_partial('university', 'new');

    return $template;

  }

  public function edit ($params) {

    // user who is signed in.
    global $user;

    $uid = $user->id;

    $role = $user->role;
     

    include get_entity_partial("university", "edit");

    return $form;

  }

  public function delete ($params) {

    include get_entity_partial("university", "delete");

    return $template;

  }

}

?>

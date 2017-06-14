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
    $this->view_all_query = $view_all_query;
    $this->update_query = $update_query;
    $this->destroy_query = $destroy_query;

    $this->select_all_query = $select_all_query;
    $this->match_name_query = $match_name_query;

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

    global $user;

    $site_root = $configs['site_root'];

    // view query for university 
    $query = $this->view_query;

    // id of university 
    $id = $params[1];

    // result of querying the university 
    $result = $this->db->prepared_query($query, array(":uni_id" => $id));

    if (empty($result)) {

      include get_entity_partial('university', '404');

      return $template;

    }

    // get first record.
    $result = $result[0];

    // get fields set defaults.
    $uni_name = isset($result['uni_name']) ? $result['uni_name']: "";
    $uni_street = isset($result['uni_street']) ? $result['uni_street']: "";
    $uni_state = isset($result['uni_state']) ? $result['uni_state']: "";
    $uni_zip = isset($result['uni_zip']) ? $result['uni_zip']: "";
    $uni_num_students = isset($result['uni_num_students']) ? $result['uni_num_students']: "";

    include get_entity_partial('university', 'view');

    return $template;

  }

  public function view_all($params) {

    // get range of universities to show. e.g. 10-20
    $range = explode('-', $params[1]);
    $start = $range[0];
    $end = $range[1];

    $query = $this->view_all_query;

    $universities = $this->db->prepared_query($query, array(
      ':start' => $start, 
      ':end' => $end, 
    ));

    if (empty($universities)) {

      $template = "No universities found.";

      return $template;

    }

    include get_entity_partial('university', 'view_all');
    
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

    $uid = $user->id;

    $role = $user->role;
     

    include get_entity_partial('university', 'make');

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

  public function drop ($params) {

    include get_entity_partial("university", "drop");

    return $template;

  }

  public function get_list () {

    $query = $this->select_all_query;

    $results = $this->db->query($query);

    return $results;

  }

  public function get_list_json ($params) {

    $query = $this->select_all_query;

    $results = $this->db->query($query);

    echo json_encode($results);

  }

  public function get_autocomplete_json ($params) {

    $name = (isset($params['name']))? $params['name'] : '';

    $query = $this->match_name_query;

    $results = $this->db->prepared_query($query, array(":name" => $name."%")); 

    echo json_encode($results);

  }


}

?>

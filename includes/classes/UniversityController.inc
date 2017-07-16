<?php

class UniversityController extends EntityController {

  private $create_query;
  private $view_query;
  private $view_all_query;
  private $update_query;
  private $delete_query;
  private $university_rsos_query;
  private $university_events_query;

  public function __construct() {

    $this->db = new Database();

    include get_entity_partial('university', 'queries');

    $this->create_query = $create_query;
    $this->view_query = $view_query;
    $this->view_all_query = $view_all_query;
    $this->update_query = $update_query;
    $this->destroy_query = $destroy_query;
    $this->university_rsos_query = $university_rsos_query;
    $this->university_events_query = $university_events_query;
    $this->select_all_query = $select_all_query;
    $this->match_name_query = $match_name_query;

  }

  public function create($params) {

    $name = check_field($params, 'name', NULL);
    $description = check_field($params, 'description', NULL);
    $location = check_field($params, 'location', NULL);
    $lat = check_field($params, 'lat', NULL);
    $lon = check_field($params, 'lon', NULL);
    $email_domain = check_field($params, 'email_domain', NULL);
    $website_url = check_field($params, 'website_url', NULL);
    $population = check_field($params, 'population', NULL);

    $before_count = $this->db->query("
    SELECT COUNT(uni_id) FROM universities;
    ");

    $this->db->prepared_query($this->create_query, array(
      ':name' => $name,
      ':description' => $description,
      ':location' => $location,
      ':lat' => $lat,
      ':lon' => $lon,
      ':email_domain' => $email_domain,
      ':website_url' => $website_url,
      ':population' => $population
    ));

    $after_count = $this->db->query("
    SELECT COUNT(uni_id) FROM universities;
    ");

    if ($before_count == $after_count) {

      set_message("Could not create university.", "error");
      
      go_home();
      die();

    } 

    set_message("Successfully created university.");

    go_home();

  }

  public function view($params) {

    global $configs;

    global $user;

    $site_root = $configs['site_root'];

    // view query for university 
    $query = $this->view_query;

    // id of university at university/id 
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
    $name = isset($result['name']) ? $result['name']: "";
    $description = isset($result['description']) ? $result['description']: "";
    $location = isset($result['location']) ? $result['location']: "";
    $lat = isset($result['lat']) ? $result['lat']: "";
    $lon = isset($result['lon']) ? $result['lon']: "";
    $website_url = isset($result['website_url']) ? $result['website_url']: "";
    $num_students = isset($result['population']) ? $result['population']: 0;

    // format num_students for output
    $num_students = number_format($num_students, 0, ".", ",");

    // find rsos associated with this university
    $associated_rsos = $this->view_associated_rsos(array(
      "uni_id" => $id
    ));

    // find events associated with this university
    $associated_events = $this->view_associated_events(array(
      "uni_id" => $id
    ));

    include get_entity_partial('university', 'view');

    return $template;

  }

  public function view_all($params) {

    global $configs;

    $site_root = $configs['site_root'];

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
    
    return $template;
  }

  public function update($params) {

    $name = isset($result['name']) ? $result['name']: "";
    $description = isset($result['description']) ? $result['description']: "";
    $location = isset($result['location']) ? $result['location']: "";
    $lat = isset($result['lat']) ? $result['lat']: "";
    $lon = isset($result['lon']) ? $result['lon']: "";
    $email_domain = isset($result['email_domain']) ? $result['email_domain']: ""; 
    $website_url = isset($result['website_url']) ? $result['website_url']: "";
    $population = isset($result['population']) ? $result['population']: "";

    $query = $this->update_query;

    $this->db->prepared_query($query, array(
      ':name' => $name,
      ':description' => $description,
      ':location' => $location,
      ':lat' => $lat,
      ':lon' => $lon,
      ':email_domain' => $email_domain,
      ':website_url' => $website_url,
      ':population' => $population
    ));

    set_message("University successfully updated.");

    go_home();
  }

  public function destroy ($params) {

    // user who is signed in.
    global $user;

    $user_id = $user->user_id;
    $role = $user->role;

    $uni_id = $params->uni_id;

    $query = $this->delete_query;

    $this->db->prepared_query($query, array(
      ':uni_id' => $uni_id
    ));

    set_message("University successfully deleted.");
    
    go_home(); 

  }

  public function make ($params) {

    global $configs;

    $site_root = $configs['site_root'];

    // user who is signed in.
    global $user;

    if ($user == NULL) {

      go_home();

    }

    $uid = $user->user_id;

    $role = $user->role;
     
    if ($role !== "SA") {

      go_home();

    }

    include get_entity_partial('university', 'make');

    return $template;

  }

  public function edit ($params) {

    // user who is signed in.
    global $user;

    if ($user == NULL) {

      go_home();

    }

    $uid = $user->user_id;

    $role = $user->role;

    if ($role !== "SA") {

      go_home();

    }

    $uni_id = isset($params['uni_id'])? $params['uni_id']: 0;

    $query = $this->view_query;

    $result = $this->db->prepared_query($query, array (
      ":uni_id" => $uni_id
    ));

    $name = isset($result['name']) ? $result['name']: "";
    $description = isset($result['description']) ? $result['description']: "";
    $location = isset($result['location']) ? $result['location']: "";
    $lat = isset($result['lat']) ? $result['lat']: "";
    $lon = isset($result['lon']) ? $result['lon']: "";
    $website_url = isset($result['website_url']) ? $result['website_url']: "";

    include get_entity_partial("university", "edit");

    return $form;

  }

  public function drop ($params) {

    // user who is signed in.
    global $user;

    if ($user == NULL) {

      go_home();

    }

    $uid = $user->user_id;

    $role = $user->role;

    if ($role !== "SA") {

      go_home();

    }

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

    // show rsos associated with a given university
  public function view_associated_rsos ($params) {

    $uni_id = isset($params['uni_id']) ? $params['uni_id'] : 0;

    $query = $this->university_rsos_query;

    $result = $this->db->prepared_query($query, array(":uni_id" => $uni_id));

    return $result;
  }

  public function view_associated_events ($params) {

    $uni_id = isset($params['uni_id']) ? $params['uni_id'] : 0;

    $query = $this->university_events_query;

    $result = $this->db->prepared_query($query, array(":uni_id" => $uni_id));

    return $result;
  }
}

?>

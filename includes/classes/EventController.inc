<?php

class EventController extends EntityController {

  private $create_query ;
  private $view_query;
  private $update_query;
  private $destroy_query;

  private $attend_event_query;
  private $unattend_event_query;
  private $rate_event_query;

  private $event_participants_query;

  private $search_events_query;

  private $get_rso_events_query;

  public function __construct() {

    $this->db = new Database();

    include get_entity_partial('event', 'queries');

    $this->create_query = $create_query;
    $this->view_query = $view_query;
    $this->update_query = $update_query;
    $this->destroy_query = $destroy_query;
    $this->attend_event_query = $attend_event_query;
    $this->unattend_event_query = $unattend_event_query;
    $this->rate_event_query = $rate_event_query;
    $this->search_events_query = $search_events_query;
    $this->event_participants_query = $event_participants_query;

    $this->get_rso_events_query = $get_rso_events_query;

  }

  public function create($params) {

    global $configs;

    $site_root = $configs['site_root'];

    global $user;

    // if no user is logged in
    if (!isset($user)) {

      // must be logged in as user. go to home page.
      go_home();

    }

    $user_id = $user->user_id;

    $role = $user->role;

    $name = isset($params['name'])? $params['name']: "";
    $description = isset($params['description'])? $params['description']: "";
    $location = isset($params['location'])? $params['location']: "";

    // calculate duration from start and end time.
    $start_time = isset($params['start-time'])? $params['start-time']: "";
    $end_time = isset($params['end-time'])? $params['end-time']: "";
    $telephone = isset($params['telephone'])? $params['telephone']: "";
    $email = isset($params['email'])? $params['email']: "";
    $lon = isset($params['lon'])? $params['lon']: "";
    $lat = isset($params['lat'])? $params['lat']: "";

    $categories = isset($params['categories'])? $params['categories']: ""; 
    $accessibility = isset($params['accessibility'])? $params['accessibility']: ""; 

    // allow user to set the rso from the list of rso's they administrate.
    // If none was selected, set to 0 to set event to PND -> pending
    $rso_id = isset($params['rso-id'])? $params['rso-id']: 0; 

    // always use the users university
    $uni_id = isset($user->uni_id)? $user->uni_id: 0; 

    $query = $this->create_query;

    // returns id of created event for redirect purposes
    $result = $this->db->prepared_query($query, array(
      ":name" => $name,
      ":description" => $description,
      ":location" => $location,
      ":start_time" => $start_time,
      ":end_time" => $end_time,
      ":telephone" => $telephone,
      ":email" => $email,
      ":lat" => $lat,
      ":lon" => $lon,
      ":accessibility" => $accessibility,
      ":categories" => $categories,
      ":user_id" => $user_id,
      ":rso_id" => $rso_id,
      ":uni_id" => $uni_id,
    ));

    // something went wrong. PDO most likely has set error message
    if (empty($result)) {

      // just go home for now. change to go_back once we are positive this works.
      go_home();
      return;

    }

    // get id of event just created.
    $result = $result[0];
    $event_id = $result['event_id'];
    $event_id = $result['event_id'];
    set_message("You've successfully created the event " . $name . ". Try inviting friends!");
    redirect_entity_view('event', $event_id);

  }

  public function view($params) {

    global $configs;

    $site_root = $configs['site_root'];

    global $user;

    // if no user is set, redirect to home-page
    if (!isset($user)) {

      go_home();

    }

    $user_id = $user->user_id;

    $uni_id = isset($user->uni_id)? $user->uni_id: 0;

    $role = $user->role;

    $event_id = $params[1];

    $query = $this->view_query;

    $result = $this->db->prepared_query($query, array(
      ":event_id" => $event_id,
      ":user_id" => $user_id,
      ":role" => $role,
      ":uni_id" => $uni_id,
    ));

    // no event was found with this event id
    if (empty($result)) {

      // return a the 404 partial associated with events.
      include get_entity_partial('event', '404');

      return $template;

    }

    // get first row from result.
    $result = $result[0];

    $name = isset($result['name'])? $result['name']: "";
    $location = isset($result['location'])? $result['location']: "";
    $description = isset($result['description'])? $result['description']: "";

    $start_time = isset($result['start_time'])? $result['start_time']: "";
    $end_time = isset($result['end_time'])? $result['end_time']: "";
    
    $telephone = isset($result['telephone'])? $result['telephone']: "";
    $email = isset($result['email'])? $result['email']: "";
    $lon = isset($result['lon'])? $result['lon']: "";
    $lat = isset($result['lat'])? $result['lat']: "";
    $is_owner = isset($result['is_owner'])? $result['is_owner']: 0;
    $is_participating = isset($result['is_participating'])? $result['is_participating']: 0;
    $rating = isset($result['rating'])? $result['rating']: "";
    $status = isset($result['status'])? $result['status']: "";

    // if the event was created by an RSO allow the following.
    $rso_id = isset($result['rso_id'])? $result['rso_id']: "";
    $rso_name = isset($result['rso_name'])? $result['rso_name']: "";

    // return the partial using the variable defined above.
    include get_entity_partial('event', 'view');

    return $template;

  }

  public function view_all($params) {

    global $configs;

    $site_root = $configs['site_root'];

    global $user;

    if ($user == NULL) {

      go_home();

    }

    $user_id = $user->user_id;
    $role = $user->role;
    $uni_id = isset($user->uni_id)? $user->uni_id: 0;

    $events = array(); 

    if (empty($results)) {

    }  

    include get_entity_partial('event', 'view_all');

    return $template;
    
  }

  public function update($params) {

    global $configs;

    $site_root = $configs['site_root'];

    // user who is signed in.
    global $user;

    $event_id = $params[1];

    $user_id = $user->user_id;
    $uni_id = isset($user->uni_id)? $user->uni_id: 0;
    $role = $user->role;
 
    $name = isset($params['name'])? $params['name']: "";
    $description = isset($params['description'])? $params['description']: "";
    $location = isset($params['location'])? $params['location']: "";

    // calculate duration from start and end time.
    $start_time = isset($params['start-time'])? $params['start-time']: "";
    $end_time = isset($params['end-time'])? $params['end-time']: "";
    $telephone = isset($params['telephone'])? $params['telephone']: "";
    $email = isset($params['email'])? $params['email']: "";
    $lon = isset($params['lon'])? $params['lon']: "";
    $lat = isset($params['lat'])? $params['lat']: "";

    $accessibility = isset($params['accessibility'])? $params['accessibility']: "PUB"; 
    
    $rso_id = isset($params['rso-id'])? $params['rso-id']: 0; 

    $categories = isset($params['categories'])? $params['categories']: ""; 

    $status = isset($params['status'])? $params['status']: 'PND';

    $query = $this->update_query;

    $result = $this->db->prepared_query($query, array(
      ":event_id" => $event_id,
      ":name" => $name,
      ":location" => $location,
      ":description" => $description,
      ":start_time" => $start_time,
      ":end_time" => $end_time,
      ":telephone" => $telephone,
      ":email" => $email,
      ":lon" => $lon,
      ":lat" => $lat,
      ":user_id" => $user_id,
      ":rso_id" => $rso_id,
      ":accessibility" => $accessibility,
      ":categories" => $categories,
      ":uni_id" => $uni_id,
      ":status" => $status
    ));

    // redirect to event page.
    if (empty($result)) {

      go_home();

    }

    $result = $result[0];

    set_message('Succesfully changed ' . $name);
    redirect_entity_view('event', $event_id);

  }

  public function destroy ($params) {

    // user who is signed in.
    global $user;

    $uid = $user->id;

    $query = $this->destroy_query;

    // $this->db->prepared_query($query, array());
    
    // redirect to page of events, or user page. something...
     
  }

  public function make ($params) {

    global $configs;

    // current user logged in.
    global $user;

    if ($user == NULL) {

      go_home();

    }

    $uni_id = isset($user->uni_id)? $user->uni_id: 0;

    $geo_query = "SELECT location, lat, lon FROM universities WHERE uni_id = :uni_id";

    // get geolocation info from a university
    $geo_info =  $this->db->prepared_query($geo_query, array(":uni_id" => $uni_id));

    if (isset($geo_info)) {

      $geo_info = $geo_info[0];
      $location = $geo_info['location'];
      $lat = $geo_info['lat'];
      $lon = $geo_info['lon'];

    }

    $id = $params[1];

    $site_root = $configs['site_root'];
    
    include get_entity_partial('event', 'make');

    return $template;

  }

  public function edit ($params) {

    global $configs;

    $site_root = $configs['site_root'];

    // current user
    global $user;

    if ($user == NULL) {

      go_home();

    }

    $user_id = $user->user_id;

    $role = $user->role;

    $uni_id = isset($user->uni_id)? $user->uni_id: 0;

    $event_id = $params[1];

    $site_root = $configs['site_root'];

    $query = $this->view_query;

    $result = $this->db->prepared_query($query, array(
      ":event_id" => $event_id,
      ":user_id" => $user_id,
      ":role" => $role,
      ":uni_id" => $uni_id,
    ));

    if (empty($result)) {

      include get_entity_partial('event', '404');

      return $template;

    }

    $result = $result[0];

    $name = isset($result['name'])? $result['name']: "";
    $location = isset($result['location'])? $result['location']: "";
    $description = isset($result['description'])? $result['description']: "";

    // calculate duration from start and end time.
    $start_time = isset($result['start_time'])? $result['start_time']: "";
    $end_time = isset($result['end_time'])? $result['end_time']: "";
    $telephone = isset($result['telephone'])? $result['telephone']: "";
    $email = isset($result['email'])? $result['email']: "";
    $lon = isset($result['lon'])? $result['lon']: "";
    $lat = isset($result['lat'])? $result['lat']: "";
    $rso_id = isset($result['rso_id'])? $result['rso_id']: "";
    $rso_name = isset($result['rso_name'])? $result['rso_name']: "";
    $status = isset($result['status'])? $result['status']: "PND";
    // if the event is rso created, check 'is-rso' input
    // and leave 'not-rso' unchecked
    if ($rso_id > 0) {
      $is_rso = "checked=\"checked\"";
      $not_rso = "";
      $hidden_rso = "";
    }
    // otherwise, check 'not-rso' and leave 'is-rso' unchecked
    else {
      $is_rso = "";
      $not_rso = "checked=\"checked\"";
      $hidden_rso = "style=\"display:none\"";
    }

    if ($status == "PND") {

      $is_pnd = "checked=\"checked\"";
      $is_act = "";

    }
    else {

      $is_pnd = "";
      $is_act = "checked=\"checked\"";

    }
    
    include get_entity_partial("event", "edit");

    return $template;

  }

  public function drop ($params) {

    global $configs;

    global $user;

    if ($user == NULL) {

      go_home();

    }

    $id = $params[1];

    $site_root = $configs['site_root'];

    // check if the user deleting this is has the permissions. 

    include get_entity_partial("event", "drop");

    return $template;

  }

  public function attend_event ($params) {

    global $user;

    $user_id = $user->user_id;
    $uni_id = isset($user->uni_id)? $user->uni_id: 0;
    $role = $user->role;

    $event_id = $params['event_id'];

    $query = $this->attend_event_query;

    $result = $this->db->prepared_query($query, array(
      ":event_id" => $event_id,
      ":user_id" => $user_id,
      ":uni_id" => $uni_id,
    ));

    die();

  }

  public function unattend_event ($params) {

    global $user;

    if ($user == NULL) {

      die();

    }

    $user_id = $user->user_id;

    $uni_id = isset($user->uni_id)? $user->uni_id: 0;

    $event_id = $params['event_id'];

    $query = $this->unattend_event_query;

    $result = $this->db->prepared_query($query, array(
      ":event_id" => $event_id,
      ":user_id" => $user_id
    ));

    die();

  }

  public function rate_event ($params) {

    global $user;

    $user_id = $user->user_id;
    $uni_id = isset($user->uni_id)? $user->uni_id: 0;
    $event_id = $params['event_id'];
    $rating = isset($params['rating'])? $params['rating']: NULL;

    $query = $this->rate_event_query;

    $result = $this->db->prepared_query($query, array(
      ":event_id" => $event_id,
      ":user_id" => $user_id,
      ":rating" => $rating
    ));

    $result = $result[0];

    // print out the new user rating and events total rating in JSON.
    echo json_encode($result);

    die();

  }

  public function event_participants_json ($params) {

    $event_id = isset($params['event_id'])? $params['event_id']: 0;

    $query = $this->event_participants_query;

    $results = $this->db->prepared_query($query, array(
      ":event_id" => $event_id,
    ));

    echo json_encode($results);

  }

  // search for events and print results out to JSON
  public function get_events_json ($params) {

    global $user;

    if ($user == NULL) {

      die();

    }

    $user_id = $user->user_id;

    $scope = isset($params['scope']) ? $params['scope']: "my-uni";

    if ($scope == "my-uni") {

      $uni_id = isset($user->uni_id)? $user->uni_id: 0;

    }
    else if ($scope == "other-uni") {

      $uni_id = isset($params['uni_id'])? $params['uni_id']: 0;

    }

    $sort_by = isset($params['sort_by']) ? $params['sort_by'] : "date";

    $accessibility = isset($params['accessibility'])? $params['accessibility']: 'PUB';

    $query = $this->search_events_query;

    $results = $this->db->prepared_query($query, array(
      ":user_id" => $user_id,
      ":uni_id" => $uni_id,
      ":scope" => $scope,
      ":sort_by" => $sort_by,
      ":accessibility" => $accessibility
    ));

    if (empty($results)) {

      die();

    }

    echo json_encode($results);

    die();

  }


  public function get_rso_events_json ($params) {

    global $user;

    if ($user == NULL) {

      die();

    }

    $rso_id = isset($params['rso_id'])? $params['rso_id']: 0;

    $query = $this->get_rso_events_query;

    $results = $this->db->prepared_query($query, array(
      ":rso_id" => $rso_id,
    ));

    echo json_encode($results);

    die();

  }

}

?>

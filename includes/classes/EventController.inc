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

  private $get_categories_query;
  private $get_categories_like_query;

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

    $this->get_categories_query = $get_categories_query;

    $this->get_categories_like_query = $get_categories_like_query;

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

    // we have to examine the users role to determine if the university can be chosen
    // (such as when SA, which has no university imports UCF events) or if it is based on users affiliation with
    // the school
    $role = $user->role;

    $name = check_field($params,'name', NULL);
    $description = check_field($params,'description', NULL);
    $location = check_field($params,'location', NULL);

    // calculate duration from start and end time.
    $start_time = check_field($params,'start-time', NULL);
    $end_time = check_field($params,'end-time', NULL);
    $telephone = check_field($params,'telephone', "");
    $email = check_field($params,'email', "");
    $lon = check_field($params,'lon', NULL);
    $lat = check_field($params,'lat', NULL);

    $categories = check_field($params,'categories', ""); 

    $cat_arr = explode(",", $categories);

    $accessibility = check_field($params,'accessibility', NULL); 

    // allow user to set the rso from the list of rso's they administrate.
    // If none was selected, set to 0 to set event to PND -> pending
    $rso_id = check_field($params,'rso-id', 0); 

    // Super Admin can choose university (like when they import UCF events)
    if ($role == "SA") {
      $uni_id = check_field($params, 'uni-id', 0); 
    } 
    // always use the users university if they are not SA
    else {
      $uni_id = isset($user->uni_id)? $user->uni_id: 0; 
    }

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
      ":user_id" => $user_id,
      ":rso_id" => $rso_id,
      ":uni_id" => $uni_id,
    ));

    // something went wrong. PDO most likely has set error message
    if (empty($result)) {

      // just go home for now. change to go_back once we are positive this works.
      set_message("Something went wrong creating your event", "error");
      go_back();
      die();

    }
    else if (isset($result[0]["_err_msg"])) {

      $msg = $result[0]["_err_msg"];
      set_message($msg, "error");
      go_back();
      die();

    }

    // get id of event just created.
    $result = $result[0];

    // the insert was succesful, so we can use the new event id (returned by procedure).
    $event_id = $result['event_id'];

    if (!empty($categories)) {
      // We will not know how many categories are being added, so I am
      // building the query string here in the controller.
      $category_query = "INSERT INTO categorized_as (event_id, label) VALUES ";

      foreach ($cat_arr as $key => $category) {

        // event id is provided by the databases auto-increment, so it is safe to concatenate here.
        $category_query .= "($event_id, :cat_$key ),";

        // name the keys for the prepared query below
        $cat_arr["cat_".$key] = $cat_arr[$key];
        unset($cat_arr[$key]);

      }

      // replace last comma with semi-colon
      $category_query = substr($category_query, 0, -1) . ";";

      $this->db->prepared_query($category_query, $cat_arr);

    }

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
      die();

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

    $uni_id = check_field($result, 'uni_id', 0);

    $uni_name = check_field($result, 'uni_name', "");

    // if the event was created by an RSO allow the following.
    $rso_id = isset($result['rso_id'])? $result['rso_id']: "";
    $rso_name = isset($result['rso_name'])? $result['rso_name']: "";

    $accessibility = isset($result['accessibility'])? $result['accessibility']: "";

    if ($accessibility == "PUB") {

      $access_msg = "This event is public to everyone";

    }
    else if ($accessibility == "PRI") {

      $access_msg = "This event is exclusive to students at ";

    }
    else if ($accessibility == "RSO") {

      $access_msg = "This event is exclusive to members of " . $rso_name;

    }

    $has_pic = entity_has_pic("events", $event_id)? 1: 0;

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
    $name = check_field($params,'name', NULL);
    $description = check_field($params,'description', NULL);
    $location = check_field($params,'location', NULL);

    // calculate duration from start and end time.
    $start_time = check_field($params,'start-time', NULL);
    $end_time = check_field($params,'end-time', NULL);
    $telephone = check_field($params,'telephone', "");
    $email = check_field($params,'email', "");
    $lon = check_field($params,'lon', NULL);
    $lat = check_field($params,'lat', NULL);

    $categories = check_field($params,'categories', ""); 

    // for faster lookup, the category labels are used as keys.
    $categories = array_flip(explode(",", $categories));

    $accessibility = check_field($params,'accessibility', NULL); 

    // allow user to set the rso from the list of rso's they administrate.
    // If none was selected, set to 0 to set event to PND -> pending
    $rso_id = check_field($params,'rso-id', 0); 

    $status = check_field($params, 'status', 'PND');

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
      ":status" => $status
    ));

    // redirect to event page.
    if (empty($result)) {

      go_home();

    }
    else if (isset($result[0]["_err_msg"])) {

      $msg = $result[0]["_err_msg"];
      set_message($msg, "error");
      go_back();
      die();

    }

    // get existing categories on event
    $current_categories = $this->db->prepared_query(
      "SELECT label FROM categorized_as WHERE event_id = :event_id",
      array(":event_id" => $event_id)
    );


    // categories not added for this event previously
    $new_cat = array();

    // categories that are not in this update request. 
    $gone_cat = array();

    // go through the existing categories
    foreach ($current_categories as $key => $category) {

      // the existing category label in databse
      $extant_label = $category['label'];

      // if the categories we want to add are already in the db
      if (isset($categories[$extant_label])) {

        // we do not need to insert an existing category
        unset($categories[$extant_label]);

        // anything not unset in this way will be a new category

      }
      // if the category in the DB is not found in our current update request
      // then it must be deleted from the db.
      else {

        $gone_cat[] = $extant_label;

      }

    }

    // all categories submitted to the update request that already exist in DB have been
    // removed from the categories to be inserted, leaving only new categories
    
    // there may be an empty string added as a category. unset it to make the list empty
    // if there are no categories on the event.
    unset($categories['']);
    $categories = array_flip($categories);
    $new_cat = $categories;

    if (!empty($new_cat)) {

      $create_cat = "INSERT INTO categorized_as (event_id, label) VALUES ";

      foreach($new_cat as $key => $category) {

        $create_cat .= "($event_id, :cat_$key),";

        // replace keys for the purposes of prepared statement
        $new_cat[":cat_" . $key] = $new_cat[$key];

        unset($new_cat[$key]);

      }

      $create_cat = substr($create_cat, 0, -1) . ";";

      $this->db->prepared_query($create_cat, $new_cat);

    }

    if (!empty($gone_cat)) {

      $destroy_cat = "DELETE FROM categorized_as WHERE FIND_IN_SET(label, :categories);";

      $this->db->prepared_query($destroy_cat, array(
        ":categories" => implode("," , $gone_cat)
      ));

    }

    $result = $result[0];

    set_message('Succesfully changed ' . $name);

    redirect_entity_view('event', $event_id);

  }

  public function destroy ($params) {

    // user who is signed in.
    global $user;

    $uid = $user->user_id;

    $event_id = $params[1];

    $password = isset($params['password'])? $params['password'] : NULL;

    $user_controller = new UserController();

    $can_destroy = $user_controller->check_credentials($uid, $password);

    if ($password == NULL || !$can_destroy) {

      $msg = "The e-mail or password provided is incorrect"; 
      set_message($msg, "error");
      go_home();
      die();

    }

    $query = $this->destroy_query;

    $this->db->prepared_query($query, array(":event_id" => $event_id));
    
    set_message("You have deleted your event succesfully");

    // redirect to page of events, or user page. something...
    redirect_entity_view("user", $uid);
     
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

    if (!empty($geo_info)) {

      $geo_info = $geo_info[0];
      $location = $geo_info['location'];
      $lat = $geo_info['lat'];
      $lon = $geo_info['lon'];

    }
    else {
      // set some default for SA which has no uni association
      $location = "4000 Central Florida Blvd, Orlando, FL 32816"; 
      $lat = 28.602427;
      $lon = -81.200060;

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

    $name = check_field($result, 'name', "");
    $location = check_field($result, 'location', "");
    $description = check_field($result, 'description', "");

    $start_time = check_field($result,'start_time', "");
    $end_time = check_field($result,'end_time', "");
    $telephone = check_field($result,'telephone', "");
    $email = check_field($result,'email', "");
    $lon = check_field($result,'lon', "");
    $lat = check_field($result,'lat', "");
    $rso_id = check_field($result,'rso_id', "");
    $rso_name = check_field($result,'rso_name', "");
    $status = check_field($result,'status', "PND");
    $accessibility = check_field($result,'accessibility', "PND");

    $current_categories = $this->db->prepared_query(
      "SELECT label FROM categorized_as WHERE event_id = :event_id",
      array(":event_id" => $event_id)
    );

    $cat_json = json_encode($current_categories);

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

    $acc_pub = "";
    $acc_pri = "";
    $acc_rso = "";

    if ($accessibility == "PUB") {

      $acc_pub = "selected=\"selected\"";

    } 
    else if ($accessibility == "PRI") {

      $acc_pri = "selected=\"selected\"";

    } 
    else if ($accessibility == "RSO") {

      $acc_rso = "selected=\"selected\"";

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

      $uni_id = check_field($params,'uni_id', 0);

    }

    $sort_by = check_field($params, 'sort_by', "date");

    $accessibility = check_field($params, 'accessibility', 'PUB');

    $categories = check_field($params, 'categories', '');

    $query = $this->search_events_query;

    $results = $this->db->prepared_query($query, array(
      ":user_id" => $user_id,
      ":uni_id" => $uni_id,
      ":scope" => $scope,
      ":sort_by" => $sort_by,
      ":accessibility" => $accessibility,
      ":categories" => $categories
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

  public function get_event_categories_json ($params) {

    $event_id = check_field($params, "event_id", 0);

    $query = $this->get_categories_query;

    $results = $this->db->prepared_query($query, array(
      ":event_id" => $event_id
    ));

    if (empty($results)) {

      echo '{"fail": true}';

      die();

    }

    echo json_encode($results);

    die();

  }

  // used to autocomplete categories
  public function get_event_categories_like_json ($params) {

    $label = check_field($params, 'label', "");

    $query = $this->get_categories_like_query;

    $results = $this->db->prepared_query($query, array(
      ":label" => $label
    ));

    if (empty($results)) {

      echo '[]';

      die();

    }

    echo json_encode($results);

    die();

  }

}

?>

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
    $this->view_all_query = $view_all_query;
    $this->update_query = $update_query;
    $this->destroy_query = $destroy_query;

  }

  public function create($params) {

    global $configs;

    $site_root = $configs['site_root'];

    global $user;

    // if no user is logged in
    if (!isset($user)) {

      // go to home page.
      go_home();

    }

    $role = $user->role;

    if ($role !== "SA" || $role !== "ADM") {

      // simply redirect home.
      go_home();

    }

    $event_name = isset($params['title'])? $params['title']: "";
    $event_loc_name = isset($params['loc-name'])? $params['loc-name']: "";
    $event_street = isset($params['street'])? $params['street']: ""; 
    $event_zip = isset($params['zip'])? $params['zip']: "";
    $event_city = isset($params['city'])? $params['city']: "";
    $state = isset($params['state'])? $params['state']: ""; 

    // calculate duration from start and end time.
    $event_duration = isset($params['duration'])? $params['duration']: "";
    $event_time = isset($params['time'])? $params['time']: "";
    $event_long = isset($params['lon'])? $params['lon']: "";
    $event_lat = isset($params['lat'])? $params['lat']: "";
    $event_org = isset($params['organization'])? $params['organization']: "";
    $hosted_by_rso;

    $query = $this->create_query;

    $this->db->prepared_query($query, array(
      ":event_name" => $event_name,
      ":event_loc_name" => $event_loc_name,
      ":event_street" => $event_street,
      ":event_zip" => $event_zip,
      ":event_city" => $event_city,
      ":state" => $state,
      ":event_duration" => $event_duration,
      ":event_time" => $event_time,
      ":event_long" => $event_long,
      ":event_lat" => $event_lat,
      ":event_org" => $event_org,
    ));

    // just go home for now... figure out a redirect later.
    go_home();

  }

  public function view($params) {

    global $configs;

    $site_root = $configs['site_root'];

    global $user;

    // if no user is set, redirect to home-page
    if (!isset($user)) {

      go_home();

    }

    $role = $user->role;

    $event_id = $params[1];

    $query = $this->view_query;

    $result = $this->db->prepared_query($query, array(
      ":event_id" => $event_id,
    ));

    // no event was found with this event id
    if (empty($result)) {

      // return a the 404 partial associated with events.
      include get_entity_partial('event', '404');

      return $template;

    }

    // get first row from result.
    $result = $result[0];

    $event_name = isset($result['event_name'])? $result['event_name']: "";
    $event_loc_name = isset($result['event_loc_name'])? $result['event_loc_name']: "";
    $event_street = isset($result['event_street'])? $result['event_street']: ""; 
    $event_zip = isset($result['event_zip'])? $result['event_zip']: "";
    $event_city = isset($result['event_city'])? $result['event_city']: "";
    $state = isset($result['state'])? $result['state']: ""; 

    // calculate duration from start and end time.
    $event_duration = isset($result['event_duration'])? $result['event_duration']: "";
    $event_time = isset($result['event_time'])? $result['event_time']: "";
    $event_long = isset($result['event_long'])? $result['event_long']: "";
    $event_lat = isset($result['event_lat'])? $result['event_lat']: "";
    $event_org = isset($result['event_org'])? $result['event_org']: "";

    // return the partial using the variable defined above.
    include get_entity_partial('event', 'view');

    return $template;

  }

  public function view_all($params) {

    global $configs;

    $site_root = $configs['site_root'];

    global $user;

    $events = array(); 

    $range = explode("-", $params[1]);

    $start = $range[0];
    $end= $range[1];

    $query = $this->view_all_query;

    $results = $this->db->prepared_query($query, array(
      ":start" => $start,
      ":end" => $end,
    ));

    if (empty($results)) {
      $events = FALSE;
    }  

    $events = $results;

    include get_entity_partial('event', 'view_all');

    return $template;
    
  }

  public function update($params) {

    global $configs;

    $site_root = $configs['site_root'];

    // user who is signed in.
    global $user;

    $uid = $user->id;

    $role = $user->role;

    if ($role !== "SA" || $role !== "ADM") {

      // simply redirect home.
      go_home();

    }

    $event_name = isset($params['event-name'])? $params['event-name']: "";
    $event_loc_name = isset($params['loc-name'])? $params['loc-name']: "";
    $event_street = isset($params['street'])? $params['street']: ""; 
    $event_zip = isset($params['zip'])? $params['zip']: "";
    $event_city = isset($params['city'])? $params['city']: "";
    $state = isset($params['state'])? $params['state']: ""; 

    // calculate duration from start and end time.
    $event_duration = isset($params['duration'])? $params['duration']: "";
    $event_time = isset($params['time'])? $params['time']: "";
    $event_long = isset($params['lon'])? $params['lon']: "";
    $event_lat = isset($params['lat'])? $params['lat']: "";
    $event_org = isset($params['organization'])? $params['organization']:

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

  public function make ($params) {

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
    
    include get_entity_partial('event', 'make');

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

  public function drop ($params) {

    global $configs;

    $id = $params[1];

    $site_root = $configs['site_root'];

    // check if the user deleting this is has the permissions. 

    include get_entity_partial("event", "drop");

    return $template;

  }

}

?>

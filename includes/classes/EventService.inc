<?php
// Takes events for events.ucf.edu/feed.json and puts new events into the
// the events table. The task is run through a cron job

class EventService {

  // instance of curl object
  private $curl;

  // check if there is a problem with request (ucf server down, timeout etc.)
  private $error = FALSE;

  // instance of event controller to perform inserts
  private $event_controller;

  // used for event data
  private $events_url = "https://events.ucf.edu/this-week/feed.json";

  private $event_data;

  // used for coordinates
  private $locations_url = "https://map.ucf.edu/locations.json";

  // the locations of UCF buildings and places provided by the URL above
  private $locations;

  public function __construct() {

    $this->init_curl();

    $this->event_controller = new EventController();

  }

  private function init_curl() {

    $curl = &$this->curl;
    $curl = curl_init();
    curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($curl, CURLOPT_HEADER, 0);

  }

  private function set_curl_url ($url) {

    $curl = &$this->curl;

    curl_setopt($curl, CURLOPT_URL, $url);

  }

  // requests UCF's locations KML file and parses it for coordinates
  private function request_ucf_locations() {

    $curl = $this->curl;

    $this->set_curl_url($this->locations_url);

    $output = curl_exec($curl);

    $locations_json = json_decode($output);

    // I was surprised to find they had so many types of locations documented
    // (includes emergency call stations, handicapped parking). To narrow it down,
    // I am using these location object types
    $allowed_types = array(
      'Building' => True,
      'Location' => True,
    );

    // this will contain the final, parsed locations, keyed by location name
    $locations = array();

    foreach ($locations_json as $location) {

      // skip this location if it is not the type we want.
      if (!isset($allowed_types[$location->object_type])) {

        continue;

      }

      // some types use the title field as an identifier, others use description.
      $name = isset($location->title)? $location->name: NULL;

      $lat = $location->googlemap_point[0];
      $lon = $location->googlemap_point[1];

      $coords = array(
        "lat" => $lat,
        "lon" => $lon,
      );

      $locations[$name] = $coords; 

    }

    $this->locations = $locations;

  }

  private function request_event_data() {

    $curl = $this->curl;

    $this->set_curl_url($this->events_url);

    $output = curl_exec($curl);

    $this->event_data = json_decode($output);
    
  }

  private function is_error () {

    return $this->error;

  }

  private function insert_events() {

    global $user;

    $user_id = $user->user_id;

    $this->request_ucf_locations();

    $this->request_event_data();
    
    $events = $this->event_data;

    $skipped_any = FALSE;

    foreach ($events as $event) {

      // human readable name
      $location = $event->location;

      // some of these events don't appear to correlate to anything in
      // locations.json... for that, I'll gracefully skip them.
      if (!isset($this->locations[$location])) {

        // let the super admin know
        $skipped_any = TRUE;
        continue;

      }

      // closest thing to a unique identifier for their locations...
      $coords = $this->locations[$location];

      $lat = $coords['lat'];
      $lon = $coords['lon'];

      // setting the keys to use our data schema
      $name = $event->title;

      $description = $event->description;

      $start_time = date("Y-m-d H:i:s", strtotime($event->starts));
      $end_time = date("Y-m-d H:i:s", strtotime($event->ends));
      $categories = $event->category;
      $email = $event->contact_email;
      $telephone = $event->contact_phone;

      // these have no counter parts, so I will put them
      // as public, active, having no RSO, and being at UCF
      $accessibility = "PUB";
      $status = "ACT";
      $rso_id = 0;
      $uni_id = 1;

      // hyphens are used in the keys to simulate html form name attributes
      $params = array(
        "name" => $name,
        "description" => $description,
        "location" => $location,
        "lat" => $lat,
        "lon" => $lon,
        "start-time" => $start_time,
        "end-time" => $end_time,
        "categories" => $categories,
        "email" => $email,
        "telephone" => $telephone,
        "accessibility" => $accessibility,
        "status" => $status,
        "rso-id" => $rso_id,
        "uni-id" => $uni_id,
      );

      $this->event_controller->create($params);

    }

    $msg = "You have succesfully imported the events.";

    if ($skipped_any) {

      $msg .= " Some events could not be imported. Check the sources for consistency.";

    }

    set_message($msg);

    // go to SA profile
    redirect_entity_view("user", $user_id);

    curl_close($this->curl);

  }

  public function import_ucf_events () {

    $this->insert_events();

    if ($this->is_error()) {

      set_message("Something went wrong importing the events");
      go_back();
      die();

    }

  }

}

?>

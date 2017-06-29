<?php
// Takes events for events.ucf.edu/feed.json and puts new events into the
// the events table. The task is run through a cron job
include_once "EntityController.inc";
include_once "EventController.inc";

class EventService {

  // instance of curl object
  private $curl;

  // requested json data;
  private $json;

  // the json string converted to php associative array
  private $data;

  // check if there is a problem with request (ucf server down, timeout etc.)
  private $error = FALSE;

  // instance of event controller to perform inserts
  private $event_controller;

  private $url = "https://events.ucf.edu/this-week/feed.json";

  public function __construct() {

    $this->init_curl();

  }

  private function init_curl() {

    $curl = &$this->curl;
    $curl = curl_init();
    $url = $this->url;

    curl_setopt($curl, CURLOPT_URL, $this->url);
    curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($curl, CURLOPT_HEADER, 0);

  }

  private function request_data() {

    $curl = $this->curl;

    $output = curl_exec($curl);

    $this->json = $output;
    $this->data = json_decode($output);
    
  }

  private function is_error () {

    return $this->error;

  }

  private function insert_events() {

    $this->request_data();

    $events = $this->data;

    foreach ($events as $event) {

      var_dump($event);

    }

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

$event = new EventService();
$event->import_ucf_events();

?>

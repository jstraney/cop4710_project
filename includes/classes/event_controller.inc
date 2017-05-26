<?php

include_once "entity_controller.inc";
include "comment_controller.inc";

class EventController extends EntityController {

  private $create_query = "";
  private $view_query = "";
  private $view_all_query = "";
  private $update_query = "";
  private $delete_query = "";

  public function __construct() {

    $this->pdo = new SupPDO();

  }

  public function create($params) {


  }

  public function view($params) {

  }

  public function view_all($params) {

  }

  public function update($params) {

  }

  public function delete($params) {

  }

  public function get_create_form ($params) {

    global $configs;

    $site_root = $configs['site_root'];

    $form = '<form method="POST" action="action.php">';
    // include google maps API 
    $form .= '<script type="text/javascript" src="https://maps.google.com/maps/api/js?key=AIzaSyBGfGPKeqJZRoQSInY5LXqvZt7lZwihCPA&libraries=places"></script>';
    // include the jquery location picker plugin
    $form .= '<script src="'.$site_root.'/js/lib/jquery-locationpicker-plugin/locationpicker.jquery.js"></script>';
    $form .= '<h3>Create Event</h3>';
    $form .= '<label>Event Title</label>';
    $form .= '<input id="name" type="text" name="name" value=""></input>';
    $form .= '<label>Location</label>';
    $form .= '<input id="location" type="text" name="location" value=""></input>';
    $form .= '<div id="map"></div>';
    $form .= '<input style="display:none "id="lon" type="text" name="lon" value=""></input>';
    $form .= '<input style="display:none" id="lat" type="text" name="lat" value=""></input>';
    $form .= '<label>Time</label>';
    $form .= '<input id="org" type="text" name="org" value=""></input>';
    $form .= '<input id="type" type="hidden" name="type" value="event"></input>';
    $form .= '<input id="submit" type="submit" name="submit" value="Create Event!"></input>';
    $form .= '<script>app.makePicker()</script>';

    return $form;

  }

  public function get_edit_form ($params) {

    $form = "<h3>Edit Event</h3>";
    $form .= '<label>Event Title</label>';
    $form .= '<input id="name" type="text" name="name" value=""></input>';
    $form .= '<label>Location</label>';
    $form .= '<p>{jquery location picker widget will go here}</p>';
    $form .= '<label>Time</label>';
    $form .= '<input id="org" type="text" name="org" value=""></input>';
    $form .= '<input id="type" type="hidden" name="type" value="event"></input>';
    $form .= '<input id="submit" type="submit" name="submit" value="Create Event!"></input>';

    return $form;

  }

  public function get_delete_form ($params) {

    $form = "<h3>Delete Event</h3>";
    $form .= "<p>Deleting events is a permanent action.";
    $form .= "Are you sure you want to delete this event?</p>";
    $form .= '<input id="type" type="hidden" name="type" value="event"></input>';
    $form .= '<input id="submit" type="submit" name="submit" value="Delete Event"></input>';

  }

}

?>

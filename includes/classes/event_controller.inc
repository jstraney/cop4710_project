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

  public function get_create_form () {

    $text = "<h3>Create Event</h3>";
    $form = '<label>Event Title</label>';
    $form .= '<input id="name" type="text" name="name" value=""></input>';
    $form .= '<label>Location</label>';
    $form .= '<p>{jquery location picker widget will go here}</p>';
    $form .= '<label>Time</label>';
    $form .= '<input id="org" type="text" name="org" value=""></input>';
    $form .= '<input id="type" type="hidden" name="type" value="event"></input>';
    $form .= '<input id="submit" type="submit" name="submit" value="Create Event!"></input>';

    return $form;

  }

  public function get_edit_form () {

    $text = "<h3>Edit Event</h3>";
    $form = '<label>Event Title</label>';
    $form .= '<input id="name" type="text" name="name" value=""></input>';
    $form .= '<label>Location</label>';
    $form .= '<p>{jquery location picker widget will go here}</p>';
    $form .= '<label>Time</label>';
    $form .= '<input id="org" type="text" name="org" value=""></input>';
    $form .= '<input id="type" type="hidden" name="type" value="event"></input>';
    $form .= '<input id="submit" type="submit" name="submit" value="Create Event!"></input>';

    return $form;

  }

  public function get_delete_form () {

    $text = "<h3>Delete Event</h3>";
    $text .= "<p>Deleting events is a permanent action.";
    $text .= "Are you sure you want to delete this event?</p>";
    $form .= '<input id="type" type="hidden" name="type" value="event"></input>';
    $form .= '<input id="submit" type="submit" name="submit" value="Delete Event"></input>';

  }

}

?>

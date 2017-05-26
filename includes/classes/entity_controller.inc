<?php

include_once "pdo.inc";

// the controller stores its own query strings and uses a pdo object to execute the
// queries on the database.
abstract class EntityController {

  // pdo object
  protected $pdo;

  // entity type
  protected $type;

  // each of the methods below takes an associative array called $params

  // the create, view, update, delete methods pass arguments to the PDO

  // inserts an entity into the proper table
  abstract public function create($params);

  // returns the view of a single entity
  abstract public function view($params);

  // returns the view of multiple entities in a list 
  abstract public function view_all($params);

  // updates an entity based on id
  abstract public function update($params);

  // deletes an entity record from the table.
  abstract public function delete($params);

  // each entity provides a form for the actions
  abstract public function get_create_form($params);
  abstract public function get_edit_form($params);
  abstract public function get_delete_form($params);

}
?>

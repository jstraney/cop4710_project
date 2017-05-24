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

  // inserts an entity into the property table
  abstract public function create($params);

  // selects a single entity of $type using $id
  // returns an associative array of its fields, as well as rendering instructions
  abstract public function view($params);

  // selects a range of entities
  abstract public function view_all($params);

  // updates an entity based on id
  abstract public function update($params);

  // deletes an entity record from the table.
  abstract public function delete($params);

  // each entity provides a form for the actions
  abstract public function get_create_form();
  abstract public function get_edit_form();
  abstract public function get_delete_form();

}
?>

<?php
// and passes parameters (e.g. ['type' => 'user', 'id'=> '12', 'action' => 'update'].
// to the PDO object to perform a query. It's useful to separate the controller from
// the PDO because we may want to perform queries on our database that do not involve
// CRUD operations.
class EntityController {

  // pdo object
  private $pdo;

  public function __construct() {

    // initiate the PDO


  }

  // the create, view, update, delete methods pass arguments to the PDO
  public function create($type, $fields) {

  }

  // selects a single entity of $type using $id
  // returns an associative array of its fields, as well as rendering instructions
  public function view($type, $id) {

    $render_array = array();

    return $render_array;

  }

  // selects a range of entities of $type. uses an array of size 2 $pages
  // to select a range of entities
  public function view_all($type, $pages) {

    $render_array_list = array();

    // use pdo to get list of entities
    // append entity render array to list

    return $render_array;

  }

  public function update($type, $id, $fields) {

  }

  public function delete($type, $id) {

  }

}
?>

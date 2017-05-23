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

  public function view($type, $id) {

  }

  public function view_all($type) {

  }

  public function update($type, $id, $fields) {

  }

  public function delete($type, $id) {

  }

}
?>

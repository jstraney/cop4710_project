<?php


// the controller stores its own query strings and uses a pdo object to execute the
// queries on the database.
abstract class EntityController {

  // db object
  protected $db;

  // entity type
  protected $type;

  public function create ($params) {

  }

  // gets readable fields from entity. is rendered by an adapter
  public function read ($params) {

  }

  public function update ($params) {

  }

  public function destroy ($params) {

  }

  public function new ($params) {

  }

  public function edit ($params) {

  }

  public function delete ($params) {

  }
}
?>

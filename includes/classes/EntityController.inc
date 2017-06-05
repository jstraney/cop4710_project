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


  public function update ($params) {

  }

  public function destroy ($params) {

  }

  public function make($params) {

  }

  public function edit ($params) {

  }

  public function drop ($params) {

  }
}
?>

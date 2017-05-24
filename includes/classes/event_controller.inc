<?php

include "entity_controller.inc";
include "pdo.inc";

class EventController extends EntityController {

  protected $pdo;
  protected $type = "user";

  private $create_query = "";
  private $view_query = "";
  private $view_all_query = "";
  private $update_query = "";
  private $delete_query = "";

  public function __construct() {

    $this->pdo = new SupPDO(); 

  }

  public function create($params) {
    // get fields from params

  }

  public function view($params) { 

    // get id from params 
    if (isset($params['id'])) {
      $id = $params['id'];
    }
  
  }

  public function view_all($params) {

    if (isset($params['page_range'])) {
      $pages = $params['page_range'];
    }

  }

  public function update($params) {

    // get the id and fields.
    if (isset($params['id'])) {
      $id = $params['id'];
    }

    if (isset($params['fields'])) {
      $fields = $params['fields'];
    }

    // get each field from fields

  }

  public function delete($params) {

    if (isset($params['id'])) {
      $id = $params['id'];
    }

  }

}

?>

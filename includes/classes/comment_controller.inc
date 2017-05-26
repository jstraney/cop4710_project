<?php

include_once "entity_controller.inc";

class CommentController extends EntityController {

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

    $form = '';

    return $form;
  }

  public function get_edit_form ($params) {

    $form = '';

    return $form;

  }

  public function get_delete_form ($params) {

    $form = "";

    return $form;

  }

}

?>

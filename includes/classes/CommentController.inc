<?php

class CommentController extends EntityController {

  private $create_query = "";
  private $view_query = "";
  private $view_all_query = "";
  private $update_query = "";
  private $delete_query = "";

  public function __construct() {

    $this->db = new Database();

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

  public function new ($params) {

    include get_entity_partial('comment','new');

    return $template;

  }

  public function edit ($params) {

    include get_entity_partial('comment','edit');

    return $template;

  }

  public function delete ($params) {

    include get_entity_partial('comment','delete');

    return $template;

  }

}

?>

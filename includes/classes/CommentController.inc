<?php
// the comment controller will serve mostly as a restful API endpoint
// for JSON data.
class CommentController extends EntityController {

  private $create_query = "";
  private $read_query = "";
  private $update_query = "";
  private $delete_query = "";
  private $comment_event_query = "";

  public function __construct() {

    $this->db = new Database();

    $this->create_query = $create_query;
    $this->update_query = $update_query;
    $this->delete_query = $delete_query;

  }

  public function create($params) {


  }

  // different from the 'view' action because it simply
  // returns a json response.
  public function read($params) {

  }

  public function update($params) {

  }

  // gets JSON data for all comments on an event.
  // used for polling and updating an events page.
  public function get_event_comments_json ($params) {

  }

}

?>

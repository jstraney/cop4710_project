<?php
// the comment controller will serve mostly as a restful API endpoint
// for JSON data.
class CommentController extends EntityController {

  private $create_query = "";
  private $read_query = "";
  private $update_query = "";
  private $destroy_query = "";
  private $comments_event_query = "";

  public function __construct() {

    $this->db = new Database();

    include get_entity_partial('comment', 'queries');

    $this->create_query = $create_query;
    $this->update_query = $update_query;
    $this->destroy_query = $destroy_query;

    $this->comments_event_query = $comments_event_query;

  }

  public function create($params) {

    global $user;

    if ($user == NULL) {

      die();

    }

    $content = $params['content'];
    $event_id = isset($params['event_id'])? $params['event_id']: 0;
    $user_id = $user->user_id;
    $uni_id = isset($user->uni_id)? $user->uni_id: 0;

    $query = $this->create_query;

    $result = $this->db->prepared_query($query, array(
      ":event_id" => $event_id,
      ":user_id" => $user_id,
      ":uni_id" => $uni_id,
      ":content" => $content,
    ));

    if (empty($result)) {

      // echo failure in json response
      echo '{"status": false}';

      die();

    }

    // echo success in json response
    echo '{"status": true, "content": "'.$content.'"}';
    die();

  }

  public function update($params) {

    global $user;

    // die if there is no user logged in.
    if ($user == NULL) {

      // failure
      echo '{"status":"false"}';
      die();

    }

    $user_id = $user->user_id;

    $comment_id = isset($params['comment_id'])? $params['comment_id']: 0;

    $content = isset($params['content'])? $params['content']: "";

    $query = $this->update_query;

    $result = $this->db->prepared_query($query, array(
      ":content" => $content, 
      ":comment_id" => $comment_id, 
      ":user_id" => $user_id, 
    ));




  }

  public function destroy($params) {

    global $user;

    if ($user == NULL) {

      // failure
      echo '{"status":"false"}';
      die();

    }

    $user_id = $user->user_id;

    $comment_id = isset($params['comment_id'])? $params['comment_id']: 0;

    $query = $this->destroy_query;

    $result = $this->db->prepared_query($query, array(
      ":comment_id" => $comment_id, 
      ":user_id" => $user_id, 
    ));

  }

  // gets JSON data for all comments on an event.
  // used for polling and updating an events page.
  public function get_event_comments_json ($params) {

    $event_id = isset($params['event_id'])? $params['event_id']: 0;
    $start = isset($params['start'])? $params['start']: 0;
    $end = isset($params['end'])? $params['end']: 9;

    $query = $this->comments_event_query;

    $results = $this->db->prepared_query($query, array(
      ":event_id" => $event_id,
      ":start" => $start,
      ":end" => $end,
    ));

    if (empty($results)) {

      echo '{"status": "fail"}';

      die();

    }

    echo json_encode($results);

    die();

  }

}

?>

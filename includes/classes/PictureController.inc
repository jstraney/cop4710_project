<?php

// This was kind of an after-thought. So it's a little.

class PictureController extends EntityController {

  // picture controller doesn't store pictures in the DB as blobs
  // but does need to check if the current user can change the entities
  // picture. This controller utilizes queries and white-listing
  // to ensure no one could post files to the site using CSRF
  private $owns_user_query;
  private $owns_event_query;
  private $owns_rso_query;
  private $owns_university_query;

  // list of permissible entity types to upload a picture for.
  // values as keys for faster lookup.
  private $white_list = array (
    'users' => TRUE, 
    'rsos' => TRUE, 
    'events' => TRUE, 
    'universities' => TRUE, 
  );

  public function __construct () {

    $this->db = new Database();

    include get_entity_partial('picture', 'queries');

    $this->owns_user_query = $owns_user_query;

    $this->owns_rso_query = $owns_rso_query;

    $this->owns_event_query = $owns_event_query;

    $this->owns_university_query = $owns_university_query;

  }

  private function type_okay ($type) {

    return isset($this->white_list[$type])? TRUE: FALSE;

  }

  private function check_ownership ($params) {

    global $user;

    if ($user === NULL) {

      return FALSE;

    }

    $user_id = $user->user_id;

    $uni_id = isset($user->uni_id) ? $user->uni_id: 0;

    $role = $user->role;

    // perform checks based on type and id of entity

    $type = check_field($params, 'type', NULL);

    $type_okay = $this->type_okay($type);

    if (!$type_okay) {

      return FALSE;

    }

    // entity_id
    $id = check_field($params, 'id', 0);

    if ($type == "users") {

      $query = $this->owns_user_query; 

    }
    else if ($type == "rsos") {

      $query = $this->owns_rso_query; 

    }
    else if ($type == "events") {

      $query = $this->owns_event_query; 

    }
    else if ($type == "universities") {

      $query = $this->owns_university_query; 

    }

    $result = $this->db->prepared_query($query, array(
      ":user_id" => $user_id,
      ":entity_id" => $id,
    )); 

    $result = $result[0];

    $is_owner = $result['is_owner'];

    if ($is_owner == 1) {

      return TRUE;

    }
    else {

      return FALSE;

    }

  }

  public function create ($params) {

    $type = check_field($params, 'type', NULL);

    $id = check_field($params, 'id', 0);

    if ($type == NULL || $id == 0) {

      return;

    }

    if (!$this->check_ownership($params)) {

      return;

    }

    if (entity_has_pic($type, $id)) {

      // pass params to update. does the same thing as creating now (overwrites)
      // but this way, we can diverge actions if we choose to later.
      $this->update($params);

      return;

    }

    // create the image if it doesn't exist.
    if (isset($_FILES) && isset($_FILES["pic"])) {

      $pic = $_FILES["pic"]["tmp_name"];

      create_entity_pic($type, $id, $pic);

      // echo response
      echo '{"success": true}';

      die();

    }

  }

  public function view ($params) {

    $type = check_field($params, 'type', NULL);

    $id = check_field($params, 'id', 0);

    $style = check_field($params, 'style', 'thumbnail');

    $default = check_field($params, 'default', 'default.jpg');

    $template = load_entity_pic($type, $id, $style, $default);

    echo '{"img" : ' . $template . '}';

    die();

  }

  public function update ($params) {

    $type = check_field($params, 'type', NULL);

    $id = check_field($params, 'id', 0);

    if ($type == NULL || $id == 0) {

      echo '{"fail" : true}';

      die();

    }

    // now we can update the picture if one was provided...
    if (isset($_FILES) && isset($_FILES["pic"])) {

      $pic = $_FILES["pic"]["tmp_name"];

      // will overwrite the file if it exists
      create_entity_pic($type, $id, $pic);

      echo '{"success" : true}';

      die();

    }

  }

  public function destroy ($params) {

    $type = check_field($params, 'type', NULL);

    $id = check_field($params, 'id', 0);

    if ($type == NULL || $id == 0) {

      return;

    }

    if (!$this->check_ownership($params)) {

      return;

    }

    destroy_entity_pic($type, $id);

    // echo response
    echo '{"success": true}';
    die();

  }

  public function make ($params) {
    // nothing
  }

  public function edit ($params) {
    // nothing
  }

  public function drop ($params) {
    // nothing
  }


}

?>

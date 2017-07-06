<?php

// This was kind of an after-thought. So it's a little.

class PictureController extends EntityController {

  public function create ($params) {

    $type = check_field($params, 'type', NULL);

    $id = check_field($params, 'id', 0);

    if ($type == NULL || $id == 0) {

      return;

    }

    if (entity_has_pic($type, $id)) {

      // pass params to update
      $this->update($params);

      return;

    }

    // now we can create a picture if one was provided...
    if (isset($_FILES) && isset($_FILES["pic"])) {

      $pic = $_FILES["pic"]["tmp_name"][0];

      create_entity_pic($type, $id, $pic);

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

      $pic = $_FILES["pic"]["tmp_name"][0];

      // will overwrite the file if it exists
      create_entity_pic($type, $id, $pic);

      echo '{"fail" : true}';

      die();

    }

  }

  public function destroy ($params) {

    $type = check_field($params, 'type', NULL);

    $id = check_field($params, 'id', 0);

    destroy_entity_pic($type, $id);

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

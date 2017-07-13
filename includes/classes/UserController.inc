<?php

class UserController extends EntityController {

  private $create_query;
  private $view_query;
  private $view_all_query;
  private $update_query;
  private $delete_query;

  private $user_university_query;
  private $user_participating_query;
  private $user_membership_query;
  private $get_students_like_query;

  // verification
  /* leaving this out for now. we can add it if we have time
  private $verify_query;
  private $drop_temp_query;
  */
  // checking credentials
  private $credentials_id_query;
  private $credentials_email_query;

  // gets the current user
  private $current_user_query;

  // university controller gets all the possible universities
  // to select when filling out the new user form.
  private $university_controller;

  public function __construct() {

    $this->db = new Database();

    // used to help build form options.
    $this->university_controller = new UniversityController();

    // the file with the queries is included. varaibles below
    // such as ${x}_query are in the queries.inc file
    include get_entity_partial('user', 'queries');

    // here I set the fields of the object to these query strings.
    $this->create_query = $create_query;
    $this->view_query = $view_query;
    $this->update_query = $update_query;
    $this->destroy_query = $destroy_query;

    $this->user_university_query = $user_university_query;
    $this->get_students_like_query = $get_students_like_query;
    $this->user_participating_query = $user_participating_query;
    $this->user_membership_query = $user_membership_query;

    /* excluded for now unless we have time to work on verification.
    $this->verify_query = $verify_query;
    $this->drop_temp_query = $drop_temp_query;
   */

    // two quereies to get credentials. one on e-mail (using login)
    // and one by id (when changing a users account info)
    $this->credentials_id_query = $credentials_id_query;
    $this->credentials_email_query = $credentials_email_query;

    $this->current_user_query = $current_user_query;

  }

  public function create($params) {

    $user_name = check_field($params, 'user-name', NULL);
    $first_name = check_field($params,'first-name', NULL); 
    $last_name = check_field($params,'last-name', NULL); 
    $email = check_field($params, 'e-mail', NULL); 
    $password = check_field($params, 'password', NULL); 
    $uni_id = check_field($params, 'uni-id', NULL);
    $user_role = check_field($params, 'role', "STU");

    $hash = encrypt_password($password);

    $query = $this->create_query;


    $result = $this->db->prepared_query($query, array(
      ":email" => $email,
      ":hash" => $hash,
      ":first_name" => $first_name,
      ":last_name" => $last_name,
      ":user_name" => $user_name,
      ":uni_id" => $uni_id,
      ":role" => $user_role,
    ));

    if (empty($result)) {

      go_home();
      die();

    } 
    else if (isset($result[0]['_err_msg'])) {

      $msg = $result[0]['_err_msg'];
      set_message($msg, "error");
      go_back();
      die();

    }

    $result = $result[0];
    $user_id = $result['user_id'];

    

    set_message("You've successfully made an account! Get started by creating an RSO or an event.");
    // log the user in.
    $session_controller = new SessionController();

    $session_controller->create(array(
      "e-mail" => $email,
      "password" => $password,
    ));

  }

  public function view($params) {

    global $configs;

    global $user;

    if (isset($user)) {

      $role = $user->role;

      $current_uid = $user->user_id;;

    }
    else {

      // id's start from 1
      $current_uid = 0;

      $role = FALSE;

    }

    $site_root = $configs['site_root'];

    $query = $this->view_query;

    $user_id = $params[1];

    $my_account = 0;

    if ($current_uid == $user_id) {
      // this is the current users profile
      $my_account = 1;

    }

    $result = $this->db->prepared_query($query, array(
      ":user_id" => $user_id,
    ));

    if (empty($result)) {

      include get_entity_partial('user', '404');

      return $template;

    }

    $viewed_user = $result[0];

    $user_name = check_field($viewed_user, 'user_name', "");
    $first_name = check_field($viewed_user,'first_name', ""); 
    $last_name = check_field($viewed_user,'last_name', ""); 
    $email = check_field($viewed_user, 'email', ""); 
    $uni_id = check_field($viewed_user, 'uni_id', 0);
    $uni_name = check_field($viewed_user, 'uni_name', "");
    $user_role = check_field($viewed_user, 'role', "STU");

    $has_pic = entity_has_pic("users", $user_id) ? 1: 0;

    include get_entity_partial('user', 'view');

    return $template;

  }

  public function view_all($params) {

    global $user;
    global $configs;

    $site_root = $configs['site_root'];

    if ($user === NULL) {

      go_home();
      die();

    }

    $user_id = $user->user_id;
    $role = $user->role;

    if ($role == "SA") {

      $uni_id = check_field($params, 'uni_id', 0);

    }
    else {

      $uni_id = isset($user->uni_id)? $user->uni_id: 0;

    }

    include get_entity_partial('user', 'view_all');

    return $template;

  }

  // returns true if users password is correct for user with user id.
  public function check_credentials($user_id, $password) {

    $credentials = $this->get_credentials_by_id($user_id);

    // no user by this id
    if (empty($credentials)) {

      // go to home page. 
      go_home();
      die();

    }

    $credentials = $credentials[0];

    $hash = $credentials['hash'];

    if (check_password($password, $hash)) {

      return TRUE;

    }

    return FALSE;

  }

  public function update($params) {

    $id = $params[1];

    global $user;

    if ($user === NULL) {

      go_home();
      die();

    }

    $user_name = check_field($params, 'user-name', NULL);
    $first_name = check_field($params,'first-name', NULL); 
    $last_name = check_field($params,'last-name', NULL); 
    $email = check_field($params, 'e-mail', NULL); 
    $password = check_field($params, 'password', NULL); 
    $uni_id = check_field($params, 'uni-id', 0);
    $user_role = $user->role;

    if ($this->check_credentials($id, $password)) {

      // check password entered against actual hash
      $query = $this->update_query;

      $result = $this->db->prepared_query($query, array(
        ":user_name" => $user_name,
        ":first_name" => $first_name,
        ":last_name" => $last_name,
        ":email" => $email,
        ":uni_id" => $uni_id,
        ":role" => $user_role,
        ":user_id" => $id,
      ));

      if (empty($result)) {

      }
      else if (isset($result[0]["_err_msg"])) {

        $msg = $result[0]["_err_msg"];
        set_message($msg, "error");
        go_back();
        die();

      }

      if (isset($_FILES['pic'])) {

        $file = $_FILES['pic']['tmp_name'];

        create_entity_pic("users", $id, $file);

      }

      set_message("You have successfully changed your profile");

    }
    else {

      set_message("There was a problem changing your profile", "error");
      go_back();
      die();

    }

    // redirect to users page.
    redirect_entity_view("user", $id);
  
  }

  public function destroy ($params) {

    // user who is signed in.
    global $user;

    // id of the account being deleted.
    $id = $params[1];

    $password = isset($params['password'])? $params['password'] : "";

    $credentials = $this->get_credentials_by_id($id);

    // no user by this id
    if (empty($credentials)) {

      // go to home page. 
      go_home();

    }

    $credentials = $credentials[0];

    $hash = $credentials['hash'];

    if (check_password($password, $hash)) {

      $query = $this->destroy_query;

      $result = $this->db->prepared_query($query, array(":uid" => $id));

      if (empty($result)) {

        go_home();

      }

    }
     
  }

  public function make ($params) {

    global $configs;

    $site_root = $configs['site_root'];

    include get_entity_partial("user", "make");

    return $template;

  }

  public function edit ($params) {

    global $configs;

    // current user logged in.
    global $user;

    if ($user == NULL) {

      go_home();

    }

    $site_root = $configs['site_root'];

    // get the current users fields. This allows us to set
    // default values in the fields, otherwise, anything left blank
    // will be overwritten in DB as blank

    // id of user being edited.
    $user_id = $params[1];

    // current user cannot edit this user if they are not super admin, or
    // are the user that is being edited.
    if (!$user->role !== "SA" && $user->user_id != $user_id) {

      // redirect to 'not found'
      include get_entity_partial('user', '404');

      return $template;

    }

    $query = $this->view_query;

    $result = $this->db->prepared_query($query, array(":user_id" => $user_id));

    // if there's no user to edit, return a 404 message.
    if (empty($result)) {

      include get_entity_partial("user", "404");

      return $template;

    }

    $viewed_user = $result[0];

    $user_name = $viewed_user['user_name'];
    $first_name = $viewed_user['first_name'];
    $last_name = $viewed_user['last_name'];
    $email = $viewed_user['email'];
    $uni_id = check_field($viewed_user, 'uni_id', 0);

    include get_entity_partial("user", "edit");

    return $template;

  }

  public function drop ($params) {

    global $configs;

    $site_root = $configs['site_root'];

    global $user;

    if ($user == NULL) {

      go_home();

    }

    $id = $params[1];

    include get_entity_partial("user", "drop");

    return $template;

  }

  // this has its own method to get a users id and hash by email
  public function get_credentials_by_email($email) {

    $query = $this->credentials_email_query;

    return $this->db->prepared_query($query, array(":email" => $email));

  }
  public function get_credentials_by_id($id) {

    $query = $this->credentials_id_query;

    return $this->db->prepared_query($query, array(":user_id" => $id));

  }

  public function get_current_user ($uid) {

    global $user;

    if (isset($user)) {

      return $user; 

    }

    $query = $this->current_user_query;

    $result = $this->db->prepared_query($query, array(":uid" => $uid));

    if (empty($result)) {

      return NULL;

    }

    $result = $result[0];

    // create a base class
    $user = new stdClass();

    $user->user_id = $uid;
    $user->user_name = $result['user_name'];
    $user->email = $result['email'];
    $user->first_name = $result['first_name'];
    $user->last_name = $result['last_name'];
    $user->role = $result['role'];

    // users with SA role do not have a university.
    $user->uni_id = isset($result['uni_id']) ? $result['uni_id'] : NULL;

    return $user; 
    
  }

  // gets list of students at university in JSON format. used in 'view students attending' lists
  public function get_students_json ($params) {

    global $user;

    $role = $user->role;
    
    if ($role == "SA") {

      $uni_id = check_field($params, 'uni_id', 0);

    }
    else {

      $uni_id = $user->uni_id;

    }

    $start = check_field($params, 'start', 0);

    $end = check_field($params, 'end', 9);

    $query = $this->user_university_query;

    $results = $this->db->prepared_query($query, array(
      ":uni_id" => $uni_id,
      ":start" => $start,
      ":end" => $end,
    ));

    if (empty($results)) {

      echo '{"fail" : true}';
      die();

    }

    echo json_encode($results);

    die();

  }

  public function get_peers_json ($params) {

    global $user;

    $uni_id = $user->uni_id;

    $name = isset($params['name']) ? $params['name'] : '';

    $query = $this->get_students_like_query;

    $results = $this->db->prepared_query($query, array(":uni_id" => $uni_id,":name" => $name));

    echo json_encode($results);

  }

  public function user_participating_json ($params) {

    $user_id = $params['user_id'];
    $start = isset($params['start'])? $params['start']: 0;
    $end = isset($params['end'])? $params['end']: 10;

    $query = $this->user_participating_query;

    $results = $this->db->prepared_query($query, array(
      ":user_id" => $user_id,
      ":start" => $start,
      ":end" => $end,
    ));

    if (empty($results)) {

      echo '{"fail": true}';

      die();

    }

    echo json_encode($results);

    die();

  }

  public function user_membership_json ($params) {

    $user_id = $params['user_id'];
    $start = isset($params['start'])? $params['start']: 0;
    $end = isset($params['end'])? $params['end']: 10;

    $query = $this->user_membership_query;

    $results = $this->db->prepared_query($query, array(
      ":user_id" => $user_id,
      ":start" => $start,
      ":end" => $end,
    ));

    if (empty($results)) {

      echo '{"fail": true}';

      die();

    }

    echo json_encode($results);

    die();

  }

}

?>

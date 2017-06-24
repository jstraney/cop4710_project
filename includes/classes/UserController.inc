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

    $user_name = isset($params['user-name'])? $params['user-name']: FALSE;
    $first_name = isset($params['first-name'])? $params['first-name']: FALSE; 
    $last_name = isset($params['last-name'])? $params['last-name']: FALSE; 
    $email = isset($params['e-mail'])? $params['e-mail']: FALSE; 
    $password = isset($params['password'])? $params['password']: FALSE; 
    $uni_id = isset($params['uni-id'])? $params['uni-id']: FALSE; 
    $user_role = isset($params['role'])? $params['role']: "STU"; 

    $requirements_met = TRUE;
    $requirements_met = $user_name && $first_name && $last_name;
    $requirements_met = $requirements_met && $email && $password && $uni_id;

    if (!$requirements_met) {
      set_message('One or more field was missing when creating your account. Please fill all required fields', 'error');
    }

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

    if ($result === FALSE) {

      set_message("There has been an error in creating your account. Contact our administrative team for assistance.", "error");

      go_home();

      return;

    } 

    set_message("You have successfully created an account! Log in to get started.");

    go_home();

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

    $my_account = FALSE;

    if ($current_uid == $user_id) {
      // this is the current users profile
      $my_account = TRUE;

    }

    $result = $this->db->prepared_query($query, array(":user_id" => $user_id));

    if (empty($result)) {

      include get_entity_partial('user', '404');

      return $template;

    }

    $viewed_user = $result[0];

    $user_name = isset($viewed_user['user_name'])? $viewed_user['user_name']: "";
    $first_name = isset($viewed_user['first_name'])? $viewed_user['first_name']: "";
    $last_name = isset($viewed_user['last_name'])? $viewed_user['last_name']: "";
    $email = isset($viewed_user['email'])? $viewed_user['email']: "";
    $uni_id = isset($viewed_user['uni_id'])? $viewed_user['uni_id']: "";
    $uni_name = isset($viewed_user['uni_name'])? $viewed_user['uni_name']: "";
    $user_role = isset($viewed_user['role'])? $viewed_user['role']: "";

    include get_entity_partial('user', 'view');

    return $template;

  }

  public function view_all($params) {
    
    $txt = "";

    return $txt;

  }

  public function update($params) {

    $id = $params[1];

    $user_name = $params['user-name']; 
    $first_name = $params['first-name']; 
    $last_name = $params['last-name']; 
    $email = $params['e-mail']; 
    $uni_id = $params['uni-id']; 
    $password = $params['password']; 
    $user_role = isset($params['role'])? $params['role'] : "STU"; 

    $credentials = $this->get_credentials_by_id($id);

    // no user by this id
    if (empty($credentials)) {

      // go to home page. 
      go_home();

    }

    $credentials = $credentials[0];

    $hash = $credentials['hash'];

    if (check_password($password, $hash)) {

      // check password entered against actual hash
      $query = $this->update_query;

      $this->db->prepared_query($query, array(
        ":user_name" => $user_name,
        ":first_name" => $first_name,
        ":last_name" => $last_name,
        ":email" => $email,
        ":uni_id" => $uni_id,
        ":hash" => $hash,
        ":role" => $user_role,
        ":user_id" => $id,
      ));

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
    $uni_id = $viewed_user['uni_id'];

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

    // university id from url
    $uni_id = explode("=", $params[1])[1];

    $query = $this->user_university_query;

    $results = $this->db->prepared_query($query, array(":uni_id" => $uni_id));

    echo json_encode($results);

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

      die();

    }

    echo json_encode($results);

    die();

  }

}

?>

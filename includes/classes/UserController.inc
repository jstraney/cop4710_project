<?php

class UserController extends EntityController {

  private $create_query ;

  private $view_query;
  private $view_all_query;
  private $update_query;
  private $delete_query;

  private $user_university_query;

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

    $requirements_met = TRUE;
    $requirements_met = $user_name && $first_name && $last_name;
    $requirements_met = $requirements_met && $email && $password && $uni_id;

    if (!$requirements_met) {
      set_message('One or more field was missing when creating your account. Please fill all required fields', 'error');
    }

    $hash = encrypt_password($password);

    $result = $this->db->prepared_query($this->create_query, array(
      ":email" => $email,
      ":hash" => $hash,
      ":first_name" => $first_name,
      ":last_name" => $last_name,
      ":user_name" => $user_name,
      ":uni_id" => $uni_id,
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

      $role = FALSE;

    }

    $site_root = $configs['site_root'];

    $query = $this->view_query;

    $uid = $params[1];

    $my_account = FALSE;

    if ($current_uid == $uid) {
      // this is the current users profile
      $my_account = TRUE;

    }

    $result = $this->db->prepared_query($query, array(":uid" => $uid));

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
        ":uid" => $id,
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

    // list of universities a student can register to
    $universities = $this->university_controller->get_list();

    include get_entity_partial("user", "make");

    return $template;

  }

  public function edit ($params) {

    global $configs;

    // current user logged in.
    global $user;

    $universities = $this->university_controller->get_list();

    $site_root = $configs['site_root'];

    // get the current users fields. This allows us to set
    // default values in the fields, otherwise, anything left blank
    // will be overwritten in DB as blank
    $query = $this->view_query;

    // id of user being edited.
    $id = $params[1];

    // current user cannot edit this user if they are not super admin, or
    // are the user that is being edited.
    if (!$user->role !== "SA" && $user->user_id != $id) {

      // redirect to 'not found'
      include get_entity_partial('user', '404');

      return $template;

    }

    $result = $this->db->prepared_query($query, array(":uid" => $id));

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
    $user->first_name = $result['first_name'];
    $user->last_name = $result['last_name'];
    $user->uni_id = $result['uni_id'];
    $user->role = $result['role'];

    return $user; 
    
  }

  public function get_students_json ($params) {

    // university id from url
    $uni_id = explode("=", $params[1])[1];

    $query = $this->user_university_query;

    $results = $this->db->prepared_query($query, array(":uni_id" => $uni_id));

    echo json_encode($results);

  }

  /* leaving out verification unless we have time.
  public function verify ($params) {

    $auth_url = $params[2];

    // do query to check if valid.
    $this->db->beginTransaction();
    // first verify and move to user table
    $this->db->prepared_query($this->verify_query, array(":auth_url" => $auth_url));
    // then drop temp user.
    $this->db->prepared_query($this->drop_temp_query, array(":auth_url" => $auth_url));

    $this->db->commit();
    
    // if valid move from temp table to user table.

  }
  */
}

?>

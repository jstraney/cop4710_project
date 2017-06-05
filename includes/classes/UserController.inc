<?php

class UserController extends EntityController {

  private $create_query ;

  private $view_query;
  private $view_all_query;
  private $update_query;
  private $delete_query;

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

  public function __construct() {

    $this->db = new Database();

    // the file with the queries is included. varaibles below
    // such as ${x}_query are in the queries.inc file
    include get_entity_partial('user', 'queries');

    // here I set the fields of the object to these query strings.
    $this->create_query = $create_query;
    $this->view_query = $view_query;
    $this->update_query = $update_query;
    $this->destroy_query = $destroy_query;

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

    $user_name = $params['user-name']; 
    $first_name = $params['first-name']; 
    $last_name = $params['last-name']; 
    $email = $params['e-mail']; 
    $password = $params['password']; 
    $university = $params['university']; 

    $hash = encrypt_password($password);

    $result = $this->db->prepared_query($this->create_query, array(
      ":email" => $email,
      ":hash" => $hash,
      ":first_name" => $first_name,
      ":last_name" => $last_name,
      ":user_name" => $user_name,
      ":university" => $university,
    ));

    if ($result === FALSE) {

      go_home();

    } 

    //send_auth_email($user_name, $email, $auth_url);
    
    $file_root = $configs['site_file_root'];

    // set up user resource directory for images/files
    if (!file_exists("$file_root/res/users/$username/portrait")) {

      mkdir("$file_root/res/users/$username/portrait");

    }

    copy("$file_root/res/users/default.png", "$file_root/res/users/portrait/default.png");

    go_home();

  }

  public function view($params) {

    global $configs;

    $site_root = $configs['site_root'];

    $query = $this->view_query;

    $uid = $params[1];

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
    $university = isset($viewed_user['university'])? $viewed_user['university']: "";

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
    $university = $params['university']; 
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
        ":university" => $university,
        ":hash" => $hash,
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

  public function new ($params) {

    global $configs;

    $site_root = $configs['site_root'];

    include get_entity_partial("user", "new");

    return $template;

  }

  public function edit ($params) {

    global $configs;

    // current user logged in.
    global $user;


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
    $university = $viewed_user['university'];

    include get_entity_partial("user", "edit");

    return $template;

  }

  public function delete ($params) {

    global $configs;

    $site_root = $configs['site_root'];

    $id = $params[1];

    include get_entity_partial("user", "delete");

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
    $user->university = $result['university'];
    $user->role = $result['role'];

    return $user; 
    
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

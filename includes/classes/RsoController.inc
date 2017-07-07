<?php

class RsoController extends EntityController {

  private $create_query ;

  private $view_query;
  private $view_all_query;
  private $update_query;
  private $delete_query;

  private $rso_members_query;
  private $rso_university_query;
  private $join_rso_query;
  private $leave_rso_query;

  public function __construct() {

    $this->db = new Database();

    include get_entity_partial('rso', 'queries');

    $this->create_query = $create_query;
    $this->view_query = $view_query;
    $this->view_all_query = $view_all_query;
    $this->update_query = $update_query;
    $this->destroy_query = $destroy_query;

    $this->rso_members_query = $rso_members_query;
    $this->rso_university_query = $rso_university_query;
    $this->administrates_rso_query = $administrates_rso_query;

    $this->join_rso_query = $join_rso_query;
    $this->leave_rso_query = $leave_rso_query;

  }

  public function create($params) {

    global $user;

    if ($user === NULL) {

      go_home();
      return;

    }

    $role = $user->role;
    $user_id = $user->user_id;

    $name = check_field($params, 'name', NULL);

    $description = check_field($params, 'description', NULL);

    // the members are sent as a comma separated list of ids
    $members = check_field($params, 'members', "");

    // the creator of the rso should technically be a member
    $members .= ($members == "" ? $user_id : ",$user_id");

    // if no administrator was chosen, you as the creator are assumed to be the administrator
    $rso_administrator = !empty($params['rso-administrator'])? $params['rso-administrator']: $user_id; 

    if ($role === "SA") {

      // super admin users can choose a university
      $uni_id = isset($params['uni-id'])? $params['uni-id']: NULL;

    }
    else {

      $uni_id = $user->uni_id;

    }

    $query = $this->create_query;

    $result = $this->db->prepared_query($query, array(
      ":role" => $role, 
      ":user_id" => $user_id, 
      ":name" => $name, 
      ":description" => $description, 
      ":members" => $members, 
      ":rso_administrator" => $rso_administrator, 
      ":uni_id" => $uni_id, 
    ));

    if (empty($result)) {

    }
    else if (isset($result[0]['_err_msg'])) {

      $msg = $result[0]['_err_msg'];
      set_message($msg, "error");
      // go back and die
      go_back();
      die();

    }

    $rso_id = $result[0]['rso_id'];

    set_message("You have successfully created the RSO: " . $name);

    redirect_entity_view('rso', $rso_id);

  }

  public function view ($params) {

    global $configs;

    global $user;

    if ($user == NULL) {
      
      go_home();

    }

    $user_id = $user->user_id;

    $user_name = $user->user_name;

    $role = $user->role;

    $site_root = $configs['site_root'];

    // view query for RSO
    $query = $this->view_query;

    $rso_id = $params[1];

    // result of querying the rso
    $result = $this->db->prepared_query($query, array(
      ":rso_id" => $rso_id,
      ":user_id" => $user_id,
      ":role" => $role,
    ));

    if (empty($result)) {

      include get_entity_partial('rso', '404');

      return $template;

    }

    $result = $result[0];

    $name = $result['name'];

    $description = $result['description'];

    $rso_administrator = $result['rso_administrator'];

    $viewed_by_admin = $rso_administrator == $user_id;

    $has_pic = entity_has_pic("rsos", $rso_id)? 1: 0;

    include get_entity_partial('rso', 'view');

    return $template;

  }

  public function view_all($params) {

    global $user;

    global $configs;

    if ($user === NULL) {

      go_home();
      die();

    }

    $user_id = isset($user->user_id)? $user->user_id: 0;

    $uni_id = isset($user->uni_id)? $user->uni_id: 0;

    $site_root = $configs['site_root'];

    include get_entity_partial('rso', 'view_all');

    return $template;

  }

  public function update($params) {

    global $user;

    if ($user == NULL) {

      go_home();

    }

    global $configs;

    $rso_id = $params[1];
    $user_id = $user->user_id;
    $uni_id = $user->uni_id;
    $role = $user->role;

    $name = check_field($params, 'name', NULL);
    $description = check_field($params, 'description', NULL);

    // the members are sent as a comma separated list of ids
    $members = check_field($params, 'members', "");

    // the creator of the rso should technically be a member
    $members .= ",$user_id";

    // if no administrator was chosen, you as the creator are assumed to be the administrator
    $rso_administrator = check_field($params, 'rso-administrator', $user_id); 

    $query = $this->update_query;

    $result = $this->db->prepared_query($query, array(
      ":rso_id"=>$rso_id,
      ":role"=>$role,
      ":user_id"=>$user_id,
      ":name"=>$name,
      ":description"=>$description,
      ":members"=>$members,
      ":rso_administrator"=>$rso_administrator,
      ":uni_id"=>$uni_id,
    ));

    if (empty($result)) {

      redirect_entity_view('rso', $rso_id);

    }
    else if (isset($result[0]["_err_msg"])) {

      $msg = $result[0]["_err_msg"];
      set_message($msg, "error");
      go_back();
      die();

    }

    set_message("Succesfully changed RSO info");

    redirect_entity_view('rso', $rso_id);

  }

  public function destroy ($params) {

    // user who is signed in.
    global $user;

    $uid = $user->user_id;

    $role = $user->role;

    $rso_id = $params[1];

    $password = check_field($params, 'password', NULL);

    $view = $this->db->prepared_query($this->view_query, array(
      ':rso_id' => $rso_id,
      ':user_id' => $uid,
      ':role' => $role,
    ));

    if (empty($view)) {

      go_home();

    }

    $view = $view[0];

    $name = $view['name'];

    $query = $this->destroy_query;

    $this->db->prepared_query($query, array(":rso_id" => $rso_id));

    set_message("$name has been successfully deleted.");

    go_home();
     
  }

  public function make ($params) {

    global $configs;

    $site_root = $configs['site_root'];

    // user who is signed in.
    global $user;

    if ($user == NULL) {

      go_home();

    }

    $user_id = $user->user_id;

    $role = $user->role;
     

    include get_entity_partial('rso', 'make');

    return $template;

  }

  public function edit ($params) {

    global $configs;

    $site_root = $configs['site_root'];

    // user who is signed in.
    global $user;

    if ($user == NULL) {

      go_home();

    }

    $user_id = $user->user_id;

    $role = $user->role;

    $rso_id = $params[1];

    $query = $this->view_query;

    $result = $this->db->prepared_query($query, array(
      ":rso_id" => $rso_id,
      ":user_id" => $user_id,
      ":role" => $role
    ));

    if (empty($result)) {

      include get_entity_partial("rso", "404");

      return $template;

    }

    $result = $result[0];

    $name = check_field($result, 'name', "");

    $description = check_field($result, 'description', "");

    $rso_administrator = check_field($result, 'rso_administrator', "");

    $members_list = $this->db->prepared_query($this->rso_members_query, array(
      ":rso_id" => $rso_id,
    ));

    $members_val = "";

    foreach ($members_list as $member) {

      $members_val .= $member['user_id'];

    }

    $members_json = json_encode($members_list);

    include get_entity_partial("rso", "edit");

    return $template;

  }

  public function drop ($params) {

    global $user;

    if ($user == NULL) {

      go_home();

    }

    global $configs;

    $site_root = $configs['site_root'];

    $rso_id = $params[1];
    $user_id = $user->user_id;
    $role = $user->role;

    $query = $this->view_query;

    $result = $this->db->prepared_query($query, array(
      ":rso_id" => $rso_id,
      ":user_id" => $user_id,
      ":role" => $role,
    ));

    if (empty($result)) {

      include get_entity_partial('rso', '404');
      
      return $template;

    }

    $result = $result[0];
    $name = $result['name'];

    include get_entity_partial("rso", "drop");

    return $template;

  }

  // gets rsos administrated by a user.
  public function rso_administrator_json ($params) {

    global $user;

    if (!isset($user)) {

      return;

    }

    $name = isset($params['name'])? $params['name']: "";

    $user_id = $user->user_id;

    $query = $this->administrates_rso_query;

    $result = $this->db->prepared_query($query, array(":user_id" => $user_id, ":name" => $name));

    if (empty($result)) {

      return;

    }

    echo json_encode($result);

    die();

  }

  // gets members of an RSO
  public function rso_members_json ($params) {

    $rso_id = isset($params['rso_id']) ? $params['rso_id']: 0;

    $query = $this->rso_members_query;

    $result = $this->db->prepared_query($query, array(":rso_id" => $rso_id));

    echo json_encode($result);

    die();

  }

  public function rso_university_json ($params) {

    global $user;

    if ($user === NULL) {

      echo '{"fail": true}';
      die();

    }

    $user_id = $user->user_id;

    $role = $user->role;

    // use the uni_id parameter if user is super admin
    if ($role == "SA") {

      $uni_id = check_field($params, 'uni_id', 0);

    }
    else {

      $uni_id = isset($user->uni_id)? $user->uni_id: 0;

    }

    $start = check_field($params, 'start', 0);
    $end = check_field($params, 'end', 9);

    $query = $this->rso_university_query;

    $results = $this->db->prepared_query($query, array(
      ':uni_id' => $uni_id,
      ':start' => $start,
      ':end' => $end,
    ));

    echo json_encode($results);
    die();

  }

  public function join_rso ($params) {

    global $user;

    if ($user == NULL) {

      die();

    }

    $user_id = $user->user_id;
    $uni_id = isset($user->uni_id)? $user->uni_id: 0;

    $rso_id = check_field($params, 'rso_id'); 

    $query = $this->join_rso_query;

    $result = $this->db->prepared_query($query, array(
      ":user_id" => $user_id,
      ":rso_id" => $rso_id,
      ":uni_id" => $uni_id,
    ));

    if (empty($result)) {

      echo '{"fail": true}'; 
      die();

    }
    else if (isset($result[0]['_err_msg'])) {

      $msg = $result[0]['_err_msg']; 

      echo '{"fail": true, "msg": "'.$msg.'"}'; 
      die();

    }

    echo '{"success": true}'; 
    die();




  }

  public function leave_rso ($params) {

    global $user;

    if ($user == NULL) {

      die();

    }

    $user_id = $user->user_id;
    $uni_id = isset($user->uni_id)? $user->uni_id: 0;

    $rso_id = check_field($params, 'rso_id'); 

    $query = $this->leave_rso_query;

    $result = $this->db->prepared_query($query, array(
      ":user_id" => $user_id,
      ":rso_id" => $rso_id,
    ));

    if (empty($result)) {

      echo '{"fail": true}'; 
      die();

    }
    else if (isset($result[0]['_err_msg'])) {
      
      $msg = $result[0]['_err_msg']; 

      echo '{"fail": true, "msg": "'.$msg.'"}'; 
      die();

    }

    echo '{"success": true}'; 
    die();

  }

}

?>

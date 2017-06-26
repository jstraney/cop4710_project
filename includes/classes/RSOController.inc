<?php

class RSOController extends EntityController {

  private $create_query ;

  private $view_query;
  private $view_all_query;
  private $update_query;
  private $delete_query;

  private $rso_members_query;

  public function __construct() {

    $this->db = new Database();

    include get_entity_partial('rso', 'queries');

    $this->create_query = $create_query;
    $this->view_query = $view_query;
    $this->update_query = $update_query;
    $this->destroy_query = $destroy_query;

    $this->rso_members_query = $rso_members_query;
    $this->administrates_rso_query = $administrates_rso_query;

  }

  public function create($params) {

    global $user;

    if ($user === NULL) {

      go_home();
      return;

    }

    $role = $user->role;
    $user_id = $user->user_id;
    $name = isset($params['name'])? $params['name']: NULL;
    $description = isset($params['description'])? $params['description']: NULL;

    // the members are sent as a comma separated list of ids
    $members = isset($params['members'])? $params['members']: "";

    // the creator of the rso should technically be a member
    $members .= ",$user_id";

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

    $site_root = $configs['site_root'];

    // view query for RSO
    $query = $this->view_query;

    $rso_id = $params[1];

    // result of querying the rso
    $result = $this->db->prepared_query($query, array(":rso_id" => $rso_id));

    if (empty($result)) {

      include get_entity_partial('rso', '404');

      return $template;

    }

    $result = $result[0];

    $name = $result['name'];

    $description = $result['description'];

    $rso_administrator = $result['rso_administrator'];

    $viewed_by_admin = $rso_administrator == $user_id;

    include get_entity_partial('rso', 'view');

    return $template;

  }

  public function view_all($params) {
    
    $txt = "";

    return $txt;

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

    $name = isset($params['name'])? $params['name']: NULL;
    $description = isset($params['description'])? $params['description']: NULL;

    // the members are sent as a comma separated list of ids
    $members = isset($params['members'])? $params['members']: "";

    // the creator of the rso should technically be a member
    $members .= ", $user_id";

    // if no administrator was chosen, you as the creator are assumed to be the administrator
    $rso_administrator = isset($params['rso-administrator'])? $params['rso-administrator']: $user_id; 

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

    set_message("Succesfully changed RSO info");

    redirect_entity_view('rso', $rso_id);

  }

  public function destroy ($params) {

    // user who is signed in.
    global $user;

    $uid = $user->user_id;

    $role = $user->role;

    $rso_id = $params[1];

    $password = isset($params['password'])? $params['password']: "";

    $view = $this->db->prepared_query($this->view_query, array(':rso_id' => $rso_id));

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

    $uid = $user->user_id;

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

    $uid = $user->user_id;

    $role = $user->role;

    $rso_id = $params[1];

    $query = $this->view_query;

    $result = $this->db->prepared_query($query, array(":rso_id" => $rso_id));

    if (empty($result)) {

      include get_entity_partial("rso", "404");

      return $template;

    }

    $result = $result[0];

    $name = isset($result['name'])? $result['name']:"";

    $description = isset($result['description'])? $result['description']:"";

    $rso_administrator = isset($result['rso_administrator'])? $result['rso_administrator']:"";

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

    $query = $this->view_query;

    $result = $this->db->prepared_query($query, array(":rso_id" => $rso_id));

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

}

?>

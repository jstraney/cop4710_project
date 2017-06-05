<?php
// place to put various helper functions

// logs the variable out to the webpage in a pretty printed fashion
// log was taken. 

function console_log($var) {
  print "<pre>";
  var_dump($var);
  print "</pre>";
}
                                                                      
// encrypts a plain-text password into a hash before inserting into database
function encrypt_password($pw) {

  $hash = password_hash($pw, PASSWORD_BCRYPT , array('cost' => 12));

  return $hash;

}

// checks a hashed password against a plain-text one.
function check_password($pw, $hash) {

  return password_verify($pw, $hash);

}

// this function simply returns the string of the path
// to the entity partial from the classes directory.
function get_entity_partial ($entity, $partial) {

  global $configs;

  $entity_partial_dir = $configs['entity_partial_dir'];

  return "$entity_partial_dir/$entity/$partial.inc";

}

function get_mail_partial ($partial) {

  return "partials/mail/$partial.inc";

}

/*
 * excluded for now. will include if time allows.
function send_auth_email ($username, $email, $auth_url) {

  include get_mail_partial("auth");

  mail($email, "Welcome to website", $template);

}
*/

function go_home () {

  global $configs;  

  $site_root = $configs['site_root'];

  header("location: $site_root");

}

// all entities with a 'view' action will usually follow the
// url pattern of 'resource/[id]', this just performs a redirect
// to the entity type with the following id.
function redirect_entity_view($resource_name, $id) {

  global $configs;

  $site_root = $configs['site_root'];

  // redirect to user/12, university/3, etc.
  header("location: $site_root/$resource_name/$id");

}

/*
  _____             _ _   _          _   _             
 / ____|           (_) | (_)        | | (_)            
| (___   __ _ _ __  _| |_ _ ______ _| |_ _  ___  _ __  
 \___ \ / _` | '_ \| | __| |_  / _` | __| |/ _ \| '_ \ 
 ____) | (_| | | | | | |_| |/ / (_| | |_| | (_) | | | |
|_____/ \__,_|_| |_|_|\__|_/___\__,_|\__|_|\___/|_| |_|
*/
/* verification and e-mail is being deferred unless we
 * have time in this project to do it.
function sanitize_email_input (&$email, &$username) {

  $email = sanitize_xss($email);

  $username = sanitize_xss($username);

  $email = str_replace(array('\n', '\r'), '', $email);

  $username = str_replace(array('\n', '\r'), '', $username);

}
*/
// strip out html tags from user input.
function sanitize_xss ($input) {

  return strip_tags($input);  

}

?>

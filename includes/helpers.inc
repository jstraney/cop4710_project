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

?>

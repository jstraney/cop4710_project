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

  return $hash;

}

// decrypts a hashed password to check against a plain-text one.
function decrypt_password($hash) {

  return $pw;

}

?>

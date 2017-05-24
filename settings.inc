<?php

// we will use this to store configurations to the web app
global $configs;

$configs = array();

// used in request url's
$configs['site_root'] = "http://localhost/cop4710_project";

// normally you'd want this outside the sites directory and modified
// to have restricted permissions to the 'other' user (in octal 0700)
// This is a class project, so I don't care...
$configs['connection'] = array(
  'db' => 'event',
  'host' => 'localhost',
  'port' => '3600',
  'user' => 'root',
  'password' => '',
  'driver' => 'mysql',
);

?>

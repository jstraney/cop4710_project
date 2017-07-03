<?php

// we will use this to store configurations to the web app
global $configs;

$configs = array();

// used in file path operations 
$configs['dir_root'] = getcwd(); 

// used in request url's
$configs['site_root'] = "http://localhost/cop4710_project";

// directory relative to the includes/classes directory. Contains
// partials for entity views and forms. Also includes queries for entities.
$configs['entity_partial_dir'] = "entity_partials";

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

spl_autoload_register(function ($class) {
  include "includes/classes/$class.inc";
});



?>

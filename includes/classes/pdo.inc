<?php

class CustomPDO extends PDO {

  // define white-list of tables 
  $white_list = array(
  
  );

  // override __construct
  public function __construct () {

    global $configs;

    $connection = $configs['connection'];

    $dsn = $connection["driver"];
    $user = $connection["user"];
    $db = $connection["db"];
    $host = $connection["host"];
    $password = $connection["password"];

    $dsn = "$dsn:host=$host;dbname=$db";

    try {

      parent::__construct($dsn, $user, $password, $options = NULL); 

    }
    catch (PDOException $e){

      $this->connection = NULL;
      // handle the error somehow.

    }

  }

  // override query 
  public function query () {

  }

}

?>

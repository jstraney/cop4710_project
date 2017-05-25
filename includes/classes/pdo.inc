<?php

class SupPDO extends PDO {

  // define white-list of tables 
  private $white_list = array(
  
  );

  // override __construct
  public function __construct ($options = array()) {

    global $configs;

    $connection = $configs['connection'];

    $dsn = $connection["driver"];
    $user = $connection["user"];
    $db = $connection["db"];
    $host = $connection["host"];
    $password = $connection["password"];

    $dsn = "$dsn:host=$host;dbname=$db";

    $options += array( 
      PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION,
      PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
      PDO::ATTR_EMULATE_PREPARES => false,   
    );

    try {

      parent::__construct($dsn, $user, $password, $options = NULL); 

    }
    catch (PDOException $e){

      $this->connection = NULL;
      // handle the error somehow.

    }

  }

  // override query. returns associative array.
  public function query ($query) {

    return parent::query($query)->fetch_all(PDO::FETCH_ASSOC);

  }

  // provide faster prepared statement. returns associative array.
  public function prepared_query ($query, $params) {

    $stmt = $this->prepare($query); 

    foreach ($params as $key => $parameter) {

      $stmt->bindParam($key, $parameter);

    }

    $stmt->execute($params);

    return $stmt->fetchAll(PDO::FETCH_ASSOC);

  }

}

?>

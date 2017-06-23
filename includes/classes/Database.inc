<?php

class CustomPDO extends PDO {

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

      parent::__construct($dsn, $user, $password, $options); 

    }
    catch (PDOException $e){

      console_log($e);
      $this->connection = NULL;
      // handle the error somehow.

    }

  }

  // override query. returns associative array.
  public function query ($query) {

    return parent::query($query)->fetchAll(PDO::FETCH_ASSOC);

  }

  // provide faster prepared statement. returns associative array.
  public function prepared_query ($query, $params) {

    $stmt = parent::prepare($query); 

    foreach ($params as $key => $parameter) {

      $stmt->bindParam($key, $parameter);

    }

    if (!$stmt->execute($params)) {

      return FALSE;

    }


    if ($stmt->rowCount() > 0) {

      return $stmt->fetchAll(PDO::FETCH_ASSOC);

    }

    else {

      return array();

    }

  }

}

class Database {

  private $pdo;

  public function __construct () {

    $this->pdo = new CustomPDO();

  }

  public function query ($query) {

    return $this->pdo->query($query);

  }

  public function prepared_query ($query, $params) {

    try {
      $result = $this->pdo->prepared_query($query, $params);

      return $result;
    }
    catch (PDOException $e) {

      // uncomment two statement below to debug
      console_log($e);
      console_log($e->getMessage());
      die();

      $msg = $e->getMessage();

      set_message($msg, 'error');
      // go back to last page.
      go_back();

      die();

    }
    catch (Exception $e) {

      // uncomment two statement below to debug
      console_log($e->getMessage());
      die();
      
      $msg = $e->getMessage();

      set_message($msg, 'error');
      // go back to last page.
      go_back();

      die();

    }

  }

  public function beginTransaction () {

    $this->pdo->beginTransaction();

  }

  public function commit () {
  
    $this->pdo->commit();
    
  }

}

?>

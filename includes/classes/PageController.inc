<?php
// this is a class of controller which just returns simple templates
// to be rendered as pages ('home', 'about', 'terms', 'privacy')
// right now it doesn't extend entity controller because I don't want
// to have to implement create, update, delete methods.
class PageController {
  
  public function home () {

    include get_entity_partial("page", "home");

    return $template;

  }

  public function about () {

    include get_entity_partial("page", "about");
    
    return $template;

  }

  public function terms () {

    include get_entity_partial("page", "terms");

    return $template;

  }

  public function privacy () {

    include get_entity_partial("page", "privacy");

    return $template;

  }

  public function help () {

    include get_entity_partial("page", "help");

    return $template;

  }

}

?>

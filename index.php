<?php
// TODOS:
// perform includes.
// get router class
// determine the route
// get data from the route
// end TODOS
include "settings.inc";
include "includes/helpers.inc";
?>
<!DOCTYPE html>
<html>
  <!-- A lot of the markup below should be moved to 'includes/partials/'-->
  <!-- (e.g. includes/partials/header.inc) --> 
  <head>
    <title>insert title here</title>
    <meta name="description" content=""/>
    <meta http-equiv="content-type" content="text/html;charset=UTF-8"/>
    <link rel="stylesheet" type="text/css" href="style/css/main.css">
    <script type="application/javascript" src="js/app.js"></script>
    <script type="application/javascript" src="js/lib/jquery/jquery-3.2.0.js"></script>
  </head>
  <body>
    <div id="header">
      <!-- These will login/logout actions -->
      <ul class="menu user">
        <li><a href="/">Login</a></li>
        <li><a href="/">New Account</a></li>
        <li><a href="/">Logout</a></li>
        <li><a href="/">Settings</a></li>
      </ul>
    </div>
    <div id="main-view">
      <!-- JAS - I'm thinking we devise a 'routing' php class which determines what kind of entity
          you want to perform actions on based on the request url
          (ex. '/orders' route would fetch the orders to perform CRUD operations on)
          (Create, Read, Update, Delete) would default to view, or read-->
      <ul class="menu actions">
        <!-- These will be links which pull in the forms that perform the CRUD actions -->
        <!-- replace [entity] with the type (e.g. orders) and [id] with unique identifier -->
        <li><a href="[entity]/new">create</a></li>
        <li><a href="[entity]">view</a></li>
        <li><a href="[entity]/[id]/update">edit</a></li>
        <li><a href="[entity]/[id]/delete">delete</a></li>
      </ul>

      <!-- Populate this container with the entity data, or form with entity actions -->
      <div id="content">
        <!-- placeholder garbage - remove at a later time -->
        <p>
          Lorem ipsum dolor sit amet, consectetur adipiscing elit. Duis
          convallis lacinia lorem quis gravida. Sed sit amet posuere orci, eget consectetur libero. Pellentesque vulputate gravida aliquet. Integer faucibus sollicitudin sapien, non ultrices nunc ullamcorper a. Nunc felis magna, euismod ut luctus nec, finibus vel mi. Interdum et malesuada fames ac ante ipsum primis in faucibus. Duis consequat mauris vitae lorem eleifend, et sagittis ex ultrices. Morbi odio urna, auctor vitae volutpat vitae, ornare eget sapien. Cras malesuada, mauris id vestibulum maximus, risus lorem tincidunt libero, consectetur suscipit nisi lorem ut diam. In in tellus mi.
        </p>
      </div>
    </div>
    <div id="footer">
      <!-- put in some links? -->
      <ul class="menu">
        <h4>Top</h4>
        <li><a href="">Home</a></li>
        <li><a href="">Login</a></li>
        <li><a href="">New Account</a></li>
        <!-- choose some of these with conditionals -->
        <li><a href="">Logout</a></li>
        <li><a href="">Settings</a></li>
      </ul>
      <ul class="menu">
        <h4>View My</h4>
        <li><a href=""></a></li>
        <li><a href=""></a></li>
        <li><a href=""></a></li>
      </ul>
      <ul class="menu">
        <h4>Technical</h4>
        <li><a href="">Help</a></li>
        <li><a href="">Terms</a></li>
        <li><a href="">Privacy</a></li>
      </ul>
    </div>
  </body>
</html>

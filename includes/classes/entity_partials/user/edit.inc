<?php

$template = <<<EOT
<div id="inner-content">
<script src="{$site_root}/js/user_form.js"></script>
<div><a href="{$site_root}/user/{$user_id}">Back to profile &raquo;</a></div>

<form method="POST" action="{$site_root}/user/{$user_id}/update" enctype="multipart/form-data">

  <h3>Edit Account</h3>

  <label>User Name</label>
  <input id="user-name" type="text" name="user-name" value="{$user_name}"></input>

  <label>First Name</label>
  <input id="first-name" type="text" name="first-name" value="{$first_name}"></input>

  <label>Last Name</label>
  <input id="last-name" type="text" name="last-name" value="{$last_name}"></input>

  <label>E-mail</label>
  <input id="e-mail" type="email" name="e-mail" value="{$email}"></input>

  <label>University</label>
  <div>
    <input id="uni-name" name="uni-name" type="text"></input>
    <input id="uni-id" name="uni-id" type="hidden"></input>
  </div>
  <!-- holding of on profile pictures for now
  <div>
    <label for="pic">Optional Profile Picture</label>
    <input id="pic" type="file" name="pic[]" accept="image/png,image/gif,image/jpg,image/jpeg"/>
  </div>
  -->
  <p>
  Please enter your password to change your information.
  </p>
  <label>Password</label>
  <input id="password" type="password" name="password" value=""></input>

  <input class="btn btn-large" id="submit" type="submit" name="submit" value="Change Info"></input>

</form>
</div>
EOT;
?>

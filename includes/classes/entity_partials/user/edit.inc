<?php
$template = <<<EOT

<div><a href="{$site_root}/user/{$id}">Back to profile &raquo;</a></div>

<form method="POST" action="{$site_root}/user/{$id}/update">

  <h3>Edit Account</h3>

  <label>User Name</label>
  <input id="user-name" type="text" name="user-name" value="{$user_name}"></input>

  <label>First Name</label>
  <input id="first-name" type="text" name="first-name" value="{$first_name}"></input>

  <label>Last Name</label>
  <input id="last-name" type="text" name="last-name" value="{$last_name}"></input>

  <label>E-mail</label>
  <input id="e-mail" type="email" name="e-mail" value="{$email}"></input>

  <label>Change University</label>
  <input id="university" type="text" name="university" value="{$university}"></input>

  <label>Password</label>
  <input id="password" type="password" name="password" value=""></input>

  <input id="submit" type="submit" name="submit" value="Change Info"></input>

</form>
EOT;
?>

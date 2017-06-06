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

  <label>University</label>
  <div>
    <select id="uni-id" name="uni-id"> 
      <option value="0">Select a University</option>
EOT;

foreach ($universities as $university) {

  // check if the uni_id matches the one on the student.
  $selected = $university['uni_id'] === $uni_id ? "selected": "not-selecte";

  $template .= '<option '.$selected.'="'.$selected.'" value="'.$university['uni_id'].'">'.$university['uni_name'].'</option>';

}

$template .= <<<EOT
    </select>
  </div>
  <label>Password</label>
  <input id="password" type="password" name="password" value=""></input>

  <input id="submit" type="submit" name="submit" value="Change Info"></input>

</form>
EOT;
?>

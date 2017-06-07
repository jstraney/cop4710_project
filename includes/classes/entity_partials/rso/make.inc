<?php
$template = <<<EOT
<form method="POST" action="{$site_root}/rso/create">

  <h2>New RSO</h2>

  <label>Name</label>
  <input name="name" type="text" placeholder="RSO title" value=""></input>

  <label>Description</label>
  <textarea name="description" type="text" placeholder="Describe the RSO" value=""></textarea>

EOT;

// super admins can create an RSO for any university
if ($role === "SA") {

  $template .= '<div>';
  $template .= '<label>University</label>';
  $template .= '<input id="uni-name" type="text" value=""/></form>';
  $template .= '<input id="uni-id" type="hidden" value=""/></form>';
  $template .= '</div>';
  
}

if ($role === "ADM" || $role === "SA") {
 // admins and the super admin do not need to provide additional info.
  $template .= '<div>';
  $template .= '<input type="submit" value="Create RSO"/>';
  $template .= '</form>';
  $template .= '</div>';

}

// since the user is a student, they are required to provide 5 members
// and delegate an RSO administrator.
// include a portion of the form to add members if this user is a student.
else {
  $template .= <<<EOT
  <h3>Choose 5 Members</h3>
  <div class="multi-select-entity student">
  </div>

</form>
EOT;
}
?>

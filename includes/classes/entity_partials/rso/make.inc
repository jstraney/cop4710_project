<?php
$template = <<<EOT
<script src="{$site_root}/js/rso_form.js"></script>
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
  $template .= '<input id="uni-name" type="text" value=""/>';
  $template .= '<input id="uni-id" name="uni-id" type="hidden" value=""/>';
  $template .= '</div>';
  
}

$template .= "<h3>Add Members</h3>";

// guidelines will vary depending on role (admins and super admins can create
// members if they want to, students must provide at least 5)
if ($role == "SA" || $role == "ADM") {

  $template .= '<div class="entry-prompt">optionally add members</div>';

}
else {

  $template .= '<div class="entry-prompt strict">provide at least 5 members</div>';

}

// since the user is a student, they are required to provide 5 members
// and delegate an RSO administrator.
// include a portion of the form to add members if this user is a student.

// I thought it'd be neat if super admins could make RSO's, but it's getting
// a little messy to check for students using auto-complete (SA has no uni association)
// hiding this part of the form for now.
if ($role !== "SA") {

  $template .= <<<EOT

  <p class="form-prompt">
    Enter the names of the students you'd like to
    add to the RSO. You will be added automatically. In addition,
    you may mark a student as an administrator. If you do not, you
    will be assumed to be the default administrator.
  </p>

  <div class="multi-select-entity student">
    <input id="members-entry" type="text"></input>
    <div class="rso-make aggregate members"></div>
    <input id="members" name="members" type="hidden"></input>
    <input id="rso-administrator" name="rso-administrator" type="hidden"></input>
  </div>
EOT;
}

$template .= <<<EOT
  <input type="submit" value="Create RSO"/>

</form>
EOT;
?>

<?php
if ($user_role == "STU") {
  $user_uni_rel = '<div>Currently Attending: <a href="';
  $user_uni_rel .= $site_root.'/university/'.$uni_id.'">'.$uni_name.'</a>';
  $user_uni_rel .= '</div>';
}
else if ($user_role == "ADM") {
  $user_uni_rel = '<div> Currently Affiliated With: <a href="';
  $user_uni_rel .= $site_root.'/university/'.$uni_id.'">'.$uni_name.'</a>';
  $user_uni_rel .= '</div>';
}
else {
  $user_uni_rel = '<div>Role: Super Admin</div>';
}

$template = <<<EOT
<div id="inner-content">
<script>
app.scope.user_id = {$user_id};
</script>
<script src="{$site_root}/js/view_user.js"></script>

<h2>{$user_name}'s Profile</h2>

{$user_uni_rel}
<div>E-mail: {$email} </div>
<div>Name: {$first_name} {$last_name}</div>
EOT;

// users can only edit their own account.
if ($my_account) {
  $template .= <<<EOT
<div><a href="{$site_root}/user/{$user_id}/edit">Change Info &raquo;</a></div>
EOT;

}

$template .= <<<EOT
<div class="user-view aggregate rsos">
<h3>{$user_name} belongs to these RSO's</h3>
<!-- populate with ajax -->
</div>

<div class="user-view aggregate events">
<h3>Events {$user_name} is attending</h3>
<!-- populate with ajax -->
</div>
EOT;
// only super admins can make a university
if ($my_account && $role === "SA") {
  $template .= <<<EOT
<div><a href="{$site_root}/university/new">Create a University &raquo;</a></div>
EOT;
}

// admins and students can make RSOs and both of them should be able to
// make events. all other constraints are handled by those controllers.
if ($my_account) {
  $template .= <<<EOT
<div><a href="{$site_root}/rso/new">Create an RSO &raquo;</a></div>
<div><a href="{$site_root}/event/new">Create an Event &raquo;</a></div>
<h3>Settings</h3>
<div><a href="{$site_root}/user/{$user_id}/delete">Delete Account &raquo;</a></div>
</div>
EOT;
}
?>

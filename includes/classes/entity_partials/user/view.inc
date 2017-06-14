<?php
$template = <<<EOT
<h2>{$user_name}'s Profile</h2>
<div>Currently attending: <a href="{$site_root}/university/{$uni_id}">{$uni_name}</a></div>
<div>E-mail: {$email} </div>
<div>Name: {$first_name} {$last_name}</div>
EOT;

// users can only edit their own account.
if ($my_account) {
  $template .= <<<EOT
<div><a href="{$site_root}/user/{$uid}/edit">Change Info &raquo;</a></div>
EOT;

}

$template .= <<<EOT
<div class="user-aggregate rsos">
<h3>{$user_name} belongs to these RSO's</h3>
</div>
<div class="user-aggregate events">
<h3>Events {$user_name} is attending</h3>
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
<div><a href="{$site_root}/user/{$uid}/delete">Delete Account &raquo;</a></div>
EOT;
}
?>

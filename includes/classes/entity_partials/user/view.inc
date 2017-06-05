<?php
$template = <<<EOT
<h2>{$user_name}'s Profile</h2>
<div>Currently attending: {$university}</div>
<div>E-mail: {$email} </div>
<div>Name: {$first_name} {$last_name}</div>
<div><a href="{$site_root}/user/{$uid}/edit">Change Info &raquo;</a></div>
<div class="user-aggregate rsos">
<h3>{$user_name} belongs to these RSO's</h3>
</div>
<div class="user-aggregate events">
<h3>Events {$user_name} is attending</h3>
</div>

<div><a href="{$site_root}/user/{$uid}/delete">Delete Account &raquo;</a></div>
EOT;
?>

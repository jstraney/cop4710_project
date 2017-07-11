<?php
$template = <<<EOT
<div id="inner-content">
<script>
// scope set by php
(function (a) {

  a.scope.rso_id = {$rso_id};
  a.scope.user_id = {$user_id};
  a.scope.user_name = "{$user_name}";
  a.scope.rso_administrator = {$rso_administrator};

  a.scope.has_pic = {$has_pic};

}) (app);
</script>

<script src="{$site_root}/js/view_rso.js"></script>

<div class="status {$status}">
This RSO is {$status}
</div>

<h2>{$name}</h2>

EOT;

if ($viewed_by_admin) {

  // include special links to edit and delete
  $template .= '<div class="entity-actions">';
  $template .= '<h4>RSO Settings</h4>';
  $template .= "<a href=\"{$site_root}/rso/{$rso_id}/edit\">Edit RSO</a>";
  $template .= "<a href=\"{$site_root}/event/new\">New Event</a>";
  $template .= "<a href=\"{$site_root}/rso/{$rso_id}/delete\">Delete RSO</a>";
  $template .= '</div>';

  // include the image widget
  $template .= <<<EOT
  <div class="widget picture"></div>
  <script>
  (function (a) {

    var configs = {type: "rsos", id: {$rso_id}, style: "large"};

    a.util.pictureWidget($('div.widget.picture'), configs);

  })(app);
  </script>
EOT;

}
else {
  // just use the RSOS Portrait
  $template .= load_entity_pic('rsos', $rso_id, 'large');
}

$template .= <<<EOT

<h3>About {$name}:</h3>
<p>{$description}</p>

<h3>Events Hosted by {$name}:</h3>
<div class="rso-view aggregate events">
<!-- populated with AJAX -->
</div>

<h3>Members in {$name}:</h3>
<div class="rso-view aggregate members">
<!-- populated with AJAX -->
</div>

EOT;
if ($status === 'inactive') {

  $template .= <<<EOT
  <form id="join" class="lefty" method="POST" action="{$site_root}/rso/{$rso_id}/join">
    <input class="btn btn-large" type="submit" value="Join {$name}!"/>
  </form>
EOT;

}

$template .= <<<EOT
</div>
</div>
EOT;
?>

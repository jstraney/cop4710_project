<?php
$template = <<<EOT
<div id="inner-content">
<script src="{$site_root}/js/rso_form.js"></script>
<div>
  <a href="{$site_root}/rso/{$rso_id}">Back to {$name} &raquo;</a>
</div>

<form method="POST" action="{$site_root}/rso/{$rso_id}/update">

  <h2>Edit {$name}</h2>

  <div>
    <label>Name</label>
    <input type="text" id="name" name="name" value="{$name}"></input>
  </div>

  <div>
    <label>Description</label>
    <textarea type="text" id="description" name="description">{$description}</textarea>
  </div>

  <label>Modify Members</label> 
  <p class="form-prompt">
  To add more members, use the text entry below. From the existing members, you may select
  a new RSO administrator, or remove members.

  <div class="multi-select-entity student">
    <input id="members-entry" type="text"></input>
    <div class="rso-make aggregate members">
    </div>
    <script>
    (function (a) {

      // this is weird but it works.
      a.scope.members = {$members_json};

    })(app);
    </script>

    <input id="members" name="members" type="hidden" value="{$members_val}"></input>

    <input id="rso-administrator" name="rso-administrator" type="hidden" value="{$rso_administrator}"></input>

  </div>

  <input class="btn btn-large" type="submit" value="Change RSO"></input>

</form>
</div>
EOT;
?>

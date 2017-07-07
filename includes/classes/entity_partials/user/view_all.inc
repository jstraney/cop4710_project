<?php
// todo: find a way to pass a parameter here to choose
// how to aggregate the users (e.g. by university, by rso)
$template = <<<EOT
<script src="{$site_root}/js/view_all_users.js"></script>
<div id="inner-content">
  <span class="prompt paginate"></span>
  <div class="view-all-users aggregate users loading">
  </div>
  <div class="paginate">
    <div class="button prev">
    </div>
    <div class="button next">
    </div>
  </div>
</div>
EOT;
?>

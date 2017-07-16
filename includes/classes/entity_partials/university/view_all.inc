<?php

$template = <<<EOT
<h2>Browse all Universities</h2>
<div class="entity-list university">
EOT;
// console_log($universities);
$template .= <<<EOT
<div id="inner-content">
  <span class="prompt paginate"></span>
  <div class="view-all-universities aggregate university">
EOT;


foreach($universities as $university) {
  // assign the university id for interpolation below
  $uni_id = $university['uni_id'];
  $university_name = $university['name'];
  $university_pic = load_entity_pic('universities', $uni_id, "thumbnail");
  
  // link to the listed university
  $template .= <<<EOT
    <div class="record university">
      <a href="{$site_root}/university/{$uni_id}">
      {$university_name}
      {$university_pic}
      </a>
    </div>  
EOT;
  }

$template .= <<<EOT
  </div>
  <div class="paginate">
    <div class="button prev"></div>
    <div class="button next"></div>
  </div>

  </div>
EOT;
?>

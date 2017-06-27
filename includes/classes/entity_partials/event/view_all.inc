<?php
$template = <<<EOT
<div id="inner-content">
<script>
(function () {
  app.scope.user_id = {$user_id};
  app.scope.uni_id = {$uni_id};
})();
</script>
<script src="{$site_root}/js/view_all_events.js"></script>
<div class="entity-list events">
<h2>Browse Events</h2>

<form id="event-search">

<!-- scope of the event search -->
<div class="scope">
  <label>See Events:</label>
  <select id="scope" name="select-by">
  <option name="scope" value="my-uni">Near my university</option>
  <option name="scope" value="other-uni">At another university</option>
  </select>
</div>

<!-- autocomplete university search -->
<div style="display:none" class="search-uni">
  <label>University</label>
  <input type="text" name="uni-name" value=""></input>
  <input type="hidden" name="uni-id" value=""></input>
</div>

<div class="sort">
  <label>Sort By:</label>
  <input id="sort-date" class="sort-by date" type="radio" name="sort-by" value="date">
  <label for="sort-date">Date</label>
  <input id="sort-location" class="sort-by location" type="radio" name="sort-by" value="location">
  <label for="sort-location">Distance</label>
</div>

<div class="access">
  <label>Event Types:</label>
  <input id="pub" type="radio" name="accessibility" value="PUB"> 
  <label for="pub">Public</label>
  <input id="pri" type="radio" name="accessibility" value="PRI">
  <label for="pri">Private</label>
  <input id="rso" type="radio" name="accessibility" value="RSO">
  <label for="rso">RSO Only</label>
</div>

</form>


<div class="events-view-all aggregate events"></div>
</div>
</div>
EOT;
?>

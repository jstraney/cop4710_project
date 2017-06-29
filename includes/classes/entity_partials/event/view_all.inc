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
  <option selected="selected" name="scope" value="my-uni">Near my university</option>
  <option name="scope" value="other-uni">At another university</option>
  </select>
</div>

<!-- autocomplete university search -->
<div style="display:none" class="search-uni">
  <label>University</label>
  <input type="text" id="uni-name" name="uni-name" value=""></input>
  <input type="hidden" id="uni-id" name="uni-id" value=""></input>
</div>

<div class="sort">
  <label>Sort By:</label>
  <input checked="checked" id="sort-date" class="sort-by date" type="radio" name="sort-by" value="date"/>
  <label for="sort-date">Date</label> |
  <input id="sort-location" class="sort-by location" type="radio" name="sort-by" value="location"/>
  <label for="sort-location">Distance</label>
</div>

<div class="access">
  <label>Event Types:</label>
  <input checked="checked" id="acc-pub" type="radio" name="accessibility" value="PUB">
  <label for="acc-pub">Public</label> |
  <input id="acc-pri" type="radio" name="accessibility" value="PRI"> 
  <label for="acc-pri">Private</label> |
  <input id="acc-rso" type="radio" name="accessibility" value="RSO"> 
  <label for="acc-rso">My RSOs Only</label>
</div>

</form>


<div class="events-view-all aggregate events"></div>
</div>
</div>
EOT;
?>

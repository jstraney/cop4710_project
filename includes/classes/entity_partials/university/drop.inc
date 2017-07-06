<div id="inner-content">
<div>
  <a href="{$site_root}/university/{$uni_id}">Back to {$name} &raquo;</a>
</div>

<form method="POST" action="{$site_root}/university/{$uni_id}/destroy">

  <h2>Delete {$name}</h2>

  <p>
    Deleting a university is permanent. All events, student membership, and other
    data associated with the university will also be lost. To confirm, enter
    your password. 
  </p>

  <input type="password"></input>

  <input class="btn btn-large" type="submit" value="Delete University"></input>

</form>
</div>
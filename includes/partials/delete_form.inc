<?php
// this form is included into the content of index.php when the url 
// contains "/delete" (e.g. "user/[id]/delete"). The first two tokens in the url
// are passed here and then put into the form (so we know we are editing a
// user with the id in the url).

$site_root = $configs['site_root'];

?>

<!-- 
all deletes should be performed as POST requests. 
php is echoed inot the action of the form, which tells the form which php script to send 
its contents to. It is important that we use POST requests because they are not
cached by browsers like GET requests.
-->

<h3>Delete [entity-type]</h3>

<form method="POST" action="<?php echo $site_root ?>delete.php">

  <p>Deleting this is permananent and cannot be undone.</p>
  
  <input type="submit">Delete Permenantly</input>

</form>

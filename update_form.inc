<?php
// this form is included into the content of index.php when the url 
// contains "/edit" (e.g. "user/[id]/edit"). The first two tokens in the url
// are passed here and then put into the form (so we know we are editing a
// user with the id in the url).

$site_root = $configs['site_root'];
include "includes/classes/{$resource_name}_controller.inc";
$controller = new $controller_class();

?>

<!-- 
all updates should be performed as POST requests. 
php is echoed inot the action of the form, which tells the form which php script to send 
its contents to. It is important that we use POST requests because they are not
cached by browsers like GET requests.
-->
<form method="POST" action="<?php echo $site_root ?>/update.php">

  <?php
  echo $controller->get_edit_form(); 
  ?>

</form>

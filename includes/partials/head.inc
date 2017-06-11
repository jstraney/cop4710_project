<?php
global $configs;

$site_root = $configs['site_root'];

// the head tag in html contains links to other files (css, js, additional metadata)
// if we do not use a full url (e.g. http://localhost...) but instead use partial
// paths (e.g. /style/css/main.css) The files cannot be found just because of the rewriting 
// being performed by .htaccess
?>
<head>
  <title>COP4710 Project</title>
  <meta name="description" content=""/>
  <meta http-equiv="content-type" content="text/html; charset=UTF-8"/>
  <link rel="stylesheet" type="text/css" href="<?php echo $site_root; ?>/style/css/main.css">
  <link rel="stylesheet" type="text/css" href="<?php echo $site_root; ?>/js/lib/jquery-ui/jquery-ui.css">
  <script type="application/javascript" src="<?php echo $site_root; ?>/js/lib/jquery/jquery-3.2.0.js"></script>
  <script type="application/javascript" src="<?php echo $site_root; ?>/js/lib/jquery-ui/jquery-ui.js"></script>
  <script type="application/javascript" src="<?php echo $site_root; ?>/js/app.js"></script>
</head>

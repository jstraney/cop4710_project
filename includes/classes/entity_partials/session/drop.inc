<?php
// this just returns a form that auto submits
$template = <<<EOT

<form name="delete" method="POST" action="session/destroy">
</form>
<script>$("form[name=delete]").submit()</script>

EOT;
?>

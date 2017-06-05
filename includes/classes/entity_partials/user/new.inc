<?php  
// I'm using a convention here to call the partial $template and then returning
// it with the entities method. when this file is 'included' by that method, the
// variable below becomes part of that functions scope.
//
$template = <<<EOT
<form method="POST" action="user/create">

 <h3>Create Account</h3>

 <label>User Name</label>
 <input id="user-name" type="text" name="user-name" value=""></input>

 <label>First Name</label>
 <input id="first-name" type="text" name="first-name" value=""></input>

 <label>Last Name</label>
 <input id="last-name" type="text" name="last-name" value=""></input>

 <label>E-mail</label>
 <input id="e-mail" type="email" name="e-mail" value=""></input>

 <label>University</label>
 <input id="university" type="text" name="university" value=""></input>

 <label>Password<label>
 <input id="password" type="password" name="password" value=""></input>

 <label>Confirm Password<label>
 <input id="confirm" type="password" name="confirm" value=""></input>

 <input id="submit" type="submit" name="submit" value="Sign Up"></input>

</form>
EOT;
?>

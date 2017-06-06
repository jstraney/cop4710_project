<?php  
// I'm using a convention here to call the partial $template and then returning
// it with the entities method. when this file is 'included' by that method, the
// variable below becomes part of that functions scope.
//
$template = <<<EOT
<form method="POST" action="user/create">
    <h3>Create Account</h3>
    <div class="form-group">
        <label for="UserNameInput">User Name</label>
        <input name="user-name" type="text" class="form-control" id="user-name" aria-describedby="UserNameHelp" placeholder="Enter a Username">
    </div>

    <div class="form-group">
        <label for="FirstNameInput">First Name</label>
        <input name="first-name" type="text" class="form-control" id="first-name" aria-describedby="FirstNameHelp" placeholder="First Name">
    </div>

    <div class="form-group">
        <label for="LastNameInput">Last Name</label>
        <input name="last-name" type="text" class="form-control" id="last-name" aria-describedby="LastNameHelp" placeholder="Last Name">
    </div>

    <div class="form-group">
        <label for="EmailInput">E-mail</label>
        <input name="e-mail" type="email" class="form-control" id="e-mail" aria-describedby="EmailHelp" placeholder="Email">
    </div>

    <div class="form-group">
        <label for="UniversityInput">College or University</label>
        <select name="uni-id" type="text" class="form-control" id="uni-id" aria-describedby="UniversityHelp" placeholder="University">
            <option value="0">Select a University</option>
EOT;

// concatenate all university options here
foreach ($universities as $university) {
  $template .= '<option value="'.$university['uni_id'].'">'.$university['uni_name'].'</option>';
}

$template .= <<<EOT
        </select>
    </div>


 <label>Password<label>
 <input id="password" type="password" name="password" value=""></input>

 <label>Confirm Password<label>
 <input id="confirm" type="password" name="confirm" value=""></input>

 <input id="submit" type="submit" name="submit" value="Sign Up"></input>

</form>
EOT;
?>

<?php  
// I'm using a convention here to call the partial $template and then returning
// it with the entities method. when this file is 'included' by that method, the
// variable below becomes part of that functions scope.
//
$template = <<<EOT
<div id="inner-content">
<script src="{$site_root}/js/user_form.js"></script>
<div class="row">
    <form method="POST" action="{$site_root}/user/create" class="col s12 " enctype="multipart/form-data">
	  <div class="row">
	  </div>
	  <div class="row">
		<div class="input-field col s8 offset-s2 ">
			<h5>Create an Account</h5>
		</div>
	  </div>
	  <div class="row">
		<div class="input-field col s8 offset-s2 ">
		  <input name="user-name" id="user-name" type="text" class="validate">
		  <label for="user_name">Username</label>
		</div>
	  </div>
      <div class="row">
        <div class="input-field col s4 offset-s2">
          <input name="first-name" id="first-name" type="text" class="validate">
          <label for="first_name">First Name</label>
        </div>
        <div class="input-field col s4 ">
          <input name="last-name" id="last-name" type="text" class="validate">
          <label for="last_name">Last Name</label>
        </div>
      </div>
      <div class="row">
        <div class="input-field col s8 offset-s2">
          <input name="e-mail" id="e-mail" type="email" class="validate">
          <label for="email">Email</label>
        </div>
	  </div>
	  <div class="row">
        <div class="input-field col s8 offset-s2">
          <input id="uni-name" type="text" name="uni-name" class="validate uni-name">
          <label for="uni-name">College or University</label>
          <input id="uni-id" name="uni-id" type="hidden" class="uni-id">
        </div>
      </div>
      <div class="row">
        <div class="input-field col s4 offset-s2">
          <input name="password" id="password" type="password" class="validate">
          <label for="password">Password</label>
        </div>
        <div class="input-field col s4 ">
          <input name="confirm" id="confirm" type="password" class="validate">
          <label for="confirm">Confirm Password</label>
        </div>
	  </div>
    <!-- holding of on profile pictures for now
    <div class="row">
      <div class="col s4 offset-s2">
        <label for="pic">Optional Profile Picture</label>
        <input id="pic" type="file" name="pic[]" accept="image/png,image/gif,image/jpeg"/>
      </div>
    </div>
    -->
	  <div class="row">
	  	<div class="col s4 offset-s2">
			  <button class="btn waves-effect waves-light" type="submit" name="action">Submit
				<i class="material-icons right">send</i>
			  </button>
		</div>
	  </div>
      </div>
    </form>
  </div>
</div>
EOT;
?>

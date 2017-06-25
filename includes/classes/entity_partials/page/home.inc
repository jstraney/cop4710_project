<?php
global $configs;

$site_root = $configs['site_root'];
$template = <<<EOT

<div id="index-banner" class="parallax-container">
    <div class="section no-pad-bot">
      <div class="container">
        <br><br>
        <h1 class="header center amber-text text-darken-4">What's up?</h1>
        <div class="row center">
          <h5 class="header col s12 light">A new way to find out what's happening on campuses across the country</h5>
        </div>
        <div class="row center">
          <a href="{$site_root}/events" id="download-button" class="btn-large waves-effect waves-light teal lighten-1">Find What's Up</a>
        </div>
        <br><br>

      </div>
    </div>
    <div class="parallax"><img src="{$site_root}/res/images/campfire.jpg" alt="Background Image 1" ></div>
</div>
<div class="container">
    <div class="section">

      <!--   Icon Section   -->
      <div class="row">
        <div class="col s12 m4">
          <div class="icon-block">
            <h2 class="center brown-text"><i class="medium material-icons">beach_access</i></h2>
            <h5 class="center">Get to the Fun</h5>

            <p class="light">Our application allows you to find your events faster letting you focus on the fun rather than the search. We can help you with directions, times, and place for all events.</p>
          </div>
        </div>

        <div class="col s12 m4">
          <div class="icon-block">
            <h2 class="center brown-text"><i class="medium material-icons">group</i></h2>
            <h5 class="center">Find Your Friends</h5>

            <p class="light">Connect and meet up with friends through Sup. Our service allows for easy coordination of events around campus and quick access for users.</p>
          </div>
        </div>

        <div class="col s12 m4">
          <div class="icon-block">
            <h2 class="center brown-text"><i class="medium material-icons">sentiment_very_satisfied</i></h2>
            <h5 class="center">Easy to work with</h5>

            <p class="light">Find what is fun for you. Across the nation are campuses with events being posted daily and there will be plenty you'll enjoy.</p>
          </div>
        </div>
      </div>

    </div>
  </div>
  <div class="parallax-container valign-wrapper">
    <div class="section no-pad-bot">
      <div class="container">
        <div class="row center">
          <h5 class="header col s12 light">A modern responsive front-end framework based on Material Design</h5>
        </div>
      </div>
    </div>
    <div class="parallax"><img src="{$site_root}/res/images/busy_bar.jpg" alt="Unsplashed background img 3"></div>
  </div>

EOT;
?>

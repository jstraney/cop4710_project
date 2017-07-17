// define the global app object
var app = app || {};

// add methods to the app object to build a front-end controller
(function (a) {

  // global JS variables.
  a.siteRoot = "http://localhost/cop4710_project/";

  // a global space for variables used in post requests. set from the php script
  // that renders the page. for example, on an RSO page, only user members matching
  // that rso's id should be requested. The rso_id would be set here to be sent to
  // POST rso/members/json
  a.scope = {};

  var sounds = {};

  function loadSounds() {

    var files = ['ryuhadoken.wav', 'ryushoryken.wav', 'ryutatsumakisenpukyaku.wav', 'sonic_boss.mp3'];

    for (var i in files) {

      var name = files[i];

      var key = name.split('.')[0];

      sounds[key] = new Howl({
        src: [ a.siteRoot + 'res/sounds/' + name]  
      });

    }

    a.playSound = function (soundName) {

      sounds[soundName].play();

    };

    sounds['sonic_boss'].volume(.2);

    a.playSonic = function () {

      a.playSound('sonic_boss'); 

    }

    a.playRandomRyu = function () {

      var ryuSounds = ['ryuhadoken', 'ryushoryken', 'ryutatsumakisenpukyaku'];

      var i = Math.floor(Math.random() * ryuSounds.length)

      var choice = ryuSounds[i];

      a.playSound(choice);

    };

  }

  // get sounds set methods.
  loadSounds();

  a.util = {};

  // used in ajaxified displays 
  a.util.loadEntityPic = function (config) {

    config = config || {};

    var type = config.type;

    var id = config.id;

    var style = config.style || "";

    var link = config.link || false;

    if (!type || !id) {

      return;

    }

    var img = $('<img>');

    var picSetUrl = a.siteRoot + 'res/' + type + "/" + id + '/main.jpg';
    var noPicUrl = a.siteRoot + 'res/' + type + "/default.jpg";

    img.on('error', function () {

      var self = $(this);

      self.attr('src', noPicUrl);

    });

    img.attr('src', picSetUrl + "#" + new Date().getTime()); 

    var wrapper = $('<div class="entity ' + type + ' ' + style + '">');

    if (link) {

      // oog, I knew it was a bad idea to pluralize the directories...
      var singular_type = type.slice(0, -1);

      var anchor = $('<a class="entity ' + type + ' ' + style + '" href="' + a.siteRoot + singular_type + '/' + id + '"></a>');

      anchor.append(img);

      wrapper.append(anchor);

    }
    else {

      wrapper.append(img);

    }

    return wrapper;

  };


  // elem is assumed to be a div or some empty container
  a.util.pictureWidget = function (elem, config) {

    config = config || {};

    if (config == {}) {

      return;

    }

    // the destroy button
    var destroy;

    destroy = $('<div class="button destroy">Delete</div>');

    var type = config.type;

    var style = config.style || "thumbnail"

    style += " " + type;

    elem.addClass(style);

    var id = config.id;

    var form = $('<form method="POST" enctype="multipart/form-data" action="'+a.siteRoot+'pic/upload" class="pic upload">');

    var typeInput = $('<input type="hidden" name="type" value="' + type + '"/>');

    var idInput = $('<input type="hidden" name="id" value="' + id + '"/>');

    var upload = $('<label for="pic">Upload Picture</label><input id="pic" type="file" name="pic[]"/>');

    form.append(typeInput, idInput, upload);

    form.submit(function (e) {

      e.preventDefault();

      var formData = new FormData();

      var input = $('input[type=file]', form)[0];

      if (!input.files || input.files.length == 0) {

        return;

      }

      formData.append('pic', input.files[0]);
      // send request to pic/upload
     
      formData.append('type', type);

      formData.append('id', id);
      
      a.createPic(formData, function (data) {

        data = data || '{}';

        data = JSON.parse(data);

        // on fail, set image source to default.
        if (data.fail) {

          pic.attr('src', noPicUrl);

          return;

        }
        else if (data.success) {
          // on success update the image source
          // appending timestamp to end to fool browser cache
          pic.attr('src', picSetUrl + '#' + new Date().getTime());

          upload.text("Update Picture");

          // show destroy button, return
          destroy.show();

          return;
        }

      });
      
    });

    // url if the entity has a picture. Each entity can have one picture.
    var picSetUrl = a.siteRoot + 'res/' + type + '/' + id + '/main.jpg';

    // url to default image if entity has no picture.
    var noPicUrl = a.siteRoot + 'res/' + type + '/default.jpg';

    var pic = $('<img class="entity ' + style + '">');

    // if there is an error loading the image, set it to the default.
    pic.on('error', function () {

      var self = $(this);

      self.attr('src', noPicUrl);

    });

    // set the source of the picture depending on if the entity has a picture.
    var has_pic = a.scope.has_pic;

    if (has_pic) {

      // appending timestamp to end to fool browser cache
      pic.attr('src', picSetUrl + "#" + new Date().getTime());

      $(upload).text("Update Picture");

      destroy.show();

    }
    else {

      pic.attr('src', noPicUrl);

    }

    upload.change(function () {

      var self = $(this);

      // send picture data to server.
      form.submit();

      // hide the upload button. re-shows if the upload fails.
      self.hide();

    });

    destroy.click(function () {

      var self = $(this);

      a.destroyPic({type: type, id: id}, function (data) {

        data = data || '{}';
        
        data = JSON.parse(data);

        // on failure
        if (data.fail) {

          // show a message
          a.util.spawnMsg("We could not delete your picture at this time. Sorry!");          

          return;

        }

        // on success
        upload.text('Upload Picture');
        // set image to default image
        pic.attr('src', noPicUrl);

        // hide this button 
        self.hide();
        
      });

    });

    // will change elem by reference
    elem.append(pic, form, destroy);

  }

  a.util.unleashTheTarantula = function () {

    a.playSonic();

    // disable text selection for click and drag
    $(document.body).css({
      '-moz-user-select' : 'none',
      '-webkit-user-select' : 'none',
      '-ms-user-select' : 'none',
      'user-select' : 'none',
    });

    // the tarantula's name is doug
    // doug is also a div.
    var doug;

    // add a new doug.
    function appendDoug () {

      doug= $('<div class="doug">');

      $(document.body).append(doug);

      var coords = doug.position();

      // possible starting positions. keyed by x's -> y's
      var positions = [
        {
          x: -200,
          y : 'rand' 
        },
        {
          x: window.innerWidth,
          y : 'rand' 
        },
        {
          x: 'rand',
          y : -200
        },
        {
          x: 'rand',
          y : window.innerHeight 
        },
      ];

      var position = positions[Math.floor(Math.random() * positions.length)];

      if (position.x == 'rand') {

        doug.x = Math.random() * window.innerWidth;
        doug.y = position.y;

      }
      else if (position.y == 'rand') {

        doug.x = position.x;
        doug.y = Math.random() * window.innerHeight;

      }

      doug.spd = 3;
      doug.w = doug.width();
      doug.h = doug.height();

    }

    // release the spider.
    appendDoug();

    var lastmousex = 0;
    var lastmousey = 0;
    var mousex = 0;
    var mousey = 0;

    // meme projectile. if any.
    var meme;

    $(document).on('mousemove', function (e) {

      lastmousex = mousex;
      lastmousey = mousey;

      mousex = e.clientX + window.scrollX; 
      mousey = e.clientY + window.scrollY; 

      if (meme) {

        memew = meme.width();
        memeh = meme.width();

        // if there is a meme, update position
        meme && meme.css({left: (mousex - (memew / 2)) + 'px', top: (mousey - (memeh /2)) + 'px'});

      }

    });

    var randomMemes = ["doge", "chzbrgr", "grmpy"];

    // update meme position on mousedown
    $(document).on('mousedown', function (e) {

      if (!meme) {

        var random = randomMemes[Math.floor(Math.random() * randomMemes.length)];

        meme = $('<div style="display:none" class="meme ' + random + '">');

        $(document.body).append(meme);

        meme.fadeIn();

      }

    });

    $(document).on('mouseup', function (e) {
      // get vectors based on mouse movement
      var v1 = mousex - lastmousex;
      var v2 = mousey - lastmousey;

      var projectile = meme;

      var projw = projectile.width();
      var projh = projectile.height();

      a.playRandomRyu();

      meme = null;

      var projInterval = setInterval(function () {

        var coords = projectile.position();

        lastx = coords.left; 
        lasty = coords.top; 

        // check if off screen
        if (lastx + projw < 0 || lasty + projh < 0 ||
          lastx > window.innerWidth || lasty > window.innerHeight) {

          // remove
          projectile.remove();

          // clear the interval to update the position
          window.clearInterval(projInterval);

        }

        projectile.css({left:(lastx + v1) + 'px', top:(lasty + v2) + 'px'});  

        var memeCenterX = lastx + (projw / 2);
        var memeCenterY = lasty + (projh / 2);

        // check if meme hits the spider
        if ((doug.x + doug.w) > memeCenterX && (doug.x < memeCenterX) &&
            (doug.y + doug.h) > memeCenterY && (doug.y < memeCenterY)) {

          // reset spider.
          doug.remove();
          doug = null;
          appendDoug();

        }

      }, 50);

    });

    // move doug every 50 ms for 20fps
    window.setInterval(function () {
      
      if ((doug.x + (doug.w / 2) < mousex)) {

        doug.x += doug.spd;

      }
      else if ((doug.x + (doug.w / 2) > mousex)) {

        doug.x -= doug.spd;

      }
      if ((doug.y + (doug.y / 2) < mousey)) {

        doug.y += doug.spd;

      }
      else if ((doug.y + (doug.y / 2) > mousey)) {

        doug.y -= doug.spd;

      }

      doug.css({top: doug.y + 'px', left: doug.x + 'px'});

    }, 60);

  };

  a.util.spawnMsg = function (msg, type) {

    var modal = $('<div style="display:none" class="sup-modal ' + type + '">' + msg + '</div>');

    $("body").append(modal);

    modal.fadeIn(500);
    modal.css({top: "15%"});

    window.setTimeout(function () {

      modal.fadeOut(1000, function () {

        modal.remove();
        
      });

    }, 3000);

  };

  // returns date object from utc String
  a.util.parseUTC = function (utcString) {

    var arr = utcString.split(/[- :]/);

    function removeLeadingZero(str) {

      return str[0] == '0'? str.slice(1): str;

    }

    var date = {
      yyyy : arr[0],
      MM : arr[1],
      dd : arr[2],
      hh : arr[3] % 12,
      mm : arr[4],
    };

    date.ampm = date.hh >= 12? "pm": "am";
    date.yy = date.yyyy.slice(2);
    date.M = removeLeadingZero(date.MM);
    date.d = removeLeadingZero(date.dd);
    date.h = removeLeadingZero(date.hh);
    date.m = removeLeadingZero(date.mm);

    date.amPm = date.hh >= 12 ? "pm" : "am";

    date.formatMdyyyy = function () {

      return date.M + '/' + date.d + '/' + date.yyyy;

    };

    date.formatMdyyyy_time = function () {

      var str = date.M + '/' + date.d + '/' + date.yyyy + ' ';
      str += date.h + ':' + date.mm + " " + date.ampm;

      return str;

    };

    return date;

  };


  // one method of the app object is to make a location picker
  a.makePicker = function (configs) {

    configs = configs || {};

    var defaultLocation = configs.defaultLocation || null; 
    var defaultLat = configs.defaultLat || null; 
    var defaultLon = configs.defaultLon || null; 

    // latitude and longitude fields of the form containing the map
    var lat = $("#lat");
    var lon = $("#lon");

    var locationInput = $("#location")

    // select the element with id of map
    $("#map").locationpicker({
      // set defaults
      enableAutocomplete: true,
      addressFormat: 'address',
      locationName: defaultLocation,
      // go knights!
      location: {
        longitude: defaultLon,
        latitude: defaultLat,
      },
      radius: 0,
      inputBinding: {
        longitudeOutput: $("#lon"),
        latitudeOutput: $("#lat"),
        locationNameInput: locationInput,
      },
      oninitialized: function (component) {

        lon.val(defaultLon);
        lat.val(defaultLat);

      },
      // set event listener to change the lat and lon fields when the map is
      // changed.
      onchanged: function (currentLocation, radius, isMarkerDropped) {
        lat.val(currentLocation.latitude);
        lon.val(currentLocation.longitude);
      }
    });
  };

  app.makeMapPresentation = function () {
    // latitude and longitude fields of the form containing the map
    var lat = $("#lat");
    var lon = $("#lon");

    // select the element with id of map
    $("#map").locationpicker({
      // set defaults
      location: {
        latitude: lat.val(),
        longitude: lon.val(),
      },
      enableAutocomplete: true,
      addressFormat: 'address',
      radius: 0,
      inputBinding: {
        longitudeInput: $("#lon"),
        latitudeInput: $("#lat"),
        locationNameInput: $("#location"),
      },
      markerDraggable: false,
      oninitialized: function (component) {

      },

    });

  }

  function apiEndpoint (params, url, callback, error) {

    params = params || {};

    callback = callback || function () {

    };

    error = error || function (err) {
      console.error(err);
    };

    var processData, contentType;

    // special case for picture uploads. in this case. it is a class of FormData
    if (typeof(params.get) === "function") {

      processData = params.get("processData");

      contentType = false;

    }
    else {

      processData = true;

      contentType = "application/x-www-form-urlencoded; charset=UTF-8";

    }

    delete params.processData;

    return $.ajax({
      url: a.siteRoot + url,
      method: "POST",
      success: callback,
      failure: error,
      data: params,
      processData: processData,
      contentType: contentType,
    });

  };

  // API endpoints. each of these methods will return the JSON encoded text of the entity data.
  
  // get all universities
  a.getUniversityList = function (params, callback, error) {

    return apiEndpoint(params, "universities/like/json", callback, error);

  }

  // get all students at a university 
  a.getStudentList = function (params, callback, error) {

    return apiEndpoint(params, "students/university/json", callback, error);
  
  }

  // Get events which a certain user is participating in. 
  a.getUserParticipating = function (params, callback, error) {

    return apiEndpoint(params, "user/participating/json", callback, error);

  }

  // get RSOS that a certain user is a member of
  a.getUserMembership = function (params, callback, error) {

    return apiEndpoint(params,"user/membership/json", callback, error);

  }

  a.getPeerStudentByName = function (params, callback, error) {

    params = params || {};
    params.name = params.name || "";

    return apiEndpoint(params, "user/peer/like/json", callback, error);

  }

  // get all rsos at a university 
  a.getRsoAtUniversityList = function (uni_id, callback, error) {

    uni_id = uni_id || 0; 

    return apiEndpoint("rsos/university=" + uni_id + "/json", callback, error);
  
  }

  a.getRsosByAdministrator = function (name, callback, error) {

    // placeholder used as params.
    return apiEndpoint({name: name}, "rso/administrated/json", callback, error);

  };

  a.getRsoMembers = function (rso_id, callback, error) { 

    rso_id = rso_id || 0;

    return apiEndpoint({rso_id: rso_id}, "rso/members/json", callback, error);

  }

  a.getRsoEvents = function (rso_id, callback, error) {

    rso_id = rso_id || 0;

    return apiEndpoint({rso_id: rso_id}, "rso/events/json", callback, error);

  }

  // get all events hosted at a university 
  a.getEventsAtUniversityList = function (uni_id, callback, error) {

    uni_id = uni_id || 0; 

    return apiEndpoint("events/university=" + uni_id + "/json", callback, error);
  
  }

  // get all events hosted by an RSO
  a.getEventsByRSOList = function (rso_id, callback, error) {

    uni_id = uni_id || 0; 

    return apiEndpoint("events/rso=" + rso_id + "/json", callback, error);
  
  }


  a.commentOnEvent = function (params, callback, error) {

    return apiEndpoint(params, "comment/create", callback);

  };

  a.destroyComment = function (params, callback, error) {

    return apiEndpoint(params, "comment/destroy", callback);

  } 

  a.updateComment = function (params, callback, error) {

    return apiEndpoint(params, "comment/update", callback);

  } 

  a.loadEventComments = function (event_id, callback, error) {

    return apiEndpoint({event_id: event_id}, "comments/event/json", callback);

  }

  a.participateInEvent = function (event_id, callback, error) {

    return apiEndpoint({event_id: event_id},"event/attend", callback);

  };

  a.unattendEvent = function (event_id, callback, error) {

    return apiEndpoint({event_id: event_id},"event/unattend", callback);

  };

  a.rateEvent = function (params, callback, error) {

    return apiEndpoint(params,"event/rate", callback);

  };


  a.loadEventParticipants = function (event_id, callback, error) {

    return apiEndpoint({event_id: event_id}, "event/participants/json", callback, error);

  }

  a.getEventsJson = function (params, callback, error) {

    return apiEndpoint(params, "events/json", callback, error);

  }

  a.getRsosAtUniversity = function (params, callback, error) {

    return apiEndpoint(params, "rsos/university/json", callback, error);

  }

  a.joinRso = function (params, callback, error) {

    return apiEndpoint(params, "rso/join", callback, error);

  }

  a.leaveRso = function (params, callback, error) {

    return apiEndpoint(params, "rso/leave", callback, error);

  }

  a.getCategories = function (params, callback, error) {

    return apiEndpoint(params, "event/categories/json", callback, error);

  }

  a.getCategoriesLike = function (params, callback, error) {

    return apiEndpoint(params, "event/categories/like/json", callback, error);

  }

  a.createPic = function (params, callback, error) {

    return apiEndpoint(params, "pic/upload", callback, error);

  }

  a.destroyPic = function (params, callback, error) {

    return apiEndpoint(params, "pic/destroy", callback, error);

  }

  // do some global effects on DOM load.
  $(function () {

    $('.message').fadeIn();

    window.setTimeout(function () {
      $('.message').fadeOut();
    }, 5000);

  });

})(app);

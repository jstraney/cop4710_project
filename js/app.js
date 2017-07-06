// define the global app object
var app = app || {};

// add methods to the app object to build a front-end controller
(function (a) {

  // a global space for variables used in post requests. set from the php script
  // that renders the page. for example, on an RSO page, only user members matching
  // that rso's id should be requested. The rso_id would be set here to be sent to
  // POST rso/members/json
  a.scope = {};

  a.util = {};

  // elem is assumed to be a div or some empty container
  a.util.pictureWidget = function (elem, config) {

    config = config || {};

    if (config == {}) {

      return;

    }

    var type = config.type;

    var id = config.id;

    var form = $('<form method="POST" enctype="multipart/form-data" action="'+a.siteRoot+'pic/upload" class="pic upload">');

    var typeInput = $('<input type="hidden" name="type" value="' + type + '"/>');

    var idInput = $('<input type="hidden" name="id" value="' + id + '"/>');

    var upload = $('<input type="file" name="pic[]"/>');

    form.append(typeInput, idInput, upload);

    form.submit(function (e) {

      e.preventDefault();
      // send request to pic/upload
      
      // on success update the image source
        // show destroy button, return
      
      // on fail, set image source to default.
        // show upload button
        
      console.log(e);
      
    });

    // url if the entity has a picture. Each entity can have one picture.
    var picSetUrl = a.siteRoot + 'res/' + type + '/' + id + '/main.jpg';

    // url to default image if entity has no picture.
    var noPicUrl = a.siteRoot + 'res/' + type + '/default.jpg';

    var pic = $('<img>');

    var create = $('<div class="button create">');

    create.click(function () {

      var self = $(this);

      // send picture data to server.
      form.submit();

      // hide the upload button. re-shows if the upload fails.
      self.hide();

    });

    var destroy = $('<div class="button upload">');

    destroy.click(function () {

      var self = $(this);

      a.picDestroy({type: type, id: id}, function (data) {

        data = data || '{}';
        
        data = JSON.parse(data);

        // on failure
        if (data.fail) {

          // show a message
          a.util.spawnMsg("We could not delete your picture at this time. Sorry!");          
          return;

        }

        // on success
        
        // set image to default image
        pic.attr('src', noPicUrl);
        // hide this button 
        self.hide();
        
      });

    });

    // will change elem by reference
    elem.append(pic, form, create, destroy);

  }

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

  // global JS variables.
  a.siteRoot = "http://localhost/cop4710_project/";

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

    return $.ajax({
      url: a.siteRoot + url,
      method: "POST",
      success: callback,
      failure: error,
      data: params
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

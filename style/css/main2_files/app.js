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
      hh : arr[3],
      mm : arr[4],
    };

    date.yy = date.yyyy.slice(2);
    date.M = removeLeadingZero(date.MM);
    date.d = removeLeadingZero(date.dd);
    date.h = removeLeadingZero(date.hh);
    date.m = removeLeadingZero(date.mm);

    date.formatMdyyyy = function () {

      return date.M + '/' + date.d + '/' + date.yyyy;

    };

    date.formatMdyyyy_time = function () {

      var str = date.M + '/' + date.d + '/' + date.yyyy + ' ';
      str += date.h + ':' + date.mm;

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
  a.getStudentList = function (uni_id, callback, error) {

    uni_id = uni_id || 0; 

    return apiEndpoint("user/university=" + uni_id + "/json", callback, error);
  
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

  // do some global effects on DOM load.
  $(function () {

    $('.message').fadeIn();

    window.setTimeout(function () {
      $('.message').fadeOut();
    }, 5000);

  });

})(app);

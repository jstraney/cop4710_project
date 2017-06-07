// define the global app object
var app = app || {};

// add methods to the app object to build a front-end controller
(function (a) {

  // global variables.
  var site_root = "/cop4710_project/";

  // one method of the app object is to make a location picker
  a.makePicker = function () {

    // latitude and longitude fields of the form containing the map
    var lat = $("#lat");
    var lon = $("#lon");

    defaultLat = 28.6024274;
    defaultLon = -81.2000598;

    // select the element with id of map
    $("#map").locationpicker({
      // set defaults
      enableAutocomplete: true,
      locationName: "Orlando, FL 32816, USA",
      // go knights!
      location: {
        longitude: defaultLon,
        latitude: defaultLat,
      },
      radius: 0,
      inputBinding: {
        longitudeOutput: $("#lon"),
        latitudeOutput: $("#lat"),
        locationNameInput: $("#location"),
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
      radius: 0,
      inputBinding: {
        longitudeInput: $("#lon"),
        latitudeInput: $("#lat"),
        locationNameInput: $("#location"),
      },
      markerDraggable: false,
      oninitialized: function (component) {
        console.log(component);
      },

    });

  }

  function apiEndpoint (url, callback, error) {
    error = error || function (err) {
      console.error(err);
    };

    return $.ajax({
      url: site_root + url,
      method: "POST",
      success: callback,
      failure: error,
    });

  };

  // API endpoints. each of these methods will return the JSON encoded text of the entity data.
  
  // get all universities
  a.getUniversityList = function (callback, error) {

    return apiEndpoint("universities/json", callback, error);

  }

  // get all students at a university 
  a.getStudentList = function (uni_id, callback, error) {

    uni_id = uni_id || 0; 

    return apiEndpoint("user/university=" + uni_id + "/json", callback, error);
  
  }

  // get all rsos at a university 
  a.getRSOsAtUniversityList = function (uni_id, callback, error) {

    uni_id = uni_id || 0; 

    return apiEndpoint("rsos/university=" + uni_id + "/json", callback, error);
  
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

  // do some global effects on DOM load.
  $(function () {

    $('.message').fadeIn();

    window.setTimeout(function () {
      $('.message').fadeOut();
    }, 5000);

  });

})(app);

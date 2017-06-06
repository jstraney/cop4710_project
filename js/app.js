// define the global app object
var app = app || {};

// add methods to the app object to build a front-end controller
(function (a) {

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

})(app);

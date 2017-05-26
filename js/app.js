// define the global app object
var app = app || {};

// add methods to the app object to build a front-end controller
(function (a) {

  // one method of the app object is to make a location picker
  a.makePicker = function () {

    // latitude and longitude fields of the form containing the map
    var lat = $("#lat");
    var lon = $("#lon");

    // select the element with id of map
    $("#map").locationpicker({
      // set defaults
      enableAutocomplete: true,
      locationName: "Orlando, FL 32816, USA",
      // go knights!
      location: {
        longitude: -81.2000598,
        latitude: 28.6024274,
      },
      radius: 0,
      inputBinding: {
        longitudeOutput: $("#lon"),
        latitudeInput: $("#lat"),
        locationNameInput: $("#location"),
      },
      // set event listener to change the lat and lon fields when the map is
      // changed.
      onchanged: function (currentLocation, radius, isMarkerDropped) {
        lat.val(currentLocation.latitude);
        lon.val(currentLocation.longitude);
      }
    });
  };

})(app);

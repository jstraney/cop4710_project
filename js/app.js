var app = app || {};

// add methods to the app object to build a front-end controller
(function (a) {

  a.makePicker = function () {

    $("#map").locationpicker({
      enableAutocomplete: true,
      radius: 0,
      inputBinding: {
        longitudeInput: $("#lon"),
        latitudeInput: $("#lat"),
        locationNameInput: $("#location"),
      }
    });
  };

})(app);

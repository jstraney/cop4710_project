(function (a) {

  $(function () {

    var rsoSelection = $('div.rso.selection');
    
    $('#is-rso').change(function () {

      rsoSelection.slideToggle();
      rsoId.removeAttr('disabled');
      
    });

    rsoInput = $("#rso-input");

    rsoId = $("#rso-id");

    $('#not-rso').change(function () {

      rsoSelection.slideUp();
      rsoId.attr('disabled','disabled');

    });

    rsoInput.devbridgeAutocomplete({

      lookup: function (query, done) {

        var name = query;

        // use api endpoint
        a.getRsosByAdministrator(name, function (data) {

          data = data || "{}";

          data = JSON.parse(data);

          result = {};

          result.suggestions = data;

          done(result);
          
        });
        

      },
      onSelect: function (suggestion) {

        rsoId.val(suggestion.data); 

        rsoInput.val(suggestion.value);

      }

    });

  });

})(app);

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

    rsoInput.autocomplete({

      source: function (req, res) {

        var name = req.term;

        // use api endpoint
        a.getRsosByAdministrator(name, function (data) {

          console.log(JSON.parse(data));

          res(JSON.parse(data));
          
        });
        

      },
      select: function (event, ui) {

        ui = ui || {};

        var item = ui.item;        

        rsoId.val(item.value); 

        ui.item.value = ui.item.label;

      }

    });

  });

})(app);

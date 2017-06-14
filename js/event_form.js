(function () {

  $(function () {

    var rsoSelection = $('div.rso.selection');
    var orgSelection = $('div.organization.selection');
    
    $('#is-rso').click(function () {

      rsoSelection.slideToggle();
      orgSelection.slideUp();
      
    });

    $('#not-rso').click(function () {

      orgSelection.slideToggle();
      rsoSelection.slideUp();

    });

    $('input', rsoSelection).autocomplete({
      source: function (req, res) {

        rsoQuery = req.term;

        // use api endpoint

      },
    });

    $('input', orgSelection).autocomplete({
      source: function (req, res) {



      },
    });

  });

})();

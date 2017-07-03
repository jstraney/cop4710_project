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

    // hidden categories field
    var categories = $('input#categories');

    var catAggregate = $('div.aggregate.categories');

    function rebuildCats () {

      // refresh the selection of records
      var catRecords = $('div.record', catAggregate);

      val = "";

      $.each(catRecords, function (index, elem) {

        val += $("span.value", elem).text() + ",";

      });

      // remove last comma
      val = val.slice(0, -1);

      categories.val(val);

    }

    function categoryFactory (label) {

      var elem = $('<div class="record category">');

      var remove = $('<div class="button destroy">X</div>');

      remove.click(function () {

        elem.remove();

        rebuildCats();

      });

      elem.html('<span class="value">' + label + '</span>');

      elem.append(remove);

      return elem;

    }

    var category = $("input#category");
    
    category.on('keypress', function(e) {

      if (e.keyCode == 13) {

        e.preventDefault();

        var label = category.val();

        catAggregate.append(categoryFactory(label));

        category.val("");

        rebuildCats();

      }

    });

    // check if the event id is in the scope (implies this is being edited.)
    if (a.scope.categories) {

      var cats = a.scope.categories;

      for (var i in cats) {

        var label = cats[i].label;

        catAggregate.append(categoryFactory(label));

      }

    }


  });

})(app);

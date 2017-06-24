(function (a) {

  // search params.
  var params = {
    "scope" : "my-uni",
    "sort-by": "date",
    "access": "PUB"
  };

  var scopeSelector;

  var eventSearchForm;

  $(function () {

    eventSearchForm = $('form#event-search');

    eventSearchForm.submit(function (e) {

      e.preventDefault();

      pollEvents();

    });

    scopeSelector = $('select#scope');

    scopeSelector.change(function () {

      scope = scopeSelector.val();

      if (scope == "my-uni") {

        setMyUniScope();

      }
      else if (scope == "other-uni") {

        setOtherUniScope();

      }
      else if (scope == "access") {

        setAccessScope();

      }

    });

    searchUni = $('div.search-uni > input');

    searchUni.autocomplete({

      source: function (req, res) {

      },
      select: function () {

      }  
    });

    function setOtherUniScope () {

      searchUni.parent().show();

    }

    var sortBy = $('input.sort-by');

    sortBy.change(function () {

      var self = $(this);

      params.sort_by = self.val();

    });

    var accessScopeFilters = $('div.access > input');

    accessScopeFilters.change(function () {

      var self = $(this);

      params.accessibility = self.val();
      
    });

    function setMyUniScope () {

      searchUni.parent().hide();

    }

    function pollEvents() {

      a.getEventsJson(params, function (data) {

        data = data || "{}";
        data = JSON.parse(data);

        console.log(data);
        
      });

    }

    pollEvents();

    // uncomment once things are working good.
    // window.setInterval(pollEvents, 2500);


  });

})(app);

(function (a) {

  // search params.
  var params = {
    "scope" : "my-uni",
    "sort-by": "date",
    "access": "PUB"
  };

  // keyed by id.
  var events = {};

  var scopeSelector;

  var eventSearchForm;

  var eventsAggregate;

  $(function () {

    eventSearchForm = $('form#event-search');

    eventSearchForm.submit(function (e) {

      e.preventDefault();

      pollEvents();

    });

    scopeSelector = $('select#scope');

    scopeSelector.change(function () {

      scope = scopeSelector.val();
      params.scope = scope;

      if (scope == "my-uni") {

        setMyUniScope();

      }
      else if (scope == "other-uni") {

        setOtherUniScope();

      }

    });

    searchUni = $('div.search-uni > input');
    uniId = $('input#uni-id');

    searchUni.autocomplete({

      source: function (req, res) {

        var name = req.term;
        app.getUniversityList({name: name}, function (data) {

          console.log(data);
          data = JSON.parse(data);

          options = data;

          res(data);

        });

      },
      select: function (event, ui ) {

        ui = ui || {};

        var item = ui.item;        

        params.uni_id = item.value;

        ui.item.value = ui.item.label;

      }
    });


    function setOtherUniScope () {

      searchUni.parent().show();

    }

    var sortBy = $('input.sort-by');

    sortBy.change(function () {

      var self = $(this);

      params.sort_by = self.val();

      pollEvents();

    });

    var accessScopeFilters = $('div.access > input');

    accessScopeFilters.change(function () {

      var self = $(this);

      params.accessibility = self.val();

      pollEvents();
      
    });

    function setMyUniScope () {

      searchUni.parent().hide();

    }

    eventsAggregate = $("div.events-view-all.aggregate.events");

    function rebuildEvents (new_events) {

    }

    function eventFactory (eventJson) {

      var event_id = eventJson.event_id;
      var name = eventJson.name;
      var description = eventJson.description;
      var location = eventJson.location;
      var lat = eventJson.lat;
      var lon = eventJson.lon;
      var distance = eventJson.distance;
      var rating = eventJson.rating;
      var start_time = eventJson.start_time;
      var end_time = eventJson.end_time;

      var elem = $('<div class="record.event">');
      var text = '<h5><a href="'+a.siteRoot+'event/'+event_id+'">' + name + '</a></h5>';
      text +='<p>' + description + '</p>';
      text +='<span class="info distance">' + distance + 'miles away</span>';
      text +='<span class="info rating">' + rating + '</span>';
      text +='<span class="info start-time">' + start_time + '</span>';
      text +='<span class="info end-time">' + end_time + '</span>';
      elem.html(text);

      return elem;

    }

    function pollEvents() {

      a.getEventsJson(params, function (data) {

        data = data || "{}";

        data = JSON.parse(data);

        console.log(data);

        eventsAggregate.html('');

        if (data.length == 0) {

          eventsAggregate.html('<p class="notice">No events match your search</p>');

          return;

        }

        for (var i = 0; i < data.length; i++) {

          var eventJson = data[i];

          eventsAggregate.append(eventFactory(eventJson));

        }
        
      });

    }

    pollEvents();

    // uncomment once things are working good.
    // window.setInterval(pollEvents, 2500);


  });

})(app);

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

    searchUni = $('input#uni-name');
    uniId = $('input#uni-id');

    searchUni.devbridgeAutocomplete({

      lookup: function (query, done) {

        var name = query;

        app.getUniversityList({name: name}, function (data) {

          data = data || "{}";
          data = JSON.parse(data);

          result = {};

          result.suggestions = data;

          done(result);

        });

      },
      onSelect: function (suggestion) {

        searchUni.val(suggestion.value)

        uniId.val(suggestion.data);

        params.uni_id = suggestion.data;

        pollEvents();

      }

    });


    function setOtherUniScope () {

      searchUni.parent().show();

      // only public events are available to outsiders
      accessScopeFilters.parent().hide();

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
      // select a variety of 'accessibilities' (pub, pri, rso)
      accessScopeFilters.parent().show();

      pollEvents();

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
      var distance = (parseFloat(eventJson.distance)*69).toFixed(2);
      var rating = eventJson.rating;
      var start_time = a.util.parseUTC(eventJson.start_time).formatMdyyyy_time();
      var end_time = a.util.parseUTC(eventJson.end_time).formatMdyyyy_time();
      var status = eventJson.status == "PND"? "Pending Approval" : "Active";
      var uni_id = eventJson.uni_id;
      var uni_name = eventJson.uni_name;

      var elem = $('<div style="display:none" class="record event">');
      var text = '<h5><a href="'+a.siteRoot+'event/'+event_id+'">' + name + '</a></h5>';
      text += '<p>' + description + '</p>';
      text += '<span class="info distance">~' + distance + ' Miles away</span>';
      text += '<span class="info rating">Rating : ' + rating + '</span>';
      text += '<span class="info start-time">Start : ' + start_time + '</span>';
      text += '<span class="info end-time">End : ' + end_time + '</span>';
      text += '<span class="info uni"><a href="'+ a.siteRoot + 'university/' + uni_id + '">';
      text += uni_name + '</a></span>';
      text += '<span class="info status">Status : ' + status + '</span>';
      elem.html(text);

      window.setTimeout(function () {

        elem.fadeIn(2000);
        elem.addClass("loaded");

      }, 200);

      return elem;

    }

    function pollEvents() {

      a.getEventsJson(params, function (data) {

        data = data || "{}";

        data = JSON.parse(data);

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

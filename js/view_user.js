(function (a) {

  function aggregateFactory (entity_id, entity_name, entity_type) {

    var box = $('<div class="record '+entity_type+'">'+entity_name+'</div>');

    var link = $('<a href="'+ a.siteRoot + entity_type + '/' + entity_id + '"></a>');

    link.append(box);

    return link;

  }

  $(function () {

    var user_id = a.scope.user_id;

    var participating = $('div.user-view.aggregate.events');

    a.getUserParticipating({user_id: user_id, start: 0, end: 10}, function (data) {

      console.log(data);
      data = data || "{}";
      data = JSON.parse(data);
       
      for (var i = 0; i < data.length; i++) {

        var event = data[i];
        var event_id = event.event_id;
        var name = event.name;

        var elem = aggregateFactory(event_id, name, "event");

        participating.append(elem);

      }

    });

    var isMember = $('div.user-view.aggregate.rsos');

    a.getUserMembership({user_id: user_id, start: 0, end: 10}, function (data) {

      data = data || "{}";
      data = JSON.parse(data);
       
      for (var i = 0; i < data.length; i++) {

        var rso = data[i];
        var rso_id = rso.rso_id;
        var name = rso.name;

        var elem = aggregateFactory(rso_id, name, "rso");

        isMember.append(elem);

      }
       
    });

  });

})(app);

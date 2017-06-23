(function (a) {

  // scope set on app when the page is rendered.
  var rso_id = a.scope.rso_id;
  var user_id = a.scope.user_id;
  var rso_administrator = a.scope.rso_administrator;

  var memberData;

  // compared to the box factory in the rso_form.js file, this is view only
  var memberBoxFactory = function (user_id, user_name) {

    var box = $('<div class="record member user-'+user_id+'">');

    var memberLink = $('<a href="'+ a.siteRoot + 'user/' + user_id +'">');
    memberLink.text(user_name);

    box.append(memberLink.wrap('<div>'));

    if (rso_administrator == user_id) {

      box.addClass('marked-administrator');

    }

    return box;

  };

  var membersBoxContainer;

  function eventRecordFactory (event_id, name, description, start_time, end_time) {

    var start = a.util.parseUTC(start_time).formatMdyyyy_time();

    var end = a.util.parseUTC(end_time).formatMdyyyy_time();

    var elem = $('<div class="record event event-'+event_id+'">');

    var name = $('<a href="'+a.siteRoot +'event/'+event_id+'"><h4>'+name+'</h4></a>');

    var time = $('<span class="time">' + start + '&nbsp; To &nbsp;' + end + '</span>');

    var description = $('<p>'+description+'</p>');

    elem.append(name, time, description);

    return elem;

  }

  var eventAggregate;

  $(function () {

    // initialize members box container
    membersBoxContainer = $('div.rso-view.aggregate.members');

    eventAggregate = $('div.rso-view.aggregate.events');

    
    a.getRsoEvents(rso_id)
    .then(function (data) {

      data = data || "{}";

      data = JSON.parse(data);
      
      for (var i = 0; i < data.length; i ++) {

        var event = data[i];

        var event_id = event.event_id;
        var name = event.name;
        var description = event.description;
        var start_time = event.start_time;
        var end_time = event.end_time;

        eventAggregate.append(eventRecordFactory(event_id, name, description, start_time, end_time));

      }

    });

    // make API call to rsos/members/[id]
    a.getRsoMembers(rso_id)
    .then(function (data) {

      data = data || "{}";

      data = JSON.parse(data);

      for (var i = 0; i < data.length; i++) {

        var member = data[i];
        var user_id = member.user_id;

        if (user_id == a.scope.user_id) {

          // hiding for now. if time allows, create 'leave' buttons on members.
          $('#join').hide();

        }

        var user_name = member.user_name;

        var memberBox = memberBoxFactory(user_id, user_name);

        membersBoxContainer.append(memberBox);

      }

    })
    .fail(function (err) {
      
    });

  });

})(app);

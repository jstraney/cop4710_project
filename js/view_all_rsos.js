(function (a) {

  var rsoAggregate;

  function rsoFactory (rsoJson) {

    rsoJson = rsoJson || {};

    var rso_id = rsoJson.rso_id || 0;
    var name = rsoJson.name || "";
    var description = rsoJson.description || "";
    var total_members = rsoJson.total_members || 0;

    var elem = $('<div style="display:none" class="record rso">');

    elem.append('<h4><a href="'+a.siteRoot+'rso/'+rso_id+'">'+name+'</a></h4>');

    elem.append('<span class="info total-members">'+total_members+' Members</span>');

    elem.append('<p>'+description+'</p>');

    var img = a.util.loadEntityPic({type: "rsos", id: rso_id, style: "medium", link: true});

    elem.append(img);

    window.setTimeout(function () {

      elem.fadeIn(1000);
      elem.addClass('loaded');

    }, 300);

    return elem;

  }

  pollRsos = function () {

    var uni_id = a.scope.uni_id || 0;

    var start = paginate.start;
    var end = paginate.end;

    rsoAggregate.addClass('loading');

    a.getRsosAtUniversity({uni_id: uni_id, start: start, end: end}, function (data) {

      data = data || '[]';
      data = JSON.parse(data);

      if (data.fail || data.length == 0) {

        rsoAggregate.html('<p class="notice">There are no more RSO\'s to view at your university at this time.</p>');

      }
      else {

        rsoAggregate.html("");

      }

      rsoAggregate.removeClass('loading');

      paginatePrompt.text( 'Viewing ' + paginate.start + ' - ' + paginate.end);

      for (var i = 0; i < data.length; i++) {

        var rsoJson = data[i];

        rsoAggregate.append(rsoFactory(rsoJson));

      } 
      
    });

  };

  var paginatePrompt;

  var paginate = {
    start: 0,
    end: 9,
    range: 10,
    next: function () {
      this.start += this.range;  
      this.end += this.range;  
      pollRsos();
    },
    prev: function () {
      this.start -= this.range;  
      this.end -= this.range;  
      pollRsos();

    },
    set_range: function (range) {
      this.range = range;
    }
  }

  $(function () {

    paginatePrompt = $('span.prompt.paginate');

    rsoAggregate = $('div.view-all-rsos.aggregate.rso')

    pollRsos();

    $('div.button.prev').click(function () {

      paginate.prev();

    });

    $('div.button.next').click(function () {

      paginate.next();

    });
  
  });

})(app);

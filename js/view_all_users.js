(function (a) {

  var userAggregate;

  function userFactory (userJson) {

    userJson = userJson || {};

    var user_id = userJson.user_id || 0;
    var uni_id = userJson.uni_id || 0;
    var uni_name = userJson.uni_name || "";

    var user_name = userJson.user_name || "";

    var elem = $('<div style="display:none" class="record user">');

    elem.append('<h4><a href="'+a.siteRoot+'user/'+user_id+'">'+user_name+'</a></h4>');

    var img = a.util.loadEntityPic({type: 'users', id: user_id, style: 'medium', link: true});

    elem.append(img);

    elem.append('<div>Currently attending <a href="' + a.siteRoot + 'university/' + uni_id + '">' + uni_name + '</a></div>');

    window.setTimeout(function () {

      elem.fadeIn();

      elem.addClass('loaded');

    }, 100);

    return elem;

  }

  pollUsers = function () {

    var uni_id = a.scope.uni_id || 0;

    var start = paginate.start;
    var end = paginate.end;

    userAggregate.addClass('loading');

    a.getStudentList({uni_id: uni_id, start: start, end: end}, function (data) {

      data = data || '[]';
      data = JSON.parse(data);

      if (data.fail) {

        userAggregate.html('<p class="notice">There are no more students to view at your university at this time.</p>');

      }
      else {

        userAggregate.html("");

      } 

      paginatePrompt.text( 'Viewing ' + paginate.start + ' - ' + paginate.end);

      userAggregate.removeClass('loading');

      for (var i = 0; i < data.length; i++) {

        var userJson = data[i];

        userAggregate.append(userFactory(userJson));

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
      pollUsers();
    },
    prev: function () {
      // don't go into negative pagination
      if (this.start - this.range < 0) {
        return;
      }
      else {
        this.start -= this.range;  
        this.end -= this.range;  
      }

      pollUsers();

    },
    set_range: function (range) {
      this.range = range;
    }
  }

  $(function () {

    paginatePrompt = $('span.prompt.paginate');

    userAggregate = $('div.view-all-users.aggregate.users')

    pollUsers();

    $('div.button.prev').click(function () {

      paginate.prev();

    });

    $('div.button.next').click(function () {

      paginate.next();

    });
  
  });

})(app);

(function (a) {

  var participate;
  var commentList;
  var comment;
  var status = a.scope.status;

  // returns a hidden form to edit an event
  function editCommentFormFactory(comment_id) {

    var form = $('<form style="display:none" class="edit-comment form lefty">');

    var textArea = $('<textarea name="content">');

    form.append(textArea);

    form.append('<input class="btn btn-large" type="submit" value="Change" >');

    form.submit(function (e) {

      e.preventDefault();

      var content = textArea.val();

      // simply rewrite the paragraph on success.
      a.updateComment({comment_id: comment_id, content: content}, function () {

        $('#comment-' + comment_id + ' > p.content').text(content);
        
      });
      
    });

    return form;

  }

  var today = Date.parse('today');

  function makeCommentTimeStr (str) {

    var date = Date.parse(str);

    var result = date.toString("h:mm ");

    if (today.toString("M/d/yy") == date.toString("M/d/yy")) {

      result += " Today";
      return result;

    }

    result += date.toString("M/d/yyyy");
    return result;

  }

  function commentFactory (comment_id, user_id, user_name, date_posted, content) {

    var elem = $('<div id="comment-'+comment_id+'" class="record event comment">');
    var avatar = $('<div>' + user_name + '</div>');
    var img = a.util.loadEntityPic({type: 'users', id: user_id, style: "thumbnail", link: true});
    avatar.append(img);
    // put in a link to the user including name. TODO retrieve name in comment data
    elem.append(avatar);

    var contentElem = $('<p class="content">'+content+'</p>');

    // append comment content.
    elem.append('<p class="content">'+content+'</p>');

    elem.append('<span class="date">'+date_posted+'</span>');

    // check if the user viewing the page is the owner of the comment.
    // NOTE: this does not prevent CSRF or XSS attacks. remember that
    // anyone can send an arbitrary post request.
    if (user_id == a.scope.current_user_id) {

      var dropCommentBtn = $('<div class="destroy button">delete</div>');

      dropCommentBtn.click(function () {

        // sends comment id to api endpoint. user id is checked server side via session
        a.destroyComment({comment_id: comment_id}, function (data) {

          // remove the comment from page on success.
          elem.remove();  

        });

      });

      elem.append(dropCommentBtn);

      var editCommentBtn = $('<div class="update button">edit</div>');

      var editCommentForm = editCommentFormFactory(comment_id); 

      editCommentBtn.click(function () {

        var self = $(this);
        
        // switch the text.
        var text = self.text();

        (text == "edit" && self.text('close')) ||
        (text == "close" && self.text('edit'));

        var content = $('p.content', elem).text();
        $('textarea', editCommentForm).val(content);

        // show/hide the edit form.
        editCommentForm.slideToggle();

      });

      elem.append(editCommentBtn);

      elem.append(editCommentForm);

    }


    // TODO include timestamp in comment display

    return elem;

  }


  $(function () {

    var participating = false;
    // pnd or act.

    var participateForm = $('<form class="lefty" style="display:none" id="participate">'); 
    participateForm.append('<input class="btn btn-large" type="submit" value="Participate!">');

    if (status == "PND") {

      $('input[type=submit]', participateForm).addClass('disabled')
      .attr('disabled','disabled');

    }

    // clicking on the button will submit ajax request to participate in event.
    participateForm.submit(function (e) {

      e.preventDefault();

      a.participateInEvent(a.scope.event_id, function (data) {

        // refresh page with you as a participant.
        pollParticipants();
        
      });

    });

    $('div.participate').append(participateForm);

    var participantList = $('div.participants');

    // builds a frame with user name and link to account.
    function participantFactory (user_id, user_name) {

      var elem = $('<div id="participant-' + user_id + '" class="record participant">');

      // if the current user is the participant being rendered. create an 'unattend' button
      if (a.scope.current_user_id == user_id) {

        participating = true;

        var unattend = $('<div class="button unattend">X</div>');

        unattend.click(function () {

          var event_id = a.scope.event_id;

          a.unattendEvent(event_id, function () {

            // remove the element from the page.
            elem.remove();
            
          });
          
        });

        elem.append(unattend);

      }

      elem.append('<a href="' + a.siteRoot + 'user/' + user_id + '">'+user_name+'</a>');

      return elem;

    }

    var pollParticipantsInterval;

    function pollParticipants () {

      // request list of participants. 
      a.loadEventParticipants(a.scope.event_id, function (data) {

        data = data || '{}';
        data = JSON.parse(data);

        // assume the current user is not participating before each poll
        participating = false;

        for(var i = 0; i < data.length; i++) {

          var participant = data[i];
          var user_id = participant.user_id;
          var user_name = participant.user_name;

          // skip participants added to page.
          if ($('div#participant-' + user_id).length > 0) {

            if (user_id == a.scope.current_user_id) {

              participating = true;

            }

            continue;

          }

          participantList.append(participantFactory(user_id, user_name));

        }

        // show form if you're not participating
        if (!participating) {

          participateForm.show();

        }
        // otherwise hide the form.
        else {

          participateForm.hide();

        }

      });

    }

    // poll every two and a half seconds, but only if the status is active
    if (status == "ACT") { 

      pollParticipantsInterval = window.setInterval(pollParticipants, 2500);

    }

    commentList = $("div.event-view.aggregate.comment");

    // interval to check for comments 
    var pollComentInterval;

    function pollComments () {

      a.loadEventComments(a.scope.event_id, function (data) {
        
        data = JSON.parse(data);

        if (data.status == "fail") {

          commentList.html('<p class="error no-comments">No comments on this event</p>');

          return;

        }
        else  {
          //remove no comments message.
          
          var errmsg = $('p.no-comments.error', commentList);
          // remove if message exists.
          errmsg.length > 0 && errmsg.remove();

        }

        // iterate through comments found.
        for (var i = 0; i < data.length; i ++) {

          var newComment = data[i];
          var comment_id = newComment.comment_id;

          // see if comment exists on page
          if ($("#comment-" + comment_id, commentList).length > 0) {

            // skip if it does.
            continue;

          }

          // add new comment.
          var user_id = newComment.user_id;
          var user_name = newComment.user_name;
          var date_posted = newComment.date_posted;
          var new_content = newComment.content;

          commentList.prepend(commentFactory(comment_id, user_id, user_name, date_posted, new_content));

        }

      });

    }

    status == "ACT" && pollComments();

    // check for comments every 3 seconds.
    // uncomment for production. commented out for testing.
    status == "ACT" && (pollCommentInterval = window.setInterval(pollComments, 3000));

    commentBody = $("#comment-body");

    var ratings = $('div#rating');
    var totalRating = $('span#total-rating');

    // creating a function to bind the value of i to the stars callback
    function bindStarHandler (star, rating) {

      star.click(function () {

        var event_id = a.scope.event_id;

        a.rateEvent({event_id: event_id, rating: rating}, function (data) {

          data = data || "{}";
          data = JSON.parse(data);
          console.log(data);

          totalRating.text(data.total_rating);

          // add class to rated.
          star.addClass('rated');  

          // remove all other stars ratings.
          $('div.star', ratings).not(star).removeClass('rated');

        });

      });

    }

    for (var i = 0; i <= 5; i++) {

      var text;

      if( i == 0 ) {
        text = "X";
      }
      else {
        text = "&starf;";
      }
      var star = $('<div class="star rate-'+ i +'">'+text+'</div>');

      // adding an event handler here normally will not
      // retain the value of i. passing i to this function binds
      // the value to the callback handler on the star.
      if (status == "ACT") { 

        bindStarHandler(star, i);

      }
      else {

        star.addClass('disabled');

      }

      ratings.append(star);

    }

    if (status != "ACT") {

      $('form#new-comment > input[type=submit]').addClass('disabled')
        .attr('disabled', 'disabled');

    }

    // stop form from doing regular submit
    $('form#new-comment').submit(function (e) {

      self = $(this)

      e.preventDefault();

      content = commentBody.val();

      a.commentOnEvent({

        event_id: a.scope.event_id,

        content: content

      }, function (e) {

        // look for new ones, which should include ours.
        pollComments();

        $('textarea', self).val('');

        
      });

    });

    var catAggregate = $('div.aggregate.categories');

    function categoryFactory (label) {

      var elem = $('<div class="record category">' + label + '</div>');
      
      return elem;

    }

    a.getCategories({event_id: a.scope.event_id}, function (data) {

      data = data || "{}";
      data = JSON.parse(data);

      console.log(data);

      if (data.fail) {

        catAggregate.append('<p class="notice"> This event is untagged </p>');
        return;

      }

      for (var i in data) {

        var category = data[i];
        var label = category.label;

        catAggregate.append(categoryFactory(label));

      }
      
    })

  });

})(app);

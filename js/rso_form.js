(function (a) {

  // entry input for members. uses autocomplete 
  var membersEntry;

  // container which has memberBoxes appended.
  var membersBoxContainer;

  // hidden input for rso administrator's user id.
  var rsoAdminInput;

  // user box currently selected as administrator, if any
  var rsoAdminBox;

  var rsoUniInput;

  var rsoUniHidden;

  // returns template memberBox
  function memberBoxFactory (user_id, user_name) {

    var box = $('<div class="record member user-' + user_id + '">');

    // load a nice little portrait
    var img = a.util.loadEntityPic({type: "users", id: user_id, style: "thumbnail", link: true});
    box.append(img);

    // x on the box to remove this member from rso
    var remove = $('<div title="remove user" class="button remove">X</div>');

    // removes id from memberIds. 
    remove.click(function () {

      delete memberIds[user_id];
      rewriteMembersInput();
      box.remove();

    });

    box.append(remove);

    var makeAdmin = $('<div title="Make administrator" class="button make-admin">&diams;</div>');

    makeAdmin.click(function () {

      // if this is the current user selected as admin
      if (rsoAdminBox == box) {

        // deselect them
        rsoAdminBox.toggleClass("marked-administrator");

        // mark current user as admin
        rsoAdminInput(a.scope.current_user);

      }

      // if there is an admin box set, toggle its class
      rsoAdminBox && rsoAdminBox.toggleClass('marked-administrator');

      // set the new box
      rsoAdminBox = box 

      // give it the css class for styles
      rsoAdminBox.toggleClass('marked-administrator');

      // set the hidden inputs value
      rsoAdminInput.val(user_id);


    });

    box.append(makeAdmin);

    // check if the rso being (edited/maybe created) has admin
    // if there is one, check if this user is admin...etc.
    a.scope.rso_administrator && a.scope.rso_administrator == user_id && 
    (function () { 
      rsoAdminBox = box;
      rsoAdminBox.addClass('marked-administrator');
    })();

    box.append('<div>'+user_name+'</div>');

    return box;

  }

  // hidden input which takes a comma separated list of id's as string
  var memberIds = {};
  var membersInput;

  // rewrites the hidden input based on contents of id_list
  function rewriteMembersInput () {

    var value = "";

    for (var i in memberIds) {
      value += i + ",";
    }

    // remove last comma
    value = value.slice(0, -1);

    // set the hidden field's value
    membersInput.val(value);
  
  }

  // on document load
  $(function () {

    // super admin can choose a university
    rsoUniInput = $('input#uni-name');
    rsoUniHidden = $('input#uni-id');

    rsoUniInput.devbridgeAutocomplete({

      lookup: function (query, done) {

        var name = query;

        app.getUniversityList({name: name}, function (data) {

          data = data || "{}";
          data = JSON.parse(data);

          result = {};
          
          result.suggestions = data;

          done(data);

        });

      },
      onSelect: function (suggestion) {

        console.log(suggestion);

      }

      
    });

    membersEntry = $('input#members-entry');
    rsoAdminInput = $('input#rso-administrator');
    membersInput = $('input#members');

    membersEntry.devbridgeAutocomplete({

      lookup: function (query, done) {

        var name = query;

        a.getPeerStudentByName({name: name}, function (data) {

          data = data || "{}";
          data = JSON.parse(data);

          var result = {};

          result.suggestions = data;

          done(result);

        });

      },
      onSelect: function (suggestion) {

        suggestion = suggestion || {};

        var user_id = suggestion.data;

        var user_name = suggestion.value;

        // put memberId into our id object
        memberIds[user_id] = user_name; 

        // append the graphical box to  members
        membersBoxContainer.append(memberBoxFactory(user_id, user_name));

        // rewrite the new value to the hidden input.
        rewriteMembersInput();

        // start over
        membersEntry.val('');

      }

    });

    membersBoxContainer = $('div.rso-make.aggregate.members');

    // check if members were included by the php include.
    if (a.scope.members) {

      var members = a.scope.members;

      for( var i = 0; i < members.length; i++) {


        var member = members[i];        
        var user_id = member.user_id;

        // do not add current user to form. prevents confusion/issues
        if (a.scope.current_user == user_id) {

          continue;

        }

        var user_name = member.user_name;

        // put memberId into our id object
        memberIds[user_id] = user_name; 

        // append the graphical box to  members
        membersBoxContainer.append(memberBoxFactory(user_id, user_name));

        // rewrite the new value to the hidden input.
        rewriteMembersInput();

      }

    }


  });

})(app);

(function (a) {

  $(function () {

    var uniName = $('input#uni-name');

    var uniId = $('input#uni-id');

    // buffer for selected university name and id.
    var selection = {};

    uniName.devbridgeAutocomplete({

      lookup: function (query, done) {

        var name = query;

        app.getUniversityList({name: name}, function (data) {

          data = data || '{}';
          data = JSON.parse(data);

          var result = {};

          result.suggestions = data;

          done(result);

        });

      },
      onSelect: function (suggestion) {

        uniId.val(suggestion.data);

      }

    });

  });

})(app);

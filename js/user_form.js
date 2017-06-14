(function () {

  $(function () {

    var uniName = $('#uni-name');

    var uniId = $('#uni-id');

    // buffer for selected university name and id.
    var options;
    var selection = {};

    uniName.autocomplete({

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

        uniId.val(item.value); 

        ui.item.value = ui.item.label;

      }
    });

  });

})();

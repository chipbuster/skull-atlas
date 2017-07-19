webmol.view.Finder = Backbone.View.extend({

    init: function (options) {

        var element = $(this.el),
            viewer = options.viewer,
            select_file = $('#select-file', element);

        this.select_file = select_file
        this.viewer = viewer;

        return this;
    },

    loadFile: function (event) {
        var file = event.target.files[0];
        this.viewer.loadFile(file);
    },

    events: {
        'change #select-file': 'loadFile'
    }

});

skullviewer.view.Finder = Backbone.View.extend({

    init: function(options) {
        
        var element = $(this.el),
            manager = options.manager,
            select_file = $('#select-file', element);

        this.select_file = select_file;
        this.manager = manager;

        return this;
    },

    loadFile: function(event) {
        var file = event.target.files[0];
        this.manager.loadFile(file);
    },
    
    testClick: function(event) {
        this.manager.testClick();
    },

    events: {
        'change #select-file': 'loadFile',
        'click #test-btn': 'testClick'
    }

});

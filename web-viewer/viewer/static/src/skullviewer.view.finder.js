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
    
    loadClick: function(event) {
        this.manager.loadClick();
    },

    deformClick: function(event) {
        this.manager.deformClick();
    },

    events: {
        'change #select-file': 'loadFile',
        'click #load-btn': 'loadClick',
        'click #deform-btn': 'deformClick'
    }

});

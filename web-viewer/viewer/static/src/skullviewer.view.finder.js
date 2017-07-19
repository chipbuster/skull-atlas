skullviewer.view.Finder = Backbone.View.extend({

    init: function(options) {
        //TODO
        return this;
    },

    loadFile: function(event) {
        var file = event.target.files[0];
        console.log(file)
        //TODO
    },
    
    events: {
        'change #select-file': 'loadFile'
    }

});

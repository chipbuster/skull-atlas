skullviewer.view.Viewer = Backbone.View.extend({

    qscene: new skullviewer.view.viewer.Scene({
            el: $('#query-canvas')
    }),
    
    rscene: new skullviewer.view.viewer.Scene({
            el: $('#result-canvas')
    }),

    init: function (options) {
        var element = $(this.el),
            viewer = this,
            manager = options.manager;
        
        this.manager = manager;

        this.qscene.init({
            viewer: viewer
        });

        this.rscene.init({
            viewer: viewer
        });

        return this;
    },

    renderSkull: function (name, obj) {
        this.qscene.renderSkull(name, obj);
        this.rscene.renderSkull(name, obj);
    },

    onMousedown: function (event, ui) {
        //TODO
        return this;
    },

    onMouseup: function (event, ui) {
        //TODO
        return this;
    },

    onMousemove: function (event, ui) {
        //TODO
        return this;
    },

    onMousewheel: function (event, ui) {
        //TODO
        return this;
    },
    
    onClick: function (event, ui) {
        //TODO
        return this;
    },

    events: {
        'mousedown': 'onMousedown',
        'mouseup': 'onMouseup',
        'mousemove': 'onMousemove',
        'mousewhell': 'onMousewheel',
        'click': 'onClick'
    }

});

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
            viewer: viewer,
            id: 'query'
        });

        this.rscene.init({
            viewer: viewer,
            id: 'result'
        });

        return this;
    },

    sceneSync: function (id, camera, light) {
        var toSyncCam = null;
        var toSyncLight = null;
        if (id === 'query') {
            toSyncCam = this.rscene.camera;
            toSyncLight = this.rscene.light;
        }
        else  {
            toSyncCam = this.qscene.camera;
            toSyncLight = this.qscene.light;
        }            
        
        // toSync.position.copy(camera.position);
        // toSync.lookAt(camera.target);
        toSyncCam.copy(camera);
        toSyncLight.copy(light);
        this.rscene.render();
        this.qscene.render();
    },
    renderSkull: function (name, obj) {
        this.rscene.renderSkull(name, obj.clone());
        this.qscene.renderSkull(name, obj.clone());
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

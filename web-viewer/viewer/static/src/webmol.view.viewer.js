webmol.view.Viewer = Backbone.View.extend({

    rotation: {
        x: 0,
        y: 0,
        lastX: 0,
        lastY: 0,
        enable: false
    },

    init: function (options) {

        var viewer = options.viewer;

        this.viewer = viewer;

        return this;
    },

    onMousedown: function (event, ui) {

        var rotation = this.rotation;

        rotation.lastX = event.clientX;
        rotation.lastY = event.clientY;
        rotation.enable = true;

        return this;
    },

    onMouseup: function (event, ui) {

        var rotation = this.rotation;

        rotation.enable = false;

        return this;
    },

    onMousemove: function (event, ui) {

        var rotation = this.rotation,
            viewer = this.viewer,
            clientX = event.clientX,
            clientY = event.clientY;

        if (rotation.enable) {

            rotation.x += (clientX - rotation.lastX) / 2;
            rotation.y += (clientY - rotation.lastY) / 2;

            viewer.rotate({
                x: rotation.y,
                y: rotation.x
            });

            rotation.lastX = clientX;
            rotation.lastY = clientY;
        }
    },

    onMousewheel: function (event, delta) {

        var viewer = this.viewer;

        viewer.zoom(delta);

        return this;
    },

    onClick: function (event, ui) {

        var element = $(this.el),
            offset = element.offset(),
            viewer = this.viewer;

        viewer.pick({
            x: event.clientX - offset.left,
            y: event.clientY - offset.top
        });

        return this;
    },

    events: {
        'mousedown': 'onMousedown',
        'mouseup': 'onMouseup',
        'mousemove': 'onMousemove',
        'mousewheel': 'onMousewheel',
        'click': 'onClick'
    }

});
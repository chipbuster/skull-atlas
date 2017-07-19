webmol.view.Browser = Backbone.View.extend({

    init: function (options) {

        var element = $(this.el),
            viewer = options.viewer,
            meshes = viewer.meshes;

        meshes.bind('add', $.proxy(this.add, this));

        this.viewer = viewer;

        return this;
    },

    add: function (mesh) {

        var element = $(this.el),
            label = $('<div>');

        label
            .addClass('mesh-label')
            .data(mesh)
            .text(mesh.node.id);

        element
            .append(label);

        return this;
    },

    events: {

    }

});

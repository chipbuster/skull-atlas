webmol.App = Backbone.View.extend({

    el: $('body'),

    views: {
        viewer: new webmol.view.Viewer({
            el: $('#canvas')
        }),
        finder: new webmol.view.Finder({
            el: $('#finder')
        }),
        browser: new webmol.view.Browser({
            el: $('#browser')
        })
    },

    viewer: new webmol.Viewer(),

    init: function (options) {

        var element = $(this.el),
            views = this.views,
            viewer = this.viewer;

        viewer.init({
            canvas: $('#canvas'),
            console: $('#console')
        });

        views.viewer.init({
            viewer: viewer
        });

        views.finder.init({
            viewer: viewer
        });

        views.browser.init({
            viewer: viewer
        });

        return this;
    }

});

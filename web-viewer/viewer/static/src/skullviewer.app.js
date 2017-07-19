skullviewer.App = Backbone.View.extend({
    
    el: $('body'),

    views:  {
        viewer: new skullviewer.view.Viewer({
            el: $('#canvas')
        }),
        finder: new skullviewer.view.Finder({
            el: $('#finder')
        }),
        browser: new skullviewer.view.Browser({
            el: $('#browser')
        })
    },

    viewer: new skullviewer.Viewer(),

    init: function (option) {

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

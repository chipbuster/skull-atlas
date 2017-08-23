skullviewer.App = Backbone.View.extend({
    
    el: $('body'),

    views: {
        viewer: new skullviewer.view.Viewer({
            el: $('#viewer')
        }),
        finder: new skullviewer.view.Finder({
            el: $('#finder')
        }),
        browser: new skullviewer.view.Browser({
            el: $('#browser')
        })
    },

    manager: new skullviewer.Manager(),

    init: function (option) {

        var element = $(this.el),
            views = this.views,
            manager = this.manager;

        manager.init({
            console: $('#console')
        });

        views.viewer.init({
            manager: manager,
        });

        views.finder.init({
            manager: manager
        });

        views.browser.init({
            manager: manager
        });

        return this;
    }

});

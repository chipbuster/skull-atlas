skullviewer.Manager = Backbone.View.extend({

    views: null,

    init: function (options) {
        console.log('SkullView Manager init');
        this.views = options.views;
    },

    loadFile: function (objFile) {

        // Remove cube and any query skull already loaded
        // scene.remove(scene.getObjectByName("temp_cube"));
        // scene.remove(scene.getObjectByName("query_skull"));

        var fileName = objFile.name,
            context = this,
            callback = function(event) {
                var text = event.target.result;
                context.loadObjToScene(text);
            };
        this.loadTextFromFile(objFile, callback);
    },

    loadTextFromFile: function(objFile, callback) {
        var reader = new FileReader();
        reader.onloadend = callback;
        reader.readAsText(objFile);
        console.log('starting read');
    },

    loadClick: function() {
        console.log('On load Click');
        var context = this;
        var skull1_id = 'query';
        var skull2_id = 'result';
        $.get('similar/?skull1=query&skull2=result', function (data) {
            context.loadObjToScene(skull1_id, data['skull1_obj']);
            context.loadObjToScene(skull2_id, data['skull2_obj']);
        });
    },

    deformClick: function() {
        console.log('On deform Click');
        var context = this;
        var skull1_id = 'result';
        var skull2_id = 'query';
        var callback = function (data) {
            frame = parseInt(data['frame']);
            maxframe = parseInt(data['max_frame'])
            console.log(frame);
            context.loadObjToScene('result', data['obj_decimated']);
            if (frame <= maxframe) {
                $.get('deform/?skull1='+skull1_id+'&skull2='+skull2_id+'&frame='+frame+'&dec=1', callback)
            }
        };
        $.get('deform/?skull1='+skull1_id+'&skull2='+skull2_id+'&frame=1&dec=1', callback);
    },

    loadObjToScene: function(id, fileContents) {
        var OBJMaterial = new THREE.MeshPhongMaterial({color: 0xf0f0f0});
        var loader = new THREE.OBJLoader();
        var object = (loader.parse(fileContents)).children[0];
        console.log(this.scene);
        console.log(object);
        if (object instanceof THREE.Mesh) {
            object.material = OBJMaterial;
            object.name = id;
            this.views.viewer.renderSkull(id, object);
        }
    },

    onMousedown: function(event, ui) {
        //TODO
        console.log("onMousedown");
        return this;
    },

    onMouseup: function(event, ui) {
        //TODO
        console.log("onMouseup");
        return this;
    },

    onMousemove: function(event, ui) {
        //TODO
        console.log("onMousemove");
        return this;
    },

    onMousewheel: function(event, ui) {
        //TODO
        console.log("onMousewheel");
        return this;
    },

    onClick: function(event, ui) {
        //TODO
        console.log("onClick");
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

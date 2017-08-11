skullviewer.Viewer = Backbone.View.extend({

    scene: new THREE.Scene(),

    camera: new THREE.PerspectiveCamera( 75, window.innerWidth/(window.innerHeight*0.8), 0.1, 1000 ),

    renderer: new THREE.WebGLRenderer(),

    init: function (options) {

        var scene = this.scene,
            camera = this.camera,
            renderer = this.renderer;
        renderer.setSize( window.innerWidth, window.innerHeight*0.8 );
        document.body.replaceChild( renderer.domElement , document.getElementById("viewer"));

        var geometry = new THREE.BoxGeometry( 2, 2, 2 );
        var material = new THREE.MeshBasicMaterial( { color: 0x2c2c2c , wireframe: true} );
        var cube = new THREE.Mesh( geometry, material );
        cube.name = "temp_cube";
        scene.add( cube );
        scene.background = new THREE.Color( 0xfcfcfc );

        camera.position.z = 5;

        var animate = function () {
            requestAnimationFrame( animate );
            cube.rotation.x += 0.01;
            cube.rotation.y += 0.01;
            renderer.render(scene, camera);
        };
        animate();
    },

    loadFile: function (objFile) {
        var scene = this.scene,
            camera = this.camera,
            renderer = this.renderer;

        // Remove cube and any query skull already loaded
        scene.remove(scene.getObjectByName("temp_cube"));
        scene.remove(scene.getObjectByName("query_skull"));

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

    loadObjToScene: function(fileContents) {
        var OBJMaterial = new THREE.MeshPhongMaterial({color: 0x8888ff});
        var loader = new THREE.OBJLoader();
        var object = (loader.parse(fileContents)).children[0];
        console.log(this.scene);
        console.log(object);
        if (object instanceof THREE.Mesh) {
            object.material = OBJMaterial;
            object.name = "query_mesh";
            console.log(this.scene)
            /*
            var geometry = new THREE.BoxGeometry( 5, 5, 5 );
            var material = new THREE.MeshBasicMaterial( { color: 0x2c2c2c , wireframe: true} );
            var cube = new THREE.Mesh( geometry, material ); 
            this.scene.add(cube);
            */
            this.scene.add(object);
            console.log(this.scene)
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

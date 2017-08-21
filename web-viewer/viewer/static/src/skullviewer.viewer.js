skullviewer.Viewer = Backbone.View.extend({

    scene: new THREE.Scene(),

    camera: new THREE.PerspectiveCamera(90, window.innerWidth/(window.innerHeight*0.8), 0.1, 1000 ),

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

        controls = new THREE.OrbitControls( camera , renderer.domElement);

        // Set initial camera position so skulls look right
        this.camera.position.set(150,0,0);

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

        camera.zoom = 0.01;
    },

    loadTextFromFile: function(objFile, callback) {
        var reader = new FileReader();
        reader.onloadend = callback;
        reader.readAsText(objFile);
        console.log('starting read');
    },

    repositionSkullMesh: function(meshobj){
        /* Incredibly crude approximation: compute bounding box and use its
           center/scale as the center/scale for the whole skull. The skulls
           are usually *almost* axis-aligned so this shouldn't come out too
           badly on the whole */

        meshobj.geometry.computeBoundingBox(); // Force computation of the bounding box
        var skullBBcenter = meshobj.geometry.center(); // Compute center based on BB

        // Subtract center from vertices to center mesh
        for (vert in meshobj.geometry.vertices){
            vert.sub(skullBBcenter);
        }

        /* If looking from the +X axis, skull starts with top of head to left,
           facing away from camera. Need to apply appropriate rotations to fix*/
        meshobj.rotation.x -= 90 * Math.PI / 180;
        meshobj.rotation.z += 180 * Math.PI / 180;

        /* Tell threejs to update the mesh */
        meshobj.geometry.verticesNeedUpdate = true;

    },

    loadObjToScene: function(fileContents) {
        // Set up the material for the mesh
        var OBJMaterial = new THREE.MeshPhongMaterial(
            {vertexColors: THREE.FaceColors, shading: THREE.FlatShading, side: THREE.DoubleSide});

        var loader = new THREE.OBJLoader();
        var object = (loader.parse(fileContents)).children[0];
        if (object instanceof THREE.Mesh) {
            object.material = OBJMaterial;
            object.name = "query_mesh";

            /* For testing purposes
            var geometry = new THREE.BoxGeometry( 5, 5, 5 );
            var material = new THREE.MeshBasicMaterial( { color: 0x2c2c2c , wireframe: true} );
            var cube = new THREE.Mesh( geometry, material );
            this.scene.add(cube);
            var animate = function () {
            requestAnimationFrame( animate );
            cube.rotation.x += 0.01;
            cube.rotation.y += 0.01;
            };
            animate();
            */

            this.repositionSkullMesh(object);
            this.scene.add(object);

            // Add lighting
            var l1 = new THREE.PointLight(0xffffff, 1000, 0);
            var l2 = new THREE.PointLight(0xffffff, 1000, 0);
            var l3 = new THREE.AmbientLight(0xffffff, 10);
            l1.position.set(50,0,0);
            l1.position.set(0,500,0);
            this.scene.add(l1);
            this.scene.add(l2);
            this.scene.add(l3);

            console.log(object);

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

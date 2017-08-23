skullviewer.view.viewer.Scene = Backbone.View.extend({

    scene: null,

    camera: null,

    render: null,

    light: null,

    controls: null,

    init: function (options) {
        var element = $(this.el),
            viwer = options.viwer;
        
        this.element = element;
        this.viwer = viwer;

        var height = element.innerHeight();
        var width = element.innerWidth();
        var canvasID = element.attr('id');
        var scene = new THREE.Scene(),
            camera = new THREE.PerspectiveCamera( 45.0, width/height, 0.1, 1000 ),
            renderer = new THREE.WebGLRenderer({canvas: document.getElementById(canvasID)}),
            light = new THREE.PointLight( 0x4d4d4d);
            // control = new THREE.OrbitControls( camera, renderer.domElement );
        light.position.set(-1000, 0, 0);
        camera.position.set(-400, 0, 0);
        camera.up = new THREE.Vector3(0,0,1);
        camera.lookAt(new THREE.Vector3(0,0,0));


        this.scene = scene;
        this.camera = camera;
        this.renderer = renderer;
        this.light = light;

        var controls;

        // controls = new THREE.OrbitControls( camera, document.getElementById('result-canvas'));
        controls = new THREE.OrbitControls( camera, renderer.domElement);
        controls.addEventListener( 'change', function(){
            renderer.render(scene, camera);
        }); // remove when using animation loop
				// enable animation loop when using damping or autorotation
				//controls.enableDamping = true;
				//controls.dampingFactor = 0.25;
		controls.enableZoom = true;
        this.controls = controls;

        renderer.setSize( width, height );
        var geometry = new THREE.BoxGeometry( 20, 20, 20 );
        var material = new THREE.MeshBasicMaterial( { color: 0x2c2c2c , wireframe: true} );
        var cube = new THREE.Mesh( geometry, material );
        cube.name = "temp_cube";
        scene.add(cube);
        scene.add(light);

        scene.background = new THREE.Color( 0xfcfcfc );
        
        renderer.render(scene, camera);
        return this;
    },

    animate: function () {
        console.log('Ani Ani');
        requestAnimationFrame( this.animate );
        this.controls.update(); // required if controls.enableDamping = true, or if controls.autoRotate = true
        this.renderer.render(this.scene, this.camera);
	},

    render: function () {
        this.renderer.render(this.scene, this.camera);
    },

    renderSkull: function (name, obj) {
        this.scene.remove(this.scene.getObjectByName('temp_cube'));
        this.scene.add(obj);
        this.renderer.render(this.scene, this.camera);
    }
});
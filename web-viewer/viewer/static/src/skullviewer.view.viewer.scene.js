skullviewer.view.viewer.Scene = Backbone.View.extend({

    scene: null,

    camera: null,

    render: null,

    light: null,

    controls: null,

    id: '',

    init: function (options) {
        var element = $(this.el),
            viewer = options.viewer;
            id = options.id;
        console.log('INIT ID', id);
        this.element = element;
        this.viewer = viewer;
        this.id = id;

        var height = element.innerHeight();
        var width = element.innerWidth();
        console.log(height, width);
        var canvasID = element.attr('id');
        var scene = new THREE.Scene(),
            camera = new THREE.PerspectiveCamera( 45.0, width/height, 0.1, 1000 ),
            renderer = new THREE.WebGLRenderer({canvas: document.getElementById(canvasID)}),
            light = new THREE.PointLight( 0xa0a0a0);
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
        var context = this;    
        camControls = new THREE.OrbitControls( camera, renderer.domElement);
        lightControls = new THREE.OrbitControls( light, renderer.domElement);
        camControls.addEventListener( 'change', function(){
            viewer.sceneSync(context.id, context.camera, context.light);
        }); // remove when using animation loop
				// enable animation loop when using damping or autorotation
				//controls.enableDamping = true;
				//controls.dampingFactor = 0.25;
		camControls.enableZoom = true;
        camControls.enablePan = false;
        lightControls.enableZoom = false;
        lightControls.enablePan = false;

        // this.controls = controls;

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
        console.log(this.id);
        this.scene.remove(this.scene.getObjectByName('temp_cube'));
        this.scene.add(obj);
        this.renderer.render(this.scene, this.camera);
    }
});
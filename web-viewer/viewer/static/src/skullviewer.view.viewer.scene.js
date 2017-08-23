skullviewer.view.viewer.Scene = Backbone.View.extend({

    scene: null,

    camera: null,

    render: null,

    init: function (options) {
        var element = $(this.el),
            viwer = options.viwer;
        
        this.element = element;
        this.viwer = viwer;

        height = element.innerHeight();
        width = element.innerWidth();
        canvasID = element.attr('id');
        var scene = new THREE.Scene(),
            camera = new THREE.PerspectiveCamera( 75, width/height, 0.1, 1000 ),
            renderer = new THREE.WebGLRenderer({canvas: document.getElementById(canvasID)});

        renderer.setSize( width, height );
        var geometry = new THREE.BoxGeometry( 2, 2, 2 );
        var material = new THREE.MeshBasicMaterial( { color: 0x2c2c2c , wireframe: true} );
        var cube = new THREE.Mesh( geometry, material );
        cube.name = "temp_cube";
        scene.add( cube );
        scene.background = new THREE.Color( 0xfcfcfc );

        camera.position.z = 5;
        renderer.render(scene, camera);
        return this;
    }
});
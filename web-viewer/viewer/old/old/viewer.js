function draw (vertices, faces) {

    var node_scene = {
            type: 'scene',
            id: 'node_scene',
            canvasId: 'canvas',
            loggingElementId: 'div-log',
        },
        node_lookAt = {
            type: 'lookAt',
            id: 'node_lookAt',
            eye : { x: 0.0, y: 10.0, z: 400 },
            look : { y:1.0 },
            up : { y: 1.0 }
        },
        node_camera = {
            type: 'camera',
            id: 'node_camera',
            optics: {
                type: 'perspective',
                fovy : 45.0,
                aspect : 1.47,
                near : 0.10,
                far : 1000.0
            }
        },
        node_light = {
            type: 'light',
            id: 'node_light',
            mode: 'point',
            color: { r: 0.8, g: 0.8, b: 0.8 },
            diffuse: true,
            specular: true,
            pos: { x: 0.0, y: 0.0, z: 500.0 }
        },
        node_pitch = {
            type: 'rotate',
            id: 'node_pitch',
            angle: 0.0,
            x : 1.0
        },
        node_yaw = {
            type: 'rotate',
            id: 'node_yaw',
            angle: 0.0,
            y : 1.0
        },
        node_material = {
            type: 'material',
            id: 'node_material',
            emit: 0.1,
            baseColor: { r: 0.8, g: 0.55, b: 0.5 },
            specularColor: { r: 0.8, g: 0.55, b: 0.5 },
            specular: 300,
            shine: 300
        },
        node_geometry = {
            type : 'geometry',
            id: 'node_geometry',
            primitive: 'triangles',
            positions: vertices,
            indices: faces
        };

    SceneJS.createNode(node_scene);
    SceneJS.withNode('node_scene').add('node', node_lookAt);
    SceneJS.withNode('node_lookAt').add('node', node_camera);
    SceneJS.withNode('node_camera').add('node', node_light);
    SceneJS.withNode('node_light').add('node', node_pitch);
    SceneJS.withNode('node_pitch').add('node', node_yaw);
    SceneJS.withNode('node_yaw').add('node', node_material);
    SceneJS.withNode('node_material').add('node', node_geometry);
    SceneJS.withNode('node_scene').render();
    SceneJS.withNode('node_scene').start();

    var canvas = $('#canvas'),
        yaw = 0,
        pitch = 0,
        lastX,
        lastY,
        dragging = false;


    canvas
        .bind('mousedown', function (event) {

            lastX = event.originalEvent.clientX;
            lastY = event.originalEvent.clientY;
            dragging = true;
        })
        .bind('mouseup', function (event) {

            dragging = false;
        })
        .bind('mousemove', function (event) {

            if (dragging) {
                yaw += (event.originalEvent.clientX - lastX) * 0.5;
                pitch += (event.originalEvent.clientY - lastY) * 0.5;

                SceneJS.withNode('node_yaw').set('angle', yaw);
                SceneJS.withNode('node_pitch').set('angle', pitch);

                lastX = event.originalEvent.clientX;
                lastY = event.originalEvent.clientY;
            }
        });
}
webmol.Viewer = Backbone.Controller.extend({

    init: function (options) {

        var canvas = $(options.canvas),
            divconsole = $(options.console) || $('<div>'),
            aspect = canvas.width() / canvas.height(),
            nodes = this.nodes,
            scene = nodes.scene,
            renderer = nodes.renderer,
            camera = nodes.camera,
            observer = nodes.observer,
            light = nodes.light,
            transform = nodes.transform,
            anchor = nodes.anchor,
            axis = nodes.axis;

        scene.canvasId = $(canvas).attr('id');
        scene.loggingElementId = $(divconsole).attr('id');

        camera.optics.aspect = aspect;

        SceneJS.createNode(scene);

        SceneJS.withNode('scene').add('node', renderer);
        SceneJS.withNode('renderer').add('node', observer);
        SceneJS.withNode('observer').add('node', camera);
        SceneJS.withNode('camera').add('node', light);
        SceneJS.withNode('light').add('node', transform);
        SceneJS.withNode('origin').add('node', anchor);
        SceneJS.withNode('origin').add('node', axis);

        this.addGrid();

        SceneJS.withNode('scene').render();
        SceneJS.withNode('scene').start();

        this.canvas = canvas;
        this.console = console;
        this.meshes = new webmol.collection.Mesh();

        this.meshes.bind('add', $.proxy(this.addMesh, this));
    },

    loadFile: function (fileObj) {

        this.removeGrid();
        this.meshes.loadFile(fileObj);

        return this;
    },

    rotate: function (euler) {

        var pitch = SceneJS.withNode('pitch'),
            yaw = SceneJS.withNode('yaw');

        pitch.set('angle', euler.x);
        yaw.set('angle', euler.y);

        return this;
    },

    zoom: function (delta) {

        var observer = SceneJS.withNode('observer'),
            eye = observer.get('eye');

        eye.z -= delta * 5;
        observer.set('eye', eye);

        return this;
    },

    pick: function (position) {

        SceneJS.withNode("scene").pick(position.x, position.y);

        return this;
    },

    selectMesh: function (mesh, select) {

        var node = mesh.node,
            id = node.id;
    },

    addMesh: function (mesh) {

        var node = mesh.node,
            id = node.id;

        SceneJS.withNode('origin').add('node', mesh.node);

        SceneJS
            .withNode(id)
            .bind("picked", $.proxy(function (event) {
                this.selectMesh(mesh, true);
            }, this));

        return this;
    },

    addGrid: function (options) {

        var options = options || {},
            grid_vertices = [],
            grid_h_vertices = [],
            grid_v_vertices = [],
            grid_indices = [],
            grid_step = options.step || 10,
            grid_width = options.width || 250,
            grid_height = options.height || 250,
            h_offset = grid_width / 2,
            v_offset = grid_height / 2,
            h_val,
            v_val,
            i,
            node_grid;

        for (i = 0; i < grid_width / grid_step + 1; i += 1){

            h_val = i * grid_step - h_offset;
            v_val = i * grid_step - v_offset

            grid_vertices.push(-h_offset, h_val, 0);
            grid_vertices.push(+h_offset, h_val, 0);

            grid_vertices.push(v_val, -v_offset, 0);
            grid_vertices.push(v_val, +v_offset, 0);
        }

        for (i = 0; i < grid_vertices.length / 3; i += 2){

            grid_indices.push(i, i + 1);
        }

        // console.log(grid_vertices, grid_indices);

        node_grid = {
            type: "node",
            id: "grid",
            nodes: [
                {
                    type: "material",
                    id: "grid_material",
                    baseColor: {
                        r: 0.,
                        g: 0.,
                        b: 0.
                    },
                    specularColor: {
                        r: 0.,
                        g: 0.,
                        b: 0.
                    },
                    specular: 0.5,
                    shine: 0.5,
                    emit: 0.5,
                    nodes: [
                        {
                            type: "geometry",
                            primitive: 'lines',
                            positions: grid_vertices,
                            indices: grid_indices
                        }
                    ]
                }
            ]
        };

        SceneJS.withNode('origin').add('node', node_grid);

    },

    removeGrid: function (options) {

        //var node = SceneJS.withNode('origin'),
        var node = SceneJS.withNode('grid');
        node.destroy();
    },


    nodes: {
        scene: {
            type: 'scene',
            id: 'scene'
        },

        renderer: {
            type: 'renderer',
            id: 'renderer',
            clearColor: {
                r: 1.0,
                g: 1.0,
                b: 1.0,
                a: 1.0
            },
            clear: {
                depth: true,
                color: true
            },
            depthRange: {
                near: 0.1,
                far: 1000
            }
        },

        observer: {
            type: 'lookAt',
            id: 'observer',
            eye: {
                x: 0.0,
                y: 0.0,
                z: 400
            },
            look: {
                x: 0.0,
                y: 0.0,
                z: 0.0
            },
            up: {
                y: 1.0
            }
        },

        camera: {
            type: 'camera',
            id: 'camera',
            optics: {
                type: 'perspective',
                fovy : 45.0,
                aspect : 1.47,
                near : 0.10,
                far : 1000.0
            }
        },

        light: {
            type: 'light',
            id: 'light',
            mode: 'point',
            color: {
                r: 0.3,
                g: 0.3,
                b: 0.3
            },
            diffuse: true,
            specular: true,
            pos: {
                x: 0.0,
                y: 0.0,
                z: 1000.0
            }
        },

        transform: {
            type: 'node',
            id: 'transform',
            nodes: [
                {
                    type: 'rotate',
                    id: 'pitch',
                    angle: 0.0,
                    x : 1.0,
                    nodes: [
                        {
                            type: 'rotate',
                            id: 'yaw',
                            angle: 0.0,
                            y : 1.0,
                            nodes: [
                                {
                                    type: 'node',
                                    id: 'origin'
                                }
                            ]
                        }
                    ]
                }
            ]
        },

        axis: {
            type: 'node',
            id: 'axis',
            nodes: [
                {
                    //x
                    type: "material",
                    baseColor: { r: 1, g: 0., b: 0. },
                    specularColor: { r: 1., g: 0., b: 0. },
                    specular: 0.9,
                    shine: 6,
                    emit: 0.0,
                    nodes: [
                        {
                            type: 'translate',
                            x: 9.8,
                            y: 0,
                            z: 0,
                            nodes: [
                                {
                                    type: 'cube',
                                    xSize: 10,
                                    ySize: 0.2,
                                    zSize: 0.2
                                }
                            ]
                        }
                    ]
                },
                {
                    //y
                    type: "material",
                    baseColor: { r: 0., g: 1., b: 0. },
                    specularColor: { r: 0., g: 1., b: 0. },
                    specular: 0.9,
                    shine: 6,
                    emit: 0.0,
                    nodes: [
                        {
                            type: 'translate',
                            x: 0,
                            y: 9.8,
                            z: 0,
                            nodes: [
                                {
                                    type: 'cube',
                                    xSize: 0.2,
                                    ySize: 10,
                                    zSize: 0.2
                                }
                            ]
                        }
                    ]
                },
                {
                    //z
                    type: "material",
                    baseColor: { r: 0., g: 0., b: 1. },
                    specularColor: { r: 0., g: 0., b: 1. },
                    specular: 0.9,
                    shine: 6.0,
                    emit: 0.0,
                    nodes: [
                        {
                            type: 'translate',
                            x: 0,
                            y: 0,
                            z: 9.8,
                            nodes: [
                                {
                                    type: 'cube',
                                    xSize: 0.2,
                                    ySize: 0.2,
                                    zSize: 10 
                                }
                            ]
                        }
                    ]
                }
            ]
        },

        anchor: {
            type: "node",
            id: "anchor",
            nodes: [
                {
                    type: "material",
                    baseColor: {
                        r: 0.0,
                        g: 0.0,
                        b: 0.0
                    },
                    specularColor: {
                        r: 0.0,
                        g: 0.0,
                        b: 0.0
                    },
                    specular: 0,
                    shine: 0,
                    emit: 0.0,
                    nodes: [
                        {
                            type: 'translate',
                            x: 0,
                            y: 0,
                            z: 0,
                            nodes: [
                                {
                                    type: "cube",
                                    xSize: 0.25,
                                    ySize: 0.25,
                                    zSize: 0.25
                                }
                            ]
                        }
                    ]
                }
            ]
        }
    }

});

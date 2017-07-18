function draw(vertices, colors, faces) {

    SceneJS.createNode({

        type: "scene",

        /* ID that we'll access the scene by when we want to render it
         */
        id: "theScene",

        /* Bind scene to a WebGL canvas:
         */
        canvasId: "canvas-01",

        /* You can optionally write logging to a DIV - scene will log to the console as well.
         */
        loggingElementId: "theLoggingDiv",

        nodes: [

            {
                type: "renderer",
                clearColor : { r:1.0, g:1.0, b:1.0, a: 1.0 },
                clear : { depth : true, color : true} ,
                depthRange : { near: .5, far: 1500 },

                nodes: [




            /* Viewing transform specifies eye position, looking
             * at the origin by default
             */
            {
                type: "lookAt",
                eye : { x: 0.0, y: 10.0, z: 700 },
                look : { y:1.0 },
                up : { y: 1.0 },
                clearColor : { r:1.0, g:1.0, b:1.0, a: 1.0 },

                nodes: [

                    /* Camera describes the projection
                     */
                    {
                        type: "camera",
                        optics: {
                            type: "perspective",
                            fovy : 45.0,
                            aspect : 1.47,
                            near : 0.10,
                            far : 1000.0
                        },

                        nodes: [


                            /* A lights node inserts point lights into scene, to illuminate everything
                             * that is encountered after them during scene traversal.
                             *
                             * You can have many of these, nested within modelling transforms if you want to move them.
                             */
                            // {
                            //     type: "light",
                            //     mode:                   "dir",
                            //     color:                  { r: 1.0, g: 1.0, b: 1.0 },
                            //     diffuse:                true,
                            //     specular:               true,
                            //     dir:                    { x: 1.0, y: -0.5, z: -1.0 }
                            // },

                            // {
                            //     type: "light",
                            //     mode:                   "dir",
                            //     color:                  { r: 1.0, g: 1.0, b: 0.8 },
                            //     diffuse:                true,
                            //     specular:               false,
                            //     dir:                    { x: 0.0, y: -0.5, z: -1.0 }
                            // },
                            {
                                type: "light",
                                mode:                   "point",
                                color:                  { r: 1.0, g: 1.0, b: 1.0 },
                                diffuse:                true,
                                specular:               true,
                                pos:                    { x: 0.0, y: 0.0, z: 2000.0 }
                            },
                            // {
                            //     type: "light",
                            //     mode:                   "point",
                            //     color:                  { r: 0.8, g: 0.8, b: 0.8 },
                            //     diffuse:                true,
                            //     specular:               true,
                            //     pos:                    { x: 0.0, y: 1000.0, z: 0.0 }
                            // },
                            // {
                            //     type: "light",
                            //     mode:                   "point",
                            //     color:                  { r: 0.8, g: 0.8, b: 0.8 },
                            //     diffuse:                true,
                            //     specular:               true,
                            //     pos:                    { x: 0.0, y: 0.0, z: 1000.0 }
                            // },

                            /* Next, modelling transforms to orient our teapot. See how these have IDs,
                             * so we can access them to set their angle attributes.
                             */
                            {
                                type: "rotate",
                                id: "pitch",
                                angle: 0.0,
                                x : 1.0,

                                nodes: [
                                    {
                                        type: "rotate",
                                        id: "yaw",
                                        angle: 0.0,
                                        y : 1.0,

                                        nodes: [

                                            /* Specify the amounts of ambient, diffuse and specular
                                             * lights our teapot reflects
                                             */
                                            {
                                                type: "material",
                                                emit: 0.1,
                                                baseColor:      { r: 1.0, g: 1.0, b: 1.0 },
                                                specularColor:  { r: 0.9, g: 0.9, b: 0.9 },
                                                specular:       0.4,
                                                shine:          5.0,
                                                //alpha: 1.0,

                                                nodes: [
                                                    {
                                                        type: "translate",
                                                        // Example translation
                                                        // x:-centroid[0],
                                                        // y:-centroid[1],
                                                        // z:-centroid[2],
                                                        x: 0,
                                                        y: 0,
                                                        z: 0,

                                                        nodes : [
                                                            {
                                                                type: "scale",
                                                                // Example scaling
                                                                x:1.0,
                                                                y:1.0,
                                                                z:1.0,

                                                                nodes: [

                                                                    {
                                                                        type : "geometry",
                                                                        primitive: "triangles",
                                                                        positions: flatten(vertices, 3),
                                                                        indices: flatten(faces, 3),
                                                                        normals: flatten(calculateNormals(vertices, faces), 3),
                                                                        colors: flatten(colors)
                                                                    }
                                                                    // {
                                                                    //     type: "teapot"
                                                                    // }
                                                                ]
                                                            }
                                                        ]
                                                    }
                                                ]
                                            }
                                        ]
                                    }
                                ]
                            }
                        ]
                    }
                ]

            }]

            }
        ]
    });

    function calculateNormals (positions, indices) {
        var nvecs = new Array(positions.length);

        for (var i = 0; i < indices.length; i++) {
            var j0 = indices[i][0];
            var j1 = indices[i][1];
            var j2 = indices[i][2];

            var v1 = positions[j0];
            var v2 = positions[j1];
            var v3 = positions[j2];

            v2 = SceneJS_math_subVec4(v2, v1, [0,0,0,0]);
            v3 = SceneJS_math_subVec4(v3, v1, [0,0,0,0]);

            // var n = SceneJS_math_normalizeVec4(SceneJS_math_cross3Vec4(v2, v3, [0,0,0,0]), [0,0,0,0]);
            var n = SceneJS_math_normalizeVec4(SceneJS_math_cross3Vec4(v2, v3, [0,0,0,0]), [0,0,0,0]);

            if (!nvecs[j0]) nvecs[j0] = [];
            if (!nvecs[j1]) nvecs[j1] = [];
            if (!nvecs[j2]) nvecs[j2] = [];

            nvecs[j0].push(n);
            nvecs[j1].push(n);
            nvecs[j2].push(n);
        }

        var normals = new Array(positions.length);

        // now go through and average out everything
        for (var i = 0; i < nvecs.length; i++) {
            var count = nvecs[i].length;
            var x = 0;
            var y = 0;
            var z = 0;
            for (var j = 0; j < count; j++) {
                x += nvecs[i][j][0];
                y += nvecs[i][j][1];
                z += nvecs[i][j][2];
            }
            normals[i] = [-(x / count), -(y / count), -(z / count)];
        }

        return normals;
    };

    // @private
    function flatten (ar, numPerElement) {
            var result = [];
            for (var i = 0; i < ar.length; i++) {
                if (numPerElement && ar[i].length != numPerElement)
                    throw "Bad geometry array element";
                for (var j = 0; j < ar[i].length; j++)
                    result.push(ar[i][j]);
            }
            return result;
    };

    /*----------------------------------------------------------------------
     * Scene rendering loop and mouse handler stuff follows
     *---------------------------------------------------------------------*/
    var yaw = 0;
    var pitch = 0;
    var lastX;
    var lastY;
    var dragging = false;

    SceneJS.withNode("theScene").render();

    var canvas = document.getElementById("canvas-01");

    function mouseDown(event) {
        lastX = event.clientX;
        lastY = event.clientY;
        dragging = true;
    }

    function mouseUp() {
        dragging = false;
    }

    /* On a mouse drag, we'll re-render the scene, passing in
     * incremented angles in each time.
     */
    function mouseMove(event) {
        if (dragging) {
            yaw += (event.clientX - lastX) * 0.5;
            pitch += (event.clientY - lastY) * 0.5;

            SceneJS.withNode("yaw").set("angle", yaw);
            SceneJS.withNode("pitch").set("angle", pitch);

            lastX = event.clientX;
            lastY = event.clientY;
        }
    }

    canvas.addEventListener('mousedown', mouseDown, true);
    canvas.addEventListener('mousemove', mouseMove, true);
    canvas.addEventListener('mouseup', mouseUp, true);


    SceneJS.withNode("theScene").start();
}
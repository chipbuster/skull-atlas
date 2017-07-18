var canvas = $('#canvas'),
    button = $('#button-start'),

    dragging = false,
    lastX,
    lastY,
    dx = 0,
    dy = 0,
    end_x = 0,
    end_y = 0,
    yaw = 0,
    pitch = 0,

    pi = Math.PI,
    sin = Math.sin,
    cos = Math.cos,
    radii = pi / 180,
    range_x = $('#range-x'),
    range_y = $('#range-y'),
    range_z = $('#range-z'),

    width = Number(canvas.css('width').split('px')[0]),
    height = Number(canvas.css('height').split('px')[0]);

function load (name, callback) {

    $.ajax({
        url: "data/" + name,
        context: document.body,
        success: callback,
        error: function (msg) { alert(msg); }
    });
}

function parse_RAW (type, text, callback) {

    var row_start,
        row_end,
        row,
        row_values,
        n_vertices,
        n_faces,
        vertices = [],
        normals = [],
        colors = [],
        faces = [],
        i;

    row_start = 0;
    row_end = text.indexOf('\n');

    row = text.slice(row_start, row_end);
    row_values = row.split(' ');
    n_vertices = row_values[0];
    n_faces = row_values[1];

    for (i = 0; i < n_vertices; i += 1) {
        row_start = row_end + 1;
        row_end = text.indexOf('\n', row_start);
        row = text.slice(row_start, row_end);
        row_values = row.split(' ');
        vertices.push([row_values[0], row_values[1], row_values[2]]);

        // if (row_values.length === 9) {
        //     normals.push([row_values[3], row_values[4], row_values[5]]);
        //     colors.push([row_values[6], row_values[7], row_values[8]]);
        // }
        // else if (row_values.length === 6) {
        //     colors.push([row_values[3], row_values[4], row_values[5], 1]);
        // }
    }

    for (i = 0; i < n_faces; i += 1) {
        row_start = row_end + 1;
        row_end = text.indexOf('\n', row_start);
        row = text.slice(row_start, row_end);
        row_values = row.split(' ');
        faces.push([row_values[0], row_values[2], row_values[1]]);
    }

    callback({
        vertices: vertices,
        // normals: normals,
        // colors: colors,
        faces: faces
    });
}

function create_mesh_node (data, callback) {

    var vertices = data.vertices,
        normals = data.normals,
        colors = data.colors,
        faces = data.faces,
        geometry;

    node_geometry = {
        type : 'geometry',
        primitive: 'triangles',
        positions: flatten(vertices, 3),
        indices: flatten(faces, 3),
        normals: flatten(calculateNormals(vertices, faces), 3),
        //colors: flatten(colors)
    };

    callback(node_geometry);
}

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
}

function flatten (ar, numPerElement) {
    var result = [];
    for (var i = 0; i < ar.length; i++) {
        if (numPerElement && ar[i].length != numPerElement)
            throw "Bad geometry array element";
        for (var j = 0; j < ar[i].length; j++)
            result.push(ar[i][j]);
    }
    return result;
}

function parse_OUT(type, text, callback) {
     var row_start,
        row_end,
        row,
        row_values,

        rank_col_number = 0,
        score_col_number = 0,
        first_mat_col_number = 0,
        point_found = false,
        rank_found = false,
        score_found = false,
        mat_found = false,
        done = false,
        counter = 0
        output = [];

    row_start = 0;
    row_end = text.indexOf('\n');

    while ( !point_found && row_end != '-1') {

        row = text.slice(row_start, row_end);

        if (row.indexOf('# OUTPUT FORMAT:') == 0) {

            point_found = true;
        }
        row_start = row_end + 1;
        row_end = text.indexOf('\n', row_start);
    }

    while (!(rank_found && score_found && mat_found) && row_end != '-1') {

        row = text.slice(row_start, row_end);

        if (row.match(/#(\s|\t)*COLNAME rank/)) {
            rank_col_number = counter;
            rank_found = true;
        } else if (row.match(/#(\s|\t)*COLNAME score/)) {
            score_col_number = counter;
            score_found = true;
        } else if (row.match(/#(\s|\t)*COLNAME mat1/)) {
            first_mat_col_number = counter;
            mat_found=true;
        }
        counter++;
        row_start = row_end + 1;
        row_end = text.indexOf('\n', row_start);
    }

    point_found = false;
    while ( !point_found && row_end != '-1') {

        row = text.slice(row_start, row_end);

        if (row.indexOf('# START PEAKS') == 0) {

            point_found = true;
        }
        row_start = row_end + 1;
        row_end = text.indexOf('\n', row_start);
    }

    while ( !done && row_end != '-1') {


        row = text.slice(row_start, row_end);
        row_values = (row.replace(/(\s|\t)+/g, ' ')).trim().split(' ');

        if (!(row.indexOf('# END PEAKS') == 0)) {
            var entry = {},
                m = [];
            entry.rank = Number(row_values[rank_col_number]);
            entry.score = Number(row_values[score_col_number]);

            for (var i=0; i<12; i++) {
                m[i] = Number(row_values[first_mat_col_number+i]);
            }

            m[12] = 0.0;
            m[13] = 0.0;
            m[14] = 0.0;
            m[15] = 1.0;

            entry.m = m;

            output.push(entry);
            } else {done = true}

        row_start = row_end + 1;
        row_end = text.indexOf('\n', row_start);
    }

    console.log(output[1]);
    console.log(output[output.length-1]);
}


function draw (name) {

    load(name, function (text) {
        parse_RAW(name, text, function (mesh_data) {
            create_mesh_node(mesh_data, function (mesh_node) {
                add_mesh_node(mesh_node);
            });
        });
    });

    // load(name, function (text) {
    //     parseOUT(name, text);
    // });

    // load(name, function (text) {
    //     parse_PDB(name, text, function (PDB_data) {
    //         create_PDB_nodes(PDB_data, function (PDB_nodes) {
    //             add_PDB_nodes(PDB_nodes);
    //         });
    //     });
    // });

    return true;
}

function draw_options (name, options) {

    load(name, function (text) {
        parse_RAW(name, text, function (mesh_data) {
            create_mesh_node(mesh_data, function (mesh_node) {
                add_mesh_node(mesh_node, options);
            });
        });
    });

    // load(name, function (text) {
    //     parse_PDB(name, text, function (PDB_data) {
    //         create_PDB_nodes(PDB_data, function (PDB_nodes) {
    //             add_PDB_nodes_single_color(PDB_nodes, options);
    //         });
    //     });
    // });
}

function setup () {

    var aspect_ratio = window.width/500;
    console.log(aspect_ratio);

    var node_scene = {
        type: 'scene',
        id: 'node_scene',
        canvasId: 'canvas',
        loggingElementId: 'div-log',
    },
    node_renderer = {
        type: "renderer",
        id: "node_renderer",
        clearColor: { r: 1.0, g: 1.0, b: 1.0, a: 1.0 },
        clear: { depth: true, color: true },
        depthRange: {near: 0.5, far: 1500 }
    },
    node_lookAt = {
        type: 'lookAt',
        id: 'node_lookAt',
        eye : { x: 0.0, y: 10.0, z: 100 },
        look : { y:1.0 },
        up : { y: 1.0 }
    },
    node_camera = {
        type: 'camera',
        id: 'node_camera',
        optics: {
            type: 'perspective',
            fovy : 45.0,
            aspect : aspect_ratio, // 1.47,
            near : 0.10,
            far : 1000.0
        }
    },
    node_light = {
        type: 'light',
        id: 'node_light',
        mode: 'point',
        color: { r: 0.9, g: 0.9, b: 0.9 },
        diffuse: true,
        specular: true,
        pos: { x: 0.0, y: 0.0, z: 1000.0 },
        flags: {
            transparent: true
        }
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
    }

    SceneJS.createNode(node_scene);
    SceneJS.withNode('node_scene').add('node', node_renderer);
    SceneJS.withNode('node_renderer').add('node', node_lookAt);
    SceneJS.withNode('node_lookAt').add('node', node_camera);
    SceneJS.withNode('node_camera').add('node', node_light);
    SceneJS.withNode('node_light').add('node', node_pitch);
    SceneJS.withNode('node_pitch').add('node', node_yaw);
    // SceneJS.withNode('node_yaw').add('node', node_material);
    // SceneJS.withNode('node_material').add('node', node_geometry);

    SceneJS.withNode('node_scene').render();
    SceneJS.withNode('node_scene').start();

};

var counter = 0;
function add_mesh_node (mesh_node, options) {
    var min = Math.min;
        base_color = options.color || { r: 0.6, g: 0.35, b: 0.3 },
        specular_color = { r: min(base_color.r+0.03, 1.0), g: min(base_color.g+0.03, 1.0), b: min(base_color.b+0.03, 1.0) },
        node_material_id = 'node_material_'+counter++,
        alpha = options.alpha || 1.0,
        node_material = {
                            type: 'material',
                            id: node_material_id,
                            emit: 0.1,
                            baseColor: base_color,
                            specularColor: specular_color,
                            specular: 0.5,
                            shine: 4,
                            alpha: alpha
                        };

    SceneJS.withNode('node_yaw').add('node', node_material);
    SceneJS.withNode(node_material_id).add('node', mesh_node);
};

function add_PDB_nodes(nodes) {
    $.each(nodes, function(symbol, symbol_nodes) {
        var node_material_id = 'node_material_'+symbol,
            color = atom_color[symbol],
            base_color_obj = {r: color[0], g: color[1], b: color[2]},
            specular_color_obj = {r:1.0, g: 1.0, b: 1.0},
            node_material = node_material = {
                                    type: 'material',
                                    id: node_material_id,
                                    emit: 0.1,
                                    baseColor: base_color_obj,
                                    specularColor: specular_color_obj,
                                    specular: 0.8,
                                    shine: 10,
                                    alpha: 0.9,
                                };

        SceneJS.withNode('node_yaw').add('node', node_material);

        symbol_nodes.forEach(function (node) {
            SceneJS.withNode(node_material_id).add('node', node);
        });
    });
}

function add_PDB_nodes_single_color(nodes, options) {
    var min = Math.min;
    base_color = options.color || { r: 0.6, g: 0.35, b: 0.3 },
    specular_color = { r: min(base_color.r+0.03, 1.0), g: min(base_color.g+0.03, 1.0), b: min(base_color.b+0.03, 1.0) },
    node_material_id = 'node_material_'+counter++,
    alpha = options.alpha || 1.0,
    node_material = {
                        type: 'material',
                        id: node_material_id,
                        emit: 0.1,
                        baseColor: base_color,
                        specularColor: specular_color,
                        specular: 0.5,
                        shine: 4,
                        alpha: alpha,
                    };

    SceneJS.withNode('node_yaw').add('node', node_material);

    $.each(nodes, function(symbol, symbol_nodes) {
        symbol_nodes.forEach(function (node) {
            SceneJS.withNode(node_material_id).add('node', node);
        });
    });
}



setup();

button
    // .click(function (event) {
    //     button.hide();
    //     files.forEach(function (file) {
    //        draw('Phage/' + file);
    //     });
    // });

    .click(function (event) {
        button.hide();
        draw_options('1ACB_l_u.rawn', {color: { r: 0.35, g: 0.8, b: 0.3 }});
        draw_options('1ACB_r_u.rawn', {color: { r: 0.6, g: 0.35, b: 0.3 }, alpha: 0.6});

        // draw_options('1ACB_l_u.pdb', {color: { r: 0.35, g: 0.8, b: 0.3 }});
        // draw_options('1ACB_r_u.pdb', {color: { r: 0.6, g: 0.35, b: 0.3 }, alpha: 0.6});
        // draw('1ACB_out.txt');
        // draw('1ACB_r_u.pdb');
        // draw('2B42_r_u.pdb');
    });

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
    })
     .bind('mousewheel', function (event, delta) {
        var node_lookat = SceneJS.withNode('node_lookAt'),
            eye = node_lookat.get('eye');

        eye.z -= 5 * delta;

        node_lookat.set('eye', eye);
    });


// picking

SceneJS.withNode("")
     .bind("picked",
          function(event) {
              alert("Picked 'my-node' at coords " + params.canvasX + ", " + params.canvasY);
          });













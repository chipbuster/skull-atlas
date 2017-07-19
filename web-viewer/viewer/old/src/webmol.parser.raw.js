webmol.parser.raw = {

    parse: function (text) {

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

        function flat (matrix) {

            var result = [];

            matrix.forEach(function (vector) {
                vector.forEach(function (element) {
                    result.push(element);
                });
            });

            return result;
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

        return {
            vertices: flat(vertices),
            normals: flat(calculateNormals(vertices, faces)),
            // colors: colors,
            faces: flat(faces)
        };
    }
};

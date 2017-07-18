webmol.parser.obj = {

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
                normals[i] = [(x / count), (y / count), (z / count)];
            }

            return normals;
        }

        function centerVerts(vertices) {
            var centered = [],
                sums = [0,0,0],
                averages = [0,0,0];

            // Calc averages
            for (i = 0; i < vertices.length; i+= 1) {
                var vert = vertices[i];
                sums[0] += parseInt(vert[0]);
                sums[1] += parseInt(vert[1]);
                sums[2] += parseInt(vert[2]);
            }
            averages[0] = sums[0]/vertices.length;
            averages[1] = sums[1]/vertices.length;
            averages[2] = sums[2]/vertices.length;

            // Center verts
            for (i = 0; i < vertices.length; i+= 1) {
                var vert = vertices[i];
                var centeredVert = [0,0,0];
                centeredVert[0] = vert[0] - averages[0];
                centeredVert[1] = vert[1] - averages[1];
                centeredVert[2] = vert[2] - averages[2];
                centered.push(centeredVert);
            }
            return centered;
        }

        function matrixMult(vector, matrix) {
            // Vector to do (1x3) * (3x3) multiplication for rotation
            var out = [0,0,0];

            for (j = 0; j < matrix.length; j+= 1) {
                out[j] = (vector[0] * matrix[0][j]) + 
                         (vector[1] * matrix[1][j]) + 
                         (vector[2] * matrix[2][j]);
            }
            return out;
        }

        function rotateVerts(vertices) {
            var xRotate = -1.57,
                yRotate = 1.57,
                xRotationMatrix = [],
                yRotationMatrix = []
                rotatedVerts = [];
            // Define rotation matrices
            xRotationMatrix.push([1, 0, 0]);
            xRotationMatrix.push([0, Math.cos(xRotate), Math.sin(xRotate)]);
            xRotationMatrix.push([0, 0-Math.sin(xRotate), Math.cos(xRotate)]);
            yRotationMatrix.push([Math.cos(yRotate), 0, 0-Math.sin(yRotate)]);
            yRotationMatrix.push([0, 1, 0]);
            yRotationMatrix.push([Math.sin(yRotate), 0, Math.cos(yRotate)]);

            // Perform X and Y rotations
            for (i = 0; i < vertices.length; i+= 1) {
                var vert = vertices[i];
                rotatedVerts.push(
                    matrixMult(
                        (matrixMult(vert, xRotationMatrix)),
                    yRotationMatrix)
                );
            }
            return rotatedVerts;
        }

        var rows = text.split('\n');
        for (i = 0; i < rows.length; i+= 1) {
          var row = rows[i];
          var row_split = row.split(/\s+/);
          if (row_split[0] === 'v') {
            vertices.push([parseInt(row_split[1]), parseInt(row_split[2]), parseInt(row_split[3])]);
          } else if (row_split[0] === 'f') {
            faces.push([parseInt(row_split[1])-1, parseInt(row_split[2])-1, parseInt(row_split[3])-1]);
          }
        }

        var centeredVertices = centerVerts(vertices);
        var finalVertices = rotateVerts(centeredVertices);

        return {
            vertices: flat(finalVertices),
            normals: flat(calculateNormals(finalVertices, faces)),
            // colors: colors,
            faces: flat(faces)
        };
    }
};       

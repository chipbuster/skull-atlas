webmol.parser.out = {

    parse: function(text) {

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

        return output;
    }

};
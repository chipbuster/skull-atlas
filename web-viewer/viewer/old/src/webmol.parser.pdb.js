webmol.parser.pdb = {

    parse: function (text) {

        var row_start,
            row_end,
            row,
            row_values,
            row_values_length,
            atom,
            atoms = new webmol.collection.Atom();

        row_start = 0;
        row_end = text.indexOf('\n');

        while (row_end != '-1') {

            row = text.slice(row_start, row_end);

            if (row.indexOf('ATOM') == 0) {
                row_values = (row.replace(/(\s)+/g, ' ')).split(' ');
                row_values_length = row_values.length;

                // ["ATOM", "1", "CA", "THR", ("X",) "1", "28.437", "15.526", "-5.270", "0.0034", "1.9080"]
                atom = new webmol.model.Atom();
                atom.init({
                    seqid: Number(row_values[1]),
                    label: row_values[2],
                    amino_label: row_values[3],
                    amino_seq: Number(row_values_length == 11 ? row_values[5] : row_values[4]),
                    position: [
                        Number(row_values_length == 11 ? row_values[6] : row_values[5]),
                        Number(row_values_length == 11 ? row_values[7] : row_values[6]),
                        Number(row_values_length == 11 ? row_values[8] : row_values[7])
                    ]
                });
                atoms.add(atom);
            }
            row_start = row_end + 1;
            row_end = text.indexOf('\n', row_start);
        }

        return atoms;
    }
};
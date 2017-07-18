var app;

$(document).ready(function () {

    $('body')
        .layout({
            north__closable: false,
            north__resizable: false,
        });

    $('#panel')
        .layout({
            south__size: .5
        });

    /*
    var datalist = $('#datalist-search'),
        list = [
            '',
            '1ACB_l_u.pdb',
            '1ACB_l_u.rawn',
            '1ACB_r_u.pdb',
            '1ACB_r_u.rawn',
            '1CLV_l_u.pdb',
            '1CLV_l_u.rawn',
            '1CLV_r_u.pdb',
            '1CLV_r_u.rawn',
            '1GPW_l_u.pdb',
            '1GPW_l_u.rawn',
            '1GPW_r_u.pdb',
            '1GPW_r_u.rawn',
            '2B42_l_u.pdb',
            '2B42_l_u.rawn',
            '2B42_r_u.pdb',
            '2B42_r_u.rawn',
            '4CPA_l_u.pdb',
            '4CPA_l_u.rawn',
            '4CPA_r_u.pdb',
            '4CPA_r_u.rawn'
        ];

    list.forEach(function (name, i, element) {

        var option = $('<option>');

        option
            .attr({
                value: name,
                label: name
            });

        datalist
            .append(option);
    });

    datalist
        .change(function () {

            var value = $(this).val();

            $('#input-search').val(value);
        });
    */

    app = new webmol.App();
    app.init();

});

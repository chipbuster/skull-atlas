var skullviewer = (function () {

    var ids = {};
    return {

        collection: {},

        controller: {},

        model: {},

        view: {},

        parser: {},

        guid: function (type) {

            var type = type || 'skullviewer';

            if (!(type in ids)) {
                ids[type] = 0;
            }
            ids[type] += 1;

            return type + '_' + ids[type];
        }
    };

}());

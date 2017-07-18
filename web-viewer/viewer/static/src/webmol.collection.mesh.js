webmol.collection.Mesh = Backbone.Collection.extend({

    model: webmol.model.Mesh,

    init: function (options) {

        return this;
    },

    loadFile: function (fileObj) {

        if (fileObj.name.indexOf('obj') !== -1) {
            this.loadOBJ(fileObj);
        }
        return this;
    
    },

    loadOBJ: function (fileObj) {

        var name = fileObj.name;
        var context = this;
        var callback = function(event) {
            var data = webmol.parser.obj.parse(event.target.result);
            context.add(context.createMeshFromOBJ(name, data));
        };
        this.loadTextFromFile(fileObj, callback);
    },

    loadTextFromFile: function (fileObj, callback) {
        var reader = new FileReader();
        reader.onloadend = callback;
        reader.readAsText(fileObj);
    },

    createMeshFromOBJ: function (name, data) {
        var nodes = [],
            color = this.getColor(),
            root = {
                type: 'node',
                id: name.split('.')[0],
                nodes: [
                    {
                        type: 'material',
                        emit: 0.1,
                        baseColor: color,
                        specularColor: {
                            r: Math.min(color.r + 0.05, 1.0),
                            g: Math.min(color.r + 0.05, 1.0),
                            b: Math.min(color.r + 0.05, 1.0)
                        },
                        specular: 0.5,
                        shine: 4,
                        alpha: 1.0,
                        nodes: nodes
                    }
                ]
            },
            mesh = new webmol.model.Mesh();
            node = {
                type: 'node',
                id: webmol.guid('surface'),
                nodes: [
                    {
                        type: 'geometry',
                        primitive: 'triangles',
                        positions: data.vertices,
                        indices: data.faces,
                        normals: data.normals
                    }
                ]
            };

        nodes.push(node);

        mesh.init(root);

        return mesh
    },

    getColor: function () {

        //return this.colors[this.length % this.colors.length];
        return {
            r: .65,
            g: .65,
            b: .6
        }
    },

    colors: [
        {
            r: 0.6,
            g: 0.3,
            b: 0.3
        },
        {
            r: 0.2,
            g: 0.2,
            b: 0.6
        },
        {
            r: 0.8,
            g: 0.4,
            b: 0.2
        },
        {
            r: 0.2,
            g: 0.4,
            b: 0.8
        }
    ]
});

    /*
    load: function (name) {

        if (name.indexOf('.pdb') !== -1) {
            this.loadPDB(name);
        }
        else if (name.indexOf('.raw') !== -1) {
            this.loadRAW(name);
        }

        return this;
    },

    
    loadPDB: function (name) {

        var path = "data/demos/";

        $.ajax({
            url: path + name,
            context: document.body,
            success: $.proxy(function (text) {
                var data = webmol.parser.pdb.parse(text);
                this.add(this.createMeshFromPDB(name, data));
            }, this),
            error: $.proxy(this.onError, this)
        });
    },

    loadRAW: function (name) {

        var path = "data/demos/",
            that = this;

        $.ajax({
            url: path + name,
            context: document.body,
            success: $.proxy(function (text) {
                var data = webmol.parser.raw.parse(text);
                this.add(this.createMeshFromRAW(name, data));
            }, this),
            error: $.proxy(this.onError, this)
        });

    },

        
    createMeshFromPDB: function (name, atoms) {

        var nodes = [],
            color = this.getColor(),
            root = {
                type: 'node',
                id: name.split('.')[0],
                nodes: [
                    {
                        type: 'material',
                        emit: 0.1,
                        baseColor: color,
                        specularColor: {
                            r: Math.min(color.r + 0.05, 1.0),
                            g: Math.min(color.r + 0.05, 1.0),
                            b: Math.min(color.r + 0.05, 1.0)
                        },
                        specular: 0.5,
                        shine: 4,
                        alpha: 1.0,
                        nodes: nodes
                    }
                ]
            },
            mesh = new webmol.model.Mesh();

        atoms.each(function (atom) {

            var node = {
                    type: "translate",
                    x: atom.position[0],
                    y: atom.position[1],
                    z: atom.position[2],
                    nodes: [
                        {
                            type: "sphere",
                            radius: atom.radius / 175.0,
                            slices: 6,
                            rings: 12
                        }
                    ]
                };

            nodes.push(node);
        });

        mesh.init(root);

        return mesh;
    },

    createMeshFromRAW: function (name, data) {

        var nodes = [],
            color = this.getColor(),
            root = {
                type: 'node',
                id: name.split('.')[0],
                nodes: [
                    {
                        type: 'material',
                        emit: 0.1,
                        baseColor: color,
                        specularColor: {
                            r: Math.min(color.r + 0.05, 1.0),
                            g: Math.min(color.r + 0.05, 1.0),
                            b: Math.min(color.r + 0.05, 1.0)
                        },
                        specular: 0.5,
                        shine: 4,
                        alpha: 1.0,
                        nodes: nodes
                    }
                ]
            },
            mesh = new webmol.model.Mesh(),
            node = {
                type: 'node',
                id: webmol.guid('surface'),
                nodes: [
                    {
                        type: 'geometry',
                        primitive: 'triangles',
                        positions: data.vertices,
                        indices: data.faces,
                        normals: data.normals
                    }
                ]
            };

        nodes.push(node);

        mesh.init(root);

        return mesh;
    },

    onError: function (error) {

        console.error(error);

        return this;
    },

    */

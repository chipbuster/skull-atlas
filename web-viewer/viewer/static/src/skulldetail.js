$(document).ready(function () {
    var SkullGrid = function () {
        return {
            cube: null,

            obj: null,

            init: function (options) {
                var element = options.element,
                    wall = options.wall,
                    id = options.id;

                this.element = element;
                this.wall = wall;
                this.id = id;

                var height = element.innerHeight();
                var width = element.innerWidth();
                var canvasID = element.attr('id');
                var scene = new THREE.Scene(),
                    camera = new THREE.PerspectiveCamera(45.0, width / height, 0.1, 1000),
                    renderer = new THREE.WebGLRenderer({ canvas: document.getElementById(canvasID) }),
                    directionalLight = new THREE.DirectionalLight(0x808080, 1.0),
                    ambientLight = new THREE.AmbientLight(0x808080);
                directionalLight.position.set(-1000, 0, 0);
                camera.position.set(-400, 0, 0);
                camera.up = new THREE.Vector3(0, 0, 1);
                camera.lookAt(new THREE.Vector3(0, 0, 0));


                this.scene = scene;
                this.camera = camera;
                this.renderer = renderer;
                this.directionalLight = directionalLight;
                this.ambientLight = ambientLight;

                // this.controls = controls;

                renderer.setSize(width, height);
                var geometry = new THREE.BoxGeometry(20, 20, 20);
                var material = new THREE.MeshBasicMaterial({ color: 0x2c2c2c, wireframe: true });
                var cube = new THREE.Mesh(geometry, material);
                cube.name = this.id;
                this.cube = cube;

                scene.add(cube);
                this.obj = cube;
                scene.add(directionalLight);
                scene.add(ambientLight);

                scene.background = new THREE.Color(0xfcfcfc);
                renderer.render(scene, camera);
                return this;
            },

            addControls: function (callback) {
                var context = this;
                var camera = this.camera,
                    light = this.directionalLight,
                    renderer = this.renderer;
                var camControls = new THREE.OrbitControls( camera, renderer.domElement);
                var lightControls = new THREE.OrbitControls( light, renderer.domElement);
                camControls.addEventListener('change', callback); 
                camControls.enableZoom = true;
                camControls.enablePan = false;
                lightControls.enableZoom = false;
                lightControls.enablePan = false;
            },

            render: function () {
                this.renderer.render(this.scene, this.camera);
            },

            renderSkull: function (obj) {
                obj.name = this.id;
                this.scene.remove(this.scene.getObjectByName(this.id));
                this.scene.add(obj);
                this.obj = obj;
                this.renderer.render(this.scene, this.camera);
            },

            renderCube: function () {
                this.scene.remove(this.scene.getObjectByName(this.id));
                this.scene.add(this.cube);
                this.obj = this.cube;
                this.renderer.render(this.scene, this.camera);
            },

            parseSkull(data) {
                var OBJMaterial = new THREE.MeshPhongMaterial({ color: 0xf0f0f0 });
                var loader = new THREE.OBJLoader();
                var object = (loader.parse(data)).children[0];
                if (object instanceof THREE.Mesh) {
                    object.material = OBJMaterial;
                    this.renderSkull(object);
                }
            },

            requestSkull: function (url) {
                var context = this;
                $.get(url, function (data) {
                    context.parseSkull(data);
                });
            },

            updataRotate: function (grotate) {
                this.obj.rotation.z = grotate;
                this.renderer.render(this.scene, this.camera);
            },

            bindClicked: function () {
                var context = this;
                this.element.click(function () {
                    context.wall.onSceneClicked(context.id);
                });
            }
        }
    };

    var SkullDetail = function () {
        return {
            data: null,

            id: "",

            type: "",

            results: null,

            mainProp: 60,

            detailViewer: null,
            leftViewer: null,
            rightViewer: null,

            mainGrid: null,
            mainScene: null,
            sideGrids: [],
            sideScenes: [],

            sideRow: 3,

            sizeCol: 2,

            init: function () {
                var dataStr = $('#data-div').text();
                var data = JSON.parse(dataStr);
                this.id = data.id;
                this.type = data.type;
                this.results = data.results;

                this.detailViewer = $('#detail-viewer');
                this.leftViewer = $('#left-viewer');
                this.rightViewer = $('#right-viewer');

                var winHeight = $(window).height();
                var height = winHeight - $('header').outerHeight();
                this.detailViewer.height(height);

                var leftProp = this.mainProp;
                var rightProp = 100 - leftProp;
                if (this.type === 'healthy') {
                    this.leftViewer.css({ height: '100%', width: '100%' });
                    this.rightViewer.css({ display: 'none' });
                } else {
                    this.leftViewer.css({ height: '100%', width: leftProp + '%' });
                    this.rightViewer.css({ height: '100%', left: leftProp + '%', width: rightProp + '%' });
                }
                this.initMainView();
                this.initSideView();

                var context = this;
                this.mainScene.addControls(function () {
                    context.onMainChange();
                });
                this.loadModels();
                this.bindScenesClicked();
            },

            initMainView: function () {
                console.log('Main Init');
                var id = 'main';
                var node = $('<canvas id="node' + id + '"/>');
                this.leftViewer.append(node);
                node.css({ width: '100%', height: '100%' });
                var scene = SkullGrid();
                scene.init({
                    element: node,
                    wall: this,
                    id: id
                });

                this.mainGrid = node;
                this.mainScene = scene;
            },

            bindScenesClicked: function () {
                if (this.type ==='healthy') {
                    return;
                }
                for (var i = 0; i < 2; ++i) {
                    this.sideScenes[i].bindClicked();
                }
            },

            onSceneClicked (id) {
                var idInt = parseInt(id);
                var qmodel = this.id;
                var rmodel = this.results[idInt];
                window.location.replace('/deform?skull1=' + rmodel + '&skull2=' + qmodel);
                // console.log(qmodel, rmodel);
            },

            onMainChange: function () {
                this.mainScene.render();
                if (this.type ==='healthy') {
                    return;
                }
                for (var i = 0; i < this.sideScenes.length; ++i) {
                    this.sideScenes[i].camera.copy(this.mainScene.camera);
                    this.sideScenes[i].directionalLight.copy(this.mainScene.directionalLight);
                    this.sideScenes[i].render();
                }
            },

            initSideView: function () {
                console.log('Side Init');
                if (this.type === 'healthy') {
                    return;
                }

                var row = this.sideRow,
                    col = this.sizeCol,
                    height = this.rightViewer.height(),
                    width = this.rightViewer.width();
                var gridH = height / row;
                var gridW = width / col;
                for (var i = 0; i < row; ++i) {
                    for (var j = 0; j < col; ++j) {
                        var id = '' + (i * col + j);
                        var node = $('<canvas id="node' + id + '"/>');
                        this.rightViewer.append(node);
                        node.css({ top: i * gridH, left: j * gridW, height: gridH, width: gridW });
                        var scene = SkullGrid();
                        scene.init({
                            element: node,
                            wall: this,
                            id: id
                        });
                        this.sideGrids.push(node);
                        this.sideScenes.push(scene);
                    }
                }
            },

            getURL(name, dec) {
                var url = '/skulls?skull=' + name + '&dec=' + dec;
                return url;
            },

            loadModels: function () {
                this.mainScene.requestSkull(this.getURL(this.id, 0));
                if (this.type ==='healthy') {
                    return;
                }
                for (var i = 0; i < this.results.length; ++i) {
                    this.sideScenes[i].requestSkull(this.getURL(this.results[i], 1));
                }
            }
        };
    };

    var skullDetail = SkullDetail();
    skullDetail.init();
});

$(document).ready(function () {
    var SkullGrid = function () {
        return {
            cube: null,

            obj: null,
            
            init: function (options) {
                var element =options.element,
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
                var OBJMaterial = new THREE.MeshPhongMaterial({color: 0xf0f0f0});
                var loader = new THREE.OBJLoader();
                var object = (loader.parse(data)).children[0];
                if (object instanceof THREE.Mesh) {
                    object.material = OBJMaterial;
                    this.renderSkull(object);
                }
            },

            onLoad: function(){
                document.getElementById(this.id + "-load").style.display = "block";
                document.getElementById(this.id + "-canvas").style.display = "none";
            },

            loadDone: function(){
                document.getElementById(this.id + "-load").style.display = "none";
                document.getElementById(this.id + "-canvas").style.display = "block";
            },

            requestSkull: function (url) {
                var context = this;
                this.onLoad()
                $.get(url, function (data) {
                    context.parseSkull(data['obj']);
                    context.loadDone();
                    console.log(data['name'])
                    $('#'+context.id+'-name').text(data['name'])
                    $('#'+context.id+'-name')[0].style['textAlign']='center'
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

    var Skullwall = function () {
        return {
            wall: null,

            healthyTab: null,
            injuredTab: null,
            previousBtn: null,
            nextBtn: null,

            tab: "",

            row: 2,

            col: 4,

            page: 0,

            globalRotation: 0,

            grids: [],

            scenes: [],

            init: function () {
                this.wall = $("#skull-wall");
                this.healthyTab = $("#healthy-btn");
                this.injuredTab = $("#injured-btn");
                this.previousBtn = $("#previous-btn");
                this.nextBtn = $("#next-btn");

                var winHeight = $(window).height();
                var height = winHeight - $('header').height() - $('footer').height();
                this.wall.height(height);
                this.initCanvas();
                var context = this;
                this.healthyTab.click(function () {
                    context.onHealthyClicked();
                });
                this.injuredTab.click(function () {
                    context.onInjuredClicked();
                });
                this.previousBtn.click(function () {
                    context.onPreviousClicked();
                });
                this.nextBtn.click(function () {
                    context.onNextClicked();
                });

                setInterval( function(){ 
                    context.onTimeOut();
                }, 100);            
                this.onHealthyClicked();
                this.bindScenesClicked();
            },

            bindScenesClicked: function () {
                for (var i = 0; i < this.row * this.col; ++i) {
                    this.scenes[i].bindClicked();
                }
            },

            onTimeOut() {
                this.globalRotation += 0.1;
                for (var i = 0; i < this.row * this.col; ++i) {
                    this.scenes[i].updataRotate(this.globalRotation);
                }
            },
        
            initCanvas: function () {
                var row = this.row,
                    col = this.col,
                    height = this.wall.height(),
                    width = this.wall.width();
                var gridH = height / row;
                var textH = 30;
                var canvH = gridH - textH;
                var gridW = width / col;
                for (var i = 0; i < row; ++i) {
                    for (var j = 0; j < col; ++j) {
                        var id = '' + (i * col + j);
                        var node = $('<canvas id="'+ id + '-canvas"/>');
                        this.wall.append(node);
                        var node_load = this.createLoader(id);
                        this.wall.append(node_load);

                        node_load.css({ top: i * gridH, left: j * gridW, height: canvH, width: gridW });
                        node.css({ top: i * gridH, left: j * gridW, height: canvH, width: gridW });

                        var text_div = $('<div id="'+id+'-name" style="position:absolute;"></div>')
                        this.wall.append(text_div)
                        text_div.css({ top: (i+1) * gridH - textH, left: j * gridW, height: textH, width: gridW })

                        var scene  =  SkullGrid();
                        scene.init({
                            element: node,
                            wall: this,
                            id: id
                        });
                        this.grids.push(node);
                        this.scenes.push(scene);
                    }
                }
            },

            createLoader: function(id) {
                var div = $('<div id="'+id+'-load" class="load" style="background-color: white;"></div>')
                var loader = $('<div id="loader"></div>')
                for (var i = 0; i < 8; i++){
                    var dot = $('<div class="dot"></div>')
                    loader.append(dot)
                }
                loader.append('<div class="lading"></div>')
                div.append(loader)
                return div
            },

            onSceneClicked (id) {
                var idInt = parseInt(id);
                idInt = (this.page - 1) * this.row * this.col + idInt + 1;
                var maxID = this.tab==='healthy' ? 93: 3;
                if (idInt > maxID) {
                    alert('Please select a model!');
                    return;
                }
                var modelName = this.tab === 'healthy' ? 'p' : 'q';
                modelName = modelName + idInt;
                window.location.replace('/detail?skull=' + modelName);
            },

            onHealthyClicked() {
                console.log('healthy');
                if (this.tab === 'healthy') {
                    return;
                }
                this.healthyTab.addClass("active");
                this.injuredTab.removeClass("active");
                this.tab = 'healthy';
                var page = 1;
                this.loadPage(page, true);
            },

            onInjuredClicked() {
                console.log('injured');
                if (this.tab === 'injured') {
                    return;
                }
                this.injuredTab.addClass("active");
                this.healthyTab.removeClass("active");
                this.tab = 'injured';
                var page = 1;
                this.loadPage(page, true);
            },

            onPreviousClicked() {
                console.log('previous');
                var page = this.page - 1;
                this.loadPage(page, false);
            },

            onNextClicked() {
                console.log('next');
                var page = this.page + 1;
                this.loadPage(page, false);
            },

            getURL(name) {
                var url = '/skulls?skull=' + name +  '&dec=1';
                return url;
            },
            
            loadPage(newPage, changed) {
                var maxID = this.tab==='healthy' ? 93: 3;
                var prefix = this.tab === 'healthy' ? 'p': 'q';
                var num = this.row * this.col;
                
                var minPage = 1;
                var maxPage = Math.ceil(maxID / num);

                newPage = Math.max(newPage, minPage);
                newPage = Math.min(newPage, maxPage);
                if (newPage === this.page && (!changed)) {
                    return;
                }

                this.page = newPage;
                var tab = this.tab;
                var nameFunc = function (i) {
                    return (prefix + i);
                };

                var context = this;
                for (var i = 0; i < num; ++i ) {
                    var id = (newPage - 1) * num + i + 1;                    
                    this.scenes[i].renderCube();
                    if (id <= maxID) {
                        var modelName = nameFunc(id);                        
                        this.scenes[i].requestSkull(this.getURL(modelName));
                    }
                }
            }
        };
    };

    var skullWall = Skullwall();
    skullWall.init();
});

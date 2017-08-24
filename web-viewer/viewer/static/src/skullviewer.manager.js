skullviewer.Manager = Backbone.View.extend({

    views: null,
    video: null,
    currentFrame: 0,
    timeDrag: false,
    active: false,
    end: false,
    lock: false,
    skull1_id: '',
    skull2_id: '',
    query: '',
    result: '',
    maxFrame: 0,

    init: function (options) {
        console.log('SkullView Manager init');

        var dataStr = $('#data-div').text();
        var data = JSON.parse(dataStr);
        this.query = data.query;
        this.result = data.result;
        this.maxFrame = data.max_frame;

        console.log(this.query, this.result, this.maxFrame);
        this.views = options.views;
        this.loadModel();

        this.initControl();
    },

    initControl: function () {
        var video = $('#myVideo');
        //remove default control when JS loaded
        video[0].removeAttribute("controls");
        $('.control').fadeIn(500);
        $('.caption').fadeIn(500);
        var context = this;

        $('.btnPlay').on('click', function () {
            context.playpause();
        });

        $('#c-left').click(function () {
            context.onLeftClicked();
        });

        $('#c-right').click(function () {
            context.onRightClicked();
        });


        //VIDEO PROGRESS BAR
        //when video timebar clicked
        $('.progress').on('mousedown', function (e) {
            context.onProgressDown(e);
        });

        $('.progress').on('mouseup', function (e) {
            context.onProgressUp(e);
        });
        $('.progress').on('mousemove', function (e) {
            context.onProgressMove(e);
        });
    },

    onLeftClicked: function () {
        var context = this;
        if (context.active || context.lock) {
            return;
        }
        if (context.currentFrame > 1) {
            context.loadStart('result');
            var lastFrame = this.currentFrame - 1;

            var progress = $('.progress');
            var position = lastFrame * progress.width() / context.maxFrame;
            position += progress.offset().left;
            context.updatebar(position);

            context.lock = true;
            $.get(this.framesURL(lastFrame, 0), function (data) {
                context.loadObjToScene('result', data);
                context.loadDone('result');
                context.currentFrame -= 1;
                context.lock = false;

            });
        }
    },

    onRightClicked: function () {
        var context = this;
        if (context.active || context.lock) {
            return;
        }
        if (context.currentFrame < context.maxFrame) {
            context.loadStart('result');
            var nextFrame = this.currentFrame + 1;

            var progress = $('.progress');
            var position = nextFrame * progress.width() / context.maxFrame;
            position += progress.offset().left;
            context.updatebar(position);

            context.lock = true;
            $.get(this.framesURL(nextFrame, 0), function (data) {
                context.loadObjToScene('result', data);
                context.loadDone('result');
                context.currentFrame += 1;

                context.lock = false;
            })
        }
    },

    onProgressDown: function (e) {
        var context = this;
        if (context.active || context.lock) {
            return;
        }
        context.timeDrag = true;
        context.updatebar(e.pageX);
    },

    onProgressUp: function (e) {
        var context = this;
        if (context.active || context.lock) {
            return;
        }
        if (context.timeDrag) {
            context.timeDrag = false;
            context.loadStart('result');
            var per = context.updatebar(e.pageX);

            // $.get('deform/?skull1=' + context.skull1_id + '&skull2=' + context.skull2_id + '&frame=' + context.currentFrame + '&dec=0', function (data) {
            //     context.loadObjToScene('result', data['obj']);
            //     context.loadDone('result');
            // })
            console.log('UP', per, 'MAX', context.maxFrame);
            var nextFrame = Math.round(per * context.maxFrame / 100);
            var progress = $('.progress');
            var position = nextFrame * progress.width() / context.maxFrame;
            position += progress.offset().left;
            context.updatebar(position);

            console.log('Next', nextFrame);
            context.lock = true;
            $.get(this.framesURL(nextFrame, 0), function (data) {
                context.loadObjToScene('result', data);
                context.loadDone('result');
                context.currentFrame = nextFrame;


                context.lock = false;
            })
        }
    },

    onProgressMove: function (e) {
        var context = this;
        if (context.active || context.lock) {
            return;
        }
        if (context.timeDrag) {
            context.updatebar(e.pageX);
        }
    },

    loadStart: function (name) {
        document.getElementById(name + "-load").style.display = "block";
        document.getElementById(name + "-canvas").style.display = "none";
    },

    loadDone: function (name) {
        document.getElementById(name + "-load").style.display = "none";
        document.getElementById(name + "-canvas").style.display = "block";
    },

    loadFile: function (objFile) {
        var fileName = objFile.name,
            context = this,
            callback = function (event) {
                var text = event.target.result;
                context.loadObjToScene(text);
            };
        this.loadTextFromFile(objFile, callback);
    },

    loadTextFromFile: function (objFile, callback) {
        var reader = new FileReader();
        reader.onloadend = callback;
        reader.readAsText(objFile);
        console.log('starting read');
    },

    skullsURL: function (name, dec) {
        var url = '/skulls?skull=' + name + '&dec=' + dec;
        return url;
    },

    framesURL: function (frame, dec) {
        var url = '/frames?query=' + this.query + '&result=' + this.result + '&frame=' + frame + '&dec=' + dec;
        return url;
    },

    loadModel: function () {
        console.log('On load Click');
        var context = this;
        context.loadStart('query');
        context.loadStart('result');

        $.get(this.skullsURL(this.query, 0), function (data) {
            context.loadObjToScene('query', data['obj']);
            context.loadDone('query');
            $('#skull_left').html("&nbsp Left: " + data['name'])
        });
        $.get(this.skullsURL(this.result, 0), function (data) {
            context.loadObjToScene('result', data['obj']);
            context.loadDone('result');
            $('#skull_right').html("&nbsp Right: " + data['name'])
        });
    },

    deformEnd: function () {
        this.end = true;
        this.active = false;
        $('.btnPlay').removeClass('paused');
        $('.btnPlay').find('.icon-pause').removeClass('icon-pause').addClass('icon-play');
    },

    deform: function () {
        // console.log('On deform Click');

        //request next frame        
        var context = this;
        var nextFrame = this.currentFrame + 1;
        var context = this;
        var callback = function (data) {
            if (!context.active) {
                //when it calls back, the player has been paused, dont update
                return;
            }
            context.currentFrame += 1;
            console.log(context.currentFrame);
            var progress = $('.progress');
            var position = context.currentFrame * progress.width() / context.maxFrame;
            position += progress.offset().left;

            context.updatebar(position);
            context.loadObjToScene('result', data);
            if (context.currentFrame == context.maxFrame) {
                context.deformEnd();
                return;
            }
            $.get(context.framesURL(context.currentFrame + 1, 1), callback);
        };
        $.get(this.framesURL(nextFrame, 1), callback);
    },

    loadObjToScene: function (id, fileContents) {
        var OBJMaterial = new THREE.MeshPhongMaterial({ color: 0xf0f0f0 });
        var loader = new THREE.OBJLoader();
        var object = (loader.parse(fileContents)).children[0];
        if (object instanceof THREE.Mesh) {
            object.material = OBJMaterial;
            object.name = id;
            this.views.viewer.renderSkull(id, object);
        }
    },

    updatebar: function (x) {
        var progress = $('.progress');
        var position = x - progress.offset().left;
        var percentage = 100 * position / progress.width();
        if (percentage > 100) {
            percentage = 100;
        }
        if (percentage < 0) {
            percentage = 0;
        }
        $('.timeBar').css('width', percentage + '%');

        var frame = Math.round(percentage * this.maxFrame / 100);
        $('#frame_num').html('&nbsp Frame: '+frame+'/'+this.maxFrame);
        return percentage
    },

    playpause: function () {
        if (!this.active) {
            // not active
            $('.btnPlay').addClass('paused');
            $('.btnPlay').find('.icon-play').addClass('icon-pause').removeClass('icon-play');
            this.active = true;

            if (this.currentFrame === this.maxFrame) {
                this.end = false;
                var progress = $('.progress');
                this.currentFrame = 0;
                this.updatebar(progress.offset().left);
            }
            this.deform();
        }
        else {
            $('.btnPlay').removeClass('paused');
            $('.btnPlay').find('.icon-pause').removeClass('icon-pause').addClass('icon-play');
            this.active = false;
        }
    },


    onMousedown: function (event, ui) {
        //TODO
        console.log("onMousedown");
        return this;
    },

    onMouseup: function (event, ui) {
        //TODO
        console.log("onMouseup");
        return this;
    },

    onMousemove: function (event, ui) {
        //TODO
        console.log("onMousemove");
        return this;
    },

    onMousewheel: function (event, ui) {
        //TODO
        console.log("onMousewheel");
        return this;
    },

    onClick: function (event, ui) {
        //TODO
        console.log("onClick");
        return this;
    },

    events: {
        'mousedown': 'onMousedown',
        'mouseup': 'onMouseup',
        'mousemove': 'onMousemove',
        'mousewheel': 'onMousewheel',
        'click': 'onClick'
    }

});

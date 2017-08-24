skullviewer.Manager = Backbone.View.extend({

    views: null,
    video: null,
    currentFrame: 0,
    maxFrame: 50,
    timeDrag: false,
    play: false,
    pause: true,
    end: false,
    skull1_id: 'p49',
    skull2_id: 'q1',

    init: function (options) {
        console.log('SkullView Manager init');
        this.views = options.views;
        this.loadModel();
    },

    loadStart: function(name){
        document.getElementById(name+"-load").style.display = "block";
        document.getElementById(name+"-canvas").style.display = "none";
    },

    loadDone: function(name){
        document.getElementById(name+"-load").style.display = "none";
        document.getElementById(name+"-canvas").style.display = "block";
    },

    loadFile: function (objFile) {

        // Remove cube and any query skull already loaded
        // scene.remove(scene.getObjectByName("temp_cube"));
        // scene.remove(scene.getObjectByName("query_skull"));

        var fileName = objFile.name,
            context = this,
            callback = function(event) {
                var text = event.target.result;
                context.loadObjToScene(text);
            };
        this.loadTextFromFile(objFile, callback);
    },

    loadTextFromFile: function(objFile, callback) {
        var reader = new FileReader();
        reader.onloadend = callback;
        reader.readAsText(objFile);
        console.log('starting read');
    },

    loadModel: function() {
        console.log('On load Click');
        var context = this;
        context.loadStart('query');
        context.loadStart('result');
        $.get('similar/?skull1='+context.skull1_id+'&skull2='+context.skull2_id, function (data) {
            context.loadObjToScene('query', data['skull2_obj']);
            context.loadObjToScene('result', data['skull1_obj']);
            context.loadDone('query');
            context.loadDone('result');
        });

        var video = $('#myVideo');
        
        //remove default control when JS loaded
        video[0].removeAttribute("controls");
        $('.control').fadeIn(500);
        $('.caption').fadeIn(500);

        $('.btnPlay').on('click', function() { context.playpause(); } );
        
        $('#c-left').click(function() {
            if (context.currentFrame > 1){
                context.loadStart('result');
                context.currentFrame -= 1;
                $.get('deform/?skull1='+context.skull1_id+'&skull2='+context.skull2_id+'&frame='+context.currentFrame+'&dec=0', function(data){
                    context.loadObjToScene('result', data['obj']);
                    context.loadDone('result');
                })
            }
        });

        $('#c-right').click(function() {
            if (context.currentFrame < context.maxFrame){
                context.currentFrame += 1;
                console.log(context.currentFrame)
                context.loadStart('result');
                $.get('deform/?skull1='+context.skull1_id+'&skull2='+context.skull2_id+'&frame='+context.currentFrame+'&dec=0', function(data){
                    context.loadObjToScene('result', data['obj']);
                    console.log(233)
                    console.log(data)
                    console.log(data['obj']);
                    context.loadStart('result');
                })
            }
        });

        
        //VIDEO PROGRESS BAR
        //when video timebar clicked
        $('.progress').on('mousedown', function(e) {
            if (context.pause){
                context.timeDrag = true;
                context.updatebar(e.pageX);
            }
        });

        $('.progress').on('mouseup', function(e) {
            if(context.timeDrag) {
                context.timeDrag = false;
                context.loadStart('result');
                var per = context.updatebar(e.pageX);
                context.currentFrame = Math.floor(per * context.maxFrame); 
                $.get('deform/?skull1='+context.skull1_id+'&skull2='+context.skull2_id+'&frame='+context.currentFrame+'&dec=0', function(data){
                    context.loadObjToScene('result', data['obj']);
                    context.loadDone('result');
                })
            }
        });
        $('.progress').on('mousemove', function(e) {
            if(context.timeDrag) {
                context.updatebar(e.pageX);
            }
        });
    },

    deform: function() {
        console.log('On deform Click');
        var context = this;
        var callback = function (data) {
            nextFrame = parseInt(data['frame']);
            context.maxFrame = parseInt(data['max_frame'])

            context.currentFrame += 1;
            console.log(context.currentFrame);
            console.log(context.maxFrame);
            var progress = $('.progress');
            var position = context.currentFrame * progress.width() / context.maxFrame;
            position += progress.offset().left;

            context.updatebar(position);
            context.loadObjToScene('result', data['obj_decimated']);
            if (context.play == true && nextFrame <= context.maxFrame) {
                $.get('deform/?skull1='+context.skull1_id+'&skull2='+context.skull2_id+'&frame='+nextFrame+'&dec=1', callback)
            }
            else if(context.currentFrame == context.maxFrame){
                context.end = true;
                context.playpause();
            }
        };
        $.get('deform/?skull1='+context.skull1_id+'&skull2='+context.skull2_id+'&frame='+(context.currentFrame+1)+'&dec=1', callback);
    },

    loadObjToScene: function(id, fileContents) {
        var OBJMaterial = new THREE.MeshPhongMaterial({color: 0xf0f0f0});
        var loader = new THREE.OBJLoader();
        var object = (loader.parse(fileContents)).children[0];
        if (object instanceof THREE.Mesh) {
            object.material = OBJMaterial;
            object.name = id;
            this.views.viewer.renderSkull(id, object);
        }
    },

    updatebar: function(x) {
        var progress = $('.progress');
        var position = x - progress.offset().left;
        var percentage = 100 * position / progress.width();
        if(percentage > 100) {
            percentage = 100;
        }
        if(percentage < 0) {
            percentage = 0;
        }
        $('.timeBar').css('width',percentage+'%'); 
        return percentage 
    },

    playpause: function() {
        if(this.pause) {

            $('.btnPlay').addClass('paused');
            $('.btnPlay').find('.icon-play').addClass('icon-pause').removeClass('icon-play');
            this.pause = false;
            this.play = true;
            if(this.end){
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
            this.pause = true;
            this.play = false;
        }
    },


    onMousedown: function(event, ui) {
        //TODO
        console.log("onMousedown");
        return this;
    },

    onMouseup: function(event, ui) {
        //TODO
        console.log("onMouseup");
        return this;
    },

    onMousemove: function(event, ui) {
        //TODO
        console.log("onMousemove");
        return this;
    },

    onMousewheel: function(event, ui) {
        //TODO
        console.log("onMousewheel");
        return this;
    },

    onClick: function(event, ui) {
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

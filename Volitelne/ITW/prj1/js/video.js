/**
 * Custom video player.
 * 
 * @author Jiri Hynek
 */
window.addEventListener("load", () => {

    const getProgressOffset = function(progress) {
        let offsetLeft = progress.offsetLeft;
        let parent = progress.parentElement;
        let position;
        while(parent) {
            position = getComputedStyle(parent)["position"];
            if(position === "relative" || position === "absolute") {
                // we need to consider dimensions set by absolutely/relatively positioned parents
                offsetLeft += parent.offsetLeft;
            } else if(position === "fixed") {
                // fullscreen
                break;
            }
            parent = parent.parentElement;
        }
        return offsetLeft;
    }

    const getProgressTime = function(e, video, progress, progressOffset) {
        // optimization
        // it is recalculated when the video player layout is changed (normal/fullscreen)
        //if(progressOffset === undefined) {
            progressOffset = getProgressOffset(progress);
        //}
        return ((e.pageX  - progressOffset) / progress.offsetWidth) * video.duration;
    }

    const formatTime = function(time) {
        const hours = Math.floor(time/3600);
        const mins = Math.floor(time/60);
        const secs = Math.floor(time%60);
        let zeroFormat = (val) => (val < 10 ? "0" : "") + val;  
        return `${hours === 0 ? "" : zeroFormat(hours) + ":"}${zeroFormat(mins)}:${zeroFormat(secs)}`;
    }

    const updateTime = function(timeElem, video, toggleFormat) {
        let dataType = timeElem.getAttribute('data-type');
        if(toggleFormat) {
            dataType = (dataType === 'played' ? 'remained' : 'played');
            timeElem.setAttribute('data-type', dataType);
        } 
        timeElem.innerHTML = formatTime(dataType === 'played' ? video.currentTime : video.duration - video.currentTime);
        
    }

    const setUpVideoPlayer = function(videoPlayer) {
        const video = videoPlayer.getElementsByTagName('video')[0];
        
        // play / pause
        const play = videoPlayer.getElementsByClassName('playpause')[0];
        const playAction = function() {
            if(video.paused || video.ended) {
                video.play();
                videoPlayer.setAttribute('data-state', "played");
            } else {
                video.pause();
                videoPlayer.setAttribute('data-state', "paused");    
            }
        }

        play.addEventListener('click', playAction);
        videoPlayer.getElementsByClassName('video-content')[0]?.addEventListener('click', playAction);

        // progress bar
        const progress = videoPlayer.getElementsByClassName('progress')[0];
        progress.setAttribute('max', video.duration);
        let progressOffset = getProgressOffset(progress);

        // setup progress max value
        video.addEventListener('loadedmetadata', function() {
            progress.setAttribute('max', video.duration);
        });

        // switch between played and remained
        const time =  videoPlayer.getElementsByClassName('time')[0];
        time.addEventListener('click', function() {
            updateTime(time, video, true);
        })

        // update progress bar and time element as video plays
        video.addEventListener('timeupdate', function() {
            if(!progress.getAttribute('max')) {
                progress.setAttribute('max', video.duration);
            }
            progress.value = video.currentTime;
            updateTime(time, video);

            if(video.ended) {
                videoPlayer.setAttribute('data-state', 'ended');
            }
        });

        // setup progress click listener
        progress.addEventListener('click', function(e) {
            video.currentTime = getProgressTime(e, video, progress, progressOffset);
        });

        // mute / unmute
        videoPlayer.getElementsByClassName('mute')[0]?.addEventListener('click', function() {
            video.muted = !video.muted;
            videoPlayer.setAttribute('data-mute', video.muted ? "muted" : "unmuted")
        });

        // full screen
        const body = document.body;

        videoPlayer.getElementsByClassName('fullscreen')[0]?.addEventListener('click', function() {
            videoPlayer.classList.toggle("fullscreen");
            // recalculate progress offset
            progressOffset = getProgressOffset(progress);
            // disable overflow on body to fix scrollbar breaking fullscreen button offset
            body.classList.toggle("fullscreen");
        });

        // progress popup
        const popup = document.createElement("div");
        popup.classList.add("video-popup");
        popup.style.position = "fixed";
        popup.innerHTML = "N/A";
        videoPlayer.appendChild(popup);

        progress.addEventListener('mousemove', function(e) {
            popup.style.left = e.clientX + 10 + "px";
            popup.style.top = e.clientY - 25 + "px";
            popup.innerHTML = formatTime(getProgressTime(e, video, progress, progressOffset));
        });

        progress.addEventListener('mouseenter', function(e) {
            popup.style.display = "block";
        });

        progress.addEventListener('mouseout', function(e) {
            popup.style.display = "none";
        });

    }

    // setup all video players in the document
    const videos = document.getElementsByClassName('video-player');
    for(let i = 0; i < videos.length; i++) {
        setUpVideoPlayer(videos[i]);
    }
});
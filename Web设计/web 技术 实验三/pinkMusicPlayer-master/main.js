var play = document.querySelector('#play')
var musicImg = document.querySelector('.music-img')
var musicInfo = document.querySelector('.music-info')
var audio = document.querySelector('#music')
var musicBar = document.querySelector('.music-progress')
var playedBar = document.querySelector('.music-played')

var musicTimed = audio.currentTime
var allTime = audio.duration

audio.loop = false;



musicBar.onclick = function (event) {
		var musicBarWidth = musicBar.clientWidth;
		var newCurrentTime = (event.offsetX / musicBarWidth) * audio.duration;
		audio.currentTime = newCurrentTime;
		var playedBarWidth = (audio.currentTime / audio.duration) * musicBarWidth;
		playedBar.style.width = playedBarWidth + 'px';
	};

setInterval(function updatePlayedBar (){
		var musicBarWidth = musicBar.clientWidth;
		var playedBarWidth = (audio.currentTime / audio.duration) * musicBarWidth;
		playedBar.style.width = playedBarWidth + 'px';
		if(audio.currentTime === audio.duration){
      play.click()
      audio.pause()
    }
	}, 500);



audio.autoplay = false;



play.addEventListener('click', function() {
  toggleMusic()
  toggleClass(play, 'play')
  toggleClass(play, 'pause')
  toggleClass(musicImg, 'suspended')
  toggleClass(musicImg, 'show-img')
  toggleClass(musicInfo, 'info-show')
})


//播放键和暂停键
var toggleMusic = function() {
  if (audio.paused) {
    audio.play()
  }else {
    audio.pause()
  }
}

var toggleClass = function(element, className) {
  if (element.classList.contains(className)) {
      element.classList.remove(className)
  } else {
      element.classList.add(className)
  }
}


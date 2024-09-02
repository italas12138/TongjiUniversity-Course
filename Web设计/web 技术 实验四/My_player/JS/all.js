///// 完成旋转木马部分，实现图片的切换/////

const carousel = [...document.querySelectorAll('.carousel img')];

let carouselImageIndex = 0;

const changeCarousel = () => {
    carousel[carouselImageIndex].classList.toggle('active');

    if(carouselImageIndex >= carousel.length - 1){
        carouselImageIndex = 0;
    } else{
        carouselImageIndex++;
    }

    carousel[carouselImageIndex].classList.toggle('active');
}

setInterval(() => {
    changeCarousel();
}, 3000);



const music = document.querySelector('#audio-source');

const playBtn = document.querySelector('#play');
const forwardBtn = document.querySelector('#prev');
const nextBtn = document.querySelector('#next');

var music_play_cnt = 0;
var currentMusic = 0;


const music_time_bar = document.querySelector('.music-seek-bar');

const music_img_background = document.querySelector('.music-img');
const songName = document.querySelector('.music-player-section p')

const seekBar = document.querySelector('.music-seek-bar');

const currentMusicTime = document.querySelector('.current-time');
const musicDuration = document.querySelector('.duration');


//将播放时间转为 00 : 00 格式的时间
const formatTime = (time) => {
  let min = Math.floor(time / 60);
  if(min < 10){
      min = `0` + min;
  }

  let sec = Math.floor(time % 60);
  if(sec < 10){
      sec = `0` + sec;
  }

  return `${min} : ${sec}`;
}



// 设置音乐的功能

const setMusic = (i) => {
  seekBar.value = 0;
  let song = songs[i];
  currentMusic = i;

  music.src = song.music_in;
  music_img_background.style.backgroundImage = "url('" + song.image_in + "')" ;
  songName.innerHTML = song.name;

  music_play_cnt = 0;
  playBtn.click();

  setTimeout(() => {
    seekBar.max = music.duration;
    musicDuration.innerHTML = formatTime(music.duration);
  }, 300);
  currentMusicTime.innerHTML = '00 : 00';
}

setMusic(0);



setInterval(() => {
  seekBar.value = music.currentTime;
  currentMusicTime.innerHTML = formatTime(music.currentTime);
  if(Math.floor(music.currentTime) == Math.floor(seekBar.max)){
      nextBtn.click();
  }
}, 500)

seekBar.addEventListener('change', () => {
  music.currentTime = seekBar.value;
})






// 修改是否播放
playBtn.addEventListener('click', () => {

  music_play_cnt++;

  if(music_play_cnt == 1)
  {
    music.play();
    playBtn.classList.remove('play');
    playBtn.classList.add('pause');
  }

  else{
    music_play_cnt = 0;
    music.pause();
    playBtn.classList.remove('pause');
    playBtn.classList.add('play');
  }

})


//上一首音乐
forwardBtn.addEventListener('click', () => {

  if(currentMusic == 0)
  {
    currentMusic = 7;
    setMusic(7);
  }
  else 
  {
    --currentMusic;
    setMusic(currentMusic);
  }

})

//下一首音乐
nextBtn.addEventListener('click', () => {

  if(currentMusic == 7)
  {
    currentMusic = 0;
    setMusic(0);
  }
  else 
  {
    ++currentMusic;
    setMusic(currentMusic);
  }

})


const volumeSlider = document.querySelector('.current_vol');

volumeSlider.addEventListener('input', () => {
  music.volume = volumeSlider.value;
})

































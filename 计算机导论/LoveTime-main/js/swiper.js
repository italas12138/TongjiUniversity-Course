var swiper = new Swiper('.swiper-container-lovewry1', {
    pagination: '.swiper-pagination-lovewry1',
    paginationClickable: true,
    spaceBetween: 30,
});

var swiper = new Swiper('.swiper-container-lovewry2', {
    pagination: '.swiper-pagination-lovewry2',
    effect: 'coverflow',
    grabCursor: true,
    centeredSlides: true,
    slidesPerView: 'auto',
    coverflow: {
        rotate: 50,
        stretch: 0,
        depth: 100,
        modifier: 1,
        slideShadows: true
    }
});

var swiper = new Swiper('.swiper-container-lovewry3', {
    pagination: '.swiper-pagination-lovewry3',
    effect: 'flip',
    grabCursor: true,
    nextButton: '.swiper-button-next-lovewry3',
    prevButton: '.swiper-button-prev-lovewry3'
});
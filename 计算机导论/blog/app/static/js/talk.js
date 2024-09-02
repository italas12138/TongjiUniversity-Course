const time = 100
const deleteTime = 10
const interval = 1000

new TypeIt("#Headline", {
    cursorSpeed: 1000,
    speed: 100
})
    .type("huan")
    .pause(time)
    .delete(4, { instant: true })
    .type("欢")
    .pause(time)
    .type("迎")
    .pause(time)
    .type("来到")
    .pause(time * 2)
    .type("同")
    .pause(time)
    .type("济")
    .pause(time)
    .type("の")
    .pause(time)
    .type("家")
    .pause(time)
    .go();



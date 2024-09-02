function time() {
    window.setTimeout("time()", 1000);
    var seconds = 1000
    var minutes = seconds * 60
    var hours = minutes * 60
    var days = hours * 24
    var years = days * 365
    var today = new Date()
    var todayYear = today.getFullYear()
    var todayMonth = today.getMonth()  // 获取当前月份(0-11,0代表1月)
    var todayDate = today.getDate()    // 获取当前日(1-31)
    var todayHour = today.getHours()   // 获取当前小时数(0-23)
    var todayMinute = today.getMinutes() // 获取当前分钟数(0-59)
    var todaySecond = today.getSeconds() // 获取当前秒数(0-59)
    document.getElementById("time").innerHTML = todayYear + "年" + todayMonth + "月"
        + todayDate + "日" + todayHour + "时" + todayMinute + "分" + todaySecond + "秒"
}
time()


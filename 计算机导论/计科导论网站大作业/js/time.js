function lovetime() {
    window.setTimeout("lovetime()", 1000);
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

    var t1 = Date.UTC(2022, 08, 10, 22, 00, 00)
    var t2 = Date.UTC(todayYear, todayMonth, todayDate, todayHour, todayMinute, todaySecond)
    var diff = t2 - t1
    var diffYears = Math.floor(diff / years)
    var diffDays = Math.floor((diff / days) - diffYears * 365)
    var diffHours = Math.floor((diff - (diffYears * 365 + diffDays) * days) / hours)
    var diffMinutes = Math.floor((diff - (diffYears * 365 + diffDays) * days - diffHours * hours) / minutes)
    var diffSeconds = Math.floor((diff - (diffYears * 365 + diffDays) * days - diffHours * hours - diffMinutes *
        minutes) / seconds)
    document.getElementById("lovetime").innerHTML = "我们已经在一起 " + diffYears + "年" + diffDays + "天" +
        diffHours + "小时" + diffMinutes + "分钟" + diffSeconds + "秒啦"
}
lovetime()

// 计时模板

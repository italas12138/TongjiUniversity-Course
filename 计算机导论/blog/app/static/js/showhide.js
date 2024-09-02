function showdiv() {
    document.getElementById("show").style.display = "block";

    document.getElementById('strHref').innerHTML = "收起";
    document.getElementById('strHref').href = "javascript:hidediv();";
}
function hidediv() {
    document.getElementById('show').style.display = 'none';
    document.getElementById('strHref').innerHTML = "查看更多";
    document.getElementById('strHref').href = "javascript:showdiv();";
}
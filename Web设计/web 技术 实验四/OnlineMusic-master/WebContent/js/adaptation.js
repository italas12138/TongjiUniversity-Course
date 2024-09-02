/**
 * @authors Your Name (you@example.org)
 * @date    2019-05-11 20:42:38
 * @version $Id$
 */
changeFixW();
window.addEventListener('resize',function(){
  changeFixW();
},false);
function changeFixW(){
  var meta = document.createElement("meta");
  var w = window.screen.width;
  //var h = window.screen.height;
  var scale_w = w/375;
  var metaAttr = {
    'name':'viewport',
    'content':'initial-scale='+scale_w+',maximum-scale='+scale_w+',minimum-scale='+scale_w+',user-scalable=no,width=375'
  };
  for(var key in metaAttr){
    meta[key] = metaAttr[key];
  }
  document.getElementsByTagName("head")[0].appendChild(meta);
}

var loginfrom=document.querySelector(".loginfrom") ;
var regfrom=document.querySelector(".regfrom") 
function TurnToLogin(){
    login();
}
function login(){
    loginfrom.style.display = "block";
    regfrom.style.display = "none";
}
function TurnToRegister(){
    register();
}
function register(){
    loginfrom.style.display = "none";
    regfrom.style.display = "block";
}
window.onscroll = function() {myFunction()};

var navbar = document.getElementById("navbar");
var sticky = navbar.offsetTop;

function myFunction() {
  if (window.pageYOffset >= sticky) {
    navbar.classList.add("sticky")
  } else {
    navbar.classList.remove("sticky");
  }
}

function myMove() {
    var elem = document.getElementById("myAnimation");   
    var pos = 300;
    var id = setInterval(frame, 10);
    function frame() {
      if (pos == 1050) {
        clearInterval(id);
      } else {
        pos++; 
       
        elem.style.left = pos + 'px'; 
      }
    }
  }
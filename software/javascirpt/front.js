var pos = 370;
var stop = false;

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
    // var id = setInterval(frame, 1);
    var pos = 300;
    var id = setInterval(frame, 10);
    function frame() {
      // console.log(stop)
      if (pos == 1050 || stop == true) {
        clearInterval(id);
        stop = false
      } else {
        pos++; 
       
        elem.style.left = pos + 'px'; 
      }
    }
  }


  function myBack() {
    var elem = document.getElementById("myAnimation");   
    var id = setInterval(frame, 1);
    function frame() {
      // console.log(stop)
      if (pos == 370 || stop == true) {
        clearInterval(id);
        stop = false
      } else {
        pos--; 
       
        elem.style.left = pos + 'px'; 
      }
    }
  }



function stopRobot() {
  stop = true;
}




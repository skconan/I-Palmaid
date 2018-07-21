var tempArrayDate = [];
var tempArrayTime = [];
var tempArrayRoom = [];
var pos = 370;
var stop = false;

function init() {
  // localStorage.date = ["20/7/2018", "21/7/2018"];
  // localStorage.time = ["10:00", "9:00"];
  // localStorage.room = ["A1", "A1"];

  let dateTemp = localStorage.getItem("date");
  let timeTemp = localStorage.getItem("time");
  let roomTemp = localStorage.getItem("room");

  let date = dateTemp.split(",");
  let time = timeTemp.split(",");
  let room = roomTemp.split(",");
  

  console.log(date.length)

  for (let i = 0; i < date.length; i++) {
    $("#history").append(
      `<tr>
      <td>${date[i]}</td>
      <td>${time[i]}</td>
      <td>${room[i]}</td>
      </tr>`
    );
    console.log(date[i])
  }

}

init();

window.onscroll = function () { myFunction() };

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
  var id = setInterval(frame, 1);
  function frame() {
    console.log(stop)
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
    console.log(stop)
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

function writeFile(room) {
  const now = new Date();
  let day = now.getDate();
  let month = now.getMonth() + 1;
  let year = now.getUTCFullYear();
  let setDate = day + "/" + month + "/" + year;

  let hour = now.getHours();
  let min = now.getMinutes();

  let setTime = hour + ":" + min;

  tempArrayDate.push(setDate)
  tempArrayTime.push(setTime)
  tempArrayRoom.push(room)

  $("#history").append(
    `<tr>
    <td>${setDate}</td>
    <td>${setTime}</td>
    <td>${room}</td>
    </tr>`)

  localStorage.date = [tempArrayDate];
  localStorage.time = [tempArrayTime];
  localStorage.room = [tempArrayRoom];
}

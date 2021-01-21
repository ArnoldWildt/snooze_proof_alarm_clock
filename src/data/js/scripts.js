function open_nav() {
  document.getElementsByClassName("container")[0].classList.add("change");
  document.getElementById("mySidenav").style.width = "250px";
}

/* Set the width of the side navigation to 0 and the left margin of the page content to 0, and the background color of body to white */
function close_nav() {
  document.getElementById("mySidenav").style.width = "0";
  document.getElementsByClassName("container")[0].classList.remove("change");
}

function change_page() {}

// Time , Is Activ, Icons
function add_alarm() {
  document.getElementsByClassName("float")[0].classList.toggle("disable");
  //Change to Alarm setup page
}

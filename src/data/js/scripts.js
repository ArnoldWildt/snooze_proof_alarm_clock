function openNav() {
  document.getElementsByClassName("container")[0].classList.add("change");
  document.getElementById("mySidenav").style.width = "250px";
}

/* Set the width of the side navigation to 0 and the left margin of the page content to 0, and the background color of body to white */
function closeNav() {
  document.getElementById("mySidenav").style.width = "0";
  document.getElementsByClassName("container")[0].classList.remove("change");
}

// Time , Is Activ, Icons
function add_alarm() {}

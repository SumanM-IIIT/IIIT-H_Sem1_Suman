function myDropdown() {
	//document.getElementById("myDropdown").classList.toggle("view");
	$("#myDropdown").attr("view").toggle();
}

window.onclick = function(event) {
  if (!event.target.matches(".dropdown_button")) {
    var dd = $(".dropdown-content");
    var x;
	var len = dd.length;
    for (x = 0; x < len; x++) {
      var openDropdown = dd[x];
      if (openDropdown.classList.contains("view")) {
        openDropdown.classList.remove("view");
      }
    }
  }
} 

$("#bg_image_button").click(function() {
	$("body").css("background-image", "url('images/kkr.jpg')");
});

$("#three_alert").on("input", function() {
	if($(this).val().match(/.*3.*/g)) {
		alert("3 is pressed !!");
	}
});

function blink() {
	$('.blinking').fadeOut(400);
    $('.blinking').fadeIn(400);
}
setInterval(blink, 400);



var f_timeout;
var do_resize = false;

$(document).ready(function() {
  $('#tab_header ul li.item').on('click', function() {
    var number = $(this).data('option');
    $('#tab_header ul li.item').removeClass('is-active');
    $(this).addClass('is-active');
    $('#tab_container .container_item').removeClass('is-active');
    $('div[data-item="' + number + '"]').addClass('is-active');

    // fix overlay in motion menu
    if (number == 5) {
      $('#motion_picture').load("controller/action.php?jpegimage=1", function() {
        setTimeout(function() {
          motion_selection_init();
        }, 500);
        (function worker() {
          if (do_resize) {
            motion_selection_init();
            do_resize = false;
          }
          var ts = new Date().getTime();
          $('#motion_picture').attr("src", "controller/action.php?jpegimage=" + ts);
          f_timeout = setTimeout(worker, 1000);
        })();
      });

    } else {
      clearTimeout(f_timeout);
      motion_selection_remove();
    }

  });

  $(window).resize(function() {
    motion_selection_remove();
    do_resize = true;
  });

});

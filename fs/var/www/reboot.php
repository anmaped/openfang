<?php
require 'session.php';
?>
<head>
<link href="css/bulma-pageloader.min.css" rel="stylesheet">
<link href="css/bulma-flat.min.css" rel="stylesheet">
<script type="text/javascript" src="scripts/jquery-3.3.1.min.js"></script>
</head>
<html>
<div class="pageloader is-active"><span class="title">Rebooting ...</span></div>
<script>
$.ajax({
  url: "cgi-bin/action.cgi?cmd=reboot",
  type: "GET",
  success: function(result) {
    console.log("reboot.");

    function ping() {
      $.ajax({
        url: "/",
        type: "GET",
        timeout: 3000,
        success: function(result) {
          console.log("online.");
          document.location = "/"

        },
        error: function(result) {
          console.log("offline.");
          setTimeout(ping, 10000);
        }
      });
    }
	ping();

  }
});
</script>
</html>

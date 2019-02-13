<?php
session_start();
$user = $_POST['user'];
$password = $_POST['password'];
if ($user == "admin" && $password == "admin" )
{
$_SESSION['user'] = $user;
$_SESSION['password'] = $password;
}
if ( ( !isset($_SESSION['user']) and !isset($_SESSION['password']) ) || isset($_GET["logout"]) )
{
session_destroy();
unset ($_SESSION['user']);
unset ($_SESSION['password']);
header('location:login.php');
}
?>
<!DOCTYPE html>
<html class="has-navbar-fixed-top">
  <head>
    <meta charset="utf-8">
    <title>Openfang 01
    </title>
    <meta content="width=device-width, initial-scale=1" name="viewport">
    <!-- CSS Framework -->
    <!-- <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/bulma/0.7.1/css/bulma.min.css"> -->
    <link href="css/bulma-flat.min.css" rel="stylesheet">
    <link href="css/bulma-switch.1.0.1.min.css" rel="stylesheet">
    <link href="css/bulma-badge.1.0.1.min.css" rel="stylesheet">
    <link href="css/bulma-quickview.1.0.1.min.css" rel="stylesheet">
    <!-- jQuery  -->
    <script src="scripts/jquery-3.3.1.min.js" type="text/javascript">
    </script>
    <!-- For Config Motion -->
    <script src="scripts/jquery.imgareaselect.pack.js" type="text/javascript">
    </script>
    <link href="css/imgareaselect-animated.css" rel="stylesheet">
    <!-- Icons -->
    <link href="https://use.fontawesome.com/releases/v5.5.0/css/all.css" rel="stylesheet">
    <link href="css/notifications.css" rel="stylesheet">
    <style id="custom_css">
      body {
        display: flex;
        min-height: 100vh;
        flex-direction: column;
      }
      #main {
        flex: 1 0 auto;
      }
      /*
      script_card <= scripts.cgi
      status_card <= status.cgi
      */
      .script_card,
      .status_card {
        max-width: 100%;
        margin-bottom: 1em;
      }
      @media screen and (min-width: 1024px) {
        .script_card,
        .status_card {
          /* reduce width for large screens */
          max-width: 75%
        }
      }
      /* table.motor_control <= status.cgi */
      table.motor_control td {
        width: 120px;
      }
      table.motor_control td button {
        width: 100%;
      }
      #liveview {
        padding: 2px;
        border: dotted 1px #4a4a4a;
        margin: 0 auto;
        display: block;
        max-height: 80vh;
      }
      #dpad_indicator {
        position: absolute;
        top: 3px;
        left: 0;
        right: 0;
        margin: 0 auto;
        z-index: 4;
        width: 1.5em;
        border-radius: 0.25em;
        background-color: rgba(10, 10, 10, 0.4);
        text-align: center;
        font-weight: bold;
        color: #fff;
        cursor: pointer;
        font-size: 0.8em;
        color: #ffdd57;
      }
      #dpad_container {
        position: absolute;
        top: 0;
        left: 0;
        right: 0;
        margin: 1em auto;
        display: none;
        z-index: 5;
        width: 9em;
      }
      #dpad {
        position: relative;
        width: 9em;
        height: 9em;
      }
      .dpad_button {
        width: 3em;
        height: 3em;
        position: absolute;
        display: inline-block;
        text-align: center;
      }
      #dpad .up {
        top: 0;
        left: 3em;
      }
      #dpad .right {
        top: 3em;
        right: 0;
      }
      #dpad .left {
        top: 3em;
        left: 0;
      }
      #dpad .down {
        top: 6em;
        left: 3em;
      }
      .dpad_button a {
        font-size: 2em;
        color: #ff3860;
        text-shadow: 2px 2px 4px #000000;
      }
      .dpad_button a:hover {
        color: #ffdd57;
      }
      #content:hover #dpad_container {
        display: block;
      }
      #content:hover #dpad_indicator {
        display: none;
      }
    </style>
  </head>
  <body>
    <nav aria-label="main navigation" class="navbar is-fixed-top" role="navigation">
      <div class="container">
        <div class="navbar-brand">
          <a class="navbar-item" href=".">
            <span class="icon">
              <i class="fas fa-video">
              </i>
            </span> 
            <span>Openfang 01
            </span>
          </a> 
          <a aria-expanded="false" aria-label="menu" class="navbar-burger" data-target="nav_menu" id="navbar_burger" role="button">
            <span aria-hidden="true">
            </span> 
            <span aria-hidden="true">
            </span> 
            <span aria-hidden="true">
            </span>
          </a>
        </div>
        <div class="navbar-menu" id="nav_menu">
          <!-- right menu -->
          <div class="navbar-end">
            <div class="navbar-item has-dropdown is-hoverable">
              <span class="navbar-link">
                <span class="icon">
                  <i aria-hidden="true" class="fa fa-cog">
                  </i>
                </span> 
                <span>Manage
                </span>
              </span>
              <div class="navbar-dropdown is-boxed">
                <a class="navbar-item onpage" data-target="settings.php" href="javascript:%20void(0)" id="status">Settings
                </a> 
                <a class="navbar-item onpage" data-target="cgi-bin/scripts.cgi" href="javascript:%20void(0)" id="scripts">Services
                </a> 
                <a class="navbar-item onpage" data-target="logs.html" href="javascript:%20void(0)" id="logs">Logs
                </a> 
                <a class="navbar-item onpage" data-target="update.php" href="javascript:%20void(0)" id="update">FW Update
                </a> 
                <a class="navbar-item prompt" data-message="Are you sure you wish to reboot?" data-target="cgi-bin/action.cgi?cmd=reboot" href="javascript:%20void(0)">Reboot
                </a>
              </div>
            </div>
            <div class="navbar-item has-dropdown" id="camcontrol_link">
              <!-- Camera Movement Controls -->
              <span class="navbar-link">
                <i class="fas fa-user-check">
                </i>
              </span>
              <div class="navbar-dropdown is-boxed is-right">
                <!-- Led: IR -->
                <span class="navbar-item">
                  <input class="switch is-small" data-checked="cgi-bin/action.cgi?cmd=ir_led_on" data-unchecked="cgi-bin/action.cgi?cmd=ir_led_off" id="ir_led" name="ir_led" type="checkbox" <?php if (exec ('. /opt/scripts/common_functions.sh; echo $(ir_led status);') == "ON" ) echo "checked=\"checked\""; ?> > 
                  <label for="ir_led">IR Led
                  </label>
                </span> 
                <!-- IR Cut -->
                <span class="navbar-item">
                  <input class="switch is-small" data-checked="cgi-bin/action.cgi?cmd=ir_cut_on" data-unchecked="cgi-bin/action.cgi?cmd=ir_cut_off" id="ir_cut" name="ir_cut" type="checkbox" <?php if (exec ('. /opt/scripts/common_functions.sh; echo $(ir_cut status);') == "ON" ) echo "checked=\"checked\""; ?> > 
                  <label for="ir_cut">IR-Cut
                  </label>
                </span> 
                <!-- Auto Night Detect -->
                <span class="navbar-item">
                  <input class="switch is-small" data-checked="cgi-bin/scripts.cgi?cmd=start&script=auto-night-detection" data-unchecked="cgi-bin/scripts.cgi?cmd=stop&script=auto-night-detection" id="auto_night_detection" name="auto_night_detection" type="checkbox" <?php if (exec ('. /opt/scripts/common_functions.sh; echo $(auto_night_mode status);') == "ON" ) echo "checked=\"checked\""; ?> > 
                  <label for="auto_night_detection">Auto Night Detection
                  </label>
                </span> 
                <!-- Motion Detection -->
                <span class="navbar-item">
                  <input class="switch is-small" data-checked="cgi-bin/action.cgi?cmd=motion_detection_on" data-unchecked="cgi-bin/action.cgi?cmd=motion_detection_off" id="motion_detection" name="motion_detection" type="checkbox" <?php if (exec ('. /opt/scripts/common_functions.sh; echo $(motion_detection status);') == "ON" ) echo "checked=\"checked\""; ?> > 
                  <label for="motion_detection">Motion Detection
                  </label>
                </span> 
                <!-- RTSP H264 -->
                <span class="navbar-item">
                  <input class="switch is-small" data-checked="cgi-bin/scripts.cgi?cmd=start&script=rtsp-h264" data-unchecked="cgi-bin/scripts.cgi?cmd=stop&script=rtsp-h264" id="rtsp_h264" name="rtsp_h264" type="checkbox" <?php if (exec ('. /opt/scripts/common_functions.sh; echo $(rtsp_h264_server status);') == "ON" ) echo "checked=\"checked\""; ?> > 
                  <label for="rtsp_h264">RTSP H264 Server
                  </label>
                </span> 
                <!-- RTSP MJPEG -->
                <span class="navbar-item">
                  <input class="switch is-small" data-checked="cgi-bin/scripts.cgi?cmd=start&script=rtsp-mjpeg" data-unchecked="cgi-bin/scripts.cgi?cmd=stop&script=rtsp-mjpeg" id="rtsp_mjpeg" name="rtsp_mjpeg" type="checkbox" <?php if (exec ('. /opt/scripts/common_functions.sh; echo $(rtsp_mjpeg status);') == "ON" ) echo "checked=\"checked\""; ?> > 
                  <label for="rtsp_mjpeg">RTSP MJPEG Server
                  </label>
                </span> 
                <!-- MQTT Status -->
                <span class="navbar-item">
                  <input class="switch is-small" data-checked="cgi-bin/scripts.cgi?cmd=start&script=mqtt-status" data-unchecked="cgi-bin/scripts.cgi?cmd=stop&script=mqtt-status" id="mqtt_status" name="mqtt_status" type="checkbox"> 
                  <label for="mqtt_status">MQTT Status Server
                  </label>
                </span> 
                <!-- MQTT Control -->
                <span class="navbar-item">
                  <input class="switch is-small" data-checked="cgi-bin/scripts.cgi?cmd=start&script=mqtt-control" data-unchecked="cgi-bin/scripts.cgi?cmd=stop&script=mqtt-control" id="mqtt_control" name="mqtt_control" type="checkbox"> 
                  <label for="mqtt_control">MQTT Control Server
                  </label>
                </span> 
                <!-- Startup Sound -->
                <span class="navbar-item">
                  <input class="switch is-small" data-checked="cgi-bin/scripts.cgi?cmd=start&script=sound-on-startup" data-unchecked="cgi-bin/scripts.cgi?cmd=stop&script=sound-on-startup" id="sound_on_startup" name="sound_on_startup" type="checkbox"> 
                  <label for="sound_on_startup">Sound on Startup
                  </label>
                </span>
              </div>
            </div>
            <div class="navbar-item has-dropdown" id="camcontrol_link">
              <a class="navbar-item onpage" href="?logout" id="status">Logout
              </a>
            </div>
          </div>
        </div>
      </div>
    </nav>
    <section class="section" id="main">
      <?php $model = exec('fw_nvram get model');
      if ($model == "") {
      echo "<p class=\"notification is-warning\" role=\"alert\" style=\"float: right; clear: right;\" id=\"newnotification\">You are using this device for the first time. Please configure it <a class=\"onpage\" data-target=\"settings.php\" href=\"javascript:hidenotification('#newnotification')\">here</a>.
        <button class=\"delete\" type=\"button\">Close
        </button>"; }
       ?>
      </p>
      <div class='box'>
        <div class="container" id="content">
          <img id="liveview" onerror="this.src='css/unable_load.png';" src="controller/action.php?jpegimage=1">
          <div id="dpad_container">
            <div id="dpad">
              <div class="dpad_button up">
                <!-- up -->
                <a class="cam_button" data-cmd="motor_up" href="javascript:void(0)">&#x25B2;
                </a>
              </div>
              <div class="dpad_button down">
                <!-- down -->
                <a class="cam_button" data-cmd="motor_down" href="javascript:void(0)">&#x25BC;
                </a>
              </div>
              <div class="dpad_button left">
                <!-- left -->
                <a class="cam_button" data-cmd="motor_left" href="javascript:void(0)">&#x25C0;
                </a>
              </div>
              <div class="dpad_button right">
                <!-- right -->
                <a class="cam_button" data-cmd="motor_right" href="javascript:void(0)">&#x25B6;
                </a>
              </div>
            </div>
          </div>
          <div id="dpad_indicator">
            &hArr;
          </div>
        </div>
      </div>
    </section>
    <footer class="footer">
      <div class="container">
        <div class="has-text-centered is-size-7">
          <p>
            <a href="https://github.com/anmaped/openfang" target="_blank">
              <strong>openfang
              </strong>
            </a> 
            <?php echo shell_exec('fw_nvram get fw_version'); ?>
          </p>
        </div>
      </div>
    </footer>
    <div class="quickview" id="quickviewDefault">
      <header class="quickview-header">
        <p class="title">Quickview
        </p>
        <span class="delete" data-dismiss="quickview" id="quickViewClose">
        </span>
      </header>
      <div class="quickview-body">
        <div class="quickview-block" id="quicViewContent">
        </div>
      </div>
      <footer class="quickview-footer">
      </footer>
    </div>
    <script src="scripts/index.html.js" type="text/javascript">
    </script> 
    <script src="scripts/notifications.js">
    </script> 
    <script>
      var notifications = new Notifications();
      notifications.init();

      function hidenotification(id) {
          $(id).addClass('is-hidden');
      }

      (function worker() {
          var ts = new Date().getTime();
          //console.log("S");

          $('#liveview').attr("src", "controller/action.php?jpegimage=" + ts);

          setTimeout(worker, 1000);
      })();


      /*(function worker() {
        $.ajax({
          url: 'controller/action.php',
          type: 'get',
          data: "jpegimage=1",
          success: function(data) {
            console.log( data );
            //var ts = new Date().getTime();
            $('#liveview').attr("src", data );
          },
          complete: function() {
            // Schedule the next request when the current one's complete
            setTimeout(worker, 1000);
          }
        });
      })();*/

      $(".switch").click(function() {
          var sw = $(this);
          sw.prop('disabled', true);
          if ( !sw.prop("checked") )
              $.get(sw.data('unchecked')).done(function(data) {
                  sw.prop('disabled', false);
              });
          else
              $.get(sw.data('checked')).done(function(data) {
                  sw.prop('disabled', false);
              });

      });

    </script>
  </body>
</html>


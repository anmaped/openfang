<?php

require '../session.php';


if( isset($_POST["hardware_model"]) ) {

  // set settings based on the selected model
  $v = exec('setmodel ' . $_POST["hardware_model"]);

  if ($v == "ok")
    echo "success: " . $_POST["hardware_model"];
  else
    echo "nosuccess: " . $_POST["hardware_model"];

}
else if ( isset($_POST["wireless_mode"]) && isset($_POST["wssid"]) && isset($_POST["wpassword"]) )
{
  $i=0;
  # check all before commit
  if ( exec('nvram get wapi wmode') != $_POST["wireless_mode"] )
  {
    $i++;
    exec('nvram set wapi wmode "' . $_POST["wireless_mode"] . '"' );
  }
  
  if ( exec('nvram get wapi wssid') != $_POST["wssid"] )
  {
    $i++;
    exec('nvram set wapi wssid "' . $_POST["wssid"] . '"' );
  }

  if ( exec('nvram get wapi wpassword') != $_POST["wpassword"] )
  {
    $i++;
    exec('nvram set wapi wpassword "' . $_POST["wpassword"] . '"');
    # wkeymgmt
    exec('nvram set wapi wkeymgmt KEY');
  }

  if ( $i != 0 )
    exec('nvram commit');

  echo "success: " . $_POST["wireless_mode"] . " writes " . $i;
}
else if ( isset($_GET["jpegimage"]) )
{
  header('Content-Type: image/jpeg');
  passthru('getimage');

  //echo "data:image/jpg;base64," . base64_encode ( shell_exec('getimage') );

}
else if ( isset($_POST["password"]) )
{
  exec('nvram set wapi adminpasswd ' . $_POST["password"]);
  exec('nvram commit');
}
else if ( isset($_GET["cmd"]) )
{
   //cmd=set_video_settings
  if ( $_GET["cmd"] == "set_video_settings" )
  {
    echo "command 'set_video_settings' with parameters " . $_POST["video_size"] . " / " . $_POST["video_format"] . " / " . $_POST["brbitrate"];
    exec('nvram set 2860 video.resolution "' . $_POST["video_size"] . '"');
    exec('nvram set 2860 video.format ' . $_POST["video_format"]);
    exec('nvram set 2860 video.bitrate ' . $_POST["brbitrate"]);
    exec('nvram commit');
  }
  else if ( $_GET["cmd"] == "set_rtsp_settings" )
  {
    echo "command 'set_rtsp_settings' with parameters " . $_POST["rtsp_port"] . " / " . $_POST["stream_format"] . " / " . $_POST["audio_enabled"];
    exec('nvram set 2860 rtsp.port "' . $_POST["rtsp_port"] . '"');
    exec('nvram set 2860 rtsp.stream ' . $_POST["stream_format"]);
    exec('nvram set 2860 rtsp.en_audio "' . $_POST["audio_enabled"] . '"');
    exec('nvram commit');
  }
  else
  {
    echo "unrecognized command.";
  }

}
else
{
  echo "unknown";
}

?>

<?php

require '../session.php';


if( isset($_POST["hardware_model"]) ) {

  exec('fw_nvram set model ' . $_POST["hardware_model"] );
  echo "success: " . $_POST["hardware_model"];

}
else if ( isset($_POST["wireless_mode"]) && isset($_POST["wssid"]) && isset($_POST["wpassword"]) )
{
  $i=0;
  # check all before commit
  if ( exec('fw_nvram get wmode') != $_POST["wireless_mode"] )
  {
    $i++;
    exec('fw_nvram set wmode "' . $_POST["wireless_mode"] . '"' );
  }
  
  if ( exec('fw_nvram get wssid') != $_POST["wssid"] )
  {
    $i++;
    exec('fw_nvram set wssid "' . $_POST["wssid"] . '"' );
  }

  if ( exec('fw_nvram get wpassword') != $_POST["wpassword"] )
  {
    $i++;
    exec('fw_nvram set wpassword "' . $_POST["wpassword"] . '"');
    # wkeymgmt
    exec('fw_nvram set wkeymgmt KEY');
  }

  echo "success: " . $_POST["wireless_mode"] . " writes " . $i;
}
else if ( isset($_GET["jpegimage"]) )
{
  header('Content-Type: image/jpeg');
  passthru('getimage');

  //echo "data:image/jpg;base64," . base64_encode ( shell_exec('getimage') );

}
else
{
  echo "unknown";
}

?>

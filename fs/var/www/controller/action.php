<?php

require '../session.php';


if( isset($_POST["hardware_model"]) ) {

  exec('nvram set rtdev model ' . $_POST["hardware_model"] );
  exec('nvram commit');
  echo "success: " . $_POST["hardware_model"];

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
else
{
  echo "unknown";
}

?>

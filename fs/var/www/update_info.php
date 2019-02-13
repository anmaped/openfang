<?php
require 'session.php';
?>

<pre><?php echo shell_exec('cat /var/log/update.log | tail -n 10');  ?></pre>

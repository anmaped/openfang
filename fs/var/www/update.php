<?php
require 'session.php';
?>

<script type="text/javascript">
    $(document).ready(function() {
        update(true);
    setInterval(function(){
    $("#result").load('update_info.php')
    }, 1000);
    });
    
    function timedRefresh(timeoutPeriod) {
        timeoutPeriod -= 1;
        $('#message').text("Rebooting ... wait..." + timeoutPeriod);
    
        if (timeoutPeriod == 0) {
            window.location.href = window.location.href;
        } else {
            setTimeout(function() {
                timedRefresh(timeoutPeriod)
            }, 1000);
        }
    };
    
    
    function update(onStart) {
        $.ajax({
            'url': 'cgi-bin/action.cgi?cmd=show_updateProgress'
        }).done(function(log) {
            if (log < 0) {
                if (onStart != true)
                {
                    $('#message').text("Error starting update progress");
                    $('#message').text("Error starting update process");
                }
                $('#start').removeAttr('disabled');
            } else {
                $('#start').attr("disabled", "disabled");
                $('#message').text("Update in progress");
                $('#progress').val(log);
                // This is the end, start the reboot count down
                if (log >= 100) {
                    timedRefresh(45);
                } else {
                    setTimeout(update, 500);
                }
            }
        });
    
    }
    
    function start() {
        $.ajax({
            'url': 'cgi-bin/action.cgi?cmd=update',
            'type': 'POST'
        }).done(function(result) {
            if (result.length > 0) {
                 $('#start').attr("disabled", "disabled");
                update(false);
            } else {
                $('#message').text("Error starting update progress");
                $('#start').removeAttr('disabled');
            }
        });
    }
    
    
</script>
<div class='card status_card'>
    <header class='card-header'>
        <p class='card-header-title'>Openfang 01 update ( current version <?php echo shell_exec('sh /opt/autoupdate.sh version'); ?>)</p>
    </header>
    <div class='card-content'>
        <div class="field is-horizontal">
            <div class="field-label is-normal">
            </div>
            <div class="field-body">
                <div class="field">
                    <div class="control">
                        <input id="start" class="button is-primary" type="submit" value="Start" onclick="start()" />
                    </div>
                </div>
            </div>
            <div class="field-body">
                <div class="field">
                    <progress id=progress name=progress class="progress is-danger" value=0 max="100"></progress>
                    <h1 id=message class="title is-4 has-text-centered"></h1>
                </div>
            </div>
        </div>
        <div class="field-body">
            <div class="field">
                <div id="result"></div>
            </div>
        </div>
    </div>
</div>


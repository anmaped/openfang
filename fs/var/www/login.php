<!DOCTYPE html>

<html>
<head>
   <meta charset="utf-8">
   <meta content="IE=edge" http-equiv="X-UA-Compatible">
   <meta content="width=device-width, initial-scale=1" name="viewport">

   <title>openfang login</title>
   <link href="css/bulma-flat.min.css" rel="stylesheet"><!-- jQuery  -->

   <script src="scripts/jquery-3.3.1.min.js" type="text/javascript">
   </script>
</head>

<body>
   <section class="has-background-light hero is-success is-fullheight">
      <div class="hero-body">
         <div class="container has-text-centered">
            <div class="column is-4 is-offset-4">
               <h3 class="title has-text-grey">
               </h3>


               <p class="subtitle has-text-grey">openfang
               <?php echo shell_exec('fw_nvram get fw_version'); ?></p>


               <div class="box">
                  <form action="index.php" method="post">
                     <div class="field">
                        <div class="control">
                           <input autofocus="" class="input is-large" id="user"
                           name="user" placeholder="Username" type="text">
                        </div>
                     </div>


                     <div class="field">
                        <div class="control">
                           <input class="input is-large" id="password" name=
                           "password" placeholder="Password" type="password">
                        </div>
                     </div>


                     <div class="field">
                        <label class="checkbox"><input type="checkbox">
                        Remember me</label>
                     </div>
                     <button class=
                     "button is-block is-info is-large is-fullwidth" type=
                     "submit">Login</button>
                  </form>
               </div>
            </div>
         </div>
      </div>
   </section>
   <script src="scripts/index.html.js" type="text/javascript">
   </script> 
   <script>
   $(function() {


   setTheme(getThemeChoice());

   });
   </script>
</body>
</html>

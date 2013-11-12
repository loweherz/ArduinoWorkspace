<?php
		
	$mypassword=$_GET['password'];
	$myusername=$_GET['username'];
	$id_sensore = $_GET['sensore'];
	$valore = $_GET['dato'];
	
	$host="localhost"; // Host name
    $username="root"; // Mysql username
    $password=""; // Mysql password
    $db_name="Sql531406_5"; // Database name
	// Connect to server and select databse.
	mysql_connect("$host", "$username", "$password")or die("cannot connect");
	mysql_select_db("$db_name")or die("cannot select DB");	
	
		if(isset($_GET['sensore']))//se la variabile è stata impostata
		{
			$id_sensore = $_GET['sensore'];
			if(isset($_GET['dato']) && is_numeric($_GET['dato']))
			{
				$valore = $_GET['dato'];
				$sql="SELECT * FROM sensore WHERE id_sensore = '$id_sensore'";//controllo che esista il sensore
				$result = mysql_query($sql);
				$count=mysql_num_rows($result);
				
				if($count==1)
				{			
					$sql1="INSERT INTO lettura SET id_sensore = '$id_sensore', lettura='$valore'";//inserisco il dato
					if(mysql_query($sql1))
					{
						echo("ok");
					}
					else
						{
							echo ('errore db');
						}
				}
				else
					echo("codice sensore errato");
			}
			else
				echo("formato valore non corretto");
		}
		else
			echo("codice sensore mancante");
		
	mysql_close();//chiudo la connessione
	ob_end_flush();
	
?>
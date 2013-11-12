<?php
	ob_start();
	
	$host="localhost"; // Host name
    $username="root"; // Mysql username
    $password=""; // Mysql password
    $db_name="Sql531406_5"; // Database name
	// Connect to server and select databse.
	mysql_connect("$host", "$username", "$password")or die("cannot connect");
	mysql_select_db("$db_name")or die("cannot select DB");
	
	if(isset($_GET['sensore']))
	{
		$id_sensore = $_GET['sensore'];

		$sql="SELECT nome,grandezza,valoreMax,valoreMin FROM sensore WHERE id_sensore='$id_sensore'";
		$result = mysql_query($sql);
		$count=mysql_num_rows($result);
		$array=mysql_fetch_array($result);

		$nome = $array['nome'];
		$grandezza = $array['grandezza'];
		$ValMax = $array['valoreMax'];
		$ValMin = $array['valoreMin'];
		
		if($count==1)
		{?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
 <html xmlns="http://www.w3.org/1999/xhtml">
		<head>
			<meta http-equiv="content-type" content="text/html; charset=utf-8"/>
			<title>Dati</title>			
		</head>
		 <body>
					<?php
						$tabella = "";//conterra il codice per la tabella
						$sql="SELECT TRUNCATE(lettura,2) as lettura, YEAR(data) as anno, MONTH(data) as mese, DAY(data) as giorno, HOUR(data) as ora, MINUTE(data) as minuti, data FROM lettura WHERE id_sensore = '$id_sensore' ORDER BY data DESC";
						$result = mysql_query($sql);
						while($array=mysql_fetch_array($result)){
							$tabella = $tabella."<td>".$array['data']."<td>".$array['lettura']."</tr>\n";
						} ?>					
			      <p>
	              <?php
			echo($nome." (".$grandezza.")");?>
	            <br>
                	<table border="1">
	        	<tr><td>data<td >lettura</tr>
				<?php
                		echo $tabella;
				echo("</table>");
			}
			else
				echo("codice sensore errato");
		}
		else
			echo("codice sensore mancante");
	mysql_close();
	ob_end_flush();
?>
  </body>
</html>
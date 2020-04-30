<html>
	<head>
		<title>
			Raspberry Pi Webserver
		</title>
	</head>
	<h1>
		<font color="red">
			Raspberry Pi Webserver
		</font> 
	</h1>
	<body>
		<?php 
			error_reporting(E_ALL); 
			ini_set("display_errors", 1); 

			if ($_FILES["file"]["error"] > 0) 
			{ 
				echo "Error: " . $_FILES["file"]["error"] . "<br />"; 
			} 
			else 
			{ 
				echo "Upload: " . $_FILES["file"]["name"] . "<br />"; 
				echo "Type: " . $_FILES["file"]["type"] . "<br />"; 
				echo "Size: " . ($_FILES["file"]["size"] / 1024) . " Kb<br />"; 
				echo "Stored in: " . $_FILES["file"]["tmp_name"]; 
				move_uploaded_file($_FILES["file"]["tmp_name"], "/var/www/Uploads/". $_FILES["file"]["name"]); 
				echo "<p>Upload successful: " . $_FILES["file"]["name"] . "</p>"; 
			}
		?> 
	</body>
</html>
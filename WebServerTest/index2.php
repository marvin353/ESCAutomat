<html>
<style>
	body {
		background-color: rgb(103, 154, 187);
	}
	h1   {
		color: white;
		font-family: Arial, Helvetica, sans-serif;
	}
	p    {
		color: red;
	    font-family: Arial, Helvetica, sans-serif;
	}
	img   {
		margin-left: 150px;
		height: 100px;
		width: 100px;
	}
	.div0    {
	
    }
	.div1    {
		 margin-left: 50px;
	}
	.div2    {
	    margin-bottom: 200px;
		margin-left: 50px;
		padding: 16px 32px;
		position: absolute;
	}
	.button {
		  width: 200px;
		  background-color: #4CAF50; /* Green */
		  border: none;
		  color: white;
		  padding: 16px 32px;
		  text-align: center;
		  text-decoration: none;
		  display: inline-block;
		  font-size: 16px;
		  margin: 4px 2px;
		  -webkit-transition-duration: 0.4s; /* Safari */
		  transition-duration: 0.4s;
		  cursor: pointer;
     }
	 .button1 {
		  background-color: white; 
		  color: black; 
		  border: 2px solid #4CAF50;
     }

	.button1:hover {
		  background-color: #4CAF50;
		  color: white;
	}
	.button3 {
		  background-color: white; 
		  color: black; 
		  border: 2px solid #f44336;
		}

	.button3:hover {
		  background-color: #f44336;
		  color: white;
	}

</style>

<head>
    <title>Chip-Automat Fridge Simulator</title>
</head>
<body>
	<div class="div0" id="div0">
		<h1>Chip-Automat Fridge Simulator</h1>
		<form method="post" action="index2.php">
			<input class="button button1" id="p1" onclick="functionKarteDrauf()" type="submit" name="call_this2" value="Karte auflegen">
			<input class="button button3" id="p2" onclick="functionKarteWeg()" type="submit" name="call_this3" value="Karte wegnehmen">
		</form>
	</div>
	<form method="post" action="index2.php">
		<input type="submit" name="call_this4" value="SaveTimestamp">
	</form>
	<form method="post" action="index2.php">
		<input type="submit" name="call_this5" value="IchWillChip">
	</form>
	<script>
		function functionKarteWeg() {
			document.getElementById('cardLabel').innerHTML = 'Keine Karte aufgelegt';
			document.getElementById('cardImage').src = "nocard.png";
		}
		function functionKarteDrauf() {
			document.getElementById('cardLabel').innerHTML = 'Karte aufgelegt';
			document.getElementById('cardImage').src = "card.png";
		}
		// Add event listeners.
		
		
    </script>
	<?php
		$servername = "localhost";
		$username = "root";
		$password = "root";
		$dbname = "chipautomat";
		$cardID = 1234;
		$chipAutomatIP = "192.168.19.100";
		
		
		if($_POST["call_this4"] == "SaveTimestamp") {
			
			saveTimestamp($servername, $username, $password, $dbname, $cardID);
		}
		
		if($_POST["call_this3"] == "Karte wegnehmen") {
		try {
		    $conn = new PDO("mysql:host=$servername;dbname=$dbname", $username, $password);
		    // set the PDO error mode to exception
		    $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
		    $sql = "INSERT INTO karte (karteda)
				VALUES (false)";
		    // use exec() because no results are returned
		    $conn->exec($sql);
		    
			echo "<br><img src='nocard.png' alt='error'>";
			echo "<br>           Karte wurde weggenommen";
		    }
		catch(PDOException $e)
		    {
		    echo $sql . "<br>" . $e->getMessage();
		    }
		
			$conn = null;
		}
		
		if($_POST["call_this2"] == "Karte auflegen") {
		try {
		    $conn = new PDO("mysql:host=$servername;dbname=$dbname", $username, $password);
		    // set the PDO error mode to exception
		    $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
		    $sql = "INSERT INTO karte (karteda)
				VALUES (true)";
		    // use exec() because no results are returned
		    $conn->exec($sql);
			echo "<br><img src='card.png' alt='error'>";
			echo "<br>           Karte wurde aufgelegt";
		    }
		catch(PDOException $e)
		    {
		    echo $sql . "<br>" . $e->getMessage();
		    }
		
			$conn = null;
		}
					
		if($_POST["call_this5"] == "IchWillChip") {
			
			// Create connection
			$conn = new mysqli($servername, $username, $password, $dbname);
			// Check connection
			if ($conn->connect_error) {
			    die("Connection failed: " . $conn->connect_error);
			} 
			
			$sql = "SELECT karteda FROM karte ORDER BY ID DESC LIMIT 1";
			$result = $conn->query($sql);
			
			if ($result->num_rows > 0) {
			    while($row = $result->fetch_assoc()) {
					
					if ($row["karteda"] === "1") {
						echo "Karte ist 1";
						//saveTimestamp($cardID);
						postGetChip($chipAutomatIP,$servername, $username, $password, $dbname, $cardID);
					} else {
						echo "Karte ist 0";
						postGetChipError($chipAutomatIP);
					}
			    }
			} else {
			    echo "error: 0 results";
			}
			$conn->close();
		}
		
		function postGetChip($chipAutomatIPL,$servernameL, $usernameL, $passwordL, $dbnameL, $cardIDL) {
			$url = "http://".$chipAutomatIPL."/chipArgs";
			$data = array('IchWillChip' => 'getChip');
			
			// use key 'http' even if you send the request to https://...
			$options = array(
			    'http' => array(
			        'header'  => "Content-type: application/x-www-form-urlencoded\r\n",
			        'method'  => 'POST',
			        'content' => http_build_query($data)
			    )
			);
			$context  = stream_context_create($options);
			$result = file_get_contents($url, false, $context);
			if ($result === FALSE) { 
				echo "error postgetchip";
				postGetChipError();
			} else {
				saveTimestamp($servernameL, $usernameL, $passwordL, $dbnameL, $cardIDL);
			}
			
			//var_dump($result);
		}
		
		function postGetChipError($chipAutomatIPL) {
			$url = "http://".$chipAutomatIPL."/chipArgs";
			$data = array('IchWillChip' => 'error');
			
			// use key 'http' even if you send the request to https://...
			$options = array(
			    'http' => array(
			        'header'  => "Content-type: application/x-www-form-urlencoded\r\n",
			        'method'  => 'POST',
			        'content' => http_build_query($data)
			    )
			);
			$context  = stream_context_create($options);
			$result = file_get_contents($url, false, $context);
			if ($result === FALSE) { 
				echo "error postgetchiperror";
			} else {
			}
			
			//var_dump($result);
		}
			
		function saveTimestamp($servernameL, $usernameL, $passwordL, $dbnameL, $cardIDL) {
			try {
		    $conn = new PDO("mysql:host=$servernameL;dbname=$dbnameL", $usernameL, $passwordL);
		    // set the PDO error mode to exception
		    $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
		    $sql = "INSERT INTO timestamps (cardID)
				VALUES ($cardIDL)";
		    // use exec() because no results are returned
		    $conn->exec($sql);
		    echo "New record created successfully";
		    }
		catch(PDOException $e)
		    {
		    echo $sql . "<br>" . $e->getMessage();
		    }
		
			$conn = null;
		
		}
			
	?>

</body>
</html>

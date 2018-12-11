
<!DOCTYPE html>

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
	<!-- Load jQuery -->
	<script src="https://code.jquery.com/jquery-3.1.1.min.js"></script>
</head>
<body>
	
	<div class="div0" id="div0">
	<h1>Chip-Automat Simulator</h1>

	<button class="button button1" id="p1"> Karte auflegen </button>
	<button class="button button3" id="p2"> Karte wegnehmen </button>
	<button class="button buttonajax" id="ajax"> ajax </button>
	<form method="post" action="index.php">
		<input type="submit" name="call_this">
	</form>
	<p id="p4"> Hover over me and this text will be shown in a popup. </p>
	</div>
		
	<div class="div1" id="div1">
		<img id="cardImage" src="nocard.png" style="width:100px">
	</div>
	
	<div class="div02" id="div2">
		<label id="cardLabel">test</label>
	</div>
	
	<script>
	  	document.getElementById('cardLabel').innerHTML = 'Keine Karte aufgelegt';
	</script>
		
	<script>
		// Add event listeners.
		$(document).ready(function(){
			$("p").css("font-weight","Bold");
			
			$("#p1").click(function() {
				//$(this).hide();
				document.getElementById('cardLabel').innerHTML = 'Karte aufgelegt';
				document.getElementById('cardImage').src = "card.png";
			});
			
			$("#p2").click(function() {
				//$(this).slideUp();
				document.getElementById('cardLabel').innerHTML = 'Keine Karte aufgelegt';
				document.getElementById('cardImage').src = "nocard.png";
			});			
      });
		
	</script>
	
</body>
</html>
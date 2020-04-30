$(document).ready(function()
{
	var socket = io.connect(location.protocol + "//" + document.domain + ":" + location.port + "/Raspberry");
	
	document.getElementById("ButtonCloseApplication").onclick = CloseApplication;

	var Inputs = document.querySelectorAll("input"), i;

	for (i = 0; i < Inputs.length; ++i) 
	{
		if(Inputs[i].value == "On")
		{
			Inputs[i].onclick = OnClick
		}
		else if(Inputs[i].value == "Off")
		{
			Inputs[i].onclick = OffClick
		}
	}
	
	function CloseApplication()
	{
		alert("Close application...");
		SendJSON("applicationStatus", "Off");
		Shutdown();
	}
	
	function OnClick()
	{
		SendJSON("gpio_on", this.name);
	}
	
	function OffClick()
	{
		SendJSON("gpio_off", this.name);
	}
	
	function Shutdown()
	{
		var xmlHttp = new XMLHttpRequest();
		xmlHttp.open("GET", "/shutdown", false);
		xmlHttp.send(null);
		return xmlHttp.responseText;
	}
});
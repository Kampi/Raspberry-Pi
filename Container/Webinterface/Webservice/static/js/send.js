function SendJSON(Element, Value)
{
	var xmlhttp = new XMLHttpRequest();
	xmlhttp.open("POST", "/transmitData");
	xmlhttp.setRequestHeader("Content-Type", "application/json");
	xmlhttp.send(JSON.stringify({ "Element" : Element, "Value" : Value }));
}
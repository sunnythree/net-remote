var xmlrequest;
var handler=function(evt){
	xmlrequest.onreadystatechange = processResponse;
	xmlrequest.open("POST","Test.java?value="+evt, true);
	xmlrequest.setRequestHeader("Content-Type"
		, "application/x-www-form-urlencoded");
	xmlrequest.send(evt+"\r\n");
}
function createXMLHttpRequest(){
	if(window.XMLHttpRequest){
		xmlrequest = new XMLHttpRequest();
	}
	else if (window.ActiveXObject){
		try{
			xmlrequest = new ActiveXObject("Msxml2.XMLHTTP");
		}
		catch (e){
			try{
				xmlrequest = new ActiveXObject("Microsoft.XMLHTTP");
			}
			catch (e){
				
			}
		}
	}
	else{
		console.log("cann't create XMLHttpRequest");
	}
}
function processResponse(){
	if(xmlrequest.readyState == 4){
		if(xmlrequest.status==200){
			console.log(xmlrequest.responseText);
		}
	}
}
window.onload=function(){
	createXMLHttpRequest();
}

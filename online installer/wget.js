//wget.js - similar to wget but written in vbscript
//based on a script by Chrissy LeMaire
//forked by Bit

var shell = new ActiveXObject("WScript.shell");



//Arguments
var url = WScript.Arguments(0);
if (WScript.Arguments.length > 1) {
  var saveTo = WScript.Arguments(1);
} else {
  parts = url.split("/");
  var saveTo = parts.slice(0, parts.length - 1);
}

//Fetch the file
var xhr = new ActiveXObject("MSXML2.ServerXMLHTTP");

xhr.open("GET", url, false);
xhr.send();

if (xhr.status === 200) {
  var stream = new ActiveXObject("ADODB.Stream");
  stream.Open();
  stream.Type = 1; //adTypeBinary
  
  stream.Write(xhr.responseBody);
  stream.Position = 0; //Set the stream position to the start
  
  var explorer = new ActiveXObject("Scripting.FileSystemObject");
  if (explorer.Fileexists(saveTo)) { explorer.DeleteFile(saveTo); }
  
  stream.SaveToFile(saveTo);
  stream.Close();
}

//Done
WScript.Quit();
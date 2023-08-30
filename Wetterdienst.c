!//Event orientiertes Heizen/Schalten per Google Kalender Version 1.12 / 27.08.2023 Lukas Helduser (Youtube: https://www.youtube.com/LukasvandeHaag)
!//Teil 4 Skript zum auslesen der Temperature vom Wetterdienst Accuweather
!//Benutzer Eingaben:
!//#######---Beginn Variabler Bereich---###########################################################################################################
string key="xxxxxxxxxxxxxxxx";  !Eingabe des API key
string ort="xxxxxx"; !Eingabe der Ortskennung
string varTemp = "Z_Aussentemp";

!//#######---Ende Variabler Bereich---##########################################################################################################




!Variablen
string error="kein";
string url;
string endurl;
integer pos=0;
string  stemp;
string data;


!Zusammenbau der URL
url="http://dataservice.accuweather.com/forecasts/v1/hourly/1hour/"+ort+"?apikey="+key+"&details=true&metric=true";
endurl = "wget --no-check-certificate --timeout=3 -O - '"#url#"'";

!Abfrage per CuxDaemon
dom.GetObject("CUxD.CUX2801001:3.CMD_SETS").State(endurl);
dom.GetObject("CUxD.CUX2801001:3.CMD_QUERY_RET").State(1);
data = dom.GetObject("CUxD.CUX2801001:3.CMD_RETS").State();

data=data.ToLatin();


pos=data.Find("\"Temperature\"");
data=data.Substr(pos,(data.Length()-pos-1));
pos=data.Find("Value");
if (pos==-1){
    itemp="0.0";
  }else{
    stemp=data.Substr(pos+7,5);
    if (stemp.Find(",")>-1){
      stemp=stemp.Substr(0,4);
    }

}

dom.GetObject(varTemp).State(stemp.ToInteger());
WriteLine(stemp);

!//Event orientiertes Heizen/Schalten per Google Kalender Version 1.12 / 27.08.2023 Lukas Helduser (Youtube: https://www.youtube.com/LukasvandeHaag)
!//Teil 1 Skript zum auslesen der Termine aus einem Google Kalender
!//Benutzer Eingaben:
!//#######---Beginn Variabler Bereich---###########################################################################################################
string APIkey ="xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
string Kalender ="xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx@group.calendar.google.com";
string NameRaumliste="ZZ_OrteVarListe";
string Heizflag="ZZ_Heizflag";
string Schaltflag="ZZ_Schaltflag";
string NameSchaltliste="ZZ_Schaltliste";

!//#######---Ende Variabler Bereich---#############################################################################################################
!//Stript Variablen. Von Benutzer nicht zu verändern !!!!
string url1 ="https://www.googleapis.com/calendar/v3/calendars/";
string urlwget ="wget --timeout=3 -O - '";
string url;
string TimemaxT2="%3A00%3A00-00%3A00";
string Data;
string DataTemp;
string error="non";
integer EventPosID=0;
string SRaumliste;
string NSchaltliste;
string AktRaum;


!//AUfbau der gesamt URL
url=urlwget+url1+Kalender+"/events?orderBy=startTime&singleEvents=true&timeMax="+system.Date("%Y-%m-%d")+"T"+((system.Date("%H").ToInteger()+2)-system.Date("%z").ToInteger()/100).ToString()+"%3A"+system.Date("%M")+"%3A00%2D00%3A00&timeMin="+system.Date("%Y-%m-%d")+"T"+((system.Date("%H").ToInteger()+1)-(system.Date("%z").ToInteger()/100)).ToString()+"%3A"+system.Date("%M")+"%3A00%2D00%3A00&key="+APIkey+"'";

!//URL mit CUx_Daemon abfragen
dom.GetObject("CUxD.CUX2801001:2.CMD_SETS").State(url);
dom.GetObject("CUxD.CUX2801001:2.CMD_QUERY_RET").State(1);
Data = dom.GetObject("CUxD.CUX2801001:2.CMD_RETS").State().ToUpper();

!//Raumliste auslesen
SRaumliste=dom.GetObject(NameRaumliste).Value().ToUpper();
!//Prüfen ob im Return von google überhaupt ein Termin enthalten ist.
if(Data.Find("CALENDAR#EVENT")>-1){
   string start;
   string stop;
   string EventID;

  !//DIe Raumliste druchgehen und Prüfen ob für den jeweiligen Raum ein Termin vorliegt.
  !//Liegt ein Termin vor dann die Daten des Termin aufbereiten und auf die NEUE Schaltliste setzen.
  foreach(AktRaum,SRaumliste.Split(";")){
    !SRaumliste.StrValueByIndex(";",0).ToUpper();
    DataTemp=Data;
    if(AktRaum!=""){
      !//Heizen
      EventPosID=DataTemp.Find(AktRaum+dom.GetObject(Heizflag).Value());
      if(EventPosID>-1){
       DataTemp=DataTemp.Substr(EventPosID,DataTemp.Length()-EventPosID);
       start=DataTemp.Substr(DataTemp.Find("START\":")+27,19);
       start=start.Replace("T"," ");
       stop=DataTemp.Substr(DataTemp.Find("END\":")+25,19);
       stop=stop.Replace("T"," ");
       NSchaltliste=NSchaltliste+AktRaum+";&;"+(start.ToTime().ToInteger().ToString())+";"+(stop.ToTime().ToInteger().ToString())+";";
       }
      !//Schalten
      EventPosID=DataTemp.Find(AktRaum+dom.GetObject(Schaltflag).Value());
      if(EventPosID>-1){
       DataTemp=DataTemp.Substr(EventPosID,DataTemp.Length()-EventPosID);
       start=DataTemp.Substr(DataTemp.Find("START\":")+27,19);
       start=start.Replace("T"," ");
       stop=DataTemp.Substr(DataTemp.Find("END\":")+25,19);
       stop=stop.Replace("T"," ");
       NSchaltliste=NSchaltliste+AktRaum+";§;"+(start.ToTime().ToInteger().ToString())+";"+(stop.ToTime().ToInteger().ToString())+";";
      }
    }
  }
}

!//Variablen für den zusammebau und Sortierung der neuen Daten
string Aliste=dom.GetObject(NameSchaltliste).State();
string Templiste="";
boolean flag=1;
integer whileID=0;

!//Die ALTE Schaltliste durch gehen und prüfen ob veraltetet Termine enthalten sind. Noch nötige Termine zwischen Speichern.
!//Ein Termin ist veraltet wenn die Ausschaltzeit in der Vergangenheit liegt.
!//Termine die in der Zukunft liegen und von Google nicht mehr geliefert werden aus der alten Liste übernehmen.
while (flag){
   if(Aliste.StrValueByIndex(";",whileID)!=""){
     if(Aliste.StrValueByIndex(";",whileID+4)<system.Date().ToTime().ToInteger()){
       if(NSchaltliste.Find(Aliste.StrValueByIndex(";",whileID)+";"+Aliste.StrValueByIndex(";",whileID+1)+";"+Aliste.StrValueByIndex(";",whileID+2))==-1){
          Templiste=Templiste+Aliste.StrValueByIndex(";",whileID)+";"+Aliste.StrValueByIndex(";",whileID+1)+
            ";"+Aliste.StrValueByIndex(";",whileID+2)+";"+Aliste.StrValueByIndex(";",whileID+3)+";";
       }
      }
     whileID=whileID+4;
    }else{
      flag=0;
   }
}

!//Inhalt der neuen Schaltliste und der Templiste zusammen in die Schaltliste schreiben
dom.GetObject(NameSchaltliste).State(Templiste+NSchaltliste);

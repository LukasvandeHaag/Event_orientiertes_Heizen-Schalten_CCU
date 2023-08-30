!//Event orientiertes Heizen per Google Kalender Version 1.10 / 24.07.2023 Lukas Helduser (Youtube: https://www.youtube.com/LukasvandeHaag)
!//Teil 3 Hilfsskript zum Daten setzen auf der CCU3 von EQ-3
!//Benutzer Eingaben:
!//#######---Beginn Variabler Bereich---###########################################################################################################
!//beachten sie bitte die Hinweise aus dem Handbuch im Kaptiel 3.1

!//Eingabe des Namen der Variable
!//Schreiben sie den Namen der Variable zwischen die Anfführungszeichen!
string Variablename = "ZZ_Grundtemp" ;

!//Eingabe des Wert
!//Schreiben sie den Wert der gesetzt werden soll in die Anfführungszeichen!
string Eingabewert = "17.0" ;



!//#######---Ende Variabler Bereich---##########################################################################################################
!//Skript Variablen
!//Hier keine Veränderungen vornehme
boolean err=0;

if (Variablename!=""){
      if(Eingabewert!=""){
        if(dom.GetObject(Variablename).Channel()>0){
            dom.GetObject(Variablename).State(Eingabewert);
          }else{
            err=1;
          }
      }else{
        err=1;
      }
}else{
    err=1;
}

if (err){
    WriteLine("Fehler in der Eingabe.\nUeberpruefen sie ihre Eingabe!");
  }else{
    WriteLine("Eingabe wurde gesetzt.");
  }

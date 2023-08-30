!//Event orientiertes Heizen/schalten per Google Kalender Version 1.12 / 27.08.2023 Lukas Helduser (Youtube: https://www.youtube.com/LukasvandeHaag)
!//Teil 2 Skript zum Schalten der Aktoren.
!//Benutzer Eingaben:
!//#######---Beginn Variabler Bereich---###########################################################################################################

!//Name der Systemvariable für die Orteliste
string NameOrteliste="ZZ_OrteVarListe";
!//Name der Systemvariable für die Schaltliste
string NameSchaltliste="ZZ_Schaltliste";
!//Name der Systemvariable für Liste der Variablenliste Schalten
string NameAktS="ZZ_AktorenVarListeSchalten";
!//Name der Systemvariable für Liste der Variablenliste Heizen
string NameAktH="ZZ_AktorenVarListeHeizen";
!//Name der Systemvariable für die Grundtemperatur
string Grundtemp="ZZ_Grundtemp";
!// Name der Systemvariable für die Aussentemperatur
string Aussentemp="Z_Aussentemp";
!// Name der Systemvariable für die Raumtemperatur
string Raumtemp="ZZ_Raumtemp";
!// Name der Systemvariable für das TemperaturFlag zum Heizen
string THF="ZZ_TemperaturFlagHeizen";
!// Gibt an ob es sich im Homematic Klassik oder Homematic IP Aktoren handelt
!//Geben sie eine 0 an für Klassik Aktoren, geben sie eine 1 an für IP Aktoren
boolean aktorenflag = 0;

!//#######---Eingabe der Offsetwerte---###---Alle Eingaben in Miunten---############################################################################
!// Aussentemperatur kleiner -8 °C
integer Offset1=30;
!// Aussentemperatur zwischen -8 bis -2 °C
integer Offset2=25;
!// Aussentemperatur zwischen -1 bis +5 °C
integer Offset3=20;
!// Aussentemperatur zwischen 6 und 12 °C
integer Offset4=15;
!// Aussentemperatur zwischen 13 und 18 °C
integer Offset5=10;
!// Ausschaltzeitpunkt Offset (Gibt die Minuten an die die Heizung grundsätzlich früher ausschalten soll)
integer OffsetAus=25;
!// Einschaltzeitpunkt Offset (Gibt die Minuten an die die Heizung grundsätzlich früher einschalten soll)
integer OffsetEin=30;
!// Tempeperaturgrenze zum Heizen die am Vortag um 20Uhr unterschritten sein muss
integer TG=17.5;
!//#######---Ende Variabler Bereich---##########################################################################################################


!//Skript Variablen
!//Hier keine Veränderungen vornehmen
string SListe=dom.GetObject(NameSchaltliste).State();
string VarNamenH=dom.GetObject(NameAktH).State();
string VarNamenS=dom.GetObject(NameAktS).State();
string RM=dom.GetObject(NameOrteliste).State();
string AktSR;
boolean flagA=1;
boolean flagB=1;
integer SID_A=0;
integer SID_B=0;
integer AT=dom.GetObject(Aussentemp).State();
integer EIN;
integer AUS;
string  RVN;
string  RVI;
string  AktAktor;
boolean HSFlag=0;
boolean HF=0;
integer T=0;


!//Tagestemperatur setzen und Auswerten
T=(system.Date("%H")+system.Date("%M")).ToInteger();
string flag=dom.GetObject(THF).State();
HF=flag.StrValueByIndex(";",0).ToInteger();
if((T>2000)&&(T<2010)){
  if(AT>TG){
    dom.GetObject(THF).State(flag.StrValueByIndex(";",0)+";0");
    }else{
    dom.GetObject(THF).State(flag.StrValueByIndex(";",0)+";1");
    }
  }
if((T>0000)&&(T<0010)){
  dom.GetObject(THF).State(flag.StrValueByIndex(";",1)+";"+flag.StrValueByIndex(";",0));
  }




!//Beginn aussere Schleife
!//Element von der Schaltliste nehmen und die dazu gehörige Raumvariablen suchen
!//Wird die Raumvariable nicht gefunden wird das Skript an der Stelle abgebrochen da ein Fehler in der Konfiguration vorliegt
while(flagA){
  if(SListe.StrValueByIndex(";",SID_A)!=""){
     !//Listenelement auslesen und EIN und AUS Zeit den Offsetwert abziehen
     AktSR=SListe.StrValueByIndex(";",SID_A).ToUpper();
     if(SListe.StrValueByIndex(";",SID_A+1)=="&"){HSFlag=1;}else{HSFlag=0;};
     if(HSFlag){
      EIN=SListe.StrValueByIndex(";",SID_A+2).ToInteger()-(OffsetEin*60);
      AUS=SListe.StrValueByIndex(";",SID_A+3).ToInteger()-(OffsetAus*60);
      }else{
      EIN=SListe.StrValueByIndex(";",SID_A+2).ToInteger();
      AUS=SListe.StrValueByIndex(";",SID_A+3).ToInteger();
      }


     !//Beginn innere Schleife
     !//Zum Listenelement passende Raumvariable suchen
     !// Liste der Raumvariablen durchgehen bis Raumvariable gleich der Variable AktSR ist und Variable RVN (Raumvariablennamen) und RVI (Raumvariable Inhalt) fühlen,
     !//Wenn Variable gefunden innerer Schleife für diesen Durchgang beeenden
      while (flagB){
          if(RM.StrValueByIndex(";",SID_B)!=""){
            if(AktSR==RM.StrValueByIndex(";",SID_B)){
              if(HSFlag==0){RVN=VarNamenS.StrValueByIndex(";",SID_B);}
              if(HSFlag==1){RVN=VarNamenH.StrValueByIndex(";",SID_B);}
              RVI=dom.GetObject(RVN).State();
              flagB=0;
              }
          SID_B=SID_B+1;
          }else{
            !//Wird keine Raumvariable gefunden innerer und äussere Schleife abbrechen
            flagB=0;
            flagA=0;
            WriteLine("ABBRUCH Raumvariable nicht gefunden!");
          }
      }
    !//innere Schleife für nächsten Durchgang in Grundstellung bringen
     SID_B=0;
     flagB=1;

     !//Nach aktueller Aussentemperatur den Offsetwert berechnen und am Einschaltzeitpunkt abziehen
     if(AT<-8){EIN=EIN-(Offset1*60);} ! kleiner -8
     if((AT>-7)&&(AT<-1)){EIN=EIN-(Offset2*60);} !zwischen -8 und -2
     if((AT>-2)&&(AT<6)){EIN=EIN-(Offset3*60);}  !zwischen -1 und +5
     if((AT>5)&&(AT<13)){EIN=EIN-(Offset4*60);}  !zwischen +6 und + 12
     if((AT>12)&&(AT<19)){EIN=EIN-(Offset5*60);}  !zwischen +13 und + 18


    !//Ausschalten
     !//Liegt der Ausschaltzeitpunkt des aktuellen Schaltlistenelement in der Vergangenheit dann Raumvariable durchgehen und Aktoren auf Grundtemp bringen WENN Heizung
     !//noch nicht ausgeschaltet ist
     if((AUS<system.Date("%s").ToInteger())&&(RVI.StrValueByIndex(";",0).ToInteger())==1){
        foreach(AktAktor,RVI.Split(";")){
          if (AktAktor.Length()>4){
            if(HSFlag){
              if(aktorenflag){
                dom.GetObject(AktAktor).DPByHssDP("SET_TEMPERATURE").State(dom.GetObject(Grundtemp).State());
              }else{
                dom.GetObject(AktAktor).DPByHssDP("SET_POINT_TEMPERATURE").State(dom.GetObject(Grundtemp).State());
              }
            }else{
              dom.GetObject(AktAktor).DPByHssDP("STATE").State(0);
            }
          }
        }
       dom.GetObject(RVN).State("0;"+RVI.Substr(2,RVI.Length()-2));
      }


    !//Einschalten
     !//Liegt der Einschaltzeitpunkt in der Vergangenheit UND Ausschaltzeitpunkt in der Zukunft Heizung einschalten WENN diese noch nicht eingeschaltet ist.
     if((EIN<=system.Date("%s").ToInteger())&&(AUS>system.Date("%s").ToInteger())&&(RVI.StrValueByIndex(";",0).ToInteger())==0){
        foreach(AktAktor,RVI.Split(";")){
          if (AktAktor.Length()>4){
            if(HSFlag){
              if(aktorenflag){
                dom.GetObject(AktAktor).DPByHssDP("SET_TEMPERATURE").State(dom.GetObject(Raumtemp).State());
              }else{
                dom.GetObject(AktAktor).DPByHssDP("SET_POINT_TEMPERATURE").State(dom.GetObject(Raumtemp).State());
              }
            }else{
              dom.GetObject(AktAktor).DPByHssDP("STATE").State(1);
            }
          }
        }
        dom.GetObject(RVN).State("1;"+RVI.Substr(2,RVI.Length()-2));
      }


     !//Schleifen ID setzen
     SID_A=SID_A+4;
  }else{
    !//Wenn Schaltliste komplett durchgegangen ist äussere Schleife beeenden
    flagA=0;
    !WriteLine(AT);
  }

}

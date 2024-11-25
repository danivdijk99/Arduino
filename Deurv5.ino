#include "RTClib.h"
#include <FastLED.h>
RTC_PCF8523 rtc;
//RTC_DS1307 rtc;
#include "PloegendienstBerekening.h"

bool bol_Zomertijd = false;

String strKleurNu;
String strKleurVolgend;

String Ploegkleur = "Wit";
long lng_offsetslot = 40;
#define Slot 12

void setup() {
  Serial.begin(115200);

  pinMode(Slot, OUTPUT);
  digitalWrite(Slot, LOW);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (!rtc.initialized() || rtc.lostPower()) {
    Serial.println("RTC is NOT initialized, let's set the time!");
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //rtc.adjust(DateTime(2024, 11, 22, 13, 19, 45));
  rtc.start();
}

void loop() {
  HaalTijdUitRtc();

if 
(
       ((int_Uur >= 5 || int_Uur >= 13 || int_Uur >= 21) && (int_Minuut >=   40) && (strKleurVolgend == Ploegkleur)) 

         //((int_Uur == 5 || int_Uur == 13 || int_Uur == 21) && (strKleurVolgend == Ploegkleur))

        || (strKleurNu == Ploegkleur) 
) 
{
  digitalWrite(Slot, HIGH);
  Serial.println("Open");
}
else {
  digitalWrite(Slot, LOW);
  Serial.println("Dicht");
}

}

void HaalTijdUitRtc() {
  int int_DagNu, intMaandNu, intJaarNu, intUurNu, int_MinuutNu;
  int int_DagVolgend, intMaandVolgend, intJaarVolgend, intUurVolgend, int_MinuutVolgend;


  //Tijd van RTC klok
  DateTime now = rtc.now() + TimeSpan(0, 0, lng_offsetslot, 0);

  if (Zomertijd(int_Jaar, int_Maand, int_Dag, int_Uur, 1)) {
    bol_Zomertijd = true;
    //Het is zomertijd. Tijd + 1 uur
    //Serial.println("Het is zomertijd");
    //Ploegkleur berekening huidige wacht
    DateTime Zomertijd(now + TimeSpan(0, 1, 0, 0));
    strKleurNu = Welke_ploeg_op_Nu(Zomertijd);

    int_DagNu = Zomertijd.day();
    intMaandNu = Zomertijd.month();
    intJaarNu = Zomertijd.year();
    intUurNu = Zomertijd.hour();
    int_MinuutNu = Zomertijd.minute();
    Serial.print(" ");
    Serial.print(int_DagNu, DEC);
    Serial.print("-");
    Serial.print(intMaandNu, DEC);
    Serial.print("-");
    Serial.print(intJaarNu, DEC);
    Serial.print(" ");
    Serial.print(intUurNu, DEC);
    Serial.print(":");
    Serial.print(int_MinuutNu, DEC);

    Serial.print(" Huidige ploeg: ");
    Serial.print(strKleurNu);


    //Ploegkleur berekening volgende wacht
    DateTime future(now + TimeSpan(0, 9, 0, 0));
    strKleurVolgend = Welke_ploeg_op_Nu(future);

    int_DagVolgend = future.day();
    intMaandVolgend = future.month();
    intJaarVolgend = future.year();
    intUurVolgend = future.hour();
    int_MinuutVolgend = future.minute();
    Serial.print(" // ");
    Serial.print(int_DagVolgend, DEC);
    Serial.print("-");
    Serial.print(intMaandVolgend, DEC);
    Serial.print("-");
    Serial.print(intJaarVolgend, DEC);
    Serial.print(" ");
    Serial.print(intUurVolgend, DEC);
    Serial.print(":");
    Serial.print(int_MinuutVolgend, DEC);

    Serial.print(" - Volgende ploeg: ");
    Serial.print(strKleurVolgend);

    int_Jaar = Zomertijd.year();
    int_Maand = Zomertijd.month();
    int_Dag = Zomertijd.day();
    int_Uur = Zomertijd.hour();
    int_Minuut = Zomertijd.minute();
    int_DagVanDeWeek = Zomertijd.dayOfTheWeek();
  } else {
    bol_Zomertijd = false;
    //Kennelijk is het wintertijd. Tijd is tijd
    //Serial.println("Het is wintertijd");
    //Ploegkleur berekening huidige wacht
    strKleurNu = Welke_ploeg_op_Nu(now);
    Serial.print("Huidige ploeg: ");
    Serial.print(strKleurNu);

    //Ploegkleur berekening volgende wacht
    DateTime future(now + TimeSpan(0, 8, 0, 0));
    strKleurVolgend = Welke_ploeg_op_Nu(future);

    Serial.print(" - Volgende ploeg: ");
    Serial.print(strKleurVolgend);
    Serial.println();

    int_Jaar = now.year();
    int_Maand = now.month();
    int_Dag = now.day();
    int_Uur = now.hour();
    int_Minuut = now.minute();
    int_DagVanDeWeek = now.dayOfTheWeek();
  }
  Serial.print("int_Jaar=");
  Serial.println(int_Jaar);
  Serial.print("int_Maand=");
  Serial.println(int_Maand);
  Serial.print("int_Dag=");
  Serial.println(int_Dag);
  Serial.print("int_Uur=");
  Serial.println(int_Uur);
  Serial.print("int_Minuut=");
  Serial.println(int_Minuut);
}

//Is het zomer of wintertijd?
boolean Zomertijd(int year, byte month, byte day, byte hour, byte tzHours) {
  //Wintertijd
  //Laatste zondag van oktober gaat de klok om 03:00 een uur terug
  if (month < 3 || month > 10) return false;  // keine Sommerzeit in Jan, Feb, Nov, Dez
  if (month > 3 && month < 10) return true;   // Sommerzeit in Apr, Mai, Jun, Jul, Aug, Sep
  if (month == 3 && (hour + 24 * day) >= (1 + tzHours + 24 * (31 - (5 * year / 4 + 4) % 7)) || month == 10 && (hour + 24 * day) < (1 + tzHours + 24 * (31 - (5 * year / 4 + 1) % 7)))
    return true;
  else
    return false;
}

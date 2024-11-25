//Tbv ploegendienstberekening
int Od, Md, Nd, dummy1, dummy2, dummy3, dummy4;
int int_Jaar, int_Maand, int_Dag, int_Uur, int_Minuut, int_DagVanDeWeek;
int int_JaarPB, int_MaandPB, int_DagPB, int_UurPB, int_MinuutPB;
//String strKleurNu, strKleurOud;
int Jaar, Maand, Dag, Uur, Minuut;
//Bepaalt welke dienst de ploegkleuren op de optegeven datum lopen.
void PloegRooster(int Ochtenddienst, int Middagdienst, int Nachtdienst, int Vrij1, int Vrij2, int Vrij3, int Vrij4)
{
  //Om te zorgen dat de vrije dagen elk jaar opschuiven wordt elk jaar beschouwd als schrikkeljaar: februari heeft altijd 29 dagen, het jaar 366.
  //Stel 1-1-90 als dag nummer 1.

  long Dagnr;
  int Maand_dagen[] = {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};
  Dagnr = (Jaar - 1990) * 366 + Maand_dagen[Maand - 1] + Dag;

  //Cyclus is 2 Ochtend, 2 Middag, 1 Vrij, 2 Nacht en dan 3 vrij
  //Elke kleur loopt 2 achtereenvolgende dagen in de 10 dagen dezelfde dienst
  Ochtenddienst = ((Dagnr + 8) % 10) / 2;
  Middagdienst = ((Dagnr + 6) % 10) / 2;
  Nachtdienst = ((Dagnr + 3) % 10) / 2;

  //In een periode van 10 dagen heeft elke kleur zijn eerste, tweede, Derde en vierde vrije dag
  Vrij1 = ((Dagnr + 1) * 2) % 10;
  Vrij2 = ((Dagnr + 2) * 2) % 10;
  Vrij3 = ((Dagnr + 3) * 2) % 10;
  Vrij4 = ((Dagnr + 4) * 2) % 10;

  Od = Ochtenddienst;
  Md = Middagdienst;
  Nd = Nachtdienst;
  dummy1 = Vrij1;
  dummy2 = Vrij2;
  dummy3 = Vrij3;
  dummy4 = Vrij4;
}

String Welke_ploeg_op_Nu(DateTime Tijdstip)
{
  String str_KleurenArray[] = {"Groen", "Geel", "Rood", "Blauw", "Wit"};
  //Bepaald welke ploeg op is bij op te geven datum en tijd.

  Jaar = Tijdstip.year();
  Maand = Tijdstip.month();
  Dag = Tijdstip.day();
  Uur = Tijdstip.hour();
  Minuut = Tijdstip.minute();

  //Als de nachtdienst op is, is het nog het rooster van de vorige dag omdat toen de nachtdienst starte
  if (Uur < 6) {
    Dag--;
  }

  //Bepaal welke ploegkleuren er op deze datum moeten lopen
  PloegRooster(Od, Md, Nd, dummy1, dummy2, dummy3, dummy4);

  //Bepalen welke ploeg aanwezig is
  if ((Uur >= 22) || (Uur < 6)) {
    return str_KleurenArray[Nd];
  }
  if ((Uur >= 6) && (Uur < 14)) {
    return str_KleurenArray[Od];
  }
  if ((Uur >= 14) && (Uur < 22)) {
    return str_KleurenArray[Md];
  }

return "";
}

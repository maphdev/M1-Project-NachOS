int
main()
{
  char c[256];

  PutString("Test de l'implémentation de PutString !\n\n");
  PutString("\"Nam sole orto magnitudine angusti gurgitis sed profundi a transitu arcebantur.\"\n\n");
  PutString("Le buffer de taille 64 a affiché un texte de plus de 64 caractères.\n");
  PutString("L'implémentation de PutString marche !\n\n");

  PutString("-----------------------------------------------------------\n\n");

  PutString("Testons maintenant l'implémentation de GetString.\n\n");

  PutString("Saisissez une chaîne de caractère de 5 caractères.\n");
  PutString("Elle sera récupérée avec GetString et renvoyée avec PutString.\n");
  while (strnlen(c, 5) != 5){
    GetString(&c, 5);
    if(strnlen(c, 5) == 5){
      break;
    }
    PutString("Veuillez saisir 5 caractères uniquement, pas plus, pas moins.\n");
  }
  PutString(&c);
  PutString("\nLa fonction GetString a bien récupéré 5 caractères.\n\n");

  PutString("Tapez du texte. Celui-ci sera récupéré avec GetString et renvoyé avec PutString.\n");
  PutString("Tapez \"quit\" pour quitter.\n");

  GetString(&c, 256);
  while (c[0] != 'q' || c[1] != 'u' || c[2] != 'i' || c[3] != 't'){     // || et && inversés ?????
    PutString(&c);
    PutString("\n");
    GetString(&c, 256);
  }
}

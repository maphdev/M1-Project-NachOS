int
main()
{

  char c;

  printf("Test de l'implémentation de GetChar et PutChar.\n\n");
  printf("Saisissez un caractère.\n");
  printf("Il sera récupéré avec GetChar et renvoyé avec PutChar.\n");
  c = GetChar();
  PutChar(c);

  printf("\n\nSaisissez un ou plusieurs caractères.\n");
  printf("Il(s) seront récupéré(s) avec GetChar et renvoyé(s) avec PutChar.\n");
  printf("Saisissez '5' pour quitter.");

  c = GetChar();

  while (c != '5'){
    PutChar(c);
    c = GetChar();
  }

  printf("\n");

  return 0;
}

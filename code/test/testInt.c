int
main()
{
  int n = 1;

  printf("Test de l'implémentation de PutInt.\n\n");
  printf("Rappel : un entier est compris entre -2147483648 et 2147483647.\n\n");

  printf("Test de dépassement.\n");
  printf("PutInt(max_int) : ");
  PutInt(2147483647);
  printf("\nPutInt(max_int+1) : ");
  PutInt(2147483648);
  printf("\nDépassement !\n");
  printf("PutInt(min_int) : ");
  PutInt(-2147483648);
  printf("\nPutInt(min_int-1) : ");
  PutInt(-2147483649);
  printf("\nDépassement !\n");
  printf("PutInt marche !\n");


  printf("\n-----------------------------------------------------------\n\n");

  printf("Test de l'implémentation de GetInt.\n\n");
  printf("Saisissez un entier compris entre -2147483648 et 2147483647.\n");
  printf("Celui-ci sera récupéré avec GetInt et renvoyé avec PutInt.\n");
  printf("Saisissez '0' pour quitter.\n");

  while (n != 0){
    GetInt(&n);
    PutInt(n);
    PutChar('\n');
  }
}

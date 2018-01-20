#ifdef CHANGED

// classe qui gère les pages physiques

#include "pageprovider.h"
#include "machine.h"
#include "system.h"

PageProvider::PageProvider(){
  pagesBitmap = new BitMap(NumPhysPages);
}

PageProvider::~PageProvider() {
  delete pagesBitmap;
}

// récupère le numéro d'une page libre et l'initialise à 0
int PageProvider::GetEmptyPage() {
  int idFreePage = pagesBitmap->Find();
  ASSERT(idFreePage != -1);
  memset(&machine->mainMemory[idFreePage*PageSize], 0, PageSize);
  return idFreePage;
}

// récupère le numéro d'un page aléatoirement
int PageProvider::GetRandomEmptyPage(){
  int randomIdFreePage;
  do {
    randomIdFreePage = rand() % NumPhysPages;
  } while(pagesBitmap->Test(randomIdFreePage));
  pagesBitmap->Mark(randomIdFreePage);
  memset(&machine->mainMemory[randomIdFreePage*PageSize], 0, PageSize);
  return randomIdFreePage;
}

// libère la page définie par n
void PageProvider::ReleasePage(int n) {
  pagesBitmap->Clear(n);
}

// renvoit le nombre de pages disponibles
int PageProvider::NumAvailPage() {
  return pagesBitmap->NumClear();
}

#endif

#ifdef CHANGED

#ifndef PAGEPROVIDER_H
#define PAGEPROVIDER_H

#include "bitmap.h"

class PageProvider {
  public:
    PageProvider();
    ~PageProvider();

    int GetEmptyPage();
    int GetRandomEmptyPage();

    void ReleasePage(int n);
    int NumAvailPage();

  private:
    BitMap *pagesBitmap;
};

#endif
#endif

// ============================================================
// main.cpp — CHUONG TRINH QUAN LY THU VIEN - NHOM 7
// ============================================================
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "common.h"
#include "mylib.h"
#include "docgia.h"
#include "fileio.h"
#include "menu.h"

using namespace std;

int main() {
  system("mode con: cols=110 lines=30");
  srand((unsigned)time(NULL));

  DS_DauSach ds;
  PTRDG treeDG = NULL;
  const char *fSach = "THUVIEN.TXT", *fDG = "DOCGIA.TXT";

  if (LoadAll(ds, treeDG, fSach, fDG) == 0)
    BaoLoi("Chua co file du lieu - bat dau voi danh sach rong");
  else {
    DocGia *tmp[2000];
    int nDG = DoCayRaMang(treeDG, tmp, 2000);
    SetColor(10);
    printf("\n  Da load du lieu tu file: %d dau sach, %d doc gia.\n", ds.n, nDG);
    SetColor(15);
    Sleep(1300);
  }

  MenuChinh(ds, treeDG);

  return 0;
}

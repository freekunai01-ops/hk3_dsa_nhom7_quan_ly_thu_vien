// ============================================================
// fileio.h — LUU / DOC FILE VAN BAN - Khai bao ham
// ============================================================
#ifndef FILEIO_H
#define FILEIO_H

#include "common.h"

int SaveAll(DS_DauSach &ds, PTRDG root, const char *fSach, const char *fDG);
int LoadAll(DS_DauSach &ds, PTRDG &root, const char *fSach, const char *fDG);

#endif

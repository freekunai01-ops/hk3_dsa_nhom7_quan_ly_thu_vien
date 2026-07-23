// ============================================================
// muontra.h — NGHIEP VU MUON / TRA - Khai bao ham logic
// ============================================================
#ifndef MUONTRA_H
#define MUONTRA_H

#include "common.h"

int InDangMuonTai(int x, int y, DocGia *dg, DS_DauSach &ds);
void VeKhungMT(const char *tieude);
void CauF_MuonSach(PTRDG root, DS_DauSach &ds);
void CauG_TraSach(PTRDG root, DS_DauSach &ds);
void CauH_DangMuon(PTRDG root, DS_DauSach &ds);

#endif

// ============================================================
// dausach.h — KHOI SACH (PHUC phu trach) - Khai bao ham logic
// ============================================================
#ifndef DAUSACH_H
#define DAUSACH_H

#include "common.h"

// Tim & Thao tac Tren mang DauSach
int TimDauSach_ISBN(DS_DauSach &ds, const char *isbn);
int ChenDauSach(DS_DauSach &ds, DauSach *p);
void GoKhoiMang(DS_DauSach &ds, int i);
int SttLonNhat(DauSach *d);
void ThemCuonVao(DauSach *d, int soLuong, const char *viTri);
NodeSach* TimCuon(DS_DauSach &ds, const char *ma, int &idxDau);
void XoaHetCuon(PTRSACH &First);
int CoCuonDangMuon(DauSach *d);
void XoaTatCaDauSach(DS_DauSach &ds);

// Undo khoi sach
void PushDS(ActDS *&Top, int loai, DauSach d);
void ClearUndoDS(ActDS *&Top);
void UndoDS_Exec(DS_DauSach &ds, ActDS *&Top);

// Cac cau chuc nang (In the loai, Tim ten, Top 10)
void CauD_InTheoTheLoai(DS_DauSach &ds);
int ChuaChuoi(const char *a, const char *b);
void CauE_TimTheoTen(DS_DauSach &ds);
void CauJ_Top10(DS_DauSach &ds);

#endif

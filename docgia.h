// ============================================================
// docgia.h — KHOI DOC GIA (N phu trach) - Khai bao ham logic
// ============================================================
#ifndef DOCGIA_H
#define DOCGIA_H

#include "common.h"

void ChenDG(PTRDG &root, DocGia x);
NodeDG* TimDG(PTRDG root, int maThe);
int XoaNodeDG(PTRDG &root, int maThe);
int DoCayRaMang(PTRDG root, DocGia *arr[], int mx);
int DemDangMuon(DocGia *dg);
int QuaHanMax(DocGia *dg);
void XoaHetMT(PTRMT &First);
void GiaiPhongCay(PTRDG &root);

// Undo khoi doc gia
void PushDG(ActDG *&Top, int loai, DocGia d);
void ClearUndoDG(ActDG *&Top);
void UndoDG_Exec(PTRDG &root, ActDG *&Top);

int SinhMaThe(PTRDG root);
void CauI_QuaHan(PTRDG root);

#endif

// ============================================================
// menu.h — GIAO DIEN (TUI), NHAP LIEU & MENU CHINH
// ============================================================
#ifndef MENU_H
#define MENU_H

#include "common.h"

// Key constants
const int KESC = 27, KENTER = 13;
const int KUP = 1072, KDOWN = 1080, KLEFT = 1075, KRIGHT = 1077;
const int KF1 = 1059, KF2 = 1060, KF3 = 1061, KF4 = 1062;
const int KF5 = 1063, KF6 = 1064, KF7 = 1065, KF8 = 1066;
const int KDEL = 1083;

// TUI functions
int DocPhim();
void VeKhung(int x1, int y1, int x2, int y2);
void VeNgang(int x1, int x2, int y);
void InCell(const char *s, int w);
void InMau(int x, int y, const char *s, int mau);
void AnConTro(int an);
int MenuHop(const char *tieude, const char *items[], int n);

// Nhap lieu & Thong bao
void BaoLoi(const char *s);
int XacNhan(const char *S);
int NhapSo(const char *tieude);
void NhapChuoi(const char *tieude, char *S, int maxlen);
void NhapTen(const char *tieude, char *S, int maxlen);
void NhapMa(const char *tieude, char *S, int maxlen);
void NhapPhai(char *phai);
void BoXuongDong(char *s);

// Ngay thang
Date HomNay();
long SoNgay(Date d);
int NgayQuaHan(Date ngayMuon);

// Man hinh giao dien TUI
void ManHinhDauSach(DS_DauSach &ds, ActDS *&undo);
void ManHinhDocGia(PTRDG &root, ActDG *&undo);
void ManHinhMuonTra(PTRDG &treeDG, DS_DauSach &ds);
void ManHinhBaoCao(PTRDG &treeDG, DS_DauSach &ds);
void MenuChinh(DS_DauSach &ds, PTRDG &treeDG);

#endif

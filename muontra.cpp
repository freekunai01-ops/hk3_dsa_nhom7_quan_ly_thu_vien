// ============================================================
// muontra.cpp — NGHIEP VU MUON / TRA - Than ham logic
// ============================================================
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include "muontra.h"
#include "dausach.h"
#include "docgia.h"
#include "mylib.h"
#include "menu.h"

using namespace std;

// in danh sach cuon DANG MUON cua 1 doc gia tu dong y
int InDangMuonTai(int x, int y, DocGia *dg, DS_DauSach &ds) {
  int dem = 0;
  for (PTRMT p = dg->dsMuonTra; p; p = p->next)
    if (p->data.trangThai == 0) {
      int idx; NodeSach *ns = TimCuon(ds, p->data.maSach, idx);
      gotoxy(x, y + dem);
      InCell(p->data.maSach, 13); putchar(' ');
      InCell(ns ? ds.nodes[idx]->tenSach : "(?)", 24); putchar(' ');
      printf("muon %02d/%02d/%d", p->data.ngayMuon.dd,
             p->data.ngayMuon.mm, p->data.ngayMuon.yyyy);
      if (NgayQuaHan(p->data.ngayMuon) > 0) {
        SetColor(12); printf(" QUA HAN"); SetColor(15);
      }
      dem++;
    }
  if (dem == 0) { gotoxy(x, y); printf("(chua muon cuon nao)"); }
  return dem;
}

// ve khung chung cho 3 man muon/tra/dang muon
void VeKhungMT(const char *tieude) {
  system("cls");
  AnConTro(0);
  InMau(26, 1, tieude, 14);
  VeKhung(2, 2, 68, 16);
  InMau(2, 18, "(nhap ma the = 0 roi Enter de quay lai menu)", 11);
}

// CAU f — MUON SACH
void CauF_MuonSach(PTRDG root, DS_DauSach &ds) {
  VeKhungMT("MUON SACH (cau f)");
  gotoxy(4, 4);
  int ma = NhapSo("Ma the doc gia: ");
  if (ma == 0) return;
  NodeDG *nd = TimDG(root, ma);
  if (!nd) { BaoLoi("Ma the khong ton tai"); return; }
  DocGia *dg = &nd->data;
  gotoxy(4, 5);
  printf("Doc gia: "); InCell(dg->ho, 18); putchar(' '); InCell(dg->ten, 10);
  printf(" (%s) - the %s", dg->phai, dg->trangThai ? "hoat dong" : "BI KHOA");
  InMau(4, 7, "Cac cuon dang muon:", 11);
  int dang = InDangMuonTai(6, 8, dg, ds);
  
  if (dg->trangThai == 0) { BaoLoi("The bi KHOA, khong duoc muon"); return; }
  if (dang >= 3) { BaoLoi("Da muon du 3 cuon, khong duoc muon them"); return; }
  if (QuaHanMax(dg) > 0) { BaoLoi("Dang giu sach QUA HAN (>7 ngay), khong cho muon"); return; }

  gotoxy(4, 12);
  char maCuon[MAXMA];
  NhapMa("Ma cuon muon muon: ", maCuon, MAXMA);
  int idx; NodeSach *ns = TimCuon(ds, maCuon, idx);
  if (!ns) { BaoLoi("Khong tim thay ma cuon nay"); return; }
  if (ns->data.trangThai == 1) { BaoLoi("Cuon nay dang co nguoi khac muon"); return; }
  if (ns->data.trangThai == 2) { BaoLoi("Cuon nay da thanh ly"); return; }
  gotoxy(4, 13);
  printf("Sach: "); InCell(ds.nodes[idx]->tenSach, 40);
  gotoxy(4, 14);
  if (!XacNhan("Xac nhan cho muon? (Y/N): ")) return;
  PTRMT p = new NodeMT;
  strcpy(p->data.maSach, maCuon);
  p->data.ngayMuon = HomNay();
  p->data.ngayTra.dd = p->data.ngayTra.mm = p->data.ngayTra.yyyy = 0;
  p->data.trangThai = 0;
  p->next = dg->dsMuonTra;
  dg->dsMuonTra = p;
  ns->data.trangThai = 1;
  ds.nodes[idx]->soLuotMuon++;
  InMau(4, 15, "Da cho muon thanh cong. Nhan phim bat ky...", 10);
  getch();
}

// CAU g — TRA SACH
void CauG_TraSach(PTRDG root, DS_DauSach &ds) {
  VeKhungMT("TRA SACH (cau g)");
  gotoxy(4, 4);
  int ma = NhapSo("Ma the doc gia: ");
  if (ma == 0) return;
  NodeDG *nd = TimDG(root, ma);
  if (!nd) { BaoLoi("Ma the khong ton tai"); return; }
  DocGia *dg = &nd->data;
  gotoxy(4, 5);
  printf("Doc gia: "); InCell(dg->ho, 18); putchar(' '); InCell(dg->ten, 10);
  InMau(4, 7, "Cac cuon dang muon:", 11);
  if (InDangMuonTai(6, 8, dg, ds) == 0) { BaoLoi("Doc gia khong muon cuon nao"); return; }
  gotoxy(4, 12);
  char maCuon[MAXMA];
  NhapMa("Ma cuon muon tra: ", maCuon, MAXMA);
  PTRMT p;
  for (p = dg->dsMuonTra; p; p = p->next)
    if (p->data.trangThai == 0 && stricmp(p->data.maSach, maCuon) == 0) break;
  if (!p) { BaoLoi("Doc gia khong dang muon cuon nay"); return; }
  gotoxy(4, 13);
  printf("[T] Tra binh thuong   [M] Bao MAT sach : ");
  char c; do { c = toupper(getch()); } while (c != 'T' && c != 'M');
  putchar(c);
  int idx; NodeSach *ns = TimCuon(ds, maCuon, idx);
  p->data.ngayTra = HomNay();
  if (c == 'T') {
    p->data.trangThai = 1;
    if (ns) ns->data.trangThai = 0;
    InMau(4, 15, "Da nhan tra sach. Nhan phim bat ky...", 10);
  } else {
    p->data.trangThai = 2;
    if (ns) ns->data.trangThai = 2;
    InMau(4, 15, "Da ghi nhan MAT sach (cuon chuyen thanh ly). Nhan phim...", 12);
  }
  getch();
}

// CAU h — LIET KE SACH DOC GIA X DANG MUON
void CauH_DangMuon(PTRDG root, DS_DauSach &ds) {
  VeKhungMT("SACH DOC GIA X DANG MUON (cau h)");
  gotoxy(4, 4);
  int ma = NhapSo("Ma the doc gia X: ");
  if (ma == 0) return;
  NodeDG *nd = TimDG(root, ma);
  if (!nd) { BaoLoi("Ma the khong ton tai"); return; }
  gotoxy(4, 5);
  printf("Doc gia: "); InCell(nd->data.ho, 18); putchar(' '); InCell(nd->data.ten, 10);
  InMau(4, 7, "Dang muon:", 11);
  int dem = InDangMuonTai(6, 8, &nd->data, ds);
  gotoxy(4, 14);
  printf("Tong: %d cuon dang muon.", dem);
  InMau(4, 15, "Nhan phim bat ky de quay lai...", 11);
  getch();
}

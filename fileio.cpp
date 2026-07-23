// ============================================================
// fileio.cpp — LUU / DOC FILE VAN BAN - Than ham logic
// ============================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileio.h"
#include "dausach.h"
#include "docgia.h"
#include "menu.h"

int DocSoDong(FILE *f) {                  // doc 1 dong chua 1 so
  char t[64];
  if (!fgets(t, 64, f)) return -999999;
  return atoi(t);
}

int DocDong(FILE *f, char *S, int mx) {
  if (!fgets(S, mx, f)) return 0;
  BoXuongDong(S);
  return 1;
}

int SaveAll(DS_DauSach &ds, PTRDG root, const char *fSach, const char *fDG) {
  FILE *f = fopen(fSach, "wt");
  if (!f) return 0;
  fprintf(f, "%d\n", ds.n);
  for (int i = 0; i < ds.n; i++) {
    DauSach *d = ds.nodes[i];
    int soCuon = 0;
    for (NodeSach *p = d->FirstDMS; p; p = p->next) soCuon++;
    fprintf(f, "%s\n%s\n%d\n%s\n%d\n%s\n%d\n%d\n", d->ISBN, d->tenSach, d->soTrang,
            d->tacGia, d->namXuatBan, d->theLoai, d->soLuotMuon, soCuon);
    for (NodeSach *p = d->FirstDMS; p; p = p->next)
      fprintf(f, "%s\n%d\n%s\n", p->data.maSach, p->data.trangThai,
              p->data.viTri);
  }
  fclose(f);

  f = fopen(fDG, "wt");
  if (!f) return 0;
  DocGia *tmp[2000];
  int nDG = DoCayRaMang(root, tmp, 2000); // dem so doc gia
  fprintf(f, "%d\n", nDG);
  if (root) {                             // duyet NLR bang stack (khong de quy)
    NodeDG *stk[2000]; int top = -1;
    stk[++top] = root;
    while (top >= 0) {
      NodeDG *p = stk[top--];
      int soMT = 0;
      for (PTRMT q = p->data.dsMuonTra; q; q = q->next) soMT++;
      fprintf(f, "%d\n%s\n%s\n%s\n%d\n%d\n", p->data.maThe, p->data.ho,
              p->data.ten, p->data.phai, p->data.trangThai, soMT);
      for (PTRMT q = p->data.dsMuonTra; q; q = q->next)
        fprintf(f, "%s\n%d %d %d\n%d %d %d\n%d\n", q->data.maSach,
                q->data.ngayMuon.dd, q->data.ngayMuon.mm, q->data.ngayMuon.yyyy,
                q->data.ngayTra.dd, q->data.ngayTra.mm, q->data.ngayTra.yyyy,
                q->data.trangThai);
      if (p->right) stk[++top] = p->right;
      if (p->left)  stk[++top] = p->left;
    }
  }
  fclose(f);
  return 1;
}

int LoadAll(DS_DauSach &ds, PTRDG &root, const char *fSach, const char *fDG) {
  FILE *f = fopen(fSach, "rt");
  if (!f) return 0;
  XoaTatCaDauSach(ds);                    // xoa sach du lieu cu truoc khi load
  GiaiPhongCay(root);                     // (tranh bi nhan doi du lieu)
  int n = DocSoDong(f);
  char line[128];
  for (int i = 0; i < n; i++) {
    DauSach *d = new DauSach;
    d->FirstDMS = NULL;
    DocDong(f, d->ISBN, 15);
    DocDong(f, d->tenSach, 100);
    d->soTrang = DocSoDong(f);
    DocDong(f, d->tacGia, 60);
    d->namXuatBan = DocSoDong(f);
    DocDong(f, d->theLoai, 40);
    d->soLuotMuon = DocSoDong(f);
    int soCuon = DocSoDong(f);
    for (int k = 0; k < soCuon; k++) {
      NodeSach *p = new NodeSach;
      DocDong(f, p->data.maSach, MAXMA);
      p->data.trangThai = DocSoDong(f);
      DocDong(f, p->data.viTri, 50);
      p->next = d->FirstDMS;
      d->FirstDMS = p;
    }
    ChenDauSach(ds, d);
  }
  fclose(f);

  f = fopen(fDG, "rt");
  if (!f) return 1;                       // chua co file doc gia: van OK
  int nDG = DocSoDong(f);
  for (int i = 0; i < nDG; i++) {
    DocGia dg;
    dg.dsMuonTra = NULL;
    dg.maThe = DocSoDong(f);
    DocDong(f, dg.ho, 50);
    DocDong(f, dg.ten, 11);
    DocDong(f, dg.phai, 5);
    dg.trangThai = DocSoDong(f);
    int soMT = DocSoDong(f);
    PTRMT duoi = NULL;
    for (int k = 0; k < soMT; k++) {      // giu dung thu tu file (chen cuoi)
      PTRMT p = new NodeMT;
      DocDong(f, p->data.maSach, MAXMA);
      DocDong(f, line, 128);
      sscanf(line, "%d %d %d", &p->data.ngayMuon.dd, &p->data.ngayMuon.mm, &p->data.ngayMuon.yyyy);
      DocDong(f, line, 128);
      sscanf(line, "%d %d %d", &p->data.ngayTra.dd, &p->data.ngayTra.mm, &p->data.ngayTra.yyyy);
      p->data.trangThai = DocSoDong(f);
      p->next = NULL;
      if (!dg.dsMuonTra) dg.dsMuonTra = p; else duoi->next = p;
      duoi = p;
    }
    ChenDG(root, dg);                     // file luu NLR -> chen lai giu dang cay
  }
  fclose(f);
  return 1;
}

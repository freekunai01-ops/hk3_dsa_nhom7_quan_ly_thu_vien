// ============================================================
// dausach.cpp — KHOI SACH (PHUC phu trach) - Than ham logic
// ============================================================
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include "dausach.h"
#include "mylib.h"
#include "menu.h"

using namespace std;

int TimDauSach_ISBN(DS_DauSach &ds, const char *isbn) {
  for (int i = 0; i < ds.n; i++)
    if (stricmp(ds.nodes[i]->ISBN, isbn) == 0) return i;
  return -1;
}

// chen giu THU TU TANG DAN THEO TEN SACH (de quy dinh)
int ChenDauSach(DS_DauSach &ds, DauSach *p) {
  if (ds.n == MAX_DAUSACH) return 0;
  int i;
  for (i = 0; i < ds.n && stricmp(ds.nodes[i]->tenSach, p->tenSach) < 0; i++);
  for (int k = ds.n - 1; k >= i; k--) ds.nodes[k+1] = ds.nodes[k];
  ds.nodes[i] = p; ds.n++;
  return 1;
}

// go 1 dau sach khoi mang (KHONG delete — de con dung cho undo/chen lai)
void GoKhoiMang(DS_DauSach &ds, int i) {
  for (int j = i + 1; j < ds.n; j++) ds.nodes[j-1] = ds.nodes[j];
  ds.n--;
}

// so thu tu lon nhat trong cac ma cuon cua 1 dau sach
int SttLonNhat(DauSach *d) {
  int mx = 0;
  for (NodeSach *p = d->FirstDMS; p; p = p->next) {
    char *g = strrchr(p->data.maSach, '-');
    if (g && atoi(g+1) > mx) mx = atoi(g+1);
  }
  return mx;
}

// them soLuong cuon vao dau sach, ma tu dong ISBN-xx, chen DAU DSLK (O(1))
void ThemCuonVao(DauSach *d, int soLuong, const char *viTri) {
  int stt = SttLonNhat(d);
  for (int k = 0; k < soLuong; k++) {
    NodeSach *p = new NodeSach;
    sprintf(p->data.maSach, "%s-%03d", d->ISBN, ++stt);   // 3 chu so: toi da 999 cuon
    p->data.trangThai = 0;             // cuon moi nhap: cho muon duoc
    strcpy(p->data.viTri, viTri);
    p->next = d->FirstDMS;
    d->FirstDMS = p;
  }
}

// tim CUON sach theo ma — tach ISBN (truoc dau '-' cuoi cung)
NodeSach* TimCuon(DS_DauSach &ds, const char *ma, int &idxDau) {
  const char *g = strrchr(ma, '-');
  if (g) {
    char isbn[20]; int len = (int)(g - ma);
    if (len > 0 && len < 20) {
      strncpy(isbn, ma, len); isbn[len] = '\0';
      int i = TimDauSach_ISBN(ds, isbn);
      if (i >= 0)
        for (NodeSach *p = ds.nodes[i]->FirstDMS; p; p = p->next)
          if (stricmp(p->data.maSach, ma) == 0) { idxDau = i; return p; }
    }
  }
  for (int i = 0; i < ds.n; i++)          // du phong: quet toan bo
    for (NodeSach *p = ds.nodes[i]->FirstDMS; p; p = p->next)
      if (stricmp(p->data.maSach, ma) == 0) { idxDau = i; return p; }
  return NULL;
}

void XoaHetCuon(PTRSACH &First) {         // giai phong ca DSLK danh muc
  while (First) { PTRSACH p = First; First = First->next; delete p; }
}

int CoCuonDangMuon(DauSach *d) {
  for (NodeSach *p = d->FirstDMS; p; p = p->next)
    if (p->data.trangThai == 1) return 1;
  return 0;
}

void XoaTatCaDauSach(DS_DauSach &ds) {
  for (int i = 0; i < ds.n; i++) {
    XoaHetCuon(ds.nodes[i]->FirstDMS);
    delete ds.nodes[i];
  }
  ds.n = 0;
}

// ------------------ UNDO KHOI SACH (stack LIFO tu cai dat) ------------------

void PushDS(ActDS *&Top, int loai, DauSach d) {
  ActDS *p = new ActDS; p->loai = loai; p->data = d; p->next = Top; Top = p;
}

void ClearUndoDS(ActDS *&Top) {
  while (Top) {
    ActDS *p = Top; Top = Top->next;
    if (p->loai == U_XOA) XoaHetCuon(p->data.FirstDMS);
    delete p;
  }
}

void UndoDS_Exec(DS_DauSach &ds, ActDS *&Top) {
  if (!Top) { BaoLoi("Khong con thao tac nao de Undo"); return; }
  ActDS *p = Top; Top = Top->next;
  char tb[120];
  int i = TimDauSach_ISBN(ds, p->data.ISBN);
  if (p->loai == U_THEM) {                // undo THEM = xoa dau sach vua them
    if (i >= 0) {
      if (CoCuonDangMuon(ds.nodes[i]))
        BaoLoi("Dau sach da co cuon dang muon, khong the Undo them");
      else {
        XoaHetCuon(ds.nodes[i]->FirstDMS);
        delete ds.nodes[i];
        GoKhoiMang(ds, i);
        sprintf(tb, "Undo: da go bo dau sach %s (vua them)", p->data.ISBN);
        BaoLoi(tb);
      }
    }
  }
  else if (p->loai == U_XOA) {            // undo XOA = chen lai (kem danh muc)
    DauSach *moi = new DauSach;
    *moi = p->data;                       // FirstDMS (ownership) tro lai
    ChenDauSach(ds, moi);
    sprintf(tb, "Undo: da phuc hoi dau sach %s (vua xoa)", p->data.ISBN);
    BaoLoi(tb);
  }
  else {                                  // undo SUA: tra lai cac truong cu
    if (i >= 0) {
      DauSach *ps = ds.nodes[i];
      GoKhoiMang(ds, i);                  // go ra vi ten co the doi -> chen lai
      strcpy(ps->tenSach, p->data.tenSach);
      ps->soTrang = p->data.soTrang;
      strcpy(ps->tacGia, p->data.tacGia);
      ps->namXuatBan = p->data.namXuatBan;
      strcpy(ps->theLoai, p->data.theLoai);   // FirstDMS giu nguyen hien tai
      ChenDauSach(ds, ps);
      sprintf(tb, "Undo: da tra lai thong tin cu cua %s", p->data.ISBN);
      BaoLoi(tb);
    }
  }
  delete p;
}

// ------------------ CAU d — IN THEO TUNG THE LOAI ------------------

void CauD_InTheoTheLoai(DS_DauSach &ds) {
  system("cls");
  if (ds.n == 0) { cout << "Chua co dau sach nao."; getch(); return; }
  int idx[MAX_DAUSACH];
  for (int i = 0; i < ds.n; i++) idx[i] = i;
  for (int i = 0; i < ds.n - 1; i++)
    for (int j = 0; j < ds.n - 1 - i; j++) {
      int kq = stricmp(ds.nodes[idx[j]]->theLoai, ds.nodes[idx[j+1]]->theLoai);
      if (kq > 0 || (kq == 0 &&
          stricmp(ds.nodes[idx[j]]->tenSach, ds.nodes[idx[j+1]]->tenSach) > 0)) {
        int t = idx[j]; idx[j] = idx[j+1]; idx[j+1] = t;
      }
    }
  printf("            DANH SACH DAU SACH THEO THE LOAI\n");
  char theLoaiCu[40] = "";
  int dong = 0;
  for (int k = 0; k < ds.n; k++) {
    DauSach *d = ds.nodes[idx[k]];
    if (stricmp(theLoaiCu, d->theLoai) != 0) {   // sang the loai moi -> in tieu de
      printf("\n--- The loai: %s ---\n", d->theLoai);
      strcpy(theLoaiCu, d->theLoai);
      dong += 2;
    }
    int soCuon = 0, coTheMuon = 0;
    for (NodeSach *p = d->FirstDMS; p; p = p->next) {
      soCuon++;
      if (p->data.trangThai == 0) coTheMuon++;
    }
    printf("  %-14s %-35s %-20s %d  (%d cuon, %d muon duoc)\n",
           d->ISBN, d->tenSach, d->tacGia, d->namXuatBan, soCuon, coTheMuon);
    if (++dong >= 15) {                   // phan trang don gian: 15 dong/trang
      printf("\n-- Nhan phim bat ky de xem tiep --\n");
      getch(); system("cls"); dong = 0;
    }
  }
  printf("\nTong: %d dau sach. Nhan phim bat ky...", ds.n);
  getch();
}

// ------------------ CAU e — TIM SACH THEO TEN ------------------

int ChuaChuoi(const char *a, const char *b) {   // a co CHUA b?
  char A[128], B[128];
  int i;
  for (i = 0; a[i] && i < 127; i++) A[i] = tolower(a[i]);
  A[i] = 0;
  for (i = 0; b[i] && i < 127; i++) B[i] = tolower(b[i]);
  B[i] = 0;
  return strstr(A, B) != NULL;
}

void CauE_TimTheoTen(DS_DauSach &ds) {
  system("cls");
  char ten[100];
  NhapTen("Nhap ten sach can tim: ", ten, 100);
  int thay = 0;
  for (int i = 0; i < ds.n; i++)
    if (ChuaChuoi(ds.nodes[i]->tenSach, ten)) {
      DauSach *d = ds.nodes[i];
      printf("\nISBN: %s | %s | %s | %d | The loai: %s\n",
             d->ISBN, d->tenSach, d->tacGia, d->namXuatBan, d->theLoai);
      for (NodeSach *p = d->FirstDMS; p; p = p->next)
        printf("   %-14s  vi tri %-10s  %s\n", p->data.maSach, p->data.viTri,
               p->data.trangThai == 0 ? "Cho muon duoc" :
               p->data.trangThai == 1 ? "Dang muon" : "Da thanh ly");
      thay++;
    }
  if (!thay) printf("\nKhong tim thay sach nao co ten chua '%s'.", ten);
  printf("\n\nNhan phim bat ky..."); getch();
}

// ------------------ CAU j — TOP 10 SACH MUON NHIEU NHAT (TUI) ------------------

void CauJ_Top10(DS_DauSach &ds) {
  int idx[MAX_DAUSACH];
  for (int i = 0; i < ds.n; i++) idx[i] = i;
  for (int i = 0; i < ds.n - 1; i++)      // selection sort giam dan theo luot muon
    for (int j = i + 1; j < ds.n; j++)
      if (ds.nodes[idx[j]]->soLuotMuon > ds.nodes[idx[i]]->soLuotMuon) {
        int t = idx[i]; idx[i] = idx[j]; idx[j] = t;
      }
  int in = (ds.n < 10) ? ds.n : 10;
  system("cls");
  AnConTro(1);
  InMau(10, 1, "BAO CAO: TOP 10 DAU SACH CO SO LUOT MUON NHIEU NHAT", 14);
  VeKhung(2, 2, 68, 16);
  gotoxy(4, 3); SetColor(11);
  printf("%-4s", "TT"); InCell("ISBN", 12); putchar(' ');
  InCell("Ten sach", 36); printf(" %s", "Luot muon");
  SetColor(15);
  VeNgang(2, 68, 4);
  for (int i = 0; i < in; i++) {
    DauSach *d = ds.nodes[idx[i]];
    gotoxy(4, 5 + i);
    printf("%-4d", i + 1); InCell(d->ISBN, 12); putchar(' ');
    InCell(d->tenSach, 36);                 // ten dai -> tu cat ".."
    SetColor(10); printf(" %5d", d->soLuotMuon); SetColor(15);
  }
  if (ds.n == 0) InMau(4, 5, "(chua co dau sach nao)", 10);
  gotoxy(4, 15);
  printf("Tong so dau sach: %d", ds.n);
  InMau(2, 18, "Nhan phim bat ky de quay lai...", 11);
  getch();
  AnConTro(0);
}

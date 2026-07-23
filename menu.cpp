// ============================================================
// menu.cpp — GIAO DIEN (TUI), NHAP LIEU & MENU CHINH
// ============================================================
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <conio.h>
#include "menu.h"
#include "mylib.h"
#include "dausach.h"
#include "docgia.h"
#include "muontra.h"
#include "fileio.h"

using namespace std;

// ------------------ TUI ------------------

int DocPhim() {
  int c = getch();
  if (c == 0 || c == 224 || c == -32) return 1000 + getch();
  return c;
}

void VeKhung(int x1, int y1, int x2, int y2) {
  gotoxy(x1, y1); putchar('+');
  for (int x = x1 + 1; x < x2; x++) putchar('-');
  putchar('+');
  for (int y = y1 + 1; y < y2; y++) {
    gotoxy(x1, y); putchar('|');
    gotoxy(x2, y); putchar('|');
  }
  gotoxy(x1, y2); putchar('+');
  for (int x = x1 + 1; x < x2; x++) putchar('-');
  putchar('+');
}

void VeNgang(int x1, int x2, int y) {
  gotoxy(x1, y); putchar('+');
  for (int x = x1 + 1; x < x2; x++) putchar('-');
  putchar('+');
}

void InCell(const char *s, int w) {
  int len = (int)strlen(s);
  if (len <= w) printf("%-*s", w, s);
  else {
    for (int i = 0; i < w - 2; i++) putchar(s[i]);
    printf("..");
  }
}

void InMau(int x, int y, const char *s, int mau) {
  gotoxy(x, y); SetColor(mau); printf("%s", s); SetColor(15);
}

void AnConTro(int an) {
  CONSOLE_CURSOR_INFO ci;
  ci.dwSize = 20;
  ci.bVisible = an ? FALSE : TRUE;
  SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
}

int MenuHop(const char *tieude, const char *items[], int n) {
  int chon = 0;
  system("cls");
  AnConTro(1);
  InMau(44, 2, tieude, 14);
  VeKhung(30, 4, 80, 5 + n);
  for (int i = 0; i < n; i++) {
    gotoxy(32, 5 + i);
    if (i == chon) SetBGColor(1);
    printf(" %-45s", items[i]);
    if (i == chon) SetBGColor(0);
  }
  InMau(30, 7 + n, "[LEN/XUONG] chon    [ENTER] vao    [F2] Luu file    [ESC] thoat", 11);
  while (true) {
    int k = DocPhim();
    if (k == KESC)   { AnConTro(0); return -1; }
    if (k == KENTER) { AnConTro(0); return chon; }
    if (k == KF2)    { AnConTro(0); return -2; }
    int cu = chon;
    if (k == KUP && chon > 0) chon--;
    if (k == KDOWN && chon < n - 1) chon++;
    if (chon != cu) {                     // chi ve lai 2 dong doi trang thai
      gotoxy(32, 5 + cu); printf(" %-45s", items[cu]);
      gotoxy(32, 5 + chon); SetBGColor(1); printf(" %-45s", items[chon]); SetBGColor(0);
    }
  }
}

// ------------------ NHAP LIEU ------------------

void BaoLoi(const char *s) {
  int x = wherex(), y = wherey();
  gotoxy(5, 24); SetColor(12); cout << s; SetColor(15);
  Sleep(1500);
  gotoxy(5, 24); clreol(); gotoxy(x, y);
}

int XacNhan(const char *S) {
  cout << S;
  char kt;
  do { kt = toupper(getch()); } while (kt != 'Y' && kt != 'N');
  cout << kt << "\n";
  return kt == 'Y';
}

int NhapSo(const char *tieude) {
  cout << tieude;
  int so = 0, dem = 0; char c;
  do {
    c = getch();
    if (c >= '0' && c <= '9' && dem < 9) { so = so*10 + (c-'0'); dem++; cout << c; }
    else if (c == 8 && dem > 0) { so /= 10; dem--; cout << "\b \b"; }
  } while (!(c == Enter && dem > 0));
  cout << "\n";
  return so;
}

void NhapChuoi(const char *tieude, char *S, int maxlen) {
  cout << tieude;
  int i = 0; char c;
  do {
    c = getch();
    if (c == ' ' && (i == 0 || S[i-1] == ' ')) continue;
    if ((isalpha(c) || c == ' ') && i < maxlen - 1) { S[i++] = c; cout << c; }
    else if (c == 8 && i > 0) { i--; cout << "\b \b"; }
  } while (!(c == Enter && i > 0));
  if (S[i-1] == ' ') { i--; cout << "\b \b"; }
  S[i] = '\0'; cout << "\n";
}

void NhapTen(const char *tieude, char *S, int maxlen) {
  cout << tieude;
  int i = 0; char c;
  do {
    c = getch();
    if (c == ' ' && (i == 0 || S[i-1] == ' ')) continue;
    if ((isalpha(c) || isdigit(c) || strchr(" .,:+-()", c)) && c != 0 && i < maxlen-1) {
      S[i++] = c; cout << c;
    }
    else if (c == 8 && i > 0) { i--; cout << "\b \b"; }
  } while (!(c == Enter && i > 0));
  if (S[i-1] == ' ') { i--; cout << "\b \b"; }
  S[i] = '\0'; cout << "\n";
}

void NhapMa(const char *tieude, char *S, int maxlen) {
  cout << tieude;
  int i = 0; char c;
  do {
    c = getch();
    if (isalpha(c) && i < maxlen-1) { S[i] = toupper(c); cout << S[i]; i++; }
    else if ((isdigit(c) || c == '-' || c == '_') && i < maxlen-1) { S[i++] = c; cout << c; }
    else if (c == 8 && i > 0) { i--; cout << "\b \b"; }
  } while (!(c == Enter && i > 0));
  S[i] = '\0'; cout << "\n";
}

void NhapPhai(char *phai) {
  cout << "Phai (1 = Nam, 2 = Nu): ";
  char c;
  do { c = getch(); } while (c != '1' && c != '2');
  strcpy(phai, (c == '1') ? "Nam" : "Nu");
  cout << phai << "\n";
}

void BoXuongDong(char *s) {
  int len = strlen(s);
  if (len > 0 && s[len-1] == '\n') s[len-1] = '\0';
}

// ------------------ NGAY THANG ------------------

Date HomNay() {
  time_t t = time(NULL);
  tm *lt = localtime(&t);
  Date d; d.dd = lt->tm_mday; d.mm = lt->tm_mon + 1; d.yyyy = lt->tm_year + 1900;
  return d;
}

long SoNgay(Date d) {
  tm x; memset(&x, 0, sizeof(x));
  x.tm_mday = d.dd; x.tm_mon = d.mm - 1; x.tm_year = d.yyyy - 1900; x.tm_hour = 12;
  return (long)(mktime(&x) / 86400);
}

int NgayQuaHan(Date ngayMuon) {
  return (int)(SoNgay(HomNay()) - SoNgay(ngayMuon)) - HAN_MUON_NGAY;
}

// ============ MAN HINH DAU SACH (TUI) ============

static void VePanelDS(const char *tieude) {
  VeKhung(72, 3, 109, 19);
  InMau(74, 4, tieude, 14);
}

static void InDongDS(DauSach *d, int stt, int y, int chonFlag) {
  int soCuon = 0, ranh = 0;
  for (NodeSach *p = d->FirstDMS; p; p = p->next) {
    soCuon++;
    if (p->data.trangThai == 0) ranh++;
  }
  gotoxy(4, y);
  if (chonFlag) SetBGColor(1);
  printf("%-4d", stt + 1); InCell(d->ISBN, 9); putchar(' ');
  InCell(d->tenSach, 19); putchar(' ');
  InCell(d->tacGia, 11);
  printf(" %-5d", d->namXuatBan); InCell(d->theLoai, 8);
  printf(" %2d/%-2d", ranh, soCuon);
  if (chonFlag) SetBGColor(0);
}

void ManHinhDauSach(DS_DauSach &ds, ActDS *&undo) {
  int chon = 0;
  char trangthai[120] = "";
  int veLai = 1;
  while (true) {
    int trang, tongtrang;
    if (veLai) {
      if (chon >= ds.n) chon = (ds.n > 0) ? ds.n - 1 : 0;
      trang = (ds.n > 0) ? chon / 10 : 0;
      tongtrang = (ds.n + 9) / 10; if (tongtrang < 1) tongtrang = 1;
      system("cls");
      AnConTro(1);
      InMau(24, 1, "QUAN LY DAU SACH (ten tang dan)", 14);
      VeKhung(2, 2, 70, 16);
      gotoxy(4, 3); SetColor(11);
      printf("%-4s", "STT"); InCell("ISBN", 9); putchar(' ');
      InCell("Ten sach", 19); putchar(' '); InCell("Tac gia", 11);
      printf(" %-5s", "Nam"); InCell("The loai", 8); printf(" %s", "Cuon");
      SetColor(15);
      VeNgang(2, 70, 4);
      int dau = trang * 10, cuoi = dau + 10; if (cuoi > ds.n) cuoi = ds.n;
      for (int i = dau; i < cuoi; i++)
        InDongDS(ds.nodes[i], i, 5 + i - dau, i == chon);
      if (ds.n == 0) InMau(4, 5, "(danh sach rong - bam F1 de them)", 12);
      gotoxy(4, 15);
      printf("Trang %d/%d - Tong %d dau sach   (Cuon = ranh/tong)", trang + 1, tongtrang, ds.n);
      InMau(2, 18, "[F1]Them [F2]Sua [F3]Xoa [F4]Undo [F5]Them cuon [F6]Thanh ly", 11);
      InMau(2, 19, "[F7]The loai [F8]Tim ten [ENTER]Xem cuon [ESC]Ve menu", 11);
      InMau(2, 20, "[LEN/XUONG] con tro   [TRAI/PHAI] trang", 11);
      if (trangthai[0]) InMau(2, 22, trangthai, 10);
      veLai = 0;
    }
    trang = (ds.n > 0) ? chon / 10 : 0;
    int k = DocPhim();
    if (k == KESC) { AnConTro(0); return; }
    else if (k == KUP && chon > 0) {
      int cu = chon; chon--;
      if (chon / 10 != trang) veLai = 1;
      else {
        InDongDS(ds.nodes[cu], cu, 5 + cu % 10, 0);
        InDongDS(ds.nodes[chon], chon, 5 + chon % 10, 1);
      }
    }
    else if (k == KDOWN && chon < ds.n - 1) {
      int cu = chon; chon++;
      if (chon / 10 != trang) veLai = 1;
      else {
        InDongDS(ds.nodes[cu], cu, 5 + cu % 10, 0);
        InDongDS(ds.nodes[chon], chon, 5 + chon % 10, 1);
      }
    }
    else if (k == KLEFT && chon - 10 >= 0) { chon -= 10; veLai = 1; }
    else if (k == KRIGHT && chon + 10 < ds.n) { chon += 10; veLai = 1; }
    else if (k == KF7) { CauD_InTheoTheLoai(ds); veLai = 1; trangthai[0] = 0; }
    else if (k == KF8) { AnConTro(0); CauE_TimTheoTen(ds); veLai = 1; trangthai[0] = 0; }
    else if (k == KF4) { UndoDS_Exec(ds, undo); strcpy(trangthai, "(vua Undo)"); veLai = 1; }
    else if (k == KENTER && ds.n > 0) {
      DauSach *d = ds.nodes[chon];
      VePanelDS("DANH MUC CUON");
      gotoxy(74, 5); InCell(d->tenSach, 32);
      int y = 7;
      for (NodeSach *p = d->FirstDMS; p && y < 18; p = p->next, y++) {
        gotoxy(74, y);
        InCell(p->data.maSach, 13); putchar(' ');
        printf("%s", p->data.trangThai == 0 ? "ranh" :
                     p->data.trangThai == 1 ? "DANG MUON" : "thanh ly");
      }
      if (!d->FirstDMS) { gotoxy(74, 7); printf("(chua co cuon nao)"); }
      gotoxy(74, 18); printf("Nhan phim bat ky...");
      getch();
      veLai = 1; trangthai[0] = 0;
    }
    else if (k == KF1) {
      if (ds.n == MAX_DAUSACH) { BaoLoi("Danh sach dau sach DAY"); continue; }
      VePanelDS("THEM DAU SACH");
      AnConTro(0);
      DauSach *d = new DauSach;
      d->FirstDMS = NULL;
      gotoxy(74, 6);  NhapMa("ISBN: ", d->ISBN, 15);
      if (TimDauSach_ISBN(ds, d->ISBN) >= 0) {
        BaoLoi("ISBN bi trung"); delete d; veLai = 1; trangthai[0] = 0; continue;
      }
      gotoxy(74, 8);  NhapTen("Ten (toi da 33): ", d->tenSach, 34);
      gotoxy(74, 10); d->soTrang = NhapSo("So trang: ");
      gotoxy(74, 12); NhapTen("Tac gia: ", d->tacGia, 20);
      gotoxy(74, 14); d->namXuatBan = NhapSo("Nam XB: ");
      gotoxy(74, 16); NhapTen("The loai: ", d->theLoai, 12);
      gotoxy(74, 18); int soCuon = NhapSo("So cuon: ");
      ThemCuonVao(d, soCuon, "Kho");
      ChenDauSach(ds, d);
      PushDS(undo, U_THEM, *d);
      sprintf(trangthai, "Da them '%s' (%d cuon).", d->tenSach, soCuon);
      veLai = 1;
    }
    else if (k == KF5 && ds.n > 0) {
      DauSach *d = ds.nodes[chon];
      VePanelDS("THEM CUON");
      AnConTro(0);
      gotoxy(74, 5); InCell(d->tenSach, 32);
      gotoxy(74, 7);  int soCuon = NhapSo("Them bao nhieu cuon: ");
      char viTri[50];
      gotoxy(74, 9);  NhapTen("Vi tri (ke): ", viTri, 16);
      ThemCuonVao(d, soCuon, viTri);
      sprintf(trangthai, "Da them %d cuon vao '%s'.", soCuon, d->tenSach);
      veLai = 1;
    }
    else if (k == KF6 && ds.n > 0) {
      VePanelDS("THANH LY CUON");
      AnConTro(0);
      char ma[MAXMA];
      gotoxy(74, 6); NhapMa("Ma cuon: ", ma, MAXMA);
      int idx; NodeSach *p = TimCuon(ds, ma, idx);
      if (!p) { BaoLoi("Khong tim thay ma cuon nay"); veLai = 1; trangthai[0] = 0; continue; }
      if (p->data.trangThai == 1) { BaoLoi("Cuon nay DANG MUON, khong thanh ly duoc"); veLai = 1; continue; }
      if (p->data.trangThai == 2) { BaoLoi("Cuon nay da thanh ly roi"); veLai = 1; continue; }
      gotoxy(74, 8);
      if (XacNhan("Chac chan thanh ly? (Y/N): ")) {
        p->data.trangThai = 2;
        sprintf(trangthai, "Da thanh ly cuon %s.", ma);
      } else strcpy(trangthai, "Da huy.");
      veLai = 1;
    }
    else if ((k == KF3 || k == KDEL) && ds.n > 0) {
      DauSach *d = ds.nodes[chon];
      if (CoCuonDangMuon(d)) {
        BaoLoi("Co cuon DANG MUON, khong the xoa dau sach nay");
        continue;
      }
      VePanelDS("XOA DAU SACH");
      AnConTro(0);
      gotoxy(74, 6); InCell(d->tenSach, 32);
      gotoxy(74, 8);
      if (XacNhan("Chac chan xoa? (Y/N): ")) {
        PushDS(undo, U_XOA, *d);
        delete ds.nodes[chon];
        GoKhoiMang(ds, chon);
        strcpy(trangthai, "Da xoa dau sach.");
      } else strcpy(trangthai, "Da huy.");
      veLai = 1;
    }
    else if (k == KF2 && ds.n > 0) {
      DauSach *d = ds.nodes[chon];
      PushDS(undo, U_SUA, *d);
      VePanelDS("SUA DAU SACH");
      AnConTro(0);
      gotoxy(74, 5); printf("ISBN %s (KHOA, khong sua)", d->ISBN);
      gotoxy(74, 6); printf("Cu: "); InCell(d->tenSach, 28);
      GoKhoiMang(ds, chon);
      gotoxy(74, 8);  NhapTen("Ten moi: ", d->tenSach, 34);
      gotoxy(74, 10); d->soTrang = NhapSo("So trang: ");
      gotoxy(74, 12); NhapTen("Tac gia: ", d->tacGia, 20);
      gotoxy(74, 14); d->namXuatBan = NhapSo("Nam XB: ");
      gotoxy(74, 16); NhapTen("The loai: ", d->theLoai, 12);
      ChenDauSach(ds, d);
      sprintf(trangthai, "Da sua dau sach %s.", d->ISBN);
      veLai = 1;
    }
  }
}

// ============ MAN HINH DOC GIA (TUI) ============

static void VePanelDG(const char *tieude) {
  VeKhung(70, 3, 108, 17);
  InMau(72, 4, tieude, 14);
}

static void InDongDG(DocGia *dg, int stt, int y, int chonFlag) {
  gotoxy(4, y);
  if (chonFlag) SetBGColor(1);
  printf("%-4d%-8d", stt + 1, dg->maThe);
  InCell(dg->ho, 20); putchar(' ');
  InCell(dg->ten, 12);
  printf(" %-5s%-9s%-4d", dg->phai,
         dg->trangThai ? "hoatdong" : "KHOA", DemDangMuon(dg));
  if (chonFlag) SetBGColor(0);
}

void ManHinhDocGia(PTRDG &root, ActDG *&undo) {
  int kieuSap = 1;                        // 1 = theo ma the, 2 = theo ten+ho (CAU b)
  int chon = 0;                           // con tro dong
  char trangthai[100] = "";
  DocGia *arr[2000]; int n = 0;
  int veLai = 1;
  while (true) {
    int trang, tongtrang;
    if (veLai) {
      n = DoCayRaMang(root, arr, 2000);   // LNR -> co san thu tu MA tang
      if (kieuSap == 2) {
        for (int i = 0; i < n - 1; i++)
          for (int j = 0; j < n - 1 - i; j++) {
            int kq = stricmp(arr[j]->ten, arr[j+1]->ten);
            if (kq > 0 || (kq == 0 && stricmp(arr[j]->ho, arr[j+1]->ho) > 0)) {
              DocGia *t = arr[j]; arr[j] = arr[j+1]; arr[j+1] = t;
            }
          }
      }
      if (chon >= n) chon = (n > 0) ? n - 1 : 0;
      trang = (n > 0) ? chon / 10 : 0;
      tongtrang = (n + 9) / 10; if (tongtrang < 1) tongtrang = 1;
      system("cls");
      AnConTro(1);
      InMau(24, 1, "QUAN LY DOC GIA", 14);
      VeKhung(2, 2, 68, 16);
      gotoxy(4, 3); SetColor(11);
      printf("%-4s%-8s", "STT", "Ma the"); InCell("Ho", 20); putchar(' ');
      InCell("Ten", 12); printf(" %-5s%-9s%-4s", "Phai", "The", "Muon");
      SetColor(15);
      VeNgang(2, 68, 4);
      int dau = trang * 10, cuoi = dau + 10; if (cuoi > n) cuoi = n;
      for (int i = dau; i < cuoi; i++)
        InDongDG(arr[i], i, 5 + i - dau, i == chon);
      if (n == 0) InMau(4, 5, "(danh sach rong - bam F1 de them)", 12);
      gotoxy(4, 15);
      printf("Trang %d/%d - Tong %d - sap theo %s   ", trang + 1, tongtrang, n,
             (kieuSap == 1) ? "MA THE" : "TEN+HO");
      InMau(2, 18, "[F1] Them   [F2] Sua   [F3] Xoa   [F4] Undo   [F5] Doi sap xep", 11);
      InMau(2, 19, "[LEN/XUONG] con tro  [TRAI/PHAI] trang  [ESC] ve menu", 11);
      if (trangthai[0]) InMau(2, 21, trangthai, 10);
      veLai = 0;
    }
    trang = (n > 0) ? chon / 10 : 0;
    tongtrang = (n + 9) / 10; if (tongtrang < 1) tongtrang = 1;
    int k = DocPhim();
    if (k == KESC) { AnConTro(0); return; }
    else if (k == KUP && chon > 0) {
      int cu = chon; chon--;
      if (chon / 10 != trang) veLai = 1;
      else {
        InDongDG(arr[cu], cu, 5 + cu % 10, 0);
        InDongDG(arr[chon], chon, 5 + chon % 10, 1);
      }
    }
    else if (k == KDOWN && chon < n - 1) {
      int cu = chon; chon++;
      if (chon / 10 != trang) veLai = 1;
      else {
        InDongDG(arr[cu], cu, 5 + cu % 10, 0);
        InDongDG(arr[chon], chon, 5 + chon % 10, 1);
      }
    }
    else if (k == KLEFT && chon - 10 >= 0) { chon -= 10; veLai = 1; }
    else if (k == KRIGHT && chon + 10 < n) { chon += 10; veLai = 1; }
    else if (k == KF5) { kieuSap = (kieuSap == 1) ? 2 : 1; trangthai[0] = 0; veLai = 1; }
    else if (k == KF4) {
      UndoDG_Exec(root, undo);
      strcpy(trangthai, "(vua Undo)");
      veLai = 1;
    }
    else if (k == KF1) {
      VePanelDG("THEM DOC GIA");
      AnConTro(0);
      DocGia dg;
      dg.maThe = SinhMaThe(root);
      gotoxy(72, 6);  printf("Ma the tu cap: %d", dg.maThe);
      gotoxy(72, 8);  NhapChuoi("Ho : ", dg.ho, 24);
      gotoxy(72, 10); NhapChuoi("Ten (tu cuoi, max 10): ", dg.ten, 11);
      gotoxy(72, 12); NhapPhai(dg.phai);
      dg.trangThai = 1;
      dg.dsMuonTra = NULL;
      ChenDG(root, dg);
      PushDG(undo, U_THEM, dg);
      sprintf(trangthai, "Da them the %d (%s %s).", dg.maThe, dg.ho, dg.ten);
      veLai = 1;
    }
    else if ((k == KF3 || k == KDEL) && n > 0) {
      DocGia *dg = arr[chon];
      if (DemDangMuon(dg) > 0) {
        BaoLoi("Doc gia dang muon sach, KHONG the xoa");
        continue;
      }
      VePanelDG("XOA DOC GIA");
      AnConTro(0);
      gotoxy(72, 6); printf("The : %d", dg->maThe);
      gotoxy(72, 7); printf("Ten : "); InCell(dg->ho, 15); putchar(' '); InCell(dg->ten, 10);
      gotoxy(72, 9);
      if (XacNhan("Chac chan xoa? (Y/N): ")) {
        int ma = dg->maThe;
        PushDG(undo, U_XOA, *dg);
        XoaNodeDG(root, ma);
        sprintf(trangthai, "Da xoa the %d.", ma);
      } else strcpy(trangthai, "Da huy.");
      veLai = 1;
    }
    else if (k == KF2 && n > 0) {
      DocGia *dg = arr[chon];
      NodeDG *nd = TimDG(root, dg->maThe);
      PushDG(undo, U_SUA, nd->data);
      VePanelDG("SUA DOC GIA");
      AnConTro(0);
      gotoxy(72, 5);  printf("The %d (ma la KHOA, khong sua)", nd->data.maThe);
      gotoxy(72, 6);  printf("Cu: "); InCell(nd->data.ho, 15); putchar(' '); InCell(nd->data.ten, 10);
      gotoxy(72, 8);  NhapChuoi("Ho moi : ", nd->data.ho, 24);
      gotoxy(72, 10); NhapChuoi("Ten moi (max 10): ", nd->data.ten, 11);
      gotoxy(72, 12); NhapPhai(nd->data.phai);
      gotoxy(72, 14); printf("The (1=hoat dong, 0=khoa): ");
      char t; do { t = getch(); } while (t != '0' && t != '1');
      nd->data.trangThai = t - '0'; putchar(t);
      sprintf(trangthai, "Da sua the %d.", nd->data.maThe);
      veLai = 1;
    }
  }
}

// ---- man hinh QUAN LY MUON TRA: menu hop con (cau f, g, h) ----
void ManHinhMuonTra(PTRDG &treeDG, DS_DauSach &ds) {
  const char *items[] = { "1. Muon sach                 (cau f)",
                          "2. Tra sach / bao mat        (cau g)",
                          "3. Sach doc gia X dang muon  (cau h)" };
  while (true) {
    int c = MenuHop("QUAN LY MUON TRA", items, 3);
    if (c < 0) return;
    if (c == 0) CauF_MuonSach(treeDG, ds);
    else if (c == 1) CauG_TraSach(treeDG, ds);
    else if (c == 2) CauH_DangMuon(treeDG, ds);
  }
}

// ---- man hinh BAO CAO: menu hop con (cau i, j) ----
void ManHinhBaoCao(PTRDG &treeDG, DS_DauSach &ds) {
  const char *items[] = { "1. Doc gia muon qua han (giam dan)   (cau i)",
                          "2. Top 10 sach muon nhieu nhat       (cau j)" };
  while (true) {
    int c = MenuHop("BAO CAO", items, 2);
    if (c < 0) return;
    if (c == 0) CauI_QuaHan(treeDG);
    else if (c == 1) CauJ_Top10(ds);
  }
}

// ---- MENU CHINH CHUONG TRINH ----
void MenuChinh(DS_DauSach &ds, PTRDG &treeDG) {
  ActDG *undoDG = NULL;
  ActDS *undoDS = NULL;
  const char *fSach = "THUVIEN.TXT", *fDG = "DOCGIA.TXT";

  const char *menu[] = { "1. Quan ly DOC GIA    (them/xoa/sua/undo - cau a,b)",
                         "2. Quan ly DAU SACH   (them/xoa/sua/undo - cau c,d,e)",
                         "3. Quan ly MUON TRA   (muon/tra - cau f,g,h)",
                         "4. BAO CAO            (qua han, top 10 - cau i,j)",
                         "5. Thoat chuong trinh (tu dong luu file)" };
  while (true) {
    int c = MenuHop("CHUONG TRINH QUAN LY THU VIEN - NHOM 7", menu, 5);
    if (c == -2) {                        // F2 = luu nhanh ngay tai menu
      if (SaveAll(ds, treeDG, fSach, fDG)) BaoLoi("Da luu tat ca vao file");
      else BaoLoi("Loi khi ghi file");
      continue;
    }
    if (c == 0) ManHinhDocGia(treeDG, undoDG);
    else if (c == 1) ManHinhDauSach(ds, undoDS);
    else if (c == 2) ManHinhMuonTra(treeDG, ds);
    else if (c == 3) ManHinhBaoCao(treeDG, ds);
    else if (c == 4 || c == -1) {         // Thoat (chon muc 5 hoac ESC)
      int luuOK = SaveAll(ds, treeDG, fSach, fDG);   // luu 1 luot khi thoat
      system("cls");                      // don man hinh cho loi thoat sach se
      AnConTro(0);                        // tra lai con tro cho console
      SetColor(10);
      if (luuOK) printf("Da luu toan bo du lieu vao file.\n");
      else       printf("CANH BAO: luu file that bai!\n");
      SetColor(15);
      printf("Cam on da su dung chuong trinh. Tam biet!\n\n");
      return;
    }
  }
}

// ============================================================
// docgia.cpp — KHOI DOC GIA (N phu trach) - Than ham logic
// ============================================================
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "docgia.h"
#include "mylib.h"
#include "menu.h"

using namespace std;

void ChenDG(PTRDG &root, DocGia x) {
  NodeDG *p = new NodeDG; p->data = x; p->left = p->right = NULL;
  if (root == NULL) { root = p; return; }
  NodeDG *q = root;
  while (true) {
    if (x.maThe < q->data.maThe) { if (!q->left)  { q->left  = p; return; } q = q->left; }
    else                         { if (!q->right) { q->right = p; return; } q = q->right; }
  }
}

NodeDG* TimDG(PTRDG root, int maThe) {
  while (root && root->data.maThe != maThe)
    root = (maThe < root->data.maThe) ? root->left : root->right;
  return root;
}

// xoa node khoi BST (khong de quy, du 3 truong hop: la / 1 con / 2 con)
int XoaNodeDG(PTRDG &root, int maThe) {
  NodeDG *cha = NULL, *p = root;
  while (p && p->data.maThe != maThe) {
    cha = p;
    p = (maThe < p->data.maThe) ? p->left : p->right;
  }
  if (!p) return 0;
  if (p->left && p->right) {              // 2 con: the mang = trai nhat cay phai
    NodeDG *chaTM = p, *tm = p->right;
    while (tm->left) { chaTM = tm; tm = tm->left; }
    p->data = tm->data;                   // keo du lieu the mang len
    p = tm; cha = chaTM;                  // chuyen sang xoa node the mang
  }
  NodeDG *con = p->left ? p->left : p->right;
  if (!cha) root = con;
  else if (cha->left == p) cha->left = con;
  else cha->right = con;
  delete p;
  return 1;
}

// duyet LNR KHONG de quy bang stack tu quan ly -> mang con tro (thu tu MA tang)
int DoCayRaMang(PTRDG root, DocGia *arr[], int mx) {
  NodeDG *stk[2000]; int top = -1, n = 0;
  NodeDG *p = root;
  while (p || top >= 0) {
    while (p) { stk[++top] = p; p = p->left; }
    p = stk[top--];
    if (n < mx) arr[n++] = &p->data;
    p = p->right;
  }
  return n;
}

// dem so cuon DANG MUON (trangThai==0 trong MUONTRA) cua 1 doc gia
int DemDangMuon(DocGia *dg) {
  int dem = 0;
  for (PTRMT p = dg->dsMuonTra; p; p = p->next)
    if (p->data.trangThai == 0) dem++;
  return dem;
}

// so ngay qua han LON NHAT trong cac cuon dang muon (<=0 neu khong qua han)
int QuaHanMax(DocGia *dg) {
  int mx = -9999;
  for (PTRMT p = dg->dsMuonTra; p; p = p->next)
    if (p->data.trangThai == 0 && NgayQuaHan(p->data.ngayMuon) > mx)
      mx = NgayQuaHan(p->data.ngayMuon);
  return mx;
}

void XoaHetMT(PTRMT &First) {
  while (First) { PTRMT p = First; First = First->next; delete p; }
}

// giai phong toan bo cay (duyet NLR bang stack, khong de quy)
void GiaiPhongCay(PTRDG &root) {
  if (!root) return;
  NodeDG *stk[2000]; int top = -1;
  stk[++top] = root;
  while (top >= 0) {
    NodeDG *p = stk[top--];
    if (p->right) stk[++top] = p->right;
    if (p->left)  stk[++top] = p->left;
    XoaHetMT(p->data.dsMuonTra);
    delete p;
  }
  root = NULL;
}

// ------------------ UNDO KHOI DOC GIA (stack LIFO tu cai dat) ------------------

void PushDG(ActDG *&Top, int loai, DocGia d) {
  ActDG *p = new ActDG; p->loai = loai; p->data = d; p->next = Top; Top = p;
}

void ClearUndoDG(ActDG *&Top) {
  while (Top) {
    ActDG *p = Top; Top = Top->next;
    if (p->loai == U_XOA) XoaHetMT(p->data.dsMuonTra);
    delete p;
  }
}

void UndoDG_Exec(PTRDG &root, ActDG *&Top) {
  if (!Top) { BaoLoi("Khong con thao tac nao de Undo"); return; }
  ActDG *p = Top; Top = Top->next;
  char tb[100];
  if (p->loai == U_THEM) {                // undo THEM = xoa the vua them
    NodeDG *nd = TimDG(root, p->data.maThe);
    if (nd && nd->data.dsMuonTra != NULL)
      BaoLoi("The nay da phat sinh muon tra, khong the Undo them");
    else {
      XoaNodeDG(root, p->data.maThe);
      sprintf(tb, "Undo: da go bo the %d (vua them)", p->data.maThe);
      BaoLoi(tb);
    }
  }
  else if (p->loai == U_XOA) {            // undo XOA = chen lai (kem lich su MT)
    ChenDG(root, p->data);
    sprintf(tb, "Undo: da phuc hoi the %d (vua xoa)", p->data.maThe);
    BaoLoi(tb);
  }
  else {                                  // undo SUA = tra lai thong tin cu
    NodeDG *nd = TimDG(root, p->data.maThe);
    if (nd) {
      strcpy(nd->data.ho, p->data.ho);
      strcpy(nd->data.ten, p->data.ten);
      strcpy(nd->data.phai, p->data.phai);
      nd->data.trangThai = p->data.trangThai;   // giu nguyen dsMuonTra hien tai
      sprintf(tb, "Undo: da tra lai thong tin cu cua the %d", p->data.maThe);
      BaoLoi(tb);
    }
  }
  delete p;
}

int SinhMaThe(PTRDG root) {               // ma NGAU NHIEN 4 chu so, khong trung
  int ma;
  do { ma = 1000 + rand() % 9000; } while (TimDG(root, ma) != NULL);
  return ma;
}

// ------------------ CAU i — DOC GIA MUON QUA HAN (TUI, giam dan) ------------------

void CauI_QuaHan(PTRDG root) {
  DocGia *arr[2000];
  int n = DoCayRaMang(root, arr, 2000);
  DocGia *qh[2000]; int songay[2000]; int m = 0;
  for (int i = 0; i < n; i++) {
    int q = QuaHanMax(arr[i]);
    if (q > 0) { qh[m] = arr[i]; songay[m] = q; m++; }
  }
  for (int i = 0; i < m - 1; i++)         // selection sort giam dan
    for (int j = i + 1; j < m; j++)
      if (songay[j] > songay[i]) {
        int t = songay[i]; songay[i] = songay[j]; songay[j] = t;
        DocGia *td = qh[i]; qh[i] = qh[j]; qh[j] = td;
      }
  int trang = 0, tongtrang = (m + 9) / 10; if (tongtrang < 1) tongtrang = 1;
  while (true) {
    system("cls");
    AnConTro(1);
    InMau(8, 1, "BAO CAO: DOC GIA MUON SACH QUA HAN (giam dan theo so ngay)", 14);
    VeKhung(2, 2, 68, 16);
    gotoxy(4, 3); SetColor(11);
    printf("%-8s", "Ma the"); InCell("Ho", 22); putchar(' ');
    InCell("Ten", 12); printf(" %s", "Qua han");
    SetColor(15);
    VeNgang(2, 68, 4);
    int dau = trang * 10, cuoi = dau + 10; if (cuoi > m) cuoi = m;
    for (int i = dau; i < cuoi; i++) {
      gotoxy(4, 5 + i - dau);
      printf("%-8d", qh[i]->maThe); InCell(qh[i]->ho, 22); putchar(' ');
      InCell(qh[i]->ten, 12);
      SetColor(12); printf(" %3d ngay", songay[i]); SetColor(15);
    }
    if (m == 0) InMau(4, 5, "(khong co doc gia nao qua han)", 10);
    gotoxy(4, 15);
    printf("Trang %d/%d - Tong %d doc gia qua han", trang + 1, tongtrang, m);
    InMau(2, 18, "[TRAI/PHAI] chuyen trang   [phim khac] quay lai", 11);
    int k = DocPhim();
    if (k == KLEFT && trang > 0) trang--;
    else if (k == KRIGHT && trang < tongtrang - 1) trang++;
    else { AnConTro(0); return; }
  }
}

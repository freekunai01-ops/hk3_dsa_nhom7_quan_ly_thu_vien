// ============================================================
// common.h — Cau truc du lieu dung chung
// Do an De 3: Quan ly Thu vien — Nhom 7
// ============================================================
#ifndef COMMON_H
#define COMMON_H

#include <cstring>

const int MAX_DAUSACH = 1000;
const int HAN_MUON_NGAY = 7;       // han muon (ngay)
const int MAXMA = 20;              // du cho ISBN + '-001'

// ---------- SACH ----------

struct Sach {                      // 1 cuon sach cu the
    char maSach[MAXMA];            // maSach = ISBN + '-001'
    int trangThai = 0;             // 0 = cho muon duoc, 1 = dang muon, 2 = thanh ly
    char viTri[50];                // ke, phong...
};

struct NodeSach {                  // danh muc sach: danh sach lien ket don
    Sach data;
    NodeSach *next = nullptr;
};
typedef NodeSach *PTRSACH;

struct DauSach {
    char ISBN[15];
    char tenSach[100];
    int soTrang;
    char tacGia[60];
    int namXuatBan;
    char theLoai[40];
    int soLuotMuon = 0;            // dem luot muon o DAU SACH
    PTRSACH FirstDMS = nullptr;    // tro toi danh muc cac cuon cua dau sach nay
};

struct DS_DauSach {                // mang con tro, luon tang dan theo TEN SACH
    int n = 0;
    DauSach *nodes[MAX_DAUSACH];
};

// ---------- MUON TRA ----------

struct Date {
    int dd, mm, yyyy;
};

struct MuonTra {                   // 1 luot muon sach
    char maSach[MAXMA];
    Date ngayMuon;
    Date ngayTra;
    int trangThai = 0;             // 0 = dang muon, 1 = da tra, 2 = lam mat
};

struct NodeMT {                    // danh sach lien ket don
    MuonTra data;
    NodeMT *next = nullptr;
};
typedef NodeMT *PTRMT;

// ---------- DOC GIA ----------

struct DocGia {
    int maThe;                     // so nguyen tu dong, khong trung
    char ho[50];                   // ho = tat ca cac tu TRU tu cuoi cung
    char ten[11];                  // ten = tu cuoi cung, toi da 10 ky tu
    char phai[5];                  // "Nam" hoac "Nu"
    int trangThai = 1;             // 0 = the bi khoa, 1 = hoat dong
    PTRMT dsMuonTra = nullptr;     // cac cuon da va dang muon
};

struct NodeDG {                    // cay nhi phan tim kiem, khoa = maThe
    DocGia data;
    NodeDG *left = nullptr;
    NodeDG *right = nullptr;
};
typedef NodeDG *PTRDG;

// ---------- UNDO ----------
const int U_THEM = 1, U_XOA = 2, U_SUA = 3;   // loai hanh dong trong stack undo

struct ActDG { 
    int loai; 
    DocGia data; 
    ActDG *next = nullptr; 
};

struct ActDS { 
    int loai; 
    DauSach data; 
    ActDS *next = nullptr; 
};

#endif

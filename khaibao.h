
#ifndef KHAIBAO_H
#define KHAIBAO_H

#include <iostream>
#include <cstring>

const int MAX_MON_HOC = 300;
const int MAX_LOP = 10000;

const int MAX_MAMH = 15;   
const int MAX_MALOP = 15;
const int MAX_MASV = 15;
const int MAX_TEN = 50;    
const int MAX_PASS = 20;


struct DiemThi {
    char maMH[MAX_MAMH];
    float diem;
    int lanThi;
    DiemThi* next = NULL;
};


struct SinhVien {
    char maSV[MAX_MASV];
    char ho[MAX_TEN];
    char ten[MAX_TEN];   
    bool phai; // true: Nam, false: Nu   
    char password[MAX_PASS];
    DiemThi* headDiem = NULL;
    SinhVien* next = NULL;
};

struct Lop {
    char maLop[MAX_MALOP];
    char tenLop[MAX_TEN];
    SinhVien* headSV = NULL; 
};

struct MonHoc {
    char maMH[MAX_MAMH];
    char tenMH[MAX_TEN];
    int soTinChi;
};

struct CauHoi {
    char id[25];               
    char maMH[MAX_MAMH];
    char noiDung[500];
    char a[200], b[200], c[200], d[200];
    char dapAn;          
    
    CauHoi* left = NULL;  
    CauHoi* right = NULL; 
};

struct DanhSachLop {
    Lop* nodes[MAX_LOP];
    int soLuong = 0;
};

struct DanhSachMonHoc {
    MonHoc data[MAX_MON_HOC]; 
    int soLuong = 0;
};

typedef CauHoi* PTR_CAUHOI; 


#endif
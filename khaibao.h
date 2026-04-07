#ifndef KHAIBAO_H
#define KHAIBAO_H

#include <iostream>
#include <cstring>

using namespace std;

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
    DiemThi* next = NULL;
};


struct SinhVien {
    char maSV[MAX_MASV];
    char ho[MAX_TEN];
    char ten[MAX_TEN];
    char phai[5];        
    char password[MAX_PASS];
    DiemThi* headDiem = NULL;
    SinhVien* next = NULL;
};

struct Lop {
    char maLop[MAX_MALOP];
    char tenLop[50];
    SinhVien* headSV = NULL; 
};

struct MonHoc {
    char maMH[MAX_MAMH];
    char tenMH[50];
};

struct CauHoi {
    int id;               
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
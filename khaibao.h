
#ifndef KHAIBAO_H
#define KHAIBAO_H

#include <iostream>
#include <cstring>
#include <string>

const int MAX_MON_HOC = 300;
const int MAX_LOP = 10000;

const int MAX_MAMH = 16;   
const int MAX_MALOP = 16;
const int MAX_TEN_LOP = 36;
const int MAX_MASV = 16;
const int MAX_HO = 24;
const int MAX_TEN_SV = 8;    
const int MAX_PASS = 21;
const int MAX_TENMH = 36;
const int MAX_CAU_HOI_MOI_MON = 2000;   // ngan hang cau hoi toi da cho 1 mon
<<<<<<< HEAD
const int MAX_CAU_THI          = 200;   // so cau toi da trong 1 de thi
=======
const int MAX_CAU_THI = 200;   // so cau toi da trong 1 de thi
>>>>>>> c2d8f9310c9f88c9481b200127ced131b1703c3a

struct DiemThi {
    char maMH[MAX_MAMH];
    float diem;
    DiemThi* next = NULL;
};

struct SinhVien {
    char maSV[MAX_MASV];
    char ho[MAX_HO];
    char ten[MAX_TEN_SV];   
    bool phai; // true: Nam, false: Nu   
    char password[MAX_PASS];
    DiemThi* headDiem = NULL;
    SinhVien* next = NULL;
};

struct Lop {
    char maLop[MAX_MALOP];
    char tenLop[MAX_TEN_LOP];
    SinhVien* headSV = NULL; 
};

struct MonHoc {
    char maMH[MAX_MAMH];
    char tenMH[MAX_TENMH];
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

struct TrangChiTiet {
    std::string noiDungCau[30];
    int soDong;
    TrangChiTiet() {
    	soDong = 0;
	}
};

typedef CauHoi* PTR_CAUHOI;

#endif
#ifndef FLOW_H
#define FLOW_H

#include "khaibao.h"
#include "saveload.h"
#include "hamchinh.h"
#include "dohoa.h"
#include "ErrorCodes.h"
#include "hamtienich.h"
#include "flowGV.h"
#include "flowSV.h"

int flowDangNhap(DanhSachLop &dsLop, SinhVien* &svLogined);

void flowGiaoVien(DanhSachLop &dsLop, DanhSachMonHoc &dsMH, PTR_CAUHOI &root);
void flowQuanLyLop_SinhVien(DanhSachLop &dsLop);
void flowQuanLySinhVienCuaLop(DanhSachLop &dsLop, Lop* lopHienTai);
void flowQuanLyMonHoc_CauHoi(DanhSachMonHoc &dsMH, PTR_CAUHOI &root);
void flowQuanLyCauHoiCuaMon(const char* maMH,const char* tenMH ,PTR_CAUHOI &root);
void flowQuanLyDiemSV(DanhSachMonHoc &dsMH, DanhSachLop &dsLop);

void flowSinhVien(DanhSachLop &dsLop, DanhSachMonHoc &dsMH, PTR_CAUHOI &root, SinhVien* sv);
void flowXemThongTinSinhVien(SinhVien* sv);
void flowXemDiemSinhVien(SinhVien* sv, DanhSachMonHoc &dsMH);
void flowThiTracNghiem(SinhVien* sv, DanhSachMonHoc &dsMH, PTR_CAUHOI &root, DanhSachLop &dsLop);
void flowXemLaiBaiLam(const char* maSV, const char* maMH);

void flowChinh(DanhSachLop &dsLop, DanhSachMonHoc &dsMH, PTR_CAUHOI &root) {
	CapNhatSTT(root);
    while (true) {
        SinhVien* svHienTai = NULL;
        int role = flowDangNhap(dsLop, svHienTai); 
        
        if (role == 0) break;
        if (role == 1) {
            flowGiaoVien(dsLop, dsMH, root);
        }
        else if (role == 2) {
        	flowSinhVien(dsLop, dsMH, root, svHienTai);
        }
    }
}

int flowDangNhap(DanhSachLop &dsLop, SinhVien* &svLogined) {
    while (true) {
        system("cls");
    	cout << "TAB: hien mat khau";
    	
        SetColor(0, 7);
        gotoxy(51, 10); cout << "DANG NHAP HE THONG";
        VeKhung(37, 11, 46, 8);
        
        gotoxy(40, 13); cout << "Tai khoan:                          ";
        gotoxy(40, 16); cout << "Mat khau :                          ";

        gotoxy(52, 13);
        string user = NhapMa(15);

        if (user == "") {
            gotoxy(38, 26); SetColor(0, 14); cout << "Ban co chac muon thoat khong?";
            char xn = XacNhan(70, 26);
            gotoxy(38, 26); cout << "                                                    ";
            if (xn == 'Y') {
                return 0;  
            }
            continue;
        }

        SinhVien* svHienTai = NULL;
        int role = kiemTraTaiKhoan(dsLop, user, svHienTai);

        if (role != 1 && role != 2) {
            InThongBaoContent(layThongBaoLoi(role), 12, 43, 18); 
            _getch();
            continue;
        }

        gotoxy(52, 16);
        string pass = NhapMatKhau(52, 16, 47, 18); 
        int passCheck = xacThucMatKhau(role, pass, svHienTai);

        if (passCheck == SUCCESS) {
            if (role == 2 && strcmp(svHienTai->password, "123456") == 0) {
                changePass(dsLop, svHienTai);
            }
            svLogined = svHienTai;
            return role; 
        }
        else {
            InThongBaoContent(layThongBaoLoi(passCheck), 12, 45, 18);
            _getch();
            continue;
        }
    }
}	

#endif
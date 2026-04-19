#ifndef HAMTIENICH_H
#define HAMTIENICH_H

#include <iostream>
#include <string>
#include <conio.h>
#include <cctype>
#include <cstring>
#include "khaibao.h"
#include "hamchinh.h"
#include "dohoa.h"

using namespace std;


// Ham nhap lieu
int NhapSo() {
    string strNum = "";
    char ch;
    while (true) {
        ch = _getch(); 
        
        if (isdigit(ch)) { 
            strNum += ch;
            cout << ch;
        } 
        else if (ch == 8 && strNum.length() > 0) { 
            strNum.pop_back(); 
            cout << "\b \b";   
        } 
        else if (ch == 13 && strNum.length() > 0) {
            break;
        }
    }
    cout << endl;
    return stoi(strNum); 
}

string NhapChuoiChu() {
    string str = "";
    char ch;
    while (true) {
        ch = _getch();
        
        if (isalpha(ch) || ch == ' ') { 
            str += ch;
            cout << ch;
        } 
        else if (ch == 8 && str.length() > 0) {
            str.pop_back();
            cout << "\b \b";
        } 
        else if (ch == 13 && str.length() > 0) {
            break;
        }
    }
    cout << endl;
    return str;
}

string NhapMa(int maxLength) {
    string ma = "";
    char ch;
    while (true) {
        ch = _getch();

        if (ch == 13) { 
            break; 
        } 

        if (ch == 27) {
            return ""; 
        }

        if (ch == 8 && ma.length() > 0) {
            ma.pop_back();
            cout << "\b \b";
        } 
        else if ((isalnum(ch) || ch == '-' || ch == '_') && ma.length() < maxLength) {
            ch = toupper(ch); 
            ma += ch;
            cout << ch;
        } 
    }
    return ma;
}

string NhapTen() {
    string str = "";
    char ch;
    while (true) {
        ch = _getch();
        
        if (isprint(ch)) { 
            str += ch;
            cout << ch;
        } 
        else if (ch == 8 && str.length() > 0) {
            str.pop_back();
            cout << "\b \b";
        } 
        else if (ch == 13 && str.length() > 0) {
            break;
        }
    }
    return str;
}

// Ham tien ich

//Ham khoi tao
void Init(DanhSachMonHoc &dsMH, DanhSachLop &dsLop, PTR_CAUHOI &root) { 
    
    dsMH.soLuong = 0;
    
    dsLop.soLuong = 0;
    for (int i = 0; i < MAX_LOP; i++) {
        dsLop.nodes[i] = NULL; 
    }
    
    root = NULL;
}

bool isCapsLockOn() {
    return (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
}

void InCanhBaoCaps(int x, int y) {
    if ((GetKeyState(VK_CAPITAL) & 0x0001) != 0) {
        SetColor(0, 14); 
        gotoxy(x, y); cout << "CANH BAO: CapsLock dang bat!  ";
    } else {
        gotoxy(x, y); cout << "                             "; 
    }
    SetColor(0, 7); 
}
// Ham dang nhap
string NhapMatKhau() {
    string pass = "";
    char ch;
    int x = 52, y = 16;
    while (true) {
        // Cập nhật cảnh báo CapsLock tại dòng 18, cột 40
        InCanhBaoCaps(40, 18);

        gotoxy(x + pass.length(), y);
        ch = _getch();
        if (ch == 13) break;          // Enter
        else if (ch == 8) {           // Backspace
            if (!pass.empty()) pass.pop_back();
            gotoxy(x, y); cout << string(pass.length() + 1, ' ');
            gotoxy(x, y); for (char c : pass) cout << '*';
        } else if (ch == 9) {         // Tab – có thể hiện tạm mật khẩu
            // tuỳ chọn
        } else {
            pass.push_back(ch);
            cout << '*';
        }
    }
    return pass;
}
int kiemTraTaiKhoan(DanhSachLop &dsLop, string user, SinhVien* &svFound) {
    if (user == "") return ERR_LOGIN_EMPTY;
    if (user == "GV") return 1;

    for (int i = 0; i < dsLop.soLuong; i++) {
        SinhVien* p = dsLop.nodes[i]->headSV;
        while (p != NULL) {
            if (user == p->maSV) {
                svFound = p; 
                return 2;    
            }
            p = p->next;
        }
    }
    return ERR_LOGIN_USER;
}

int xacThucMatKhau(int role, string pass, SinhVien* svFound) {
    if (pass == "") return ERR_LOGIN_EMPTY;
    
    if (role == 1) { 
        return (pass == "GV") ? SUCCESS : ERR_LOGIN_PASS;
    }
    if (role == 2 && svFound != NULL) { 
        return (pass == svFound->password) ? SUCCESS : ERR_LOGIN_PASS;
    }
    return ERR_LOGIN_PASS;
}

void nhapLop(DanhSachLop &dsLop) {
	string ma_nhap = NhapMa(50);
	string ten_nhap = NhapTen();
	const char* ma = ma_nhap.c_str();
	const char* ten = ten_nhap.c_str();
	
	//themLop(dsLop, (char*)ma, (char*)ten);
}



int MenuDong(char options[][50], int soLuong, int toaDoX, int toaDoY) {
    int cursor = 0;
    while (true) {
        for (int i = 0; i < soLuong; i++) {
            gotoxy(toaDoX, toaDoY + (i * 2)); // i * 2 để các menu cách nhau 1 dòng cho thoáng
            if (i == cursor) {
                SetColor(11, 0); 
                cout << " > " << options[i];
                // In thêm khoảng trắng để lấp đầy dòng màu nền
                for (int j = strlen(options[i]); j < 20; j++) cout << " "; 
            } else {
                SetColor(0, 7);
                cout << "   " << options[i];
                for (int j = strlen(options[i]); j < 20; j++) cout << " ";
            }
        }
        
        char ch = _getch();
        if (ch == -32 || ch == 224) { 
            ch = _getch();
            if (ch == 72 && cursor > 0) cursor--; 
            if (ch == 80 && cursor < soLuong - 1) cursor++; 
        }
        else if (ch == 13) {
        	SetColor(0, 7);
        	return cursor;
		} 
        else if (ch == 27) {
        	SetColor(0, 7);
        	return -1; 
		}
    }
}

// Hàm quét sạch vùng ruột của khung Content để tải nội dung mới - Nguoi ngoai hanh tinh
void XoaVungContent() {
    for (int i = 6; i <= 27; i++) {
        gotoxy(33, i);
        // In 83 khoảng trắng để xóa dòng cũ
        cout << "                                                                                   "; 
    }
}

// Cập nhật lại hàm InThongBao để nằm gọn trong vùng Content
void InThongBaoContent(string thongBao, int maMau, int x, int y) {
    SetColor(0, maMau);
    gotoxy(x, y);
    cout << thongBao;
    SetColor(0, 7);
}

string layThongBaoLoi(int errorCode) {
    switch (errorCode) {
        case SUCCESS: return "Thanh cong!";
        
        // Nhóm 100: Lớp
        case ERR_LOP_FULL: return "Loi 101: Bo nho danh sach lop da day!";
        case ERR_LOP_DUPLICATE: return "Loi 102: Ma lop nay da ton tai!";
        case ERR_LOP_NOT_FOUND: return "Loi 103: Khong tim thay ma lop!";
        case ERR_LOP_EMPTY: return "Loi 104: Danh sach lop dang trong!";
        
        // Nhóm 200: Sinh viên
        case ERR_SV_DUPLICATE: return "Loi 201: Ma sinh vien bi trung!";
        case ERR_SV_NOT_FOUND: return "Loi 202: Khong tim thay sinh vien!";
        case ERR_SV_FULL: return "Loi 203: Khong the them sinh vien vao luc nay!";
        
        // Nhóm 300: Đăng nhập
        case ERR_LOGIN_USER: return "Loi 301: Tai khoan khong ton tai!";
        case ERR_LOGIN_PASS: return "Loi 302: Mat khau khong chinh xac!";
        case ERR_LOGIN_EMPTY: return "Loi 303: Vui long nhap day du thong tin!";
        
        default: return "Loi khong xac dinh!";
    }
}

int KiemTraInDanhSachLop(DanhSachLop dsLop) {
	if (dsLop.soLuong == 0) return ERR_LOP_EMPTY;
	
	return SUCCESS;
}
#endif
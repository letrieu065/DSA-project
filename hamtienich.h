#ifndef HAMTIENICH_H
#define HAMTIENICH_H

#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <cctype>
#include <cstring>
#include <sstream>
#include "khaibao.h"
#include "hamchinh.h"
#include "dohoa.h"
#include "saveload.h"
#include "ErrorCodes.h"


using namespace std;

void ShowCur(bool CursorVisibility);
char XacNhan(int x, int y);
int InChuoiGioiHan(string s, int x, int y, int maxCols);
void InNoiDungCanLe(string noiDung, int X, int Y, int maxW);

// Ham nhap lieu
int NhapSo(int suggest = -1) {
    string strNum = "";
    
    if (suggest != -1) {
        strNum = to_string(suggest);
        cout << strNum;
    }

    char ch;
    while (true) {
        ch = _getch();

        if (isdigit(ch)) {
            if (strNum.length() < 2) { 
                strNum += ch;
                cout << ch;
            }
        }
        else if (ch == 8) {
            if (!strNum.empty()) {
                strNum.pop_back();
                cout << "\b \b";
            }
        }
        else if (ch == 13) {
            if (!strNum.empty())
                break;
        }
        else if (ch == 27) {
            return -1; 
        }
        else if (ch == 0 || ch == -32 || ch == 224) {
            _getch();
        }
    }

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
		if (ch == 0 || ch == -32 || ch == 224) {
            getch();
            continue; 
        }
        if (ch == 27) {
        	str = "";
        	break;
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
		    if (ma.length() > 0) { 
		        break; 
		    } else {
		        continue; 
		    }
		}
		
		if (ch == 0 || ch == -32 || ch == 224) {
            getch(); 
            continue; 
        } 
        if (ch == 27) {
        	ma = "";
        	break;
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

string NhapTen(string suggest = "") {
    string str = suggest; 
    cout << str; 
    
    char ch;
    while (true) {
        ch = _getch();
        
        if (ch == 0 || ch == -32 || ch == 224) {
            _getch(); continue;
        }

        if (isalpha(ch) || ch == ' ') {
            
            if (ch == ' ') {
                if (str.length() == 0 || str.back() == ' ') continue;
            }
            
            if (str.length() < 50) {
                if (isalpha(ch)) {
                    if (str.length() == 0 || str.back() == ' ') {
                        ch = toupper(ch); 
                    } else {
                        ch = tolower(ch); 
                    }
                }

                str += ch;
                cout << ch;
            }
        }
        else if (ch == 27) return ""; 
        else if (ch == 8 && str.length() > 0) { 
            str.pop_back();
            cout << "\b \b";
        }
        else if (ch == 13) { 
            while (!str.empty() && str.back() == ' ') {
                str.pop_back();
                cout << "\b \b";
            }
            if (!str.empty()) break; 
        }
    }
    return str;
}

bool NhapGioiTinh(int x, int y, bool defaultPhai = true) {
    bool phai = defaultPhai;
    char ch;
    while (true) {
        gotoxy(x, y);
        SetColor(0, 14); 
        cout << (phai ? "Nam" : "Nu "); 
        SetColor(0, 7);

        ch = _getch();
        if (ch == 13) return phai; 
        if (ch == 27) return defaultPhai; 
        
        if (ch == -32 || ch == 0 || ch == ' ') { 
            if (ch == -32 || ch == 0) _getch(); 
            phai = !phai; 
        }
    }
}

char ChonDapAn(int x, int y) {
    char dsDapAn[] = {'A', 'B', 'C', 'D'};
    int index = 0;
    char ch;

    ShowCur(false); 

    while (true) {
        for (int i = 0; i < 4; i++) {
            gotoxy(x + (i * 6), y); 
            
            if (i == index) {
                SetColor(0, 11); 
                cout << "[" << dsDapAn[i] << "]";
            } else {
                SetColor(0, 7); 
                cout << " " << dsDapAn[i] << " "; 
            }
        }
        SetColor(0, 7);

        ch = _getch();

        if (ch == 13) { 
            ShowCur(true); 
            return dsDapAn[index];
        }
        
        if (ch == 27) {
            
            char xn = XacNhan(80, 26);
            
            gotoxy(50, 26); cout << "                                                    ";
            gotoxy(80, 26); cout << "              ";
            
            if (xn == 'Y') return '\0'; 
            else {
                ShowCur(false);
                continue;
            }
        }

        if (ch == -32 || ch == 224) { 
            ch = _getch();
            if (ch == 75) { //mũi tên trái
                index = (index > 0) ? index - 1 : 3; 
            }
            else if (ch == 77) { //mũi tên phải
                index = (index < 3) ? index + 1 : 0;
            }
        }
    }
}

// Ham nhap nhieu dong co gioi han vung (maxCols: so ky tu tren 1 dong, maxRows: so dong toi da)
string xuongdongGioiHan(int X, int Y, int maxCols, int maxRows, int maxChars, char* noiDungCu = NULL) {
    int curX = X, curY = Y, len = 0;
    int posX[500], posY[500];
    string result = "";
    gotoxy(curX, curY);
    
	string s="";
	if (noiDungCu != NULL) s = noiDungCu;
	
    while (true) {
        char ch = _getch();
        
        if (ch == 27) {

            int savedX = curX;
            int savedY = curY;
            
            char xn = XacNhan(80, 26);
         
            gotoxy(50, 26); cout << "                                                    ";
            gotoxy(80, 26); cout << "              ";
            
            if (xn == 'Y') return ""; 
            else {
                ShowCur(true);
                gotoxy(savedX, savedY);
                continue;
            }
        }
        if (ch == '\r') break; 

        if (ch == 8) { // phím backspace
            if (len > 0) {
                len--;
                curX = posX[len]; curY = posY[len];
                gotoxy(curX, curY); cout << " "; 
				gotoxy(curX, curY);
                result.pop_back();
            }
            continue;
        }

        if (ch < 32 || (unsigned char)ch > 126) continue;
        if (len >= maxChars) continue;

        if (curX >= X + maxCols) {
            
            if (curY >= Y + maxRows - 1) continue;

            int wordStart = len - 1;
            while (wordStart > 0 && result[wordStart - 1] != ' ') wordStart--;

            if (wordStart > 0 && result[wordStart - 1] == ' ') {
                for (int i = wordStart; i < len; i++) {
                    gotoxy(posX[i], posY[i]); cout << " ";
                }
                curY++; curX = X;
                for (int i = wordStart; i < len; i++) {
                    posX[i] = curX; posY[i] = curY;
                    gotoxy(curX, curY); cout << result[i];
                    curX++;
                }
            } else {
                curY++; curX = X;
            }
        }

        posX[len] = curX; posY[len] = curY;
        gotoxy(curX, curY); cout << ch;
        result += ch; len++; curX++;
    }
    return result;
    return s;
}

void InNoiDungCanLe(string noiDung, int X, int Y, int maxW) {
    int curX = X, curY = Y;
    string word;
    stringstream ss(noiDung);

    gotoxy(X, Y);
    while (ss >> word) {
       
        if (curX + word.length() > X + maxW) {
            curY++;        
            curX = X;       
            gotoxy(curX, curY);
        }
        cout << word << " ";
        curX += word.length() + 1;
    }
}

int InChuoiGioiHan(string s, int x, int y, int maxCols) {
    int curX = x, curY = y;
    int startY = y;

    for (int i = 0; i < s.length(); i++) {
        if (curX >= x + maxCols) {
            curX = x;
            curY++;
        }

        gotoxy(curX, curY);
        cout << s[i];
        curX++;
    }

    return curY - startY + 1; 
}

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

string NhapMatKhau(int x, int y, int capsX, int capsY) { 
    string pass = "";
    char ch;
    bool isHidden = true; 

    bool lastCaps = isCapsLockOn();
    InCanhBaoCaps(capsX, capsY); 

    while (true) {
        bool currentCaps = isCapsLockOn();
        if (currentCaps != lastCaps) {
            InCanhBaoCaps(capsX, capsY);
            lastCaps = currentCaps;
        }

        gotoxy(x + pass.length(), y);

        if (_kbhit()) {
            ch = _getch();

            if (ch == 13) { 
                if (!pass.empty()) break;
            }
            else if (ch == 27) { 
                return "";
            }
            else if (ch == 9) { 
                isHidden = !isHidden; 
                
                gotoxy(x, y);
                for (int i = 0; i < pass.length(); i++) {
                    cout << (isHidden ? '*' : pass[i]);
                }
            }
            else if (ch == 8) { 
                if (!pass.empty()) {
                    pass.pop_back();
                    int pos = x + pass.length();
                    gotoxy(pos, y);
                    cout << ' ';
                    gotoxy(pos, y);
                }
            }
            else if (ch >= 32 && ch <= 126 && pass.length() < 20) {
                pass.push_back(ch);
                cout << (isHidden ? '*' : ch);
            }
        }
        Sleep(20);
    }

    gotoxy(capsX, capsY);
    cout << "                                           ";

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

int MenuDong(char options[][50], int soLuong, int toaDoX, int toaDoY) {
    ShowCur(false); 
    int cursor = 0;
    int oldCursor = 0;

    for (int i = 0; i < soLuong; i++) {
        gotoxy(toaDoX, toaDoY + (i * 2)); 
        if (i == cursor) {
            SetColor(11, 0); 
            cout << " > " << options[i];
            for (int j = strlen(options[i]); j < 20; j++) cout << " "; 
        } else {
            SetColor(0, 7);
            cout << "   " << options[i];
            for (int j = strlen(options[i]); j < 20; j++) cout << " ";
        }
    }
    
    while (true) {
        char ch = _getch();
        if (ch == -32 || ch == 224) { 
            ch = _getch();
            oldCursor = cursor;

            if (ch == 72 && cursor > 0) cursor--; 
            if (ch == 80 && cursor < soLuong - 1) cursor++; 

            if (oldCursor != cursor) {
                gotoxy(toaDoX, toaDoY + (oldCursor * 2));
                SetColor(0, 7);
                cout << "   " << options[oldCursor];
                for (int j = strlen(options[oldCursor]); j < 20; j++) cout << " ";

                gotoxy(toaDoX, toaDoY + (cursor * 2));
                SetColor(11, 0);
                cout << " > " << options[cursor];
                for (int j = strlen(options[cursor]); j < 20; j++) cout << " ";
            }
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

void XoaVungContent() {
    SetColor(0, 7);
    for (int i = 6; i <= 27; i++) {
        gotoxy(33, i);
        cout << "                                                                                   "; 
    }
}

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
        case ERR_LOP_DUPLICATE_NAME: return "Loi 106: Ten lop nay da ton tai!";
        
        // Nhóm 200: Sinh viên
        case ERR_SV_DUPLICATE: return "Loi 201: Ma sinh vien bi trung!";
        case ERR_SV_NOT_FOUND: return "Loi 202: Khong tim thay sinh vien!";
        case ERR_SV_FULL: return "Loi 203: Khong the them sinh vien vao luc nay!";
        
        // Nhóm 300: Đăng nhập
        case ERR_LOGIN_USER: return "Loi 301: Tai khoan khong ton tai!";
        case ERR_LOGIN_PASS: return "Loi 302: Mat khau khong chinh xac!";
        case ERR_LOGIN_EMPTY: return "Loi 303: Vui long nhap day du thong tin!";
        case ERR_LOGIN_PASS_DUPLICATE: return "Loi 304: Mat khau trung mat khau cu!";
        case ERR_LOGIN_PASS_NOT_MATCH: return "Loi 305: Mat khau moi khong khop";
        case ERR_LOGIN_PASS_WEAK: return "Loi 306: Mat khau yeu";
        
        default: return "Loi khong xac dinh!";
    }
}

int KiemTraInDanhSachLop(DanhSachLop dsLop) {
	if (dsLop.soLuong == 0) return ERR_LOP_EMPTY;
	
	return SUCCESS;
}

// Ham in danh sach sinh vien
void inDanhSachSinhVien(Lop* lop, int x, int y) {
    if (lop == NULL) return;

    SinhVien* p = lop->headSV;
    int dong = y;

    VeKhung(x-2, y-1, 70, demSoSV(lop) + 3);

    gotoxy(x, y);      cout << "STT";
    gotoxy(x+5, y);    cout << "Ma SV";
    gotoxy(x+20, y);   cout << "Ho";
    gotoxy(x+35, y);   cout << "Ten";
    gotoxy(x+50, y);   cout << "Phai";

    dong++;

    int stt = 1;

    while (p != NULL) {
        gotoxy(x, dong);     cout << stt++;
        gotoxy(x+5, dong);   cout << p->maSV;
        gotoxy(x+20, dong);  cout << p->ho;
        gotoxy(x+35, dong);  cout << p->ten;
        gotoxy(x+50, dong);  
		cout << (p->phai == 1 ? "Nam" : "Nu");

        dong++;
        p = p->next;
    }
}

void ShowCur(bool CursorVisibility) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursor = { 1, CursorVisibility };
    SetConsoleCursorInfo(handle, &cursor);
}

char XacNhan(int x, int y) {
    const char* option[] = {"YES", "NO"};
    int index = 0;
    int numOptions = 2;
    char ch;

    ShowCur(false);
    gotoxy(50, 26);
    SetColor(0, 14);
    cout << "Ban co chac muon thoat khong?";

    while (true) {
        for (int i = 0; i < numOptions; i++) {
            gotoxy(x + (i * 6), y);

            if (i == index) {
                SetColor(0, 11);
                cout << "[" << option[i] << "]";
            } else {
                SetColor(0, 7);
                cout << " " << option[i] << " ";
            }
        }
        SetColor(0, 7);
        ch = _getch();

        if (ch == 13) { // Enter
            ShowCur(true);
            return option[index][0]; // trả về 'Y' hoặc 'N'
        }

        if (ch == 27) { // ESC
            ShowCur(true);
            return ' ';
        }

        if (ch == -32 || ch == (char)224) { // phím mũi tên
            ch = _getch();
            if (ch == 75) { // mũi tên trái
                index = (index > 0) ? index - 1 : numOptions - 1;
            } else if (ch == 77) { // mũi tên phải
                index = (index < numOptions - 1) ? index + 1 : 0;
            }
        }
    }
    SetColor(0, 7);
}

bool XacNhanMenu(int x, int y, string thongBao) {
    int choice = 0;
    while (true) {
        gotoxy(x, y); 
        SetColor(0, 12); cout << thongBao;
        
        gotoxy(x + thongBao.length() + 2, y);
        if (choice == 0) SetColor(11, 0); else SetColor(0, 7);
        cout << " CO ";

        gotoxy(x + thongBao.length() + 8, y);
        if (choice == 1) SetColor(11, 0); else SetColor(0, 7);
        cout << " KHONG ";

        SetColor(0, 7);

        char ch = _getch();
        if (ch == 224 || ch == -32) {
            ch = _getch();
            if (ch == 75 || ch == 77) choice = 1 - choice;
        }
        else if (ch == 13) return (choice == 0);
        else if (ch == 27) return false; 
    }
}

void changePass(DanhSachLop &dsLop, SinhVien* svHienTai) {
    bool isChanged = false; 
    while (!isChanged) {
        XoaVungContent();
        VeGiaoDienDoiMatKhau();
        
        gotoxy(30, 23); SetColor(0, 8); 
        cout << "TAB: An/Hien mat khau  |  ESC: Huy thao tac va Quay lai";
        SetColor(0, 7);

        string pass_old = NhapMatKhau(54, 13, 47, 20);
        if (pass_old == "") return; 

        if (pass_old != string(svHienTai->password)) {
            InThongBaoContent("Mat khau cu khong chinh xac!", 12, 45, 20);
            _getch(); continue; 
        }

        string pass_new = NhapMatKhau(54, 15, 47, 20);
        if (pass_new == "") return;

        if (pass_new == string(svHienTai->password)) {
            InThongBaoContent("Mat khau moi khong duoc giong mat khau cu!", 12, 40, 20);
            _getch(); continue;
        }
        
        if (pass_new.length() < 3) { 
            InThongBaoContent(layThongBaoLoi(ERR_LOGIN_PASS_WEAK), 12, 40, 20);
            _getch(); continue;
        }

        string pass_new_check = NhapMatKhau(54, 17, 47, 20);
        if (pass_new_check == "") return;

        if (pass_new != pass_new_check) {
            InThongBaoContent(layThongBaoLoi(ERR_LOGIN_PASS_NOT_MATCH), 12, 45, 20);
            _getch(); continue;
        }

        strcpy(svHienTai->password, pass_new.c_str());
        SaveLop(dsLop); 
        
        InThongBaoContent("Doi mat khau thanh cong!", 10, 42, 20);
        Sleep(1200); 
        isChanged = true;
    }
}

void TienHanhThiTracNghiem(SinhVien* sv, MonHoc monChon, CauHoi* deThi[], int soCau, int thoiGianPhut) {
    ShowCur(false);
    
    char dapAnDaChon[1000];
    for (int i = 0; i < soCau; i++) {
        dapAnDaChon[i] = ' '; 
    }

    int cauHienTai = 0;
    
    int tongThoiGianGiay = thoiGianPhut * 60;
    time_t batDau = time(0);
    int thoiGianConLai = tongThoiGianGiay;
    int thoiGianDaIn = -1; 

    VeKhungPhongThi(sv, monChon.tenMH, soCau);
    HienThiCauHoiThi(deThi[cauHienTai], cauHienTai + 1, soCau, dapAnDaChon[cauHienTai]);

    while (true) {
        
        time_t hienTai = time(0);
        thoiGianConLai = tongThoiGianGiay - (hienTai - batDau);

        if (thoiGianConLai <= 0) { 
            thoiGianConLai = 0;
            InDongHo(thoiGianConLai);
            Sleep(1000); 
            break; 
        }

        if (thoiGianConLai != thoiGianDaIn) {
            InDongHo(thoiGianConLai);
            thoiGianDaIn = thoiGianConLai;
        }

        if (_kbhit()) {
            int ch = _getch();

            if (ch == 224 || ch == 0 || ch == -32) {
                ch = _getch();
                if (ch == 75 && cauHienTai > 0) { 
                    cauHienTai--;
                    HienThiCauHoiThi(deThi[cauHienTai], cauHienTai + 1, soCau, dapAnDaChon[cauHienTai]);
                } 
                else if (ch == 77 && cauHienTai < soCau - 1) { 
                    cauHienTai++;
                    HienThiCauHoiThi(deThi[cauHienTai], cauHienTai + 1, soCau, dapAnDaChon[cauHienTai]);
                }
            }
            else if (ch == 'a' || ch == 'A') {
                dapAnDaChon[cauHienTai] = 'A';
                HienThiCauHoiThi(deThi[cauHienTai], cauHienTai + 1, soCau, dapAnDaChon[cauHienTai]);
            }
            else if (ch == 'b' || ch == 'B') {
                dapAnDaChon[cauHienTai] = 'B';
                HienThiCauHoiThi(deThi[cauHienTai], cauHienTai + 1, soCau, dapAnDaChon[cauHienTai]);
            }
            else if (ch == 'c' || ch == 'C') {
                dapAnDaChon[cauHienTai] = 'C';
                HienThiCauHoiThi(deThi[cauHienTai], cauHienTai + 1, soCau, dapAnDaChon[cauHienTai]);
            }
            else if (ch == 'd' || ch == 'D') {
                dapAnDaChon[cauHienTai] = 'D';
                HienThiCauHoiThi(deThi[cauHienTai], cauHienTai + 1, soCau, dapAnDaChon[cauHienTai]);
            }
            else if (ch == 27) { 
                break;
            }
        }

        Sleep(50); 
    }

    system("cls"); 
    
    int soCauDung = 0;
    for (int i = 0; i < soCau; i++) {
        if (dapAnDaChon[i] == deThi[i]->dapAn) {
            soCauDung++;
        }
    }
    
    float diemThi = (float)soCauDung / soCau * 10.0f;
    
    SetColor(0, 10);
    gotoxy(40, 10); cout << "DA NOP BAI THANH CONG!";
    gotoxy(40, 12); cout << "So cau dung: " << soCauDung << " / " << soCau;
    gotoxy(40, 14); cout << "DIEM CUA BAN: " << diemThi;
    
    // TODO: Ghi kết quả diemThi vào Danh sách liên kết điểm của SinhVien
    // TODO: Ghi đáp án đã chọn ra file .txt để xem lại chi tiết (Yêu cầu H)
    
    _getch();
}
void InGiuaKhung(string text, int startX, int toaDoY, int doRongKhung) {
    int textLength = text.length();
    
    // Nếu chữ dài hơn cả khung thì in từ đầu khung luôn để tránh lỗi
    if (textLength >= doRongKhung) {
        gotoxy(startX, toaDoY);
        cout << text;
        return;
    }
    
    // Tính toán tọa độ X chuẩn xác để căn giữa
    int xCenter = startX + (doRongKhung - textLength) / 2;
    
    gotoxy(xCenter, toaDoY);
    cout << text;
}

void XoaTitle() {
    SetColor(0, 7);
    gotoxy(33, 2); cout << "                                                                                   ";
    SetColor(0, 7);
}

void XoaSmallTitle() {
    SetColor(0, 7);
    gotoxy(33, 3); cout << "                                                                                   ";
    SetColor(0, 7);
}

void InTitle(string title) {
    SetColor(0, 11);
    InGiuaKhung(title, 32, 2, 85);
    SetColor(0, 7);
}

void InSmallTitle(string title) {
    SetColor(0, 11);
    InGiuaKhung(title, 32, 3, 85);
    SetColor(0, 7);
}

#endif
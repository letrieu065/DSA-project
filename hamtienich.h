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


using namespace std;

void ShowCur(bool CursorVisibility);
char XacNhan(int x, int y);
int InChuoiGioiHan(string s, int x, int y, int maxCols);

// Ham nhap lieu//
int NhapSo() {
    string strNum = "";
    char ch;

    while (true) {
        ch = _getch();

        if (isdigit(ch)) {
            strNum += ch;
            cout << ch;
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

string NhapTen() {
    string str = "";
    char ch;
    while (true) {
        ch = _getch();
        
        if (isprint(ch)) { 
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
    return str;
}
bool NhapGioiTinh(int x, int y) {
    bool gioiTinh = true; 
    int ch;
    
    while (true) {
        gotoxy(x, y);
        if (gioiTinh) {
            SetColor(0, 10); cout << "  NAM  ";
            SetColor(0, 7);  cout << " / Nu ";
        } else {
            SetColor(0, 7);  cout << " Nam / ";
            SetColor(0, 13); cout << "  NU   ";
            SetColor(0, 7);
        }

        ch = _getch(); 

        if (ch == 224 || ch == 0) {
            ch = _getch(); 
            
            if (ch == 75 || ch == 77) {
                gioiTinh = !gioiTinh;
            }
        } 
        else if (ch == 9) {
            gioiTinh = !gioiTinh;
        }
        else if (ch == 13) {
            return gioiTinh;
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

// Ham dang nhap
string NhapMatKhau() {
    string pass = "";
    char ch;
    int x = 52, y = 16;

    bool lastCaps = isCapsLockOn();
    InCanhBaoCaps(47, 18); 

    while (true) {
        bool currentCaps = isCapsLockOn();
        if (currentCaps != lastCaps) {
            InCanhBaoCaps(47, 18);
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
                cout << '*';
            }
        }

        Sleep(20);
    }

    gotoxy(40, 18);
    cout << "                                          ";

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
    int cursor = 0;
    while (true) {
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

void XoaVungContent() {
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

#endif
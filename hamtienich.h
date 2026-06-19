#ifndef HAMTIENICH_H
#define HAMTIENICH_H

#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <cctype>
#include <ctime>
#include <cstring>
#include <sstream>
#include "khaibao.h"
#include "hamchinh.h"
#include "hamve.h"
#include "saveload.h"
#include "ErrorCodes.h"
#include "dohoa.h"

//// Trong hamtienich.h

using namespace std;

void ShowCur(bool CursorVisibility);
char XacNhan(int x, int y);
int InChuoiGioiHan(string s, int x, int y, int maxCols);
void InNoiDungCanLe(string noiDung, int X, int Y, int maxW);
void XoaVungContent();
void InThongBaoContent(string thongBao, int maMau, int x, int y);
void XoaThongBao(int x, int y, int length);
string NhapMa(int maxLength);
void XoaTitle();
void XoaSmallTitle();
int NhapSo(int suggest, int maxLength, int minVal, int maxVal);
string NhapChuoiChu();
string NhapTen(string suggest);
string NhapCoDieuHuong(int X, int Y, int maxCols, int maxRows, string noiDungCu, int &keyLog);
void InThongBaoLoiDangNhap(string error);

// Tim kiem
Lop* TimKiemLopThongMinh(DanhSachLop &dsLop);
SinhVien* TimKiemSinhVienTrongLopThongMinh(Lop* lop);          
MonHoc*   TimKiemMonHocThongMinh(DanhSachMonHoc &dsMH);         

// Ham nhap lieu
int NhapSo(int suggest = -1, int maxLength = 6, int minVal = 1, int maxVal = 999999) {
    string strNum = "";

    if (suggest != -1) {
        strNum = to_string(suggest);
        cout << strNum;
    }

    while (true) {
        int ch = _getch();

        if (isdigit(ch)) {
            if ((int)strNum.length() < maxLength) {
                if (strNum.empty() && ch == '0') continue;
                strNum += (char)ch;
                cout << (char)ch;
            }
        }
        else if (ch == 8) {  
            if (!strNum.empty()) {
                strNum.pop_back();
                cout << "\b \b";
            }
        }
        else if (ch == 13) { 
            if (strNum.empty()) continue;

            int val = stoi(strNum);

            if (val < minVal || val > maxVal) {
                for (int i = 0; i < (int)strNum.length(); i++) cout << "\b \b";
                strNum = "";
                continue;
            }

            return val;
        }
        else if (ch == 27) {  
            return INT_MIN;  
        }
        else if (ch == 0 || ch == (char)-32 || ch == 224) {
            _getch();  
        }
    }
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
    int startX, startY;
    getXY(startX, startY); 

    int indexX = str.length(); 
    cout << str; 

    char ch;
    while (true) {
        gotoxy(startX + indexX, startY);
        ch = _getch();

        if (ch == -32 || ch == 224) {
            ch = _getch();
            if (ch == 75 && indexX > 0) indexX--;
            else if (ch == 77 && indexX < str.length()) indexX++; 
            continue;
        }

        if (ch == 8 && indexX > 0) {
            str.erase(indexX - 1, 1);
            indexX--;
            gotoxy(startX, startY);
            cout << str << "  "; 
            continue;
        }
        if (ch == 27) return ""; 
        if (ch == 13) {
            while (!str.empty() && str.back() == ' ') str.pop_back();
            if (!str.empty()) break;
            else continue;
        }
        if (isalpha(ch) || ch == ' ') {
            if (ch == ' ') {
                if (indexX == 0 || str[indexX - 1] == ' ') continue;
            }
            
            if (str.length() < 50) {
                if (isalpha(ch)) {
                    if (indexX == 0 || str[indexX - 1] == ' ') ch = toupper(ch);
                    else ch = tolower(ch);
                }

                string s(1, ch);
                str.insert(indexX, s);
                indexX++;

                gotoxy(startX, startY);
                cout << str;
            }
        }
    }
    return str;
}

string NhapCoDieuHuong(int X, int Y, int maxCols, int maxRows, string noiDungCu, int &keyLog) {
    ShowCur(true);
    string s = noiDungCu;

    gotoxy(X, Y);
    cout << s;
    
    while (true) {
        gotoxy(X + s.length(), Y);
        int ch = _getch();
        if (ch == 224 || ch == 0) {
            ch = _getch();
            keyLog = ch;
            return s;
        }
        else if (ch == 13 || ch == 27) { 
            keyLog = ch;
            return s;
        }
        else if (ch == 8 && s.length() > 0) { 
            s.pop_back();
            gotoxy(X + s.length(), Y);
            cout << " ";
            gotoxy(X + s.length(), Y);
        }
        else if (ch >= 32 && ch <= 126 && s.length() < maxCols) { 
            s += (char)ch;
            cout << (char)ch;
        }
    }
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

char ChonDapAn(int x, int y, int* keyLog = NULL) {
    char dsDapAn[] = {'A', 'B', 'C', 'D'};
    int index = 0;
 
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
 
        int ch = _getch();
 
        if (ch == 13) {
            ShowCur(true);
            if (keyLog != NULL) *keyLog = 13;
            return dsDapAn[index];
        }
 
        if (ch == 27) {
            if (keyLog != NULL) {
                ShowCur(true);
                *keyLog = 27;
                return dsDapAn[index];
            }
            // Khong co keyLog: tu xu ly (hanh vi cu)
            gotoxy(50, 26); SetColor(0, 14); cout << "Ban co chac muon thoat khong?";
            char xn = XacNhan(82, 26);
            gotoxy(50, 26); cout << "                                                    ";
            gotoxy(80, 26); cout << "              ";
            if (xn == 'Y') return '\0';
            else { ShowCur(false); continue; }
        }
 
        if (ch == -32 || ch == 224) {
            int ext = _getch();
            if (ext == 75) {                               // mui ten trai
                index = (index > 0) ? index - 1 : 3;
            }
            else if (ext == 77) {                          // mui ten phai
                index = (index < 3) ? index + 1 : 0;
            }
            else if ((ext == 72 || ext == 80) && keyLog != NULL) { // Up/Down
                ShowCur(true);
                *keyLog = ext;
                return dsDapAn[index];
            }
        }
    }
}


static bool xuongDong(string& result, int len, int* posX, int* posY, int X, int Y, int maxCols, int maxRows, int& curX, int& curY) {
    if (curY >= Y + maxRows - 1) return false;

    // Tim dau tu gan nhat de wrap
    int wordStart = len;
    while (wordStart > 0 && result[wordStart - 1] != ' ')
        wordStart--;

    if (wordStart < len) {
        // Xoa phan tu tren dong cu
        for (int i = wordStart; i < len; i++) {
            gotoxy(posX[i], posY[i]); cout << ' ';
        }
        curY++; curX = X;
        // Ve lai tren dong moi
        for (int i = wordStart; i < len; i++) {
            posX[i] = curX; posY[i] = curY;
            gotoxy(curX, curY); cout << result[i];
            curX++;
        }
    } else {
        curY++; curX = X;
    }
    return true;
}

string xuongdongGioiHan(int X, int Y, int maxCols, int maxRows, int maxChars, char* noiDungCu = NULL, int* keyLog = NULL) {
    int curX = X, curY = Y, len = 0;
    int posX[230], posY[230];
    string result;

    // --- Pre-load noi dung cu ---
    if (noiDungCu != NULL && noiDungCu[0] != '\0') {
        for (int si = 0; noiDungCu[si] != '\0' && len < maxChars; si++) {
            char c = noiDungCu[si];
            if (c < 32 || c > 126) continue;               // bo ky tu dac biet
            if (len == 0 && c == ' ') continue;             // bo space dau
            if (c == ' ' && !result.empty() && result.back() == ' ') continue; // bo double-space

            if (curX >= X + maxCols)
                if (!xuongDong(result, len, posX, posY, X, Y, maxCols, maxRows, curX, curY)) break;

            posX[len] = curX; posY[len] = curY;
            gotoxy(curX, curY); cout << c;
            result += c; len++; curX++;
        }
    }

    gotoxy(curX, curY); 
    while (true) {
        int ch = _getch();

        // Phim dieu huong (mui ten, INS, DEL...)
        if (ch == 0 || ch == 224) {
            int ext = _getch();
            if (keyLog != NULL && (ext == 72 || ext == 80)) {
                *keyLog = ext;
                return result;
            }
            continue;
        }

        // ESC
        if (ch == 27) {
            if (keyLog != NULL) { *keyLog = 27; return result; }
            gotoxy(50, 26); SetColor(0, 14); cout << "Ban co chac muon thoat khong?";
            char xn = XacNhan(82, 26);
            gotoxy(50, 26); cout << "                                                                ";
            if (xn == 'Y') return "";
            ShowCur(true); gotoxy(curX, curY);
            continue;
        }
        // Enter
		if (ch == '\r') {
		    // Xoa space o cuoi chuoi neu co
		    while (!result.empty() && result.back() == ' ') {
		        len--;
		        curX = posX[len]; curY = posY[len];
		        gotoxy(curX, curY); cout << ' ';
		        result.pop_back();
		    }
		    if (keyLog != NULL) { *keyLog = 13; return result; }
		    break;
		}
        // Backspace
        if (ch == 8) {
            if (len > 0) {
                len--;
                curX = posX[len]; curY = posY[len];
                gotoxy(curX, curY); cout << ' ';
                gotoxy(curX, curY);
                result.pop_back();
            }
            continue;
        }
        // Ky tu in duoc
        if (ch < 32 || ch > 126) continue;
        if (len >= maxChars) continue;
        if (len == 0 && ch == ' ') continue;                         
        if (ch == ' ' && !result.empty() && result.back() == ' ') continue; 
        if (curX >= X + maxCols)
            if (!xuongDong(result, len, posX, posY, X, Y, maxCols, maxRows, curX, curY)) continue;

        posX[len] = curX; posY[len] = curY;
        gotoxy(curX, curY); cout << (char)ch;
        result += (char)ch; len++; curX++;
    }
    return result;
}



inline string ToLower(string s) {
    for (int i = 0; i < s.length(); i++) {
        s[i] = tolower((unsigned char)s[i]);
    }
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
    int soXoa = 82 - capsX;
    for (int i = 0; i < soXoa; i++) cout << " ";   

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
    XoaTitle();
    XoaSmallTitle();
    SetColor(0, 7);
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
        
        // Nhom 100: Lop
        case ERR_LOP_FULL: return "Loi 101: Bo nho danh sach lop da day!";
        case ERR_LOP_DUPLICATE: return "Loi 102: Ma lop nay da ton tai!";
        case ERR_LOP_NOT_FOUND: return "Loi 103: Khong tim thay ma lop!";
        case ERR_LOP_EMPTY: return "Loi 104: Danh sach lop dang trong!";
        case ERR_LOP_DUPLICATE_NAME: return "Loi 106: Ten lop nay da ton tai!";
        
        // Nhom 200: Sinh vien
        case ERR_SV_DUPLICATE: return "Loi 201: Ma sinh vien bi trung!";
        case ERR_SV_NOT_FOUND: return "Loi 202: Khong tim thay sinh vien!";
        case ERR_SV_FULL: return "Loi 203: Khong the them sinh vien vao luc nay!";
        
        // Nhom 300: Dang nhap
        case ERR_LOGIN_USER: return "Loi 301: Tai khoan khong ton tai!";
        case ERR_LOGIN_PASS: return "Loi 302: Mat khau khong dung!";
        case ERR_LOGIN_EMPTY: return "Loi 303: Vui long nhap day du thong tin!";
        case ERR_LOGIN_PASS_DUPLICATE: return "Loi 304: Mat khau trung mat khau cu!";
        case ERR_LOGIN_PASS_NOT_MATCH: return "Loi 305: Mat khau moi khong khop";
        case ERR_LOGIN_PASS_WEAK: return "Loi 306: Mat khau yeu";

        // Nhom 400: Mon hoc
        case ERR_MH_FULL: return "";
        case ERR_MH_DUPLICATE_CODE: return "";
        case ERR_MH_NOT_FOUND: return "";
        case ERR_MH_HAS_QUESTION: return "";
        case ERR_MH_HAS_POINT: return "";
        case ERR_MH_INVALID_DATA: return "";
        case ERR_MH_DUPLICATE_NAME: return "";
        
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
    for (int i = 0; i < soCau; i++) dapAnDaChon[i] = ' ';

    int cauHienTai = 0;
    int tongThoiGianGiay = thoiGianPhut * 60;
    time_t batDau = time(0);
    int thoiGianConLai = tongThoiGianGiay;
    int thoiGianDaIn = -1;

    VeKhungPhongThi(sv, monChon.tenMH, soCau);

    HienThiCauHoiThi(deThi[cauHienTai], cauHienTai + 1, soCau, dapAnDaChon[cauHienTai]);
    VeMiniMap(dapAnDaChon, soCau, cauHienTai);
    CapNhatCounter(dapAnDaChon, soCau);

    while (true) {
        time_t hienTai = time(0);
        thoiGianConLai = tongThoiGianGiay - (int)(hienTai - batDau);

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

        if (!_kbhit()) { Sleep(50); continue; }

        int ch = _getch();

        // Chuyen cau
        if (ch == 224 || ch == 0 || ch == -32) {
            ch = _getch();
            bool changed = false;
            if (ch == 75 && cauHienTai > 0)           { cauHienTai--; changed = true; }
            else if (ch == 77 && cauHienTai < soCau-1){ cauHienTai++; changed = true; }

            if (changed) {
                HienThiCauHoiThi(deThi[cauHienTai], cauHienTai + 1, soCau, dapAnDaChon[cauHienTai]);
                VeMiniMap(dapAnDaChon, soCau, cauHienTai);  
            }
        }
        // Chon dap an
        else if (ch=='a'||ch=='A'||ch=='b'||ch=='B'||ch=='c'||ch=='C'||ch=='d'||ch=='D') {
            dapAnDaChon[cauHienTai] = toupper(ch);
            HienThiCauHoiThi(deThi[cauHienTai], cauHienTai + 1, soCau, dapAnDaChon[cauHienTai]);
            VeMiniMap(dapAnDaChon, soCau, cauHienTai);       
            CapNhatCounter(dapAnDaChon, soCau);              
        }
        // Nop bai
        else if (ch == 27) {
            int chuaLam[MAX_CAU_THI], soChuaLam = 0;
            for (int i = 0; i < soCau; i++)
                if (dapAnDaChon[i] == ' ') chuaLam[soChuaLam++] = i + 1;

            int px = 20, py = 10, pw = 76, ph = 6;

            SetColor(0, 0);
            for (int r = py; r <= py + ph; r++) {
                gotoxy(px, r);
                for (int c = 0; c <= pw; c++) cout << " ";
            }

            if (soChuaLam > 0) {
                SetColor(0, 12); VeKhung(px, py, pw, ph);

                SetColor(0, 15);
                gotoxy(px+2, py+1);
                cout << "Ban con " << soChuaLam << " cau CHUA tra loi: ";

                SetColor(0, 15);
                gotoxy(px+2, py+4); cout << "Nop bai luon?";
                gotoxy(px+18, py+4);
                SetColor(2, 15);  cout << "  [Y] Co  ";
                SetColor(4, 15);  cout << "  [N] Quay lai  ";
                SetColor(0, 7);

                int confirm = _getch();
                if (toupper(confirm) == 'Y') break;

            } else {
                SetColor(0, 10); VeKhung(px, py, pw, ph);
                SetColor(0, 15);
                gotoxy(px+2, py+2); cout << "Ban da lam het " << soCau << "/" << soCau << " cau. Nop bai?";
                gotoxy(px+18, py+4);
                SetColor(2, 15);  cout << "  [Y] Nop bai  ";
                SetColor(4, 15);  cout << "  [N] Xem lai  ";
                SetColor(0, 7);

                int confirm = _getch();
                if (toupper(confirm) == 'Y') break;
            }

            VeKhungPhongThi(sv, monChon.tenMH, soCau);
            HienThiCauHoiThi(deThi[cauHienTai], cauHienTai+1, soCau, dapAnDaChon[cauHienTai]);
            VeMiniMap(dapAnDaChon, soCau, cauHienTai);
            CapNhatCounter(dapAnDaChon, soCau);
            InDongHo(thoiGianConLai);
        }
    }
    system("cls"); 
    
    int soCauDung = 0;
    for (int i = 0; i < soCau; i++) {
        if (dapAnDaChon[i] == deThi[i]->dapAn) {
            soCauDung++;
        }
    }
    
    float diemThi = (float)soCauDung / soCau * 10.0f;
    
    HienThiKetQuaThi(soCauDung, soCau, diemThi);
    
    DiemThi* p = sv->headDiem;
    bool daCoDiem = false;
    while (p != NULL) {
        if (strcmp(p->maMH, monChon.maMH) == 0) {
            p->diem = diemThi; // Ghi đè điểm mới
            daCoDiem = true;
            break;
        }
        p = p->next;
    }
    
    // Nếu sinh viên chưa từng thi môn này, tạo node điểm mới
    if (!daCoDiem) {
        DiemThi* newDiem = new DiemThi;
        strcpy(newDiem->maMH, monChon.maMH);
        newDiem->diem = diemThi;
        newDiem->next = sv->headDiem;
        sv->headDiem = newDiem;
    }
	
    string dsND[1000];
	char dsDung[1000];

	for (int i = 0; i < soCau; i++) {
	dsND[i] =
        string(deThi[i]->noiDung) + "\n" +
        "A. " + string(deThi[i]->a) + "\n" +
        "B. " + string(deThi[i]->b) + "\n" +
        "C. " + string(deThi[i]->c) + "\n" +
        "D. " + string(deThi[i]->d);
	
	dsDung[i] = deThi[i]->dapAn;
	}
    SaveBaiLam(sv->maSV, monChon.maMH, dsND, dsDung, dapAnDaChon, soCau);
    
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

void InTextWrap(string text, int x, int &y, int width) {
    if (text.empty()) return;
    while ((int)text.length() > width) {
        if (y > 25) break;
        gotoxy(x, y); 
        int pos = text.rfind(' ', width);
        if (pos == string::npos) pos = width;
        cout << text.substr(0, pos);
        text = text.substr(pos);
        while (!text.empty() && text[0] == ' ') text.erase(0, 1);
        y++;
    }
    if (!text.empty() && y <= 25) {
        gotoxy(x, y);
        cout << text;
    }
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

void InThongBaoLoiDangNhap(string error) {
    SetColor(0, 12);
    InGiuaKhung(error, 38, 18, 44);
    SetColor(0, 7);
}


Lop* TimKiemLopThongMinh(DanhSachLop &dsLop) {
    string tuKhoa = ""; 
    int cursor = 0;

    while (true) {
        XoaVungContent();
        
        InTitle("TIM KIEM LOP HOC");
        gotoxy(35, 8); SetColor(0, 14); cout << "Nhap tu khoa de loc: ";
        SetColor(0, 15); cout << tuKhoa << "_"; 

        gotoxy(35, 11); SetColor(0, 14); printf(" %-5s %-15s %-30s", "STT", "Ma Lop", "Ten Lop");

        string tuKhoaLow = ToLower(tuKhoa);

        Lop* dsLoc[500]; 
        int sttGoc[500];
        int soLuongLoc = 0;

        for (int i = 0; i < dsLop.soLuong; i++) {
            string maLopLow = ToLower(dsLop.nodes[i]->maLop);
            string tenLopLow = ToLower(dsLop.nodes[i]->tenLop);
            
            if (tuKhoa.empty() || maLopLow.find(tuKhoaLow) != string::npos || tenLopLow.find(tuKhoaLow) != string::npos) {
                dsLoc[soLuongLoc] = dsLop.nodes[i];
                sttGoc[soLuongLoc] = i + 1;
                soLuongLoc++;
            }
        }

        if (soLuongLoc == 0) cursor = 0;
        else if (cursor >= soLuongLoc) cursor = soLuongLoc - 1;
        else if (cursor < 0) cursor = 0;

        int yPos = 13;
        int maxDong = 12;

        int batDau = (cursor / maxDong) * maxDong;
        int ketThuc = batDau + maxDong;
        if (ketThuc > soLuongLoc) ketThuc = soLuongLoc;

        for (int i = batDau; i < ketThuc; i++) {
            gotoxy(35, yPos);

            if (i == cursor) {
                SetColor(15, 0);
            } else {
                SetColor(0, 7);
            }

            string ma = string(dsLoc[i]->maLop).substr(0, 15);
            string ten = string(dsLoc[i]->tenLop).substr(0, 30);
            
            printf(" %-5d %-15s %-30s", sttGoc[i], ma.c_str(), ten.c_str());
            
            SetColor(0, 7);
            yPos++;
        }
        
        if (soLuongLoc == 0) {
            gotoxy(52, 14); SetColor(0, 12); cout << "(Khong tim thay lop hoc nao)";
        }

        gotoxy(35, 26); SetColor(0, 8); cout << "Go chu de loc | Phim mui ten LEN/XUONG de chon lop";
        gotoxy(35, 27); SetColor(0, 11); cout << "ENTER: Mo lop | BACKSPACE: Xoa | ESC: Thoat";
        SetColor(0, 7);

        int ch = _getch();

        if (ch == 0 || ch == 224) {
            ch = _getch();
            if (ch == 72) {
                if (cursor > 0) cursor--;
            } 
            else if (ch == 80) {
                if (cursor < soLuongLoc - 1) cursor++;
            }
            continue;
        }
        else if (ch == 27) {
            return NULL; 
        }
        else if (ch == 8) {
            if (!tuKhoa.empty()) {
                tuKhoa.pop_back(); 
                cursor = 0;
            }
        }
        else if (ch == 13) {
            if (soLuongLoc > 0) {
                return dsLoc[cursor]; 
            }
        }
        else if (ch >= 32 && ch <= 126) {
            if (ch == 32 && tuKhoa.empty()) continue; 
            
            if (tuKhoa.length() < 20) { 
                tuKhoa += (char)ch;
                cursor = 0;
            }
        }
    }
}

SinhVien* TimKiemSinhVienTrongLopThongMinh(Lop* lop) {
    if (lop == NULL || lop->headSV == NULL) return NULL;
    
    string tuKhoa = "";
    int cursor = 0;

    SinhVien* arrSV[100];
    int totalSV = 0;
    for (SinhVien* p = lop->headSV; p != NULL; p = p->next) {
        arrSV[totalSV++] = p;
    }

    while (true) {
        XoaVungContent();
        
        InTitle("TIM KIEM SINH VIEN TRONG LOP");
        gotoxy(35, 8); SetColor(0, 14); cout << "Nhap (Ma SV/Ho va ten): ";
        SetColor(0, 15); cout << tuKhoa << "_"; 
        
        gotoxy(35, 11); SetColor(0, 14); 
        printf(" %-5s %-15s %-30s %-10s", "STT", "Ma SV", "Ho va Ten", "Phai");
        string tuKhoaLow = ToLower(tuKhoa);
        
        SinhVien* dsLoc[100]; 
        int sttGoc[100];
        int soLuongLoc = 0;

        for (int i = 0; i < totalSV; i++) {
            string maSVLow = ToLower(arrSV[i]->maSV);
            string hoTenLow = ToLower(string(arrSV[i]->ho) + " " + string(arrSV[i]->ten));
            if (tuKhoa.empty() || maSVLow.find(tuKhoaLow) != string::npos || hoTenLow.find(tuKhoaLow) != string::npos) {
                dsLoc[soLuongLoc] = arrSV[i];
                sttGoc[soLuongLoc] = i + 1;
                soLuongLoc++;
            }
        }
        
        if (soLuongLoc == 0) cursor = 0;
        else if (cursor >= soLuongLoc) cursor = soLuongLoc - 1;
        else if (cursor < 0) cursor = 0;

        int yPos = 13;
        int maxDong = 12; 
        
        int batDau = (cursor / maxDong) * maxDong;
        int ketThuc = batDau + maxDong;
        if (ketThuc > soLuongLoc) ketThuc = soLuongLoc;

        for (int i = batDau; i < ketThuc; i++) {
            gotoxy(35, yPos);
            
            if (i == cursor) SetColor(15, 0);
            else SetColor(0, 7); 
            
            string phai = (dsLoc[i]->phai == 1) ? "Nam" : "Nu";
            string hoTen = string(dsLoc[i]->ho) + " " + string(dsLoc[i]->ten);
            if (hoTen.length() > 30) hoTen = hoTen.substr(0, 27) + "...";
            
            printf(" %-5d %-15s %-30s %-10s", sttGoc[i], dsLoc[i]->maSV, hoTen.c_str(), phai.c_str());
            
            SetColor(0, 7); 
            yPos++;
        }

        gotoxy(35, 27); SetColor(0, 11); cout << "ENTER: Chon | BACKSPACE: Xoa | ESC: Thoat";
        SetColor(0, 7);
        
        int ch = _getch();
        if (ch == 0 || ch == 224) {
            ch = _getch(); 
            if (ch == 72) { if (cursor > 0) cursor--; } 
            else if (ch == 80) { if (cursor < soLuongLoc - 1) cursor++; }
            continue; 
        }
        else if (ch == 27) return NULL;
        else if (ch == 8) {
            if (!tuKhoa.empty()) {
                tuKhoa.pop_back(); 
                cursor = 0; 
            }
        }
        else if (ch == 13) { 
            if (soLuongLoc > 0) return dsLoc[cursor]; 
        }
        else if (ch >= 32 && ch <= 126) { 
            if (ch == 32 && tuKhoa.empty()) continue; 
            if (tuKhoa.length() < 25) { 
                tuKhoa += (char)ch;
                cursor = 0; 
            }
        }
    }
}

MonHoc* TimKiemMonHocThongMinh(DanhSachMonHoc &dsMH) {
    string tuKhoa = "";
    int cursor = 0;

    while (true) {
        XoaVungContent();
        InTitle("TIM KIEM MON HOC");
        gotoxy(35, 8); SetColor(0, 14); cout << "Nhap (Ma MH/Ten MH): ";
        SetColor(0, 15); cout << tuKhoa << "_"; 
        
        gotoxy(35, 11); SetColor(0, 14); 

        printf(" %-5s %-15s %-35s %-10s", "STT", "Ma MH", "Ten Mon Hoc", "Tin Chi");
        
        string tuKhoaLow = ToLower(tuKhoa);
        
        MonHoc* dsLoc[500]; 
        int sttGoc[500];
        int soLuongLoc = 0;

        for (int i = 0; i < dsMH.soLuong; i++) {
            string maMHLow = ToLower(dsMH.data[i].maMH);
            string tenMHLow = ToLower(dsMH.data[i].tenMH);
            
            if (tuKhoa.empty() || maMHLow.find(tuKhoaLow) != string::npos || tenMHLow.find(tuKhoaLow) != string::npos) {
                dsLoc[soLuongLoc] = &dsMH.data[i]; 
                sttGoc[soLuongLoc] = i + 1;
                soLuongLoc++;
            }
        }
        
        if (soLuongLoc == 0) cursor = 0;
        else if (cursor >= soLuongLoc) cursor = soLuongLoc - 1;
        else if (cursor < 0) cursor = 0;

        int yPos = 13;
        int maxDong = 12;
        
        int batDau = (cursor / maxDong) * maxDong;
        int ketThuc = batDau + maxDong;
        if (ketThuc > soLuongLoc) ketThuc = soLuongLoc;

        for (int i = batDau; i < ketThuc; i++) {
            gotoxy(35, yPos);
            
            if (i == cursor) SetColor(15, 0);
            else SetColor(0, 7); 

            string ma = string(dsLoc[i]->maMH);
            if (ma.length() > 15) ma = ma.substr(0, 15);
            
            string ten = string(dsLoc[i]->tenMH);
            if (ten.length() > 35) ten = ten.substr(0, 32) + "...";
            
            printf(" %-5d %-15s %-35s %-10d", sttGoc[i], ma.c_str(), ten.c_str(), dsLoc[i]->soTinChi);
            
            SetColor(0, 7); 
            yPos++;
        }

        gotoxy(35, 27); SetColor(0, 11); cout << "ENTER: Chon | BACKSPACE: Xoa | ESC: Thoat";
        SetColor(0, 7);
        
        int ch = _getch();
        if (ch == 0 || ch == 224) {
            ch = _getch(); 
            if (ch == 72) { if (cursor > 0) cursor--; } 
            else if (ch == 80) { if (cursor < soLuongLoc - 1) cursor++; } 
            continue; 
        }
        else if (ch == 27) return NULL; 
        else if (ch == 8) { 
            if (!tuKhoa.empty()) {
                tuKhoa.pop_back(); 
                cursor = 0; 
            }
        }
        else if (ch == 13) { 
            if (soLuongLoc > 0) return dsLoc[cursor]; 
        }
        else if (ch >= 32 && ch <= 126) { 
            if (ch == 32 && tuKhoa.empty()) continue; 
            if (tuKhoa.length() < 25) { 
                tuKhoa += (char)ch;
                cursor = 0; 
            }
        }
    }
}
#endif

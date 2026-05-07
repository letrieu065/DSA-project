#ifndef SAVELOAD_H
#define SAVELOAD_H

#include "khaibao.h"
#include <fstream>
#include <string>
#include <cstdio>
#include <cstring>

using namespace std;

void KiemTraVaTaoFileCauHoi(const char* maMH, const char* tenMH);
void LuuFileCauHoiTheoMon(const char* maMH, const char* tenMH, PTR_CAUHOI root);
void LayDanhSachCauHoiTheoMon(PTR_CAUHOI root, const char* maMH, CauHoi* dsTam[], int &n);
void LoadCauHoiTuFile(const char* maMH, const char* tenMH, PTR_CAUHOI &root);


void SaveMonHoc(DanhSachMonHoc &dsMH) {
    ofstream fOut("data/MONHOC.txt", ios::out); 
    if (!fOut.is_open()) return;
    fOut << dsMH.soLuong << endl;
    for (int i = 0; i < dsMH.soLuong; i++) {
        fOut << dsMH.data[i].maMH << endl;
        fOut << dsMH.data[i].soTinChi << endl;
        fOut << dsMH.data[i].tenMH << endl;
    }
    fOut.close();
}

void LoadMonHoc(DanhSachMonHoc &dsMH) {
    ifstream fIn("data/MONHOC.txt", ios::in);
    if (!fIn.is_open()) return;
    int n;
    fIn >> n;
    fIn.ignore();
    for (int i = 0; i < n; i++) {
        fIn.getline(dsMH.data[i].maMH, MAX_MAMH);
        fIn >> dsMH.data[i].soTinChi;
        fIn.ignore();
        fIn.getline(dsMH.data[i].tenMH, MAX_TEN);
    }
    dsMH.soLuong = n; 
    fIn.close();
}

void SaveLop(DanhSachLop &dsLop) {
    ofstream fOut("data/LOP.txt", ios::out); 
    if (!fOut.is_open()) return;
    fOut << dsLop.soLuong << endl;
    for (int i = 0; i < dsLop.soLuong; i++) {
        fOut << dsLop.nodes[i]->maLop << endl;
        fOut << dsLop.nodes[i]->tenLop << endl;
    }
    fOut.close();
}

void LoadLop(DanhSachLop &dsLop) {
    ifstream fIn("data/LOP.txt", ios::in); 
    if (!fIn.is_open()) return;
    int n;
    fIn >> n;
    fIn.ignore();
    for (int i = 0; i < n; i++) {
        dsLop.nodes[i] = new Lop;
        fIn.getline(dsLop.nodes[i]->maLop, MAX_MALOP);
        fIn.getline(dsLop.nodes[i]->tenLop, 50);
        dsLop.nodes[i]->headSV = NULL; 
    }
    dsLop.soLuong = n; 
    fIn.close();
}


void KiemTraVaTaoFileCauHoi(const char* maMH, const char* tenMH) {
    string fileName = "data/" + string(tenMH) + " " + string(maMH) + ".txt";
    ifstream f(fileName.c_str());
    if (!f.good()) {
        ofstream fout(fileName.c_str());
        fout.close(); 
    }
    f.close();
}


void LayDanhSachCauHoiTheoMon(PTR_CAUHOI root, const char* maMH, CauHoi* dsTam[], int &n) {
    if (root == NULL) return;
    LayDanhSachCauHoiTheoMon(root->left, maMH, dsTam, n);
    if (strcmp(root->maMH, maMH) == 0) {
        dsTam[n++] = root;
    }
    LayDanhSachCauHoiTheoMon(root->right, maMH, dsTam, n);
}

void LuuFileCauHoiTheoMon(const char* maMH, const char* tenMH, PTR_CAUHOI root) {
    CauHoi* dsTam[1000];
    int n = 0;
    LayDanhSachCauHoiTheoMon(root, maMH, dsTam, n); 
    
    string fileName = "data/" + string(tenMH) + " " + string(maMH) + ".txt";
    ofstream fout(fileName.c_str());
    if (!fout.is_open()) return;

    for (int i = 0; i < n; i++) {
        fout << dsTam[i]->id << endl;
        fout << dsTam[i]->noiDung << endl;
        fout << dsTam[i]->a << endl;
        fout << dsTam[i]->b << endl;
        fout << dsTam[i]->c << endl;
        fout << dsTam[i]->d << endl;
        fout << dsTam[i]->dapAn << endl;
    }
    fout.close();
}

void LoadCauHoiTuFile(const char* maMH, const char* tenMH, PTR_CAUHOI &root) {
    string fileName = "data/" + string(tenMH) + " " + string(maMH) + ".txt";
    ifstream fIn(fileName.c_str());
    if (!fIn.is_open()) return;

    while (!fIn.eof()) {
        char tempID[25];
        if (!(fIn.getline(tempID, 25)) || strlen(tempID) == 0) break; 

        CauHoi* newNode = new CauHoi();
        strcpy(newNode->id, tempID);
        strcpy(newNode->maMH, maMH);

        fIn.getline(newNode->noiDung, 800);
        fIn.getline(newNode->a, 200);
        fIn.getline(newNode->b, 200);
        fIn.getline(newNode->c, 200);
        fIn.getline(newNode->d, 200);
        
        fIn >> newNode->dapAn;
        fIn.ignore(); 

        newNode->left = newNode->right = NULL;
        root = ThemCauHoi(root, newNode); 

        string idStr = tempID;
        string maStr = maMH;
        if (idStr.find(maStr) == 0) {
            int stt = atoi(idStr.substr(maStr.length()).c_str());
            if (stt >= nextCauHoiSoThuTu) nextCauHoiSoThuTu = stt + 1;
        }
    }
    fIn.close();
}

void XoaFileNeuTrong(const char* maMH, const char* tenMH, PTR_CAUHOI root) {
    CauHoi* dsTam[1000];
    int n = 0;
    LayDanhSachCauHoiTheoMon(root, maMH, dsTam, n);

    if (n == 0) {
        string fileName = "data/" + string(tenMH) + " " + string(maMH) + ".txt";
        remove(fileName.c_str());
    }
}

// SAVE -- LOAD 

void saveData(DanhSachLop &dsLop, DanhSachMonHoc &dsMH, PTR_CAUHOI &root) {
    SaveMonHoc(dsMH);
    SaveLop(dsLop);
    for (int i = 0; i < dsMH.soLuong; i++) {
        LuuFileCauHoiTheoMon(dsMH.data[i].maMH, dsMH.data[i].tenMH, root);
    }
}

void loadData(DanhSachLop &dsLop, DanhSachMonHoc &dsMH, PTR_CAUHOI &root) {
    LoadMonHoc(dsMH);
    LoadLop(dsLop);
    for (int i = 0; i < dsMH.soLuong; i++) {
        LoadCauHoiTuFile(dsMH.data[i].maMH, dsMH.data[i].tenMH, root);
    }
}

#endif
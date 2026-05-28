
#ifndef SAVELOAD_H
#define SAVELOAD_H

#include <fstream>
#include <string>
#include <cstdio>
#include <cstring>
#include <direct.h>
#include "khaibao.h"
#include "hamchinh.h"

using namespace std;

void KiemTraVaTaoFileCauHoi(const char* maMH, const char* tenMH);
void LuuFileCauHoiTheoMon(const char* maMH, const char* tenMH, PTR_CAUHOI root);
void LayDanhSachCauHoiTheoMon(PTR_CAUHOI root, const char* maMH, CauHoi* dsTam[], int &n);
void LoadCauHoiTuFile(const char* maMH, const char* tenMH, PTR_CAUHOI &root);
void saveData(DanhSachLop &dsLop, DanhSachMonHoc &dsMH, PTR_CAUHOI &root);
void SaveBaiLam(const char* maSV, const char* maMH, string dsNDCauHoi[], char dsDapAnDung[], char dsDapAnChon[], int soCau);


int DemSoDiem(DiemThi* headDiem) {
    int count = 0;
    for (DiemThi* p = headDiem; p != NULL; p = p->next) {
        count++;
    }
    return count;
}

void SaveLop(DanhSachLop &dsLop) {
    ofstream fOut("data/LOP.txt", ios::out); 
    if (!fOut.is_open()) return;
    
    fOut << dsLop.soLuong << endl;
    
    for (int i = 0; i < dsLop.soLuong; i++) {
        fOut << dsLop.nodes[i]->maLop << endl;
        fOut << dsLop.nodes[i]->tenLop << endl;
        
        int soSV = demSoSV(dsLop.nodes[i]);
        fOut << soSV << endl;
        
        SinhVien* pSV = dsLop.nodes[i]->headSV;
        while (pSV != NULL) {
            fOut << pSV->maSV << endl;
            fOut << pSV->ho << endl;
            fOut << pSV->ten << endl;
            fOut << pSV->phai << endl; 
            fOut << pSV->password << endl;
            
            int soDiem = DemSoDiem(pSV->headDiem);
            fOut << soDiem << endl;
            
            DiemThi* pDiem = pSV->headDiem;
            while (pDiem != NULL) {
                fOut << pDiem->maMH << endl;
                fOut << pDiem->diem << endl;
                pDiem = pDiem->next;
            }
            pSV = pSV->next;
        }
    }
    fOut.close();
}

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

void LoadLop(DanhSachLop &dsLop) {
    ifstream fIn("data/LOP.txt", ios::in); 
    if (!fIn.is_open()) return;
    
    int soLop;
    if (!(fIn >> soLop)) return; 
    fIn.ignore();
    
    dsLop.soLuong = 0; 
    for (int i = 0; i < soLop; i++) {
        dsLop.nodes[i] = new Lop;
        fIn.getline(dsLop.nodes[i]->maLop, MAX_MALOP);
        fIn.getline(dsLop.nodes[i]->tenLop, MAX_TEN);
        dsLop.nodes[i]->headSV = NULL;
        
        int soSV;
        fIn >> soSV; fIn.ignore();
        
        SinhVien* tailSV = NULL;
        for (int j = 0; j < soSV; j++) {
            char tempMa[MAX_MASV] = {0}; 
            if (!fIn.getline(tempMa, MAX_MASV) || strlen(tempMa) < 2) {

                continue; 
            }

            SinhVien* svMoi = new SinhVien;
            strcpy(svMoi->maSV, tempMa);
            fIn.getline(svMoi->ho, MAX_TEN);
            fIn.getline(svMoi->ten, MAX_TEN);
            fIn >> svMoi->phai; fIn.ignore();
            fIn.getline(svMoi->password, MAX_PASS);
            svMoi->headDiem = NULL;
            svMoi->next = NULL;
            

            int soDiem;
            fIn >> soDiem; fIn.ignore();
            DiemThi* tailDiem = NULL;
            for (int k = 0; k < soDiem; k++) {
                DiemThi* diemMoi = new DiemThi;
                fIn.getline(diemMoi->maMH, MAX_MAMH);
                fIn >> diemMoi->diem; fIn.ignore();
                diemMoi->next = NULL;
                if (svMoi->headDiem == NULL) svMoi->headDiem = diemMoi;
                else tailDiem->next = diemMoi;
                tailDiem = diemMoi;
            }

            if (dsLop.nodes[i]->headSV == NULL) dsLop.nodes[i]->headSV = svMoi;
            else tailSV->next = svMoi;
            tailSV = svMoi;
        }
        dsLop.soLuong++;
    }
    fIn.close();
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

void KiemTraVaTaoFileCauHoi(const char* maMH, const char* tenMH) {
    _mkdir("data/CAUHOI"); // Lệnh này sẽ tự tạo folder CAUHOI nếu nó chưa tồn tại
    string fileName = "data/CAUHOI/" + string(tenMH) + " " + string(maMH) + ".txt";
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
    
    _mkdir("data/CAUHOI"); // Tự tạo folder trước khi lưu
    string fileName = "data/CAUHOI/" + string(tenMH) + " " + string(maMH) + ".txt";
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
    string fileName = "data/CAUHOI/" + string(tenMH) + " " + string(maMH) + ".txt";
    ifstream fIn(fileName.c_str());
    if (!fIn.is_open()) return;

    char tempID[25];
    while (fIn.getline(tempID, 25) && strlen(tempID) > 0) {

        CauHoi* newNode = new CauHoi();
        strcpy(newNode->id, tempID);
        strcpy(newNode->maMH, maMH);
        fIn.getline(newNode->noiDung, 499);
        fIn.getline(newNode->a, 200);
        fIn.getline(newNode->b, 200);
        fIn.getline(newNode->c, 200);
        fIn.getline(newNode->d, 200);
        fIn >> newNode->dapAn;
        fIn.ignore();
        newNode->left = newNode->right = NULL;
        root = ThemCauHoi(root, newNode);

        string idStr = tempID;
        size_t pos = idStr.find_last_of('_');
        if (pos != string::npos) {
            int stt = atoi(idStr.substr(pos + 1).c_str()); 
            if (stt >= getNextSTT(maMH)) {
                getNextSTT(maMH) = stt + 1; 
            }
        }
    }
    fIn.close();
}

void XoaFileNeuTrong(const char* maMH, const char* tenMH, PTR_CAUHOI root) {
    CauHoi* dsTam[1000];
    int n = 0;
    LayDanhSachCauHoiTheoMon(root, maMH, dsTam, n);

    if (n == 0) {
        // Cập nhật đường dẫn xóa file
        string fileName = "data/CAUHOI/" + string(tenMH) + " " + string(maMH) + ".txt";
        remove(fileName.c_str());
    }
}

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

void SaveBaiLam(const char* maSV, const char* maMH, string dsNDCauHoi[], char dsDapAnDung[], char dsDapAnChon[], int soCau) {
    _mkdir("data");
    _mkdir("data/bailam");

    string fileName = "data/bailam/" + string(maSV) + " " + string(maMH) + ".txt";
    ofstream fOut(fileName.c_str());

    if (!fOut.is_open()) {
        return; 
    }
    for (int i = 0; i < soCau; i++) {
        fOut << "CAU " << i + 1 << ": " << dsNDCauHoi[i] << endl;
        fOut << "Dap an dung: " << dsDapAnDung[i] << endl;
        fOut << "Sinh vien chon: " << dsDapAnChon[i] << endl;
        fOut << "------------------------------------------" << endl; 
    }
    fOut.close();
}
#endif
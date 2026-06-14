
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
bool LuuFileCauHoiTheoMon(const char* maMH, const char* tenMH, PTR_CAUHOI root);
void LayDanhSachCauHoiTheoMon(PTR_CAUHOI root, const char* maMH, CauHoi* dsTam[], int &n);
bool LoadCauHoiTuFile(const char* maMH, const char* tenMH, PTR_CAUHOI &root);
bool saveData(DanhSachLop &dsLop, DanhSachMonHoc &dsMH, PTR_CAUHOI &root);
void SaveBaiLam(const char* maSV, const char* maMH, string dsNDCauHoi[], char dsDapAnDung[], char dsDapAnChon[], int soCau);
void XoaFileNeuTrong(const char* maMH, const char* tenMH, PTR_CAUHOI root);
bool LoadMonHoc(DanhSachMonHoc &dsMH);
bool LoadLop(DanhSachLop &dsLop);
bool SaveMonHoc(DanhSachMonHoc &dsMH);
bool SaveLop(DanhSachLop &dsLop);
bool loadData(DanhSachLop &dsLop, DanhSachMonHoc &dsMH, PTR_CAUHOI &root);

static bool gDaLoadLopThanhCong = false;
static bool gDaLoadMonHocThanhCong = false;
static bool gDaLoadCauHoiThanhCong = false;

bool ThayTheFileAnToan(const string& tempName, const string& fileName) {
    string backupName = fileName + ".bak";
    bool coFileCu = false;
    {
        ifstream f(fileName.c_str());
        coFileCu = f.good();
    }

    remove(backupName.c_str());
    if (coFileCu && rename(fileName.c_str(), backupName.c_str()) != 0) {
        remove(tempName.c_str());
        return false;
    }

    if (rename(tempName.c_str(), fileName.c_str()) != 0) {
        if (coFileCu) rename(backupName.c_str(), fileName.c_str());
        remove(tempName.c_str());
        return false;
    }

    remove(backupName.c_str());
    return true;
}



int DemSoDiem(DiemThi* headDiem) {
    int count = 0;
    for (DiemThi* p = headDiem; p != NULL; p = p->next) {
        count++;
    }
    return count;
}

bool SaveLop(DanhSachLop &dsLop) {
    if (!gDaLoadLopThanhCong) return false;

    string tempName = "data/LOP.txt.tmp";
    string fileName = "data/LOP.txt";
    ofstream fOut(tempName.c_str(), ios::out); 
    if (!fOut.is_open()) return false;
    
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
    if (!fOut) {
        remove(tempName.c_str());
        return false;
    }
    return ThayTheFileAnToan(tempName, fileName);
}

bool SaveMonHoc(DanhSachMonHoc &dsMH) {
    if (!gDaLoadMonHocThanhCong) return false;

    string tempName = "data/MONHOC.txt.tmp";
    string fileName = "data/MONHOC.txt";
    ofstream fOut(tempName.c_str(), ios::out); 
    if (!fOut.is_open()) return false;
    fOut << dsMH.soLuong << endl;
    for (int i = 0; i < dsMH.soLuong; i++) {
        fOut << dsMH.data[i].maMH << endl;
        fOut << dsMH.data[i].soTinChi << endl;
        fOut << dsMH.data[i].tenMH << endl;
    }
    fOut.close();
    if (!fOut) {
        remove(tempName.c_str());
        return false;
    }
    return ThayTheFileAnToan(tempName, fileName);
}

bool LoadLop(DanhSachLop &dsLop) {
    ifstream fIn("data/LOP.txt", ios::in); 
    if (!fIn.is_open()) return false;
    
    int soLop;
    if (!(fIn >> soLop) || soLop < 0 || soLop > MAX_LOP) return false; 
    fIn.ignore();
    
    DanhSachLop dsTam;
    dsTam.soLuong = 0;
    for (int i = 0; i < MAX_LOP; i++) dsTam.nodes[i] = NULL;

    for (int i = 0; i < soLop; i++) {
        dsTam.nodes[i] = new Lop;
        if (!fIn.getline(dsTam.nodes[i]->maLop, MAX_MALOP)) return false;
        if (!fIn.getline(dsTam.nodes[i]->tenLop, MAX_TEN_LOP)) return false;
        dsTam.nodes[i]->headSV = NULL;
        
        int soSV;
        if (!(fIn >> soSV) || soSV < 0) return false;
        fIn.ignore();
        
        SinhVien* tailSV = NULL;
        for (int j = 0; j < soSV; j++) {
            char tempMa[MAX_MASV] = {0}; 
            if (!fIn.getline(tempMa, MAX_MASV) || strlen(tempMa) < 2) {
                return false; 
            }

            SinhVien* svMoi = new SinhVien;
            strcpy(svMoi->maSV, tempMa);
            if (!fIn.getline(svMoi->ho, MAX_HO)) return false;
            if (!fIn.getline(svMoi->ten, MAX_TEN_SV)) return false;
            if (!(fIn >> svMoi->phai)) return false;
            fIn.ignore();
            if (!fIn.getline(svMoi->password, MAX_PASS)) return false;
            svMoi->headDiem = NULL;
            svMoi->next = NULL;
            

            int soDiem;
            if (!(fIn >> soDiem) || soDiem < 0) return false;
            fIn.ignore();
            DiemThi* tailDiem = NULL;
            for (int k = 0; k < soDiem; k++) {
                DiemThi* diemMoi = new DiemThi;
                if (!fIn.getline(diemMoi->maMH, MAX_MAMH)) return false;
                if (!(fIn >> diemMoi->diem)) return false;
                fIn.ignore();
                diemMoi->next = NULL;
                if (svMoi->headDiem == NULL) svMoi->headDiem = diemMoi;
                else tailDiem->next = diemMoi;
                tailDiem = diemMoi;
            }

            if (dsTam.nodes[i]->headSV == NULL) dsTam.nodes[i]->headSV = svMoi;
            else tailSV->next = svMoi;
            tailSV = svMoi;
        }
        dsTam.soLuong++;
    }
    fIn.close();

    dsLop = dsTam;
    gDaLoadLopThanhCong = true;
    return true;
}

bool LoadMonHoc(DanhSachMonHoc &dsMH) {
    ifstream fIn("data/MONHOC.txt", ios::in);
    if (!fIn.is_open()) return false;
    int n;
    if (!(fIn >> n) || n < 0 || n > MAX_MON_HOC) return false;
    fIn.ignore();
    DanhSachMonHoc dsTam;
    dsTam.soLuong = 0;
    for (int i = 0; i < n; i++) {
        if (!fIn.getline(dsTam.data[i].maMH, MAX_MAMH)) return false;
        if (!(fIn >> dsTam.data[i].soTinChi)) return false;
        fIn.ignore();
        if (!fIn.getline(dsTam.data[i].tenMH, MAX_TENMH)) return false;
    }
    dsTam.soLuong = n;
    dsMH = dsTam;
    gDaLoadMonHocThanhCong = true;
    fIn.close();
    return true;
}

void KiemTraVaTaoFileCauHoi(const char* maMH, const char* tenMH) {
    _mkdir("data");
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

bool LuuFileCauHoiTheoMon(const char* maMH, const char* tenMH, PTR_CAUHOI root) {
    CauHoi* dsTam[1000];
    int n = 0;
    LayDanhSachCauHoiTheoMon(root, maMH, dsTam, n); 
    _mkdir("data");
    _mkdir("data/CAUHOI"); // Tự tạo folder trước khi lưu
    string fileName = "data/CAUHOI/" + string(tenMH) + " " + string(maMH) + ".txt";
    string tempName = fileName + ".tmp";
    ofstream fout(tempName.c_str());
    if (!fout.is_open()) return false;

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
    if (!fout) {
        remove(tempName.c_str());
        return false;
    }
    return ThayTheFileAnToan(tempName, fileName);
}

bool LoadCauHoiTuFile(const char* maMH, const char* tenMH, PTR_CAUHOI &root) {
    string fileName = "data/CAUHOI/" + string(tenMH) + " " + string(maMH) + ".txt";
    ifstream fIn(fileName.c_str());
    if (!fIn.is_open()) return false;

    char tempID[25];
    while (fIn.getline(tempID, 25) && strlen(tempID) > 0) {

        CauHoi* newNode = new CauHoi();
        strcpy(newNode->id, tempID);
        strcpy(newNode->maMH, maMH);
        if (!fIn.getline(newNode->noiDung, 499)) return false;
        if (!fIn.getline(newNode->a, 200)) return false;
        if (!fIn.getline(newNode->b, 200)) return false;
        if (!fIn.getline(newNode->c, 200)) return false;
        if (!fIn.getline(newNode->d, 200)) return false;
        if (!(fIn >> newNode->dapAn)) return false;
        fIn.ignore();
        newNode->left = newNode->right = NULL;
        root = ThemCauHoi(root, newNode);
    }
    fIn.close();
    return true;
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

bool saveData(DanhSachLop &dsLop, DanhSachMonHoc &dsMH, PTR_CAUHOI &root) {
    bool ok = true;
    ok = SaveMonHoc(dsMH) && ok;
    ok = SaveLop(dsLop) && ok;
    if (!gDaLoadCauHoiThanhCong) return false;
    for (int i = 0; i < dsMH.soLuong; i++) {
        ok = LuuFileCauHoiTheoMon(dsMH.data[i].maMH, dsMH.data[i].tenMH, root) && ok;
    }
    return ok;
}

bool loadData(DanhSachLop &dsLop, DanhSachMonHoc &dsMH, PTR_CAUHOI &root) {
    bool okMonHoc = LoadMonHoc(dsMH);
    bool okLop = LoadLop(dsLop);
    if (!okMonHoc || !okLop) return false;
    bool okCauHoi = true;
    for (int i = 0; i < dsMH.soLuong; i++) {
        okCauHoi = LoadCauHoiTuFile(dsMH.data[i].maMH, dsMH.data[i].tenMH, root) && okCauHoi;
    }
    gDaLoadCauHoiThanhCong = okCauHoi;
    return okCauHoi;
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

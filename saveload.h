#ifndef SAVELOAD_H
#define SAVELOAD_H
#include "khaibao.h"

#include <fstream>

void SaveMonHoc(DanhSachMonHoc &dsMH) {
	ofstream fOut("MONHOC.txt", ios::out);
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
	ifstream fIn("MONHOC.txt", ios::in);
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
	ofstream fOut("LOP.txt", ios::out);
	if (!fOut.is_open()) return;
	fOut << dsLop.soLuong << endl;
	for (int i = 0; i < dsLop.soLuong; i++) {
		fOut << dsLop.nodes[i]->maLop << endl;
		fOut << dsLop.nodes[i]->tenLop << endl;
	}
	fOut.close();
}
void LoadLop(DanhSachLop &dsLop) {
	ifstream fIn("LOP.txt", ios::in);
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
void SaveCauHoi();
void LoadCauHoi();
void SaveSinhVien();
void LoadSinhVien();
void saveData(DanhSachLop &dsLop, DanhSachMonHoc &dsMH, PTR_CAUHOI &root) {
	SaveMonHoc(dsMH);
	SaveLop(dsLop);
}
void loadData(DanhSachLop &dsLop, DanhSachMonHoc &dsMH, PTR_CAUHOI &root) {
	LoadMonHoc(dsMH);
	LoadLop(dsLop);
}
//
#endif
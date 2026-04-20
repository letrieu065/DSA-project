#ifndef SAVELOAD_H
#define SAVELOAD_H
#include "khaibao.h"

#include <fstream>

void SaveMonHoc();
void LoadMonHoc();
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

#endif
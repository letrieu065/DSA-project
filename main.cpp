#include <iostream>
#include <windows.h>
#include "khaibao.h"
#include "hamtienich.h"
#include "dohoa.h"
#include "saveload.h"
#include "hamchinh.h"
#include "flow.h"

using namespace std;
	
int main() {
	DanhSachMonHoc DANHSACHMONHOC;
	DanhSachLop DANHSACHLOP;
	CauHoi* Root;
	SetFontSize(24);
	Root = NULL;
	bool daLoadDuLieu = loadData(DANHSACHLOP, DANHSACHMONHOC, Root);
	if (!daLoadDuLieu) {
		cout << "Loi: Khong the doc day du du lieu tu thu muc data. Vui long kiem tra lai." << endl;
		system("pause");
		return 1;   // thoat ngay, khong cho dung chuong trinh voi du lieu khong day du
	}
	flowChinh(DANHSACHLOP, DANHSACHMONHOC, Root);
	if (!saveData(DANHSACHLOP, DANHSACHMONHOC, Root)) {
		cout << "Loi: Khong the luu day du du lieu. Hay kiem tra file data va quyen ghi." << endl;
		system("pause");
	}
	GiaiPhongToanBoBoNho(DANHSACHLOP, Root);
	return 0;
}
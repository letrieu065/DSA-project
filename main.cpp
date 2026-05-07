#include <iostream>
#include "khaibao.h"
#include "hamtienich.h"
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
	loadData(DANHSACHLOP, DANHSACHMONHOC, Root);
	flowChinh(DANHSACHLOP, DANHSACHMONHOC, Root);
	saveData(DANHSACHLOP, DANHSACHMONHOC, Root);
	return 0;
}

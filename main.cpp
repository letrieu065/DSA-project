#include <iostream>
#include "khaibao.h"
#include "hamtienich.h"
#include "saveload.h"
#include "hamchinh.h"
#include "flow.h"

using namespace std;
DanhSachMonHoc DANHSACHMONHOC;
DanhSachLop DANHSACHLOP;
CauHoi* Root;

int main() {
	SetFontSize(24);
	Init(DANHSACHMONHOC, DANHSACHLOP, Root);
	loadData(DANHSACHLOP, DANHSACHMONHOC, Root);
	flowChinh(DANHSACHLOP, DANHSACHMONHOC, Root);
	saveData(DANHSACHLOP, DANHSACHMONHOC, Root);
	return 0;
}

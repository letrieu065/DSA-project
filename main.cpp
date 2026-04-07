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
	LoadLop(DANHSACHLOP);
	flowChinh(DANHSACHLOP, DANHSACHMONHOC, Root);
}

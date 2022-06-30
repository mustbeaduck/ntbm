#ifndef __OFFSETS__HEADER__
#define __OFFSETS__HEADER__

#include "Windows.h"
#include "stdint.h"
#include "point.h"

extern uintptr_t * visRefFlag;
extern uintptr_t * currentTool;

extern uintptr_t gSelectTool;
extern uintptr_t gCPaintToolManager;
extern uintptr_t gFreehandSelectionTool;

//CImgWnd *
extern uintptr_t * imgWnd;
//MTI *
extern Point * mti;

extern void (__thiscall * refreshHandler)(uintptr_t self);

//CFreehandSelectTool::
extern void (__thiscall * addPoint)(uintptr_t self, Point p);
extern void (__thiscall * deleteArrayContents)(uintptr_t self);
extern void (__thiscall * onActivate)(uintptr_t self, int flag);
extern HRESULT (__thiscall * createPolyRegion)(uintptr_t self, double p);

//CSelectTool::
extern void (__thiscall * stEndDrag)(uintptr_t self, uintptr_t imgWnd, Point * mti);

//CDC::
extern uintptr_t * (__fastcall * fromHandle) (HANDLE hdc); 

//GDI
extern uint32_t (__fastcall * getPixel) (HANDLE hdc, int x, int y); 
extern uint32_t (__fastcall * setPixel) (HANDLE hdc, int x, int y, uint32_t col); 

void setTheOffsets(uintptr_t base);
#endif



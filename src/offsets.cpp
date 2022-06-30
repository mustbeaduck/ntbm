#include "offsets.hpp"

Point * mti;
uintptr_t * imgWnd;
uintptr_t gSelectTool;
uintptr_t * visRefFlag;
uintptr_t * currentTool;
uintptr_t gCPaintToolManager;
uintptr_t gFreehandSelectionTool;


void (__thiscall * refreshHandler)(uintptr_t self);

//CFreehandSelectTool::
void (__thiscall * addPoint)(uintptr_t self, Point p);
void (__thiscall * deleteArrayContents)(uintptr_t self);
void (__thiscall * onActivate)(uintptr_t self, int flag);
HRESULT (__thiscall * createPolyRegion)(uintptr_t self, double p);

//CSelectTool::
void (__thiscall * stEndDrag)(uintptr_t self, uintptr_t imgWnd, Point * mti);

//CDC::
uintptr_t * (__fastcall * fromHandle) (HANDLE hdc); 
//GDI
uint32_t (__fastcall * getPixel) (HANDLE hdc, int x, int y); 
uint32_t (__fastcall * setPixel) (HANDLE hdc, int x, int y, uint32_t col); 

void setTheOffsets(uintptr_t base)
{
    /*
                            functions
    */
    // i feel like i should have pick C just so i could
    // tell compiler to shut the fuck up about pointer casting :)
    
    onActivate = (void (*)(uintptr_t, int))(base + 0x66020);
    addPoint = (void (*)(uintptr_t, Point))(base + 0x66458);
    deleteArrayContents = (void (*)(uintptr_t))(base + 0x6618C);

    createPolyRegion = (HRESULT (*)(uintptr_t, double))(base + 0x6559C); 
    stEndDrag = (void (*)(uintptr_t, uintptr_t, Point*))(base + 0x24F10);

    //CDC
    fromHandle = (uintptr_t* (*)(HANDLE))(*( uintptr_t * )(base + 0xB3AC0));

    getPixel = (uint32_t (*)(HANDLE, int, int))(*( uintptr_t * )(base + 0xB2EB8));
    setPixel = (uint32_t (*)(HANDLE, int, int, uint32_t))(*( uintptr_t * )(base + 0xB2EE8));
    
    refreshHandler = (void (*)(uintptr_t))(base + 0x348b4);
    /*
                            objects
    */
    
    currentTool = ( uintptr_t * ) (base + 0xe41d8);
    visRefFlag = ( uintptr_t * ) (base + 0xead30);
    imgWnd = ( uintptr_t * ) (base + 0xec198); 
    gFreehandSelectionTool = base + 0xeb450;
    gCPaintToolManager = base + 0xead08;
    mti = ( Point * ) (base + 0xe9d28);    
    gSelectTool = base + 0xE9D50;
}

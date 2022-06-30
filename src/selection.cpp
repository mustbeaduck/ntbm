#include "selection.hpp"
#include "offsets.hpp"
#include "stdint.h"
#include "vector"

//CDC handle
HANDLE * hnd;

int oldWidth = 0;
int oldHeight = 0;
std::vector<std::vector<bool>> selMask;

static void selectTool()
{
    *visRefFlag = 0;
    refreshHandler(gCPaintToolManager);
    *currentTool = gFreehandSelectionTool;
    onActivate(gFreehandSelectionTool, 1);
}

static void initSelectionMask()
{
    int * width = ( int * ) (*imgWnd + 0xf0);
    int * height = width + 1;

    //if image sizes has changed
    //or selection mask was not yet created
    if ( oldWidth != *width or oldHeight != *height ) {
        selMask.resize(*height); 
        if ( oldWidth != *width ) {
            for ( int i = 0; i < selMask.size(); i+=1 ) {
                selMask[i].resize(*width);
            }
        }
        oldHeight = *height;
        oldWidth = *width;
    }

    //reset the selection mask
    for ( auto v : selMask ) {
        std::fill(v.begin(), v.end(), false);
    }

    uintptr_t ptr = *( uintptr_t * )(*imgWnd + 0x90);
    //image window cdc handle
    hnd = * ( HANDLE ** )(ptr + 0x18);
}
  
void createSelection(Point pnt)
{
    initSelectionMask();

    /*
            creating custom spiral-shaped selection
            for concept proof only this code snipet
            assumes that picture size will be 64x64 pixels 
            or larger
    */
    Point conceptProof[] = {
        {27, 56}, {13, 50}, {6, 36}, {10, 23}, {17,13}, {28, 7},
        {41, 10}, {52, 20}, {53, 35}, {48, 43}, {38, 46}, {29, 42},
        {27,32}, {30, 27}, {35, 25}, {36, 30}, {34, 34}, {38, 39},
        {45, 35}, {46, 23}, {40, 15}, {28, 14}, {19, 18}, {16, 24},
        {12, 36}, {18, 47}, {29, 50}
    }; 

    selectTool();

    //chances are CFreehandTool::onActivate does it for ya
    //deleteArrayContents(gFreehandSelectionTool);

    //populate the selection with new points
    for ( Point p : conceptProof ) {
        addPoint(gFreehandSelectionTool, p);  
    }

    double some_val = *( double * ) (*imgWnd + 0xa8);
    if ( !createPolyRegion(gFreehandSelectionTool, some_val) ) {
        MessageBox(NULL, "error", "CreatePolyRegion failed :(", MB_OK);    
        return;
    }

    Point p1 = *( Point * ) (gFreehandSelectionTool + 0xa8);
    Point p2 = *( Point * ) (gFreehandSelectionTool + 0xb0);
    mti[0] = p1;
    mti[2] = p2;

    //MessageBox(NULL, "done", "done", MB_OK);    
    stEndDrag(gSelectTool, *imgWnd, mti);
} 

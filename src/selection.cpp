#include "selection.hpp"
#include "offsets.hpp"
#include "stdint.h"
#include "vector"
#include "stack"
#include "list"

//CDC handle
HANDLE * hnd;

int oldWidth = 0;
int oldHeight = 0;
std::vector<std::vector<bool>> selMask;

struct Pars {
    int x;
    int y; 
    int dir;
};

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
  

//iterative flood fill adjusted for (quiet retarded)edge detection
std::list<Point> floodFillEdge(Point p) 
{
    //validating starting cords
    //is overall unnecessary
    std::list<Point> Path;
    Path.push_back({p.x, p.y});
   
    Pars pars;
    int prev_dir = 0;
    std::stack<Pars> call_stack;
    call_stack.push({p.x, p.y, 0});

    uint32_t color = getPixel(hnd, p.x, p.y);

    //recursion starts here
    while ( !call_stack.empty() ) {

        pars = call_stack.top();
        call_stack.pop();
        
        //fprintf(out, "floodFill at %d %d\n", pars.x, pars.y);

        //physical border check
        if ( pars.x >= 0 and pars.x < oldWidth and pars.y >= 0 and pars.y < oldHeight ) {
            //floodfill condition
            if ( color == getPixel(hnd, pars.x, pars.y) ) {
                //check if it was already visited
                if ( selMask[pars.y][pars.x] == 0 ) {
                    //preserve specific order for each direction 
                    switch( pars.dir ) {
                        case 0:
                            call_stack.push({pars.x, pars.y-1, 1}); call_stack.push({pars.x+1, pars.y, 2});
                            call_stack.push({pars.x, pars.y+1, 3}); call_stack.push({pars.x-1, pars.y, 4});
                            break;
                        case 1:
                            call_stack.push({pars.x-1, pars.y, 4}); call_stack.push({pars.x, pars.y-1, 1});
                            call_stack.push({pars.x+1, pars.y, 2});
                            break;
                        case 2:
                            call_stack.push({pars.x, pars.y-1, 1}); call_stack.push({pars.x+1, pars.y, 2});
                            call_stack.push({pars.x, pars.y+1, 3});
                            break;
                        case 3:
                            call_stack.push({pars.x+1, pars.y, 2}); call_stack.push({pars.x, pars.y+1, 3});
                            call_stack.push({pars.x-1, pars.y, 4});
                            break;
                        case 4:
                            call_stack.push({pars.x, pars.y+1, 3}); call_stack.push({pars.x-1, pars.y, 4});
                            call_stack.push({pars.x, pars.y-1, 1});
                            break;
                    }

                    //update selection mask
                    selMask[pars.y][pars.x] = 1;
                    continue;
                }
                //since there is no border proc
                //skip to next iteration
            }
        }

        Point pnt = {};
        //determine a border point to add
        switch ( pars.dir ) {
            case 1: pnt.x = pars.x; pnt.y = pars.y+1; break;
            case 2: pnt.x = pars.x; pnt.y = pars.y; break;
            case 3: pnt.x = pars.x+1; pnt.y = pars.y; break;
            case 4: pnt.x = pars.x+1; pnt.y = pars.y+1; break;
        }

        if ( prev_dir == pars.dir ) {
            Path.back() = pnt;
        } else {
            Path.push_back(pnt);
        }
        
        prev_dir = pars.dir;
    }
    
    return Path;
}


void createSelection(Point pnt)
{
    initSelectionMask();
    
    selectTool();

    uintptr_t val = * ( uintptr_t * ) (gFreehandSelectionTool + 0x20);
    uintptr_t * addr = ( uintptr_t * ) (gFreehandSelectionTool + 0x36);
    *addr = val;

    deleteArrayContents(gFreehandSelectionTool);

    std::list<Point> Path = floodFillEdge(pnt);

    for ( Point p : Path ) {
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

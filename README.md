<h3> ntbm </h3>
<p><i>my attempt to bring magic wand selection to msPaint</i></p>

<h2>Done so far:</h2>
<ul> 
    <li>Find all of the necessary offsets
    <li>Create custom selection area by user points
</ul>

<h2>TODO:</h2>
<ul>
    <li> get PDB specific data from the PE to load debug symbols from the microsoft server
    <li> extract version specific offsets from the PDB file
    <li> decent selection algorithm 
    <li> ...
    <li> come up with new tasks i will never finish
</ul>

<h3> Creating custom selection: </h3>

```c
    Point conceptProof[] = {
        {27, 56}, {13, 50}, {6, 36}, {10, 23}, {17,13}, {28, 7},
        {41, 10}, {52, 20}, {53, 35}, {48, 43}, {38, 46}, {29, 42},
        {27,32}, {30, 27}, {35, 25}, {36, 30}, {34, 34}, {38, 39},
        {45, 35}, {46, 23}, {40, 15}, {28, 14}, {19, 18}, {16, 24},
        {12, 36}, {18, 47}, {29, 50}
    }; 

    selectTool();
    //populate the selection with new points
    for ( Point p : conceptProof ) {
        addPoint(gFreehandSelectionTool, p);  
    }
```

<img src="https://github.com/mustbeaduck/ntbm/blob/master/pics/cust_sel.gif" alt = "> a gif image should be somwhere here future me must've changed fucking name again <" />



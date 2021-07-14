# Cache-Simulator

## Usage guide

1.  clone repo:

        git clone https://github.com/Youssef-Agiza/Cache-Simulator.git <dir_name>

2.  Navigate to repo directory:

        cd <dir_name>

3.  To build executable, run:

        make

4.  To run cache simulation run:

        ./cacheSim.exe

    The output is stored in "outputs" folder.

5.  Drawing plots, run:

        make plotOutputs

6.  Output destination:\
    The output .csv files are store "outputs" directory, and plots are stores in "plots" directory. \
     **note:** Those directory must exist for the program to work correctly. They are create by default in after running the make command. But if not, the user should create them manually or run:

        make outputDir

## Plotter program

Plotting is done using python library matplotlib.
The plotting program is called plotter.py. plotter.py expectes input format in .csv that has 7 columns, the first one has the values that will be plotted on the x-axis. The first row is expected to have the titles of each colum, with the column titles are expected to be as follows: <x_col_title>,A,B,C,D,E,F.

     ./plotter/plotter.py  <exp_number> <file_name> <x_col_title>(optional)

- _<exp_number>_ is the number of the experiment(expects 1 or 2 only). If _<exp_number>_ is 1, the column that has the x-axis values will be expected to have the title "line sizes," otherwise, it should have the title "ways."

- _<file_name>_ is the **.csv** file that has the experiment data.
- _<x_col_title>_ is an optional parameter to explicitly specify the title of the column the has the x-axis values. If _<x_col_title>_ is given, _<exp_number>_ is ignored.

## Implementation Details

1. Replacement Policies

   - **Least Recently Used(LRU) policy idea:**

     Every set in the cache will have a member "leastUsed" which will increase every time a set is accessed but the memory line is not used. That is, "least used" indicates how much recently the memory line was **not** accessed. th For example, assume we have 4-way sets and we accessed the second block in set number 5. Now, all the blocks except the second have not been "recently used," thus, we increase the leastUsed value for all the block except the second block. Then, to find the memory line to be replaced (least recently used), we loop over all the block in the set and choose the one with the _largest_ value. Bottom line is: leastUsed increases when the line was accessed longer time ago and decreases the more recently it was accessed.

   - **Least Frequenctly Used(LFU) policy idea:**

     Every set block stores a counter(frequency). Every time a memory line is accessed, its frequency increases. Then, to find the line to be replaced(least frequently used), we loop over all the blocks in the set and choose the one with the _smallest_ value.

## Limitations

1. Code optimization

   In this project we didn't focus mainly on optimizing the code but rather on simulating the caching proccess correctly. Thus, some code parts are not optimized, some variables are allocated even if they are not needed, some loops are written explicitly more than ones instead of encapsulating them in a function. This can be further optimized but it would an overkill for this project so we prefered to keep it simple.

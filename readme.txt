File Description:
* src/cacheSim.cpp:
    Main function invoker file.
* src/Random.cpp, headers/Random.h
    Class files that implement the different 6 memory generators that are used in the two experiments.
* src/SetAssociativeCache.cpp, headers/SetAssociativeCache.h
    Class files that implement the set associative cache and the differnet flags of simulation (enums).
* src/Utilities.cpp, headers/Utilities.h
    Two files that contain the utility functions used to do the experiments and the different tests.
* headres/PCH.h
    A pre-compiled-header that has all the different used included files we used in the project.
* plotter/plotter.py
    A python script that plots the contents of the *.csv files and saves the figures in the folder ./plots/
* Makefile
    make cacheSim.exe:
        To compile the source and header files.
    make plotOutputs:
        To plot the outputs from the folder ./outputs
    make clean:
        To remove the not needed object files after the compilation.
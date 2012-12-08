NERSC8VisReadiness
==================
Vis code for evaluations of architectures for NERSC8

Example
==================
$ cd bin
$ cmake -DVTK_DIR=//work/ext/nersc-mic/VTK-build/ -DCMAKE_BUILD_TYPE=Debug ../ && make -j 4
$ ctest

Test project /work/ext/nersc-mic/NERSC8VisReadiness/bin
Start 1: N8IsoComb
1/2 Test #1: N8IsoComb ........................   Passed    8.96 sec
Start 2: N8IsoVPIC
2/2 Test #2: N8IsoVPIC ........................   Passed   95.75 sec

100% tests passed, 0 tests failed out of 2
Total Test time (real) = 104.72 sec

Intrumentation
==================
A log file is produced as the tests run recording each pipeline object's execution time.
For example the above run generated the following log:
$ cat N8Iso.log

# Fri Dec  7 18:41:18 2012
# N8ISO
#   dataset=/work/ext/nersc-mic/NERSC8VisReadiness/data//datasets/comb.vtk
#   array=Density
#   isoValues= 10 0.3 0.6
#   nPolyDivisions=1
#   nInputCells=429397
#   nIsoCells=1250428
0 vtkN8DataSetReader::RequestData 1.35493e+09 1.35493e+09 0.0927422
0 vtkN8ContourFilter::RequestData 1.35493e+09 1.35493e+09 2.06998
0 vtkN8TriangleFilter::RequestData 1.35493e+09 1.35493e+09 0.0339301
0 vtkN8InterpolatingSubdivisionFilter::RequestData 1.35493e+09 1.35493e+09 1.48752
0 TotalRunTime 1.35493e+09 1.35493e+09 7.9356

# Fri Dec  7 18:42:49 2012
# N8ISO
#   dataset=/work/ext/nersc-mic/NERSC8VisReadiness/data//datasets/vpic.vtk
#   array=absJ
#   isoValues= 1 0.024 0.054
#   nPolyDivisions=0
#   nInputCells=182923213
#   nIsoCells=19520002
0 vtkN8DataSetReader::RequestData 1.35493e+09 1.35493e+09 12.9656
0 vtkN8ContourFilter::RequestData 1.35493e+09 1.35493e+09 13.9747
0 TotalRunTime 1.35493e+09 1.35493e+09 90.0301

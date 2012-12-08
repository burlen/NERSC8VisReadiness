cmake_minimum_required(VERSION 2.8)

project(NERSC8Readiness)

set(VTK_DIR "../VTK-build" CACHE PATH "Path to a VTK build")
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

#TODO MPI lib/inc etc
add_definitions(-DSQTK_WITHOUT_MPI)

set(VTK_N8_SOURCES
  LogBuffer.cxx
  TestUtils.cxx
  vtkN8ContourFilter.cxx
  vtkN8DataSetReader.cxx
  vtkN8InterpolatingSubdivisionFilter.cxx
  vtkN8LinearSubdivisionFilter.cxx
  vtkN8TriangleFilter.cxx
  vtkSQLog.cxx
  )

set(N8_ISO_SOURCES
  N8Iso.cxx
  ${VTK_N8_SOURCES}
  )

add_executable(N8Iso ${N8_ISO_SOURCES})
target_link_libraries(N8Iso
  vtkParallelCore
  vtkIOLegacy
  vtkFiltersCore
  vtkFiltersModeling
  vtkRenderingCore
  vtkRenderingOpenGL
  vtkRenderingFreeType
  vtkRenderingFreeTypeOpenGL
  vtkTestingRendering
  vtkInteractionStyle
  )

enable_testing()

add_test(NAME N8IsoComb
  COMMAND N8Iso
  --baseline=baselines/N8Iso-Comb
  --dataset=datasets/comb.vtk
  --array=Density
  --isoValues 10 0.3 0.6
  --nPolyDivisions=1
  --camPos -20 -20 30
  --focalPt 10 0 30
  --viewUp 0 0 1
  --zoom=1.5
  )

add_test(NAME N8IsoVPIC
  COMMAND N8Iso
  --baseline=baselines/N8Iso-VPIC
  --dataset=datasets/vpic.vtk
  --array=absJ
  --isoValues 1 0.024 0.054
  --nPolyDivisions=0
  --camPos 1262 -508 -1597
  --focalPt 495 495 252
  --viewUp 1 0 0
  --zoom=1.5
  )

set(N8_CTESTS
  N8IsoVPIC
  N8IsoComb
  )
set_tests_properties(${N8_CTESTS} PROPERTIES TIMEOUT 300)

/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkN8LinearSubdivisionFilter.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkN8LinearSubdivisionFilter - generate a subdivision surface using the Linear Scheme
// .SECTION Description
// vtkN8LinearSubdivisionFilter is a filter that generates output by
// subdividing its input polydata. Each subdivision iteration create 4
// new triangles for each triangle in the polydata.

// .SECTION Thanks
// This work was supported by PHS Research Grant No. 1 P41 RR13218-01
// from the National Center for Research Resources.

// .SECTION See Also
// vtkN8InterpolatingSubdivisionFilter vtkButterflySubdivisionFilter

#ifndef __vtkN8LinearSubdivisionFilter_h
#define __vtkN8LinearSubdivisionFilter_h

#include "vtkFiltersModelingModule.h" // For export macro
#include "vtkN8InterpolatingSubdivisionFilter.h"

class vtkIntArray;
class vtkPointData;
class vtkPoints;
class vtkPolyData;

class VTKFILTERSMODELING_EXPORT vtkN8LinearSubdivisionFilter : public vtkN8InterpolatingSubdivisionFilter
{
public:
  // Description:
  // Construct object with NumberOfSubdivisions set to 1.
  static vtkN8LinearSubdivisionFilter *New();
  vtkTypeMacro(vtkN8LinearSubdivisionFilter,vtkN8InterpolatingSubdivisionFilter);

protected:
  vtkN8LinearSubdivisionFilter () {};
  ~vtkN8LinearSubdivisionFilter () {};

  void GenerateSubdivisionPoints (vtkPolyData *inputDS,
                                  vtkIntArray *edgeData,
                                  vtkPoints *outputPts,
                                  vtkPointData *outputPD);

private:
  vtkN8LinearSubdivisionFilter(const vtkN8LinearSubdivisionFilter&);  // Not implemented.
  void operator=(const vtkN8LinearSubdivisionFilter&);  // Not implemented.
};

#endif


// VTK-HeaderTest-Exclude: vtkN8LinearSubdivisionFilter.h

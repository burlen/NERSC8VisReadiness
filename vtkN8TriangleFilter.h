/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkN8TriangleFilter.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkN8TriangleFilter - convert input polygons and strips to triangles
// .SECTION Description
// vtkN8TriangleFilter generates triangles from input polygons and triangle
// strips.  It also generates line segments from polylines unless PassLines
// is off, and generates individual vertex cells from vtkVertex point lists
// unless PassVerts is off.

#ifndef __vtkN8TriangleFilter_h
#define __vtkN8TriangleFilter_h

#include "vtkFiltersCoreModule.h" // For export macro
#include "vtkPolyDataAlgorithm.h"

class VTKFILTERSCORE_EXPORT vtkN8TriangleFilter : public vtkPolyDataAlgorithm
{
public:
  static vtkN8TriangleFilter *New();
  vtkTypeMacro(vtkN8TriangleFilter,vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Turn on/off passing vertices through filter (default: on).
  // If this is on, then the input vertex cells will be broken
  // into individual vertex cells (one point per cell).  If it
  // is off, the input vertex cells will be ignored.
  vtkBooleanMacro(PassVerts,int);
  vtkSetMacro(PassVerts,int);
  vtkGetMacro(PassVerts,int);

  // Description:
  // Turn on/off passing lines through filter (default: on).
  // If this is on, then the input polylines will be broken
  // into line segments.  If it is off, then the input lines
  // will be ignored and the output will have no lines.
  vtkBooleanMacro(PassLines,int);
  vtkSetMacro(PassLines,int);
  vtkGetMacro(PassLines,int);

protected:
  vtkN8TriangleFilter() : PassVerts(1), PassLines(1) {};
  ~vtkN8TriangleFilter() {};

  // Usual data generation method
  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

  int PassVerts;
  int PassLines;
private:
  vtkN8TriangleFilter(const vtkN8TriangleFilter&);  // Not implemented.
  void operator=(const vtkN8TriangleFilter&);  // Not implemented.
};

#endif

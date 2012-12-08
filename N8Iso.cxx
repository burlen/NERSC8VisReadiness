#include "vtkPolyDataAlgorithm.h"
#include "vtkMultiProcessController.h"
#include "vtkTesting.h"
#include "vtkSQLog.h"
#include "TestUtils.h"

#include "vtkN8DataSetReader.h"
#include "vtkN8ContourFilter.h"
#include "vtkN8TriangleFilter.h"
#include "vtkN8LinearSubdivisionFilter.h"


#include "vtksys/CommandLineArguments.hxx"
typedef vtksys::CommandLineArguments argT;

#include <iostream>
using std::cerr;
using std::endl;
#include <string>
using std::string;
#include <vector>
using std::vector;

int main(int argc, char **argv)
{
  vtkMultiProcessController *controller=Initialize(&argc,&argv);
  int worldRank=controller->GetLocalProcessId();
  int worldSize=controller->GetNumberOfProcesses();

  // command line options &
  // default values
  string inputFileName;
  string array;
  string dataRoot = getenv("VTK_DATA_ROOT");
  string tempDir = "./";
  string baseline;
  string logFileName;
  double isoValues[3];
  int nPolyDivisions = 0;
  double camPos[3]={0.0, 0.0 , -10.0};
  double focalPt[3]={0.0, 0.0, 10.0};
  double viewUp[3]={0.0, 0.0, 1.0};
  double zoom=1.0;

  if (worldRank == 0)
    {
    // parse command line
    vtksys::CommandLineArguments arg;
    arg.StoreUnusedArguments(1);
    arg.Initialize(argc, argv);

    arg.AddArgument(
          "--dataset",
          argT::EQUAL_ARGUMENT,
          &inputFileName,
          "vtk file to process");

    arg.AddArgument(
          "--array",
          argT::EQUAL_ARGUMENT,
          &array,
          "array to process");

    arg.AddArgument(
          "--baseline",
          argT::EQUAL_ARGUMENT,
          &baseline,
          "name of baseline image");

    arg.AddArgument(
          "--log",
          argT::EQUAL_ARGUMENT,
          &logFileName,
          "where to write log file");

    arg.AddArgument(
          "--tempDir",
          argT::EQUAL_ARGUMENT,
          &tempDir,
          "where to write temp files");

    vector<double> tmpIsoVals;
    arg.AddArgument(
          "--isoValues",
          argT::MULTI_ARGUMENT,
          &tmpIsoVals,
          "space delimited list: n lo hi");

    arg.AddArgument(
          "--nPolyDivisions",
          argT::EQUAL_ARGUMENT,
          &nPolyDivisions,
          "number of times to divide polys before rendering");

    vector<double> tmpCamPos;
    arg.AddArgument(
          "--camPos",
          argT::MULTI_ARGUMENT,
          &tmpCamPos,
          "camera position");

    vector<double> tmpFocalPt;
    arg.AddArgument(
          "--focalPt",
          argT::MULTI_ARGUMENT,
          &tmpFocalPt,
          "camera position");

    vector<double> tmpViewUp;
    arg.AddArgument(
          "--viewUp",
          argT::MULTI_ARGUMENT,
          &tmpViewUp,
          "camera position");

    arg.AddArgument(
          "--zoom",
          argT::EQUAL_ARGUMENT,
          &zoom,
          "factor to zoom in by.");

    if ( !arg.Parse()
       || inputFileName.empty()
       || baseline.empty()
       || tmpIsoVals.size()!=3 )
      {
      cerr
        << "Usage: " << endl
        << arg.GetHelp() << endl
        << endl;
      return Finalize(controller,1);
      }

    std::copy(tmpIsoVals.begin(), tmpIsoVals.end(), isoValues);
    std::copy(tmpCamPos.begin(), tmpCamPos.end(), camPos);
    std::copy(tmpFocalPt.begin(), tmpFocalPt.end(), focalPt);
    std::copy(tmpViewUp.begin(), tmpViewUp.end(), viewUp);

    inputFileName=NativePath(dataRoot+"/"+inputFileName);
    }
  Broadcast(controller,inputFileName);
  Broadcast(controller,array);
  Broadcast(controller,baseline);
  Broadcast(controller,logFileName);
  Broadcast(controller,tempDir);
  controller->Broadcast(isoValues,3,0);
  controller->Broadcast(&nPolyDivisions,1,0);

  // log config
  vtkSQLog *log=vtkSQLog::GetGlobalInstance();
  if ( logFileName.empty() )
    {
    logFileName = tempDir+"/N8Iso.log";
    }
  logFileName = NativePath(logFileName);
  log->SetFileName(logFileName.c_str());
  log->SetGlobalLevel(1);
  log->GetHeader()
    << "# N8ISO" << "\n"
    << "#   dataset=" << inputFileName << "\n"
    << "#   array=" << array << "\n"
    << "#   isoValues=";
  for (size_t i=0; i<3; ++i)
    {
    log->GetHeader() << " " << isoValues[i];
    }
  log->GetHeader()
    << "\n"
    << "#   nPolyDivisions=" << nPolyDivisions << "\n";

  // dataset source
  vtkN8DataSetReader *src = vtkN8DataSetReader::New();
  src->SetFileName(inputFileName.c_str());

  // iso
  vtkN8ContourFilter *iso = vtkN8ContourFilter::New();
  iso->SetInputConnection(0, src->GetOutputPort());
  iso->SetComputeScalars(1);
  iso->GenerateValues((int)isoValues[0],isoValues[1],isoValues[2]);
  if (!array.empty())
    {
    iso->SetInputArrayToProcess(
        0,
        0,
        0,
        vtkDataObject::FIELD_ASSOCIATION_POINTS,
        array.c_str());
    }
  src->Delete();

  vtkPolyDataAlgorithm *alg = iso;

  if (nPolyDivisions)
    {
    // triangles
    vtkN8TriangleFilter *tri = vtkN8TriangleFilter::New();
    tri->SetPassVerts(0);
    tri->SetPassLines(0);
    tri->SetInputConnection(0, iso->GetOutputPort());
    iso->Delete();

    // subdivision
    vtkN8LinearSubdivisionFilter *sub = vtkN8LinearSubdivisionFilter::New();
    sub->SetNumberOfSubdivisions(nPolyDivisions);
    sub->SetInputConnection(0, tri->GetOutputPort());
    tri->Delete();

    alg = sub;
    }

  // execute
  GetParallelExec(worldRank, worldSize, alg, 0.0);
  alg->Update();

  // render
  int testStatus = SerialRender(
        controller,
        alg->GetOutput(),
        true,
        tempDir,
        baseline,
        "N8Iso",
        900,600,
        camPos[0], camPos[1],camPos[2],
        focalPt[0],focalPt[1],focalPt[2],
        viewUp[0],viewUp[1],viewUp[2],
        zoom);

  log->GetHeader()
    << "#   nInputCells=" << src->GetOutput()->GetNumberOfCells() << "\n"
    << "#   nIsoCells=" << alg->GetOutput()->GetNumberOfCells() << "\n";

  alg->Delete();

  return Finalize(controller,testStatus==vtkTesting::PASSED?0:1);
}

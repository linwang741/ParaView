/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkPVActorComposite.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 1998-1999 Kitware Inc. 469 Clifton Corporate Parkway,
Clifton Park, NY, 12065, USA.

All rights reserved. No part of this software may be reproduced, distributed,
or modified, in any form or by any means, without permission in writing from
Kitware Inc.

IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY DERIVATIVES THEREOF,
EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES, INCLUDING,
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE IS PROVIDED ON AN
"AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE NO OBLIGATION TO PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

=========================================================================*/
#include "vtkPVActorComposite.h"
#include "vtkKWWidget.h"
#include "vtkKWView.h"
#include "vtkObjectFactory.h"
#include "vtkKWWindow.h"
#include "vtkPVApplication.h"
#include "vtkPVData.h"
#include "vtkPVSource.h"
#include "vtkImageOutlineFilter.h"
#include "vtkGeometryFilter.h"
//#include "vtkPVImageTextureFilter.h"
#include "vtkTexture.h"

//----------------------------------------------------------------------------
vtkPVActorComposite* vtkPVActorComposite::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkPVActorComposite");
  if(ret)
    {
    return (vtkPVActorComposite*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkPVActorComposite;
}

int vtkPVActorCompositeCommand(ClientData cd, Tcl_Interp *interp,
			       int argc, char *argv[]);

//----------------------------------------------------------------------------
vtkPVActorComposite::vtkPVActorComposite()
{
  static int instanceCount = 0;

  // Create a unique id for creating tcl names.
  ++instanceCount;
  this->InstanceCount = instanceCount;
  
  this->CommandFunction = vtkPVActorCompositeCommand;
  
  this->Properties = vtkKWWidget::New();
  this->Name = NULL;

  this->Mapper->ImmediateModeRenderingOn();
  
  this->NumCellsLabel = vtkKWLabel::New();
  this->BoundsLabel = vtkKWLabel::New();
  this->XRangeLabel = vtkKWLabel::New();
  this->YRangeLabel = vtkKWLabel::New();
  this->ZRangeLabel = vtkKWLabel::New();
  this->ScalarRangeLabel = vtkKWLabel::New();
  
  this->AmbientScale = vtkKWScale::New();

  this->ColorMenu = vtkKWOptionMenu::New();
  
  this->PVData = NULL;
  this->DataSetInput = NULL;
  this->Mode = VTK_PV_ACTOR_COMPOSITE_POLY_DATA_MODE;
  
  //this->TextureFilter = NULL;
  
  this->ActorTclName = NULL;
  this->MapperTclName = NULL;
  this->OutlineTclName = NULL;
  this->GeometryTclName = NULL;
}


//----------------------------------------------------------------------------
void vtkPVActorComposite::CreateParallelTclObjects(vtkPVApplication *pvApp)
{
  int numProcs, id;
  char tclName[100];
  
  this->SetApplication(pvApp);
  
  // Get rid of previous object created by the superclass.
  if (this->Mapper)
    {
    this->Mapper->Delete();
    this->Mapper = NULL;
    }
  // Make a new tcl object.
  sprintf(tclName, "Mapper%d", this->InstanceCount);
  this->Mapper = (vtkPolyDataMapper*)pvApp->MakeTclObject("vtkPolyDataMapper", tclName);
  this->MapperTclName = NULL;
  this->SetMapperTclName(tclName);
  
  // Get rid of previous object created by the superclass.
  if (this->Actor)
    {
    this->Actor->Delete();
    this->Actor = NULL;
    }
  // Make a new tcl object.
  sprintf(tclName, "Actor%d", this->InstanceCount);
  this->Actor = (vtkActor*)pvApp->MakeTclObject("vtkActor", tclName);
  this->ActorTclName = NULL;
  this->SetActorTclName(tclName);
  
  pvApp->BroadcastScript("%s SetMapper %s", this->ActorTclName, 
			this->MapperTclName);
  
  // Hard code assignment based on processes.
  numProcs = pvApp->GetController()->GetNumberOfProcesses() ;
  for (id = 0; id < numProcs; ++id)
    {
    pvApp->RemoteScript(id, "%s SetNumberOfPieces %d", this->MapperTclName, numProcs);
    pvApp->RemoteScript(id, "%s SetPiece %d", this->MapperTclName, id);
    //pvApp->RemoteScript(id, "%s SetNumberOfPieces 2", this->MapperTclName);
    //pvApp->RemoteScript(id, "%s SetPiece 1", this->MapperTclName);
    }
}

//----------------------------------------------------------------------------
vtkPVActorComposite::~vtkPVActorComposite()
{
  vtkPVApplication *pvApp = this->GetPVApplication();

  this->Properties->Delete();
  this->Properties = NULL;
  
  this->SetName(NULL);
  
  this->NumCellsLabel->Delete();
  this->NumCellsLabel = NULL;
  
  this->BoundsLabel->Delete();
  this->BoundsLabel = NULL;
  this->XRangeLabel->Delete();
  this->XRangeLabel = NULL;
  this->YRangeLabel->Delete();
  this->YRangeLabel = NULL;
  this->ZRangeLabel->Delete();
  this->ZRangeLabel = NULL;
  this->ScalarRangeLabel->Delete();
  this->ScalarRangeLabel = NULL;
  
  this->AmbientScale->Delete();
  this->AmbientScale = NULL;
  
  this->ColorMenu->Delete();
  this->ColorMenu = NULL;
  
  this->SetInput(NULL);
  
  //if (this->TextureFilter != NULL)
  //{
  // this->TextureFilter->Delete();
  //this->TextureFilter = NULL;
  //}
  
  pvApp->BroadcastScript("%s Delete", this->MapperTclName);
  this->SetMapperTclName(NULL);
  this->Mapper = NULL;

  pvApp->BroadcastScript("%s Delete", this->ActorTclName);
  this->SetActorTclName(NULL);
  this->Actor = NULL;

  if (this->OutlineTclName)
    {
    pvApp->BroadcastScript("%s Delete", this->OutlineTclName);
    this->SetOutlineTclName(NULL);
    }
  
  if (this->GeometryTclName)
    {
    pvApp->BroadcastScript("%s Delete", this->GeometryTclName);
    this->SetGeometryTclName(NULL);
    }
  
}


//----------------------------------------------------------------------------
void vtkPVActorComposite::CreateProperties()
{
  const char *actorPage;
  vtkPVApplication *pvApp = this->GetPVApplication();  
    
  // invoke superclass always
  this->vtkKWActorComposite::CreateProperties();
  
  actorPage = this->GetClassName();
  this->Notebook->AddPage(actorPage);
  this->Properties->SetParent(this->Notebook->GetFrame(actorPage));
  this->Properties->Create(this->Application, "frame","");
  this->Script("pack %s -pady 2 -fill x -expand yes",
               this->Properties->GetWidgetName());
  
  this->NumCellsLabel->SetParent(this->Properties);
  this->NumCellsLabel->Create(this->Application, "");
  this->BoundsLabel->SetParent(this->Properties);
  this->BoundsLabel->Create(this->Application, "");
  this->BoundsLabel->SetLabel("bounds:");
  this->XRangeLabel->SetParent(this->Properties);
  this->XRangeLabel->Create(this->Application, "");
  this->YRangeLabel->SetParent(this->Properties);
  this->YRangeLabel->Create(this->Application, "");
  this->ZRangeLabel->SetParent(this->Properties);
  this->ZRangeLabel->Create(this->Application, "");
  this->ScalarRangeLabel->SetParent(this->Properties);
  this->ScalarRangeLabel->Create(this->Application, "");
  
  //this->ColorByCellCheck->SetParent(this->Properties);
  //this->ColorByCellCheck->Create(this->Application, "-text {Color By Cell Data:}");
  //this->ColorByCellCheck->SetCommand(this, "ColorByCellCheckCallBack");
  
  this->AmbientScale->SetParent(this->Properties);
  this->AmbientScale->Create(this->Application, "-showvalue 1");
  this->AmbientScale->DisplayLabel("Ambient Light");
  this->AmbientScale->SetRange(0.0, 1.0);
  this->AmbientScale->SetResolution(0.1);
  this->AmbientScale->SetCommand(this, "AmbientChanged");
  
  this->ColorMenu->SetParent(this->Properties);
  this->ColorMenu->Create(this->Application, "");    

  this->Script("pack %s",
	       this->NumCellsLabel->GetWidgetName());
  this->Script("pack %s",
	       this->BoundsLabel->GetWidgetName());
  this->Script("pack %s",
	       this->XRangeLabel->GetWidgetName());
  this->Script("pack %s",
	       this->YRangeLabel->GetWidgetName());
  this->Script("pack %s",
	       this->ZRangeLabel->GetWidgetName());
  this->Script("pack %s",
	       this->ScalarRangeLabel->GetWidgetName());
  this->Script("pack %s",
               this->ColorMenu->GetWidgetName());
    
  this->UpdateProperties();  
}


//----------------------------------------------------------------------------
void vtkPVActorComposite::UpdateProperties()
{
  char tmp[350], cmd[1024];
  float bounds[6];
  float range[2];
  int i, j, numArrays, numComps;
  vtkFieldData *fieldData;
  vtkPVApplication *pvApp = this->GetPVApplication();  
  vtkDataArray *array;

  pvApp->BroadcastScript("%s Update", this->MapperTclName);
  this->GetPVData()->GetBounds(bounds);
  this->GetPVData()->GetScalarRange(range);
      
  sprintf(tmp, "number of cells: %d", 
	  this->GetPVData()->GetNumberOfCells());
  this->NumCellsLabel->SetLabel(tmp);

  sprintf(tmp, "x range: %f to %f", bounds[0], bounds[1]);
  this->XRangeLabel->SetLabel(tmp);
  sprintf(tmp, "y range: %f to %f", bounds[2], bounds[3]);
  this->YRangeLabel->SetLabel(tmp);
  sprintf(tmp, "z range: %f to %f", bounds[4], bounds[5]);
  this->ZRangeLabel->SetLabel(tmp);
  sprintf(tmp, " Scalar Range: %f to %f", range[0], range[1]);
  this->ScalarRangeLabel->SetLabel(tmp);
  
    
  this->AmbientScale->SetValue(this->GetActor()->GetProperty()->GetAmbient());

  this->ColorMenu->AddEntryWithCommand("Property",
	                               this, "ColorByProperty");
  if (this->GetPVData()->GetVTKData()->GetPointData()->GetScalars())
    {
    this->ColorMenu->AddEntryWithCommand("Point Scalars",
					     this, "ColorByPointScalars");
    }
  if (this->GetPVData()->GetVTKData()->GetCellData()->GetScalars())
    {
    this->ColorMenu->AddEntryWithCommand("Cell Scalars",
					     this, "ColorByCellScalars");
    }
  fieldData = this->GetPVData()->GetVTKData()->GetPointData()->GetFieldData();
  if (fieldData)
    {
    numArrays = fieldData->GetNumberOfArrays();
    for (i = 0; i < numArrays; i++)
      {
      array = fieldData->GetArray(i);
      numComps = array->GetNumberOfComponents();
      for (j = 0; j < numComps; ++j)
        {
        sprintf(cmd, "ColorByPointFieldComponent %s %d",
                fieldData->GetArrayName(i), j);
        if (numComps == 1)
          {
          sprintf(tmp, "Point %s", fieldData->GetArrayName(i));
          } 
        else
          {
          sprintf(tmp, "Point %s %d", fieldData->GetArrayName(i), j);
          }
        this->ColorMenu->AddEntryWithCommand(tmp, this, cmd);
        } 
      }
    }
  fieldData = this->GetPVData()->GetVTKData()->GetCellData()->GetFieldData();
  if (fieldData)
    {
    numArrays = fieldData->GetNumberOfArrays();
    for (i = 0; i < numArrays; i++)
      {
      array = fieldData->GetArray(i);
      numComps = array->GetNumberOfComponents();
      for (j = 0; j < numComps; ++j)
        {
        sprintf(cmd, "ColorByCellFieldComponent %s %d",
                fieldData->GetArrayName(i), j);
        if (numComps == 1)
          {
          sprintf(tmp, "Cell %s", fieldData->GetArrayName(i));
          } 
        else
          {
          sprintf(tmp, "Cell %s %d", fieldData->GetArrayName(i), j);
          }
        this->ColorMenu->AddEntryWithCommand(tmp, this, cmd);
        } 
      }
    }

}

//----------------------------------------------------------------------------
void vtkPVActorComposite::ColorByProperty()
{
  vtkPVApplication *pvApp = this->GetPVApplication();
  pvApp->BroadcastScript("%s ScalarVisibilityOff", this->MapperTclName);
  this->GetView()->Render();
}
//----------------------------------------------------------------------------
void vtkPVActorComposite::ColorByPointScalars()
{
  vtkPVApplication *pvApp = this->GetPVApplication();

  pvApp->BroadcastScript("%s ScalarVisibilityOn", this->MapperTclName);
  pvApp->BroadcastScript("%s SetScalarModeToUsePointData",
                         this->MapperTclName);
  this->GetView()->Render();
}
//----------------------------------------------------------------------------
void vtkPVActorComposite::ColorByCellScalars()
{
  vtkPVApplication *pvApp = this->GetPVApplication();

  pvApp->BroadcastScript("%s ScalarVisibilityOn", this->MapperTclName);
  pvApp->BroadcastScript("%s SetScalarModeToUseCellData",
                         this->MapperTclName);
  this->GetView()->Render();
}
//----------------------------------------------------------------------------
void vtkPVActorComposite::ColorByPointFieldComponent(char *name, int comp)
{
  vtkPVApplication *pvApp = this->GetPVApplication();

  pvApp->BroadcastScript("%s ScalarVisibilityOn", this->MapperTclName);
  pvApp->BroadcastScript("%s SetScalarModeToUsePointFieldData",
                         this->MapperTclName);
  pvApp->BroadcastScript("%s ColorByArrayComponent %s %d",
                         this->MapperTclName, name, comp);
  this->GetView()->Render();
}
//----------------------------------------------------------------------------
void vtkPVActorComposite::ColorByCellFieldComponent(char *name, int comp)
{
  vtkPVApplication *pvApp = this->GetPVApplication();

  pvApp->BroadcastScript("%s ScalarVisibilityOn", this->MapperTclName);
  pvApp->BroadcastScript("%s SetScalarModeToUseCellFieldData",
                         this->MapperTclName);
  pvApp->BroadcastScript("%s ColorByArrayComponent %s %d",
                         this->MapperTclName, name, comp);
  this->GetView()->Render();
}


//----------------------------------------------------------------------------
void vtkPVActorComposite::AmbientChanged()
{
  vtkPVApplication *pvApp = this->GetPVApplication();
  float ambient = this->AmbientScale->GetValue();
  
  //pvApp->BroadcastScript("%s SetAmbient %f", this->GetTclName(), ambient);

  
  this->SetAmbient(ambient);
  this->GetView()->Render();
}

//----------------------------------------------------------------------------
void vtkPVActorComposite::SetAmbient(float ambient)
{
  this->GetActor()->GetProperty()->SetAmbient(ambient);
}

//----------------------------------------------------------------------------
void vtkPVActorComposite::ShowDataNotebook()
{
  this->GetPVData()->GetPVSource()->ShowProperties();
}




//----------------------------------------------------------------------------
void vtkPVActorComposite::Initialize()
{
  if (this->PVData->GetVTKData()->IsA("vtkPolyData"))
    {
    this->SetModeToPolyData();
    }
  else if (this->PVData->GetVTKData()->IsA("vtkImageData"))
    {
    int *ext;
    this->PVData->GetVTKData()->UpdateInformation();
    ext = this->PVData->GetVTKData()->GetWholeExtent();
    if (ext[1] > ext[0] && ext[3] > ext[2] && ext[5] > ext[4])
      {
      this->SetModeToImageOutline();
      }
    else
      {
      this->SetModeToDataSet();
      }      
    }
  else 
    {
    this->SetModeToDataSet();
    }

  // Mapper needs an input, so the mode needs to be set first.
  this->ResetScalarRange();
}

//----------------------------------------------------------------------------
void vtkPVActorComposite::SetInput(vtkPVData *data)
{
  vtkPVApplication *pvApp = this->GetPVApplication();
  char *vtkDataTclName = NULL;
  
  if (this->PVData == data)
    {
    return;
    }
  this->Modified();
  
  if (this->PVData)
    {
    // extra careful for circular references
    vtkPVData *tmp = this->PVData;
    this->PVData = NULL;
    tmp->UnRegister(this);
    }
  
  if (data)
    {
    this->PVData = data;
    data->Register(this);
    vtkDataTclName = data->GetVTKDataTclName();
    }
    
}

//----------------------------------------------------------------------------
void vtkPVActorComposite::ResetScalarRange()
{
  float range[2];

  this->GetInputScalarRange(range);
  if (range[0] > range[1])
    {
    return;
    }
  
  // The mapper complains if the range is a single point.
  if (range[0] == range[1])
    {
    range[1] += 0.0001;
    }
  this->SetScalarRange(range[0], range[1]);
}



//----------------------------------------------------------------------------
void vtkPVActorComposite::GetInputScalarRange(float range[2]) 
{ 
  vtkPVApplication *pvApp = this->GetPVApplication();
  vtkMultiProcessController *controller = pvApp->GetController();
  int numProcs = controller->GetNumberOfProcesses();
  
  pvApp->BroadcastScript("%s Update", this->MapperTclName);
  this->GetPVData()->GetScalarRange(range);
}

//----------------------------------------------------------------------------
void vtkPVActorComposite::SetScalarRange(float min, float max) 
{ 
  vtkPVApplication *pvApp = this->GetPVApplication();

  pvApp->BroadcastScript("%s SetScalarRange %f %f", this->MapperTclName, min, max);
}

//----------------------------------------------------------------------------
void vtkPVActorComposite::SetName (const char* arg) 
{ 
  vtkDebugMacro(<< this->GetClassName() << " (" << this << "): setting " 
                << this->Name << " to " << arg ); 
  if ( this->Name && arg && (!strcmp(this->Name,arg))) 
    { 
    return;
    } 
  if (this->Name) 
    { 
    delete [] this->Name; 
    } 
  if (arg) 
    { 
    this->Name = new char[strlen(arg)+1]; 
    strcpy(this->Name,arg); 
    } 
  else 
    { 
    this->Name = NULL;
    }
  this->Modified(); 
} 

//----------------------------------------------------------------------------
char* vtkPVActorComposite::GetName()
{
  return this->Name;
}

//----------------------------------------------------------------------------
void vtkPVActorComposite::Select(vtkKWView *v)
{
  // invoke super
  this->vtkKWComposite::Select(v); 
  vtkKWMenu* MenuProperties = v->GetParentWindow()->GetMenuProperties();
  char* rbv = 
    MenuProperties->CreateRadioButtonVariable(MenuProperties,"Radio");

  // now add our own menu options
  if (MenuProperties->GetRadioButtonValue(MenuProperties,"Radio") >= 10)
    {
    if (this->LastSelectedProperty == 10)
      {
      this->View->ShowViewProperties();
      }
    if (this->LastSelectedProperty == 100 || this->LastSelectedProperty == -1)
      {
      this->ShowProperties();
      }
    }

  delete [] rbv;
}

//----------------------------------------------------------------------------
void vtkPVActorComposite::Deselect(vtkKWView *v)
{
  // invoke super
  this->vtkKWComposite::Deselect(v);
}

//----------------------------------------------------------------------------
void vtkPVActorComposite::ShowProperties()
{
  vtkKWWindow *pw = this->View->GetParentWindow();
  pw->ShowProperties();
  pw->GetMenuProperties()->CheckRadioButton(pw->GetMenuProperties(),
					    "Radio", 100);

  // unpack any current children
  this->Script("catch {eval pack forget [pack slaves %s]}",
               this->View->GetPropertiesParent()->GetWidgetName());
  
  if (!this->PropertiesCreated)
    {
    this->InitializeProperties();
    }
  
  this->Script("pack %s -pady 2 -padx 2 -fill both -expand yes -anchor n",
               this->Notebook->GetWidgetName());
  this->View->PackProperties();
}

//----------------------------------------------------------------------------
void vtkPVActorComposite::SetVisibility(int v)
{
  vtkPVApplication *pvApp;
  pvApp = (vtkPVApplication*)(this->Application);
  if (this->ActorTclName)
    {
    pvApp->BroadcastScript("%s SetVisibility %d", this->ActorTclName, v);
    }
}
  
//----------------------------------------------------------------------------
int vtkPVActorComposite::GetVisibility()
{
  vtkProp *p = this->GetProp();
  
  if (p == NULL)
    {
    return 0;
    }
  
  return p->GetVisibility();
}

//----------------------------------------------------------------------------
vtkPVApplication* vtkPVActorComposite::GetPVApplication()
{
  if (this->Application == NULL)
    {
    return NULL;
    }
  
  if (this->Application->IsA("vtkPVApplication"))
    {  
    return (vtkPVApplication*)(this->Application);
    }
  else
    {
    vtkErrorMacro("Bad typecast");
    return NULL;
    } 
}

//----------------------------------------------------------------------------
// Not used.
void vtkPVActorComposite::SetMode(int mode)
{
  vtkPVApplication *pvApp = this->GetPVApplication();
  
  if (pvApp == NULL)
    {
    vtkErrorMacro("I cannot set the mode with no application.");
    }
  
  this->Mode = mode;

  if (this->PVData == NULL)
    {
    return;
    }

  if (mode == VTK_PV_ACTOR_COMPOSITE_POLY_DATA_MODE)
    {
    pvApp->BroadcastScript("%s SetInput %s", this->MapperTclName, 
			   this->PVData->GetVTKDataTclName());
    }
  else if (mode == VTK_PV_ACTOR_COMPOSITE_IMAGE_OUTLINE_MODE)
    {
    if (this->OutlineTclName == NULL)
      {
      char tclName[150];
      sprintf(tclName, "ActorCompositeOutline%d", this->InstanceCount);
      pvApp->MakeTclObject("vtkImageOutlineFilter", tclName);
      this->SetOutlineTclName(tclName);
      }
    pvApp->BroadcastScript("%s SetInput %s", this->OutlineTclName, 
			   this->PVData->GetVTKDataTclName());
    pvApp->BroadcastScript("%s SetInput [%s GetOutput]", 
			   this->MapperTclName, this->OutlineTclName);   
    }
  else if (mode == VTK_PV_ACTOR_COMPOSITE_DATA_SET_MODE)
    {
    if (this->GeometryTclName == NULL)
      {
      char tclName[150];
      sprintf(tclName, "ActorCompositeGeometry%d", this->InstanceCount);
      pvApp->MakeTclObject("vtkGeometryFilter", tclName);
      this->SetGeometryTclName(tclName);
      }
    pvApp->BroadcastScript("%s SetInput %s", this->GeometryTclName,
			   this->PVData->GetVTKDataTclName());
    pvApp->BroadcastScript("%s SetInput [%s GetOutput]", 
			   this->MapperTclName, this->GeometryTclName);    
    }
  
}



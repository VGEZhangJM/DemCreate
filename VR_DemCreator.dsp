# Microsoft Developer Studio Project File - Name="VR_DemCreator" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=VR_DemCreator - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VR_DemCreator.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VR_DemCreator.mak" CFG="VR_DemCreator - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VR_DemCreator - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "VR_DemCreator - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VR_DemCreator - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 GdiPlus.lib v4501v.lib /nologo /subsystem:windows /dll /machine:I386

!ELSEIF  "$(CFG)" == "VR_DemCreator - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../VRDLL/Lib/VR_DemCreator"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 GdiPlus.lib v4501v.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../VRBin/VR_DemCreatorD.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "VR_DemCreator - Win32 Release"
# Name "VR_DemCreator - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CalcKRGEquation.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcOLSquation.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcPolyInterpolation.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcRBFEquation.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_Aspect.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_AspectSet.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_Cells.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_Cluster_Analysis.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_Curvature.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_CurvatureSet.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_DemSet.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_DimensionAnalyse.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_GeoMorphological.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_GeoMorphological_CZX.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_GeoMorphological_HXY.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_GeoMorphological_LSD.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_Grid_Fractal.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_Grid_Height.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_Grid_IDW.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_Grid_KRG.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_Grid_PLY.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_Grid_RBF.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_Grid_SPD.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_GridCreator.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_GridDem.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_GridVertex.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_GridVertexList.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_HeightSet.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_LeastSquare.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_LineFeature.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_LineFeatureAddress.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_LocalTerrain.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_LocalTerrainCharacter.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_LocalTerrainCreator.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_MapCommand.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_MapTool.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_OppositeHeight.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_Param_IDW.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_Param_KRG.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_Param_LTC.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_Param_RBF.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_Param_SPD.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_PointFeature.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_PointFeatureAddress.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_PointPattern_KFunction.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_PointPattern_NN.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_SemiVariogram.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_Slope.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_SlopeSet.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_Statistics.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_TerrainCV.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_TerrainIncision.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_TerrainRelif.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_TerrainRoughness.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_TinCreator.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_TinEdge.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_TinEdgeAddress.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_TinPITACreator.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_TinTDTACreator.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_TinTriangle.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_TinTriangleList.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_TinTriangleLOP.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_TinVertex.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_TinVertexAddress.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_Wavelets.cpp
# End Source File
# Begin Source File

SOURCE=.\DCR_WorkSpace.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawHistogram.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawSemiVariogram.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface_DemCreator.cpp
# End Source File
# Begin Source File

SOURCE=.\SetCalcHeightDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetCALCLSD_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetCellCountDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetCheckPointDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetCurvatureDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetDeltaHPerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetDimensionAnalyseDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetFractalDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetFractalResultDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetGridCreatorIDWDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetGridCreatorKRGDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetGridCreatorPLYDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetGridCreatorRBFDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetGridCreatorSPDDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetIDWParamDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetImportTxtDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetKRGParamDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetLimitDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetLocalTCDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetMatInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetRBFParamDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetRECombineTEDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetRemainsErrorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetSearchPointDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetSearchPointExDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetSemiVariogramDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetSlopeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetSPDParamDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetStatDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetTerrainCVDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetTerrainCVResultDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetTerrainFactorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetTerrainRelifDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetTerrainRelifResultDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetWindownSize.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\VR_DemCreator.cpp
# End Source File
# Begin Source File

SOURCE=.\VR_DemCreator.def
# End Source File
# Begin Source File

SOURCE=.\VR_DemCreator.rc
# End Source File
# Begin Source File

SOURCE=.\WzdDirDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CalcKRGEquation.h
# End Source File
# Begin Source File

SOURCE=.\CalcOLSquation.h
# End Source File
# Begin Source File

SOURCE=.\CalcPolyInterpolation.h
# End Source File
# Begin Source File

SOURCE=.\CalcRBFEquation.h
# End Source File
# Begin Source File

SOURCE=.\DCR_Aspect.h
# End Source File
# Begin Source File

SOURCE=.\DCR_AspectSet.h
# End Source File
# Begin Source File

SOURCE=.\DCR_Cells.h
# End Source File
# Begin Source File

SOURCE=.\DCR_Cluster_Analysis.h
# End Source File
# Begin Source File

SOURCE=.\DCR_Curvature.h
# End Source File
# Begin Source File

SOURCE=.\DCR_CurvatureSet.h
# End Source File
# Begin Source File

SOURCE=.\DCR_DemSet.h
# End Source File
# Begin Source File

SOURCE=.\DCR_DimensionAnalyse.h
# End Source File
# Begin Source File

SOURCE=.\DCR_GeoMorphological.h
# End Source File
# Begin Source File

SOURCE=.\DCR_GeoMorphological_CZX.h
# End Source File
# Begin Source File

SOURCE=.\DCR_GeoMorphological_HXY.h
# End Source File
# Begin Source File

SOURCE=.\DCR_GeoMorphological_LSD.h
# End Source File
# Begin Source File

SOURCE=.\DCR_Grid_Fractal.h
# End Source File
# Begin Source File

SOURCE=.\DCR_Grid_Height.h
# End Source File
# Begin Source File

SOURCE=.\DCR_Grid_IDW.h
# End Source File
# Begin Source File

SOURCE=.\DCR_Grid_KRG.h
# End Source File
# Begin Source File

SOURCE=.\DCR_Grid_PLY.h
# End Source File
# Begin Source File

SOURCE=.\DCR_Grid_RBF.h
# End Source File
# Begin Source File

SOURCE=.\DCR_Grid_SPD.h
# End Source File
# Begin Source File

SOURCE=.\DCR_GridCreator.h
# End Source File
# Begin Source File

SOURCE=.\DCR_GridDem.h
# End Source File
# Begin Source File

SOURCE=.\DCR_GridVertex.h
# End Source File
# Begin Source File

SOURCE=.\DCR_GridVertexList.h
# End Source File
# Begin Source File

SOURCE=.\DCR_HeightSet.h
# End Source File
# Begin Source File

SOURCE=.\DCR_LeastSquare.h
# End Source File
# Begin Source File

SOURCE=.\DCR_LineFeature.h
# End Source File
# Begin Source File

SOURCE=.\DCR_LineFeatureAddress.h
# End Source File
# Begin Source File

SOURCE=.\DCR_LocalTerrain.h
# End Source File
# Begin Source File

SOURCE=.\DCR_LocalTerrainCharacter.h
# End Source File
# Begin Source File

SOURCE=.\DCR_LocalTerrainCreator.h
# End Source File
# Begin Source File

SOURCE=.\DCR_MapCommand.h
# End Source File
# Begin Source File

SOURCE=.\DCR_MapTool.h
# End Source File
# Begin Source File

SOURCE=.\DCR_OppositeHeight.h
# End Source File
# Begin Source File

SOURCE=.\DCR_Param_IDW.h
# End Source File
# Begin Source File

SOURCE=.\DCR_Param_KRG.h
# End Source File
# Begin Source File

SOURCE=.\DCR_Param_LTC.h
# End Source File
# Begin Source File

SOURCE=.\DCR_Param_RBF.h
# End Source File
# Begin Source File

SOURCE=.\DCR_Param_SPD.h
# End Source File
# Begin Source File

SOURCE=.\DCR_PointFeature.h
# End Source File
# Begin Source File

SOURCE=.\DCR_PointFeatureAddress.h
# End Source File
# Begin Source File

SOURCE=.\DCR_PointPattern_KFunction.h
# End Source File
# Begin Source File

SOURCE=.\DCR_PointPattern_NN.h
# End Source File
# Begin Source File

SOURCE=.\DCR_SemiVariogram.h
# End Source File
# Begin Source File

SOURCE=.\DCR_Slope.h
# End Source File
# Begin Source File

SOURCE=.\DCR_SlopeSet.h
# End Source File
# Begin Source File

SOURCE=.\DCR_Statistics.h
# End Source File
# Begin Source File

SOURCE=.\DCR_TerrainCV.h
# End Source File
# Begin Source File

SOURCE=.\DCR_TerrainIncision.h
# End Source File
# Begin Source File

SOURCE=.\DCR_TerrainRelif.h
# End Source File
# Begin Source File

SOURCE=.\DCR_TerrainRoughness.h
# End Source File
# Begin Source File

SOURCE=.\DCR_TinCreator.h
# End Source File
# Begin Source File

SOURCE=.\DCR_TinEdge.h
# End Source File
# Begin Source File

SOURCE=.\DCR_TinEdgeAddress.h
# End Source File
# Begin Source File

SOURCE=.\DCR_TinPITACreator.h
# End Source File
# Begin Source File

SOURCE=.\DCR_TinTDTACreator.h
# End Source File
# Begin Source File

SOURCE=.\DCR_TinTriangle.h
# End Source File
# Begin Source File

SOURCE=.\DCR_TinTriangleList.h
# End Source File
# Begin Source File

SOURCE=.\DCR_TinTriangleLOP.h
# End Source File
# Begin Source File

SOURCE=.\DCR_TinVertex.h
# End Source File
# Begin Source File

SOURCE=.\DCR_TinVertexAddress.h
# End Source File
# Begin Source File

SOURCE=.\DCR_Wavelets.h
# End Source File
# Begin Source File

SOURCE=.\DCR_WorkSpace.h
# End Source File
# Begin Source File

SOURCE=.\DrawHistogram.h
# End Source File
# Begin Source File

SOURCE=.\DrawSemiVariogram.h
# End Source File
# Begin Source File

SOURCE=.\IInterface_DemCreator.h
# End Source File
# Begin Source File

SOURCE=.\Interface_DemCreator.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SetCalcHeightDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetCALCLSD_Dlg.h
# End Source File
# Begin Source File

SOURCE=.\SetCellCountDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetCheckPointDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetCurvatureDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetDeltaHPerDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetDimensionAnalyseDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetFractalDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetFractalResultDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetGridCreatorIDWDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetGridCreatorKRGDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetGridCreatorPLYDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetGridCreatorRBFDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetGridCreatorSPDDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetIDWParamDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetImportTxtDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetKRGParamDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetKRGParamDlg1.h
# End Source File
# Begin Source File

SOURCE=.\SetLimitDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetLocalTCDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetMatInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetRBFParamDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetRECombineTEDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetRemainsErrorDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetSearchPointDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetSearchPointExDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetSemiVariogramDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetSlopeDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetSPDParamDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetStatDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetTerrainCVDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetTerrainCVResultDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetTerrainFactorDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetTerrainRelifDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetTerrainRelifResultDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetWindownSize.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\WzdDirDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\backgrou.bmp
# End Source File
# Begin Source File

SOURCE=.\res\backgroud.bmp
# End Source File
# Begin Source File

SOURCE=.\res\lishudu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pan.cur
# End Source File
# Begin Source File

SOURCE=.\res\VR_DemCreator.rc2
# End Source File
# Begin Source File

SOURCE=.\res\zoomin.cur
# End Source File
# Begin Source File

SOURCE=.\res\zoomout.cur
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

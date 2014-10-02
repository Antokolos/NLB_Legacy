# Microsoft Developer Studio Project File - Name="NLB_Bldr" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=NLB_Bldr - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NLB_Bldr.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NLB_Bldr.mak" CFG="NLB_Bldr - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NLB_Bldr - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "NLB_Bldr - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NLB_Bldr - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "NLB_Bldr - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "NLB_Bldr - Win32 Release"
# Name "NLB_Bldr - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CBookPage.cpp
# End Source File
# Begin Source File

SOURCE=.\CBookVar.cpp
# End Source File
# Begin Source File

SOURCE=.\CColorSelectButton.cpp
# End Source File
# Begin Source File

SOURCE=.\CDrawable.cpp
# End Source File
# Begin Source File

SOURCE=.\CLink.cpp
# End Source File
# Begin Source File

SOURCE=.\CModificationsList.cpp
# End Source File
# Begin Source File

SOURCE=.\CProbVar.cpp
# End Source File
# Begin Source File

SOURCE=.\CStringVar.cpp
# End Source File
# Begin Source File

SOURCE=.\CTriggeredString.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogBookVarProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogBookVars.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogComperr.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogFindTxt.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogLeafs.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogLinkProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogModification.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogPageProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogProbability.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogProbVarProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogSearchResults.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogStrVarProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogStrVars.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogTriggeredStringConditionProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogTriggeredStringProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogTriggeredStrings.cpp
# End Source File
# Begin Source File

SOURCE=.\funcs.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\NLB_Bldr.cpp
# End Source File
# Begin Source File

SOURCE=.\NLB_Bldr.rc
# End Source File
# Begin Source File

SOURCE=.\NLB_BldrDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\NLB_BldrView.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CBookPage.h
# End Source File
# Begin Source File

SOURCE=.\CBookVar.h
# End Source File
# Begin Source File

SOURCE=.\CColorSelectButton.h
# End Source File
# Begin Source File

SOURCE=.\CDrawable.h
# End Source File
# Begin Source File

SOURCE=.\CLink.h
# End Source File
# Begin Source File

SOURCE=.\CModificationsList.h
# End Source File
# Begin Source File

SOURCE=.\consts.h
# End Source File
# Begin Source File

SOURCE=.\CProbVar.h
# End Source File
# Begin Source File

SOURCE=.\CStringVar.h
# End Source File
# Begin Source File

SOURCE=.\CTriggeredString.h
# End Source File
# Begin Source File

SOURCE=.\DialogBookVarProperties.h
# End Source File
# Begin Source File

SOURCE=.\DialogBookVars.h
# End Source File
# Begin Source File

SOURCE=.\DialogComperr.h
# End Source File
# Begin Source File

SOURCE=.\DialogFindTxt.h
# End Source File
# Begin Source File

SOURCE=.\DialogLeafs.h
# End Source File
# Begin Source File

SOURCE=.\DialogLinkProperties.h
# End Source File
# Begin Source File

SOURCE=.\DialogModification.h
# End Source File
# Begin Source File

SOURCE=.\DialogPageProperties.h
# End Source File
# Begin Source File

SOURCE=.\DialogProbability.h
# End Source File
# Begin Source File

SOURCE=.\DialogProbVarProperties.h
# End Source File
# Begin Source File

SOURCE=.\DialogSearchResults.h
# End Source File
# Begin Source File

SOURCE=.\DialogStrVarProperties.h
# End Source File
# Begin Source File

SOURCE=.\DialogStrVars.h
# End Source File
# Begin Source File

SOURCE=.\DialogTriggeredStringConditionProperties.h
# End Source File
# Begin Source File

SOURCE=.\DialogTriggeredStringProperties.h
# End Source File
# Begin Source File

SOURCE=.\DialogTriggeredStrings.h
# End Source File
# Begin Source File

SOURCE=.\funcs.h
# End Source File
# Begin Source File

SOURCE=.\liststuff.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\NLB_Bldr.h
# End Source File
# Begin Source File

SOURCE=.\NLB_BldrDoc.h
# End Source File
# Begin Source File

SOURCE=.\NLB_BldrView.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\types.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\add_link.cur
# End Source File
# Begin Source File

SOURCE=.\add_page.cur
# End Source File
# Begin Source File

SOURCE=.\res\add_page.cur
# End Source File
# Begin Source File

SOURCE=.\arrow.cur
# End Source File
# Begin Source File

SOURCE=.\res\bookvar.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\leaf.ico
# End Source File
# Begin Source File

SOURCE=.\res\link.ico
# End Source File
# Begin Source File

SOURCE=.\res\NLB_Bldr.ico
# End Source File
# Begin Source File

SOURCE=.\res\NLB_Bldr.rc2
# End Source File
# Begin Source File

SOURCE=.\res\NLB_BldrDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\page.ico
# End Source File
# Begin Source File

SOURCE=.\res\probvar.ico
# End Source File
# Begin Source File

SOURCE=.\RES\strvar.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\trigstr.ico
# End Source File
# Begin Source File

SOURCE=.\res\void.ico
# End Source File
# End Group
# Begin Group "ASD"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ASD\CL1List.h
# End Source File
# Begin Source File

SOURCE=.\ASD\CStack.h
# End Source File
# End Group
# Begin Group "ClipFuncs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ClipFuncs\ClipFuncs.cpp
# End Source File
# Begin Source File

SOURCE=.\ClipFuncs\ClipFuncs.h
# End Source File
# End Group
# Begin Group "MathTrans"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MathTrans\CMathTrans.cpp
# End Source File
# Begin Source File

SOURCE=.\MathTrans\CMathTrans.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\NLB_Bldr.reg
# End Source File
# Begin Source File

SOURCE=.\res\nlbe_10.bdy
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDialogTriggeredStringConditionProperties
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "NLB_Bldr.h"
LastPage=0

ClassCount=21
Class1=CNLB_BldrApp
Class2=CNLB_BldrDoc
Class3=CNLB_BldrView
Class4=CMainFrame

ResourceCount=18
Resource1=IDD_DIALOG_BOOKVAR_PROPERTIES
Class5=CAboutDlg
Resource2=IDD_DIALOG_TRIGGERED_STRING_PROPERTIES
Class6=CDialogPageProperties
Resource3=IDD_DIALOG_LINK_PROPERTIES
Class7=CDialogLinkProperties
Resource4=IDD_DIALOG_PAGE_PROPERTIES
Class8=CDialogComperr
Resource5=IDD_DIALOG_TRIGGERED_STRINGS
Resource6=IDD_DIALOG_COMPERR
Class9=CDialogBookVarProperties
Class10=CDialogBookVars
Resource7=IDD_DIALOG_STRVARS
Class11=CDialogLeafs
Resource8=IDD_DIALOG_MODIFICATION
Class12=CDialogProbability
Resource9=IDD_DIALOG_PROBVAR_PROPERTIES
Class13=CDialogProbVarProperties
Resource10=IDD_DIALOG_SEARCH_RESULTS
Resource11=IDD_DIALOG_BOOKVARS
Class14=CDialogSearchResults
Class15=CDialogFindTxt
Resource12=IDD_DIALOG_FIND_TXT
Class16=CDialogModification
Resource13=IDR_MAINFRAME
Class17=CDialogStrVars
Resource14=IDD_DIALOG_STRVAR_PROPERTIES
Class18=CDialogStrVarProperties
Resource15=IDD_DIALOG_LEAFS
Class19=CDialogTriggeredStrings
Resource16=IDD_ABOUTBOX
Class20=CDialogTriggeredStringProperties
Resource17=IDD_DIALOG_PROBABILITY
Class21=CDialogTriggeredStringConditionProperties
Resource18=IDD_DIALOG_TRIGGERED_STRING_CONDITION_PROPERTIES

[CLS:CNLB_BldrApp]
Type=0
HeaderFile=NLB_Bldr.h
ImplementationFile=NLB_Bldr.cpp
Filter=N

[CLS:CNLB_BldrDoc]
Type=0
HeaderFile=NLB_BldrDoc.h
ImplementationFile=NLB_BldrDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=CNLB_BldrDoc

[CLS:CNLB_BldrView]
Type=0
HeaderFile=NLB_BldrView.h
ImplementationFile=NLB_BldrView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=ID_EDIT_TRIGGEREDSTRINGS


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=ID_EDIT_CHANGEALLPAGES




[CLS:CAboutDlg]
Type=0
HeaderFile=NLB_Bldr.cpp
ImplementationFile=NLB_Bldr.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1350569987
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_EXPORTTO_NLBENGINEV10
Command6=ID_FILE_MRU_FILE1
Command7=ID_APP_EXIT
Command8=ID_EDIT_CUT
Command9=ID_EDIT_COPY
Command10=ID_EDIT_PASTE
Command11=ID_EDIT_FIND
Command12=ID_EDIT_EDITPAGE_DELETEPAGE
Command13=ID_EDIT_EDITPAGE_EDITPROPERTIES
Command14=ID_EDIT_EDITLINK_DELETELINK
Command15=ID_EDIT_EDITLINK_EDITPROPERTIES
Command16=ID_EDIT_ADDSTARTPOINT
Command17=ID_EDIT_ADDLINK
Command18=ID_EDIT_ADDBOOKPAGE
Command19=ID_EDIT_SELECTALL
Command20=ID_EDIT_BOOKVARIABLES
Command21=ID_EDIT_PROBABILITYVARIABLES
Command22=ID_EDIT_STRINGVARIABLES
Command23=ID_EDIT_LEAFS
Command24=ID_EDIT_CHANGEALLPAGES
Command25=ID_VIEW_TOOLBAR
Command26=ID_VIEW_STATUS_BAR
Command27=ID_BUILD_COMPILE
Command28=ID_APP_ABOUT
CommandCount=28

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_EDIT_SELECTALL
Command2=ID_EDIT_COPY
Command3=ID_EDIT_FIND
Command4=ID_FILE_NEW
Command5=ID_FILE_OPEN
Command6=ID_FILE_SAVE
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_NEXT_PANE
Command11=ID_PREV_PANE
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_EDIT_CUT
Command15=ID_EDIT_UNDO
CommandCount=15

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_EDIT_FIND
Command8=ID_EDIT_ADDSTARTPOINT
Command9=ID_EDIT_ADDLINK
Command10=ID_EDIT_ADDBOOKPAGE
Command11=ID_EDIT_BOOKVARIABLES
Command12=ID_EDIT_PROBABILITYVARIABLES
Command13=ID_EDIT_STRINGVARIABLES
Command14=ID_EDIT_TRIGGEREDSTRINGS
Command15=ID_EDIT_LEAFS
Command16=ID_EDIT_EDITLINK_DELETELINK
Command17=ID_EDIT_EDITLINK_EDITPROPERTIES
Command18=ID_EDIT_EDITPAGE_DELETEPAGE
Command19=ID_EDIT_EDITPAGE_EDITPROPERTIES
Command20=ID_EDIT_CHANGEALLPAGES
Command21=ID_BUILD_COMPILE
Command22=ID_FILE_EXPORTTO_NLBENGINEV10
CommandCount=22

[DLG:IDD_DIALOG_PAGE_PROPERTIES]
Type=1
Class=CDialogPageProperties
ControlCount=24
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342312448
Control4=IDC_EDIT_PAGE_CAPTION,edit,1350631552
Control5=IDC_STATIC,static,1342312448
Control6=IDC_EDIT_PAGE_VAR,edit,1350631552
Control7=IDC_EDIT_PAGE_TEXT,edit,1353781380
Control8=IDC_STATIC,static,1342312448
Control9=IDC_BUTT_PAGE_CLR,button,1342242827
Control10=IDC_BUTT_BRDR_CLR,button,1342242827
Control11=IDC_BUTT_TXT_CLR,button,1342242827
Control12=IDC_LIST_LINK_NUMBERS,SysListView32,1350631437
Control13=IDC_BUTTON_MOVE_UP,button,1342242816
Control14=IDC_BUTTON_MOVE_DOWN,button,1342242816
Control15=IDC_BUTTON_EDIT,button,1342242816
Control16=IDC_BUTTON_DELETE,button,1342242816
Control17=IDC_STATIC,static,1342312448
Control18=IDC_BUTTON_REVERT_ORDER,button,1342242816
Control19=IDC_BUTTON_ABORT_MASS_OPERATION,button,1342242816
Control20=IDC_PROGRESS_MASS_OPERATION,msctls_progress32,1350565889
Control21=IDC_BUTTON_PREV_PAGE,button,1342242816
Control22=IDC_BUTTON_NEXT_PAGE,button,1342242816
Control23=IDC_STATIC_PERCENT_COMPLETE,static,1342308352
Control24=IDC_CHECK_USE_PAGE_CAPTION,button,1342242819

[CLS:CDialogPageProperties]
Type=0
HeaderFile=DialogPageProperties.h
ImplementationFile=DialogPageProperties.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDialogPageProperties

[DLG:IDD_DIALOG_LINK_PROPERTIES]
Type=1
Class=CDialogLinkProperties
ControlCount=12
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342312448
Control4=IDC_STATIC,static,1342312448
Control5=IDC_EDIT_LINK_CAPTION,edit,1350631552
Control6=IDC_EDIT_LINK_CONSTRAINTS,edit,1350631552
Control7=IDC_BUTTON_SET_LINK_COLOR,button,1342242827
Control8=IDC_LIST_MODIFICATIONS,SysListView32,1350631437
Control9=IDC_BUTTON_EDIT,button,1342242816
Control10=IDC_BUTTON_ADD,button,1342242816
Control11=IDC_BUTTON_DELETE,button,1342242816
Control12=IDC_STATIC,static,1342312448

[CLS:CDialogLinkProperties]
Type=0
HeaderFile=DialogLinkProperties.h
ImplementationFile=DialogLinkProperties.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_LIST_MODIFICATIONS
VirtualFilter=dWC

[DLG:IDD_DIALOG_COMPERR]
Type=1
Class=CDialogComperr
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDC_LIST_COMPERR,SysListView32,1350631437

[CLS:CDialogComperr]
Type=0
HeaderFile=DialogComperr.h
ImplementationFile=DialogComperr.cpp
BaseClass=CDialog
Filter=D
LastObject=CDialogComperr
VirtualFilter=dWC

[DLG:IDD_DIALOG_BOOKVARS]
Type=1
Class=CDialogBookVars
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDC_LIST_BOOKVARS,SysListView32,1350631437
Control3=IDC_BUTTON_EDIT_BOOKVAR,button,1342242816
Control4=IDC_BUTTON_ADD_BOOKVAR,button,1342242816
Control5=IDC_BUTTON_DELETE_BOOKVAR,button,1342242816

[DLG:IDD_DIALOG_BOOKVAR_PROPERTIES]
Type=1
Class=CDialogBookVarProperties
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342312448
Control4=IDC_STATIC_BOOKVAR_BODY,static,1342312448
Control5=IDC_EDIT_BOOKVAR_NAME,edit,1350631552
Control6=IDC_EDIT_BOOKVAR_BODY,edit,1350631552
Control7=IDC_CHECK_GLOBAL,button,1342242819
Control8=IDC_STATIC_INITVAL,static,1073872896
Control9=IDC_EDIT_INITVAL,edit,1082204288

[CLS:CDialogBookVarProperties]
Type=0
HeaderFile=DialogBookVarProperties.h
ImplementationFile=DialogBookVarProperties.cpp
BaseClass=CDialog
Filter=D
LastObject=CDialogBookVarProperties
VirtualFilter=dWC

[CLS:CDialogBookVars]
Type=0
HeaderFile=DialogBookVars.h
ImplementationFile=DialogBookVars.cpp
BaseClass=CDialog
Filter=D
LastObject=CDialogBookVars
VirtualFilter=dWC

[DLG:IDD_DIALOG_LEAFS]
Type=1
Class=CDialogLeafs
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDC_LIST_LEAFS,SysListView32,1350631437
Control3=IDC_BUTTON_GOTO,button,1342242816

[CLS:CDialogLeafs]
Type=0
HeaderFile=DialogLeafs.h
ImplementationFile=DialogLeafs.cpp
BaseClass=CDialog
Filter=D
LastObject=CDialogLeafs
VirtualFilter=dWC

[DLG:IDD_DIALOG_PROBABILITY]
Type=1
Class=CDialogProbability
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDC_LIST_PROBABILITY_VARS,SysListView32,1350631437
Control3=IDC_BUTTON_EDIT_PROBVAR,button,1342242816
Control4=IDC_BUTTON_ADD_PROBVAR,button,1342242816
Control5=IDC_BUTTON_DELETE_PROBVAR,button,1342242816

[CLS:CDialogProbability]
Type=0
HeaderFile=DialogProbability.h
ImplementationFile=DialogProbability.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_LIST_PROBABILITY_VARS
VirtualFilter=dWC

[DLG:IDD_DIALOG_PROBVAR_PROPERTIES]
Type=1
Class=CDialogProbVarProperties
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342312448
Control4=IDC_EDIT_PROBVAR_NAME,edit,1350631552
Control5=IDC_STATIC,button,1342177287
Control6=IDC_EDIT_DICE_K,edit,1342251138
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDIT_DICE_Y,edit,1342251136

[CLS:CDialogProbVarProperties]
Type=0
HeaderFile=DialogProbVarProperties.h
ImplementationFile=DialogProbVarProperties.cpp
BaseClass=CDialog
Filter=D
LastObject=CDialogProbVarProperties
VirtualFilter=dWC

[DLG:IDD_DIALOG_FIND_TXT]
Type=1
Class=CDialogFindTxt
ControlCount=12
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_STR_TO_FIND,edit,1350631552
Control5=IDC_CHECK_FIND_IN_PAGES,button,1342242819
Control6=IDC_CHECK_FIND_IN_LNKS,button,1342242819
Control7=IDC_CHECK_FIND_IN_BOOKVARS,button,1342242819
Control8=IDC_CHECK_FIND_IN_PROBVARS,button,1342242819
Control9=IDC_STATIC,button,1342177287
Control10=IDC_STATIC,button,1342177287
Control11=IDC_CHECK_CASE_SENSITIVE,button,1342242819
Control12=IDC_CHECK_WHOLE_WORDS,button,1342242819

[DLG:IDD_DIALOG_SEARCH_RESULTS]
Type=1
Class=CDialogSearchResults
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDC_LIST_RESULTS,SysListView32,1350631437

[CLS:CDialogSearchResults]
Type=0
HeaderFile=DialogSearchResults.h
ImplementationFile=DialogSearchResults.cpp
BaseClass=CDialog
Filter=D
LastObject=CDialogSearchResults
VirtualFilter=dWC

[CLS:CDialogFindTxt]
Type=0
HeaderFile=DialogFindTxt.h
ImplementationFile=DialogFindTxt.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDialogFindTxt

[DLG:IDD_DIALOG_MODIFICATION]
Type=1
Class=CDialogModification
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342312448
Control4=IDC_STATIC,static,1342312448
Control5=IDC_COMBO_VARNAME,combobox,1344339971
Control6=IDC_COMBO_MODIFICATION,combobox,1344339971

[CLS:CDialogModification]
Type=0
HeaderFile=DialogModification.h
ImplementationFile=DialogModification.cpp
BaseClass=CDialog
Filter=D
LastObject=CDialogModification
VirtualFilter=dWC

[CLS:CDialogStrVars]
Type=0
HeaderFile=DialogStrVars.h
ImplementationFile=DialogStrVars.cpp
BaseClass=CDialog
Filter=D
LastObject=CDialogStrVars
VirtualFilter=dWC

[DLG:IDD_DIALOG_STRVARS]
Type=1
Class=CDialogStrVars
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDC_LIST_STRVARS,SysListView32,1350631437
Control3=IDC_BUTTON_EDIT_STRVAR,button,1342242816
Control4=IDC_BUTTON_ADD_STRVAR,button,1342242816
Control5=IDC_BUTTON_DELETE_STRVAR,button,1342242816

[DLG:IDD_DIALOG_STRVAR_PROPERTIES]
Type=1
Class=CDialogStrVarProperties
ControlCount=14
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342312448
Control4=IDC_STATIC,static,1342312448
Control5=IDC_EDIT_STRVAR_NAME,edit,1350631552
Control6=IDC_EDIT_STRVAR_BODY,edit,1353781380
Control7=IDC_LIST_STRVAR_VARS,listbox,1353777409
Control8=IDC_BUTTON_MOVE_UP,button,1342242816
Control9=IDC_BUTTON_MOVE_DOWN,button,1342242816
Control10=IDC_BUTTON_REFRESH_LIST,button,1342242816
Control11=IDC_STATIC,static,1342312448
Control12=IDC_CHECK_SAVE_IN_THE_BOOKMARKS_FILE,button,1342242819
Control13=IDC_LIST_STRVAR_DEPENDENCIES,listbox,1353777409
Control14=IDC_STATIC,static,1342312448

[CLS:CDialogStrVarProperties]
Type=0
HeaderFile=DialogStrVarProperties.h
ImplementationFile=DialogStrVarProperties.cpp
BaseClass=CDialog
Filter=D
LastObject=CDialogStrVarProperties
VirtualFilter=dWC

[DLG:IDD_DIALOG_TRIGGERED_STRINGS]
Type=1
Class=CDialogTriggeredStrings
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDC_BUTTON_EDIT_TRIGGERED_STRING,button,1342242816
Control3=IDC_BUTTON_ADD_TRIGGERED_STRINGS,button,1342242816
Control4=IDC_BUTTON_DELETE_TRIGGERED_STRING,button,1342242816
Control5=IDC_LIST_TRIGGERED_STRINGS,SysListView32,1350631437

[CLS:CDialogTriggeredStrings]
Type=0
HeaderFile=DialogTriggeredStrings.h
ImplementationFile=DialogTriggeredStrings.cpp
BaseClass=CDialog
Filter=D
LastObject=CDialogTriggeredStrings
VirtualFilter=dWC

[DLG:IDD_DIALOG_TRIGGERED_STRING_PROPERTIES]
Type=1
Class=CDialogTriggeredStringProperties
ControlCount=11
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342312448
Control4=IDC_STATIC,static,1342312448
Control5=IDC_LIST_CONDITIONS,SysListView32,1350631437
Control6=IDC_ADD_CONDITION,button,1342242816
Control7=IDC_DELETE_CONDITION,button,1342242816
Control8=IDC_BUTTON_EDIT_CONDITION,button,1342242816
Control9=IDC_EDIT_TSVARNAME,edit,1350631552
Control10=IDC_STATIC,static,1342312448
Control11=IDC_COMBO_DEFAULT_VALUE,combobox,1344339971

[CLS:CDialogTriggeredStringProperties]
Type=0
HeaderFile=DialogTriggeredStringProperties.h
ImplementationFile=DialogTriggeredStringProperties.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDialogTriggeredStringProperties

[DLG:IDD_DIALOG_TRIGGERED_STRING_CONDITION_PROPERTIES]
Type=1
Class=CDialogTriggeredStringConditionProperties
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342312448
Control4=IDC_STATIC,static,1342312448
Control5=IDC_COMBO_CONDITION,combobox,1344339971
Control6=IDC_COMBO_VALUE,combobox,1344339971

[CLS:CDialogTriggeredStringConditionProperties]
Type=0
HeaderFile=DialogTriggeredStringConditionProperties.h
ImplementationFile=DialogTriggeredStringConditionProperties.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDialogTriggeredStringConditionProperties


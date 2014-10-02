// NLB_BldrDoc.cpp : implementation of the CNLB_BldrDoc class
//

#include "stdafx.h"
#include "NLB_Bldr.h"

#include "NLB_BldrDoc.h"
#include "consts.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNLB_BldrDoc

IMPLEMENT_DYNCREATE(CNLB_BldrDoc, CDocument)

BEGIN_MESSAGE_MAP(CNLB_BldrDoc, CDocument)
	//{{AFX_MSG_MAP(CNLB_BldrDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNLB_BldrDoc construction/destruction

CNLB_BldrDoc::CNLB_BldrDoc()
{
	// TODO: add one-time construction code here

}

CNLB_BldrDoc::~CNLB_BldrDoc()
{
}

BOOL CNLB_BldrDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_iSelectedPagesCount = 0;	// В новом документе еще ничего не выделено
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CNLB_BldrDoc serialization

void CNLB_BldrDoc::Serialize(CArchive& ar)
{	
	POSITION pos;	
	int iLnksCount;
	int iPagesCount;		
	int iBookVarsCount;
	int iStrVarsCount;
	int iProbVarsCount;
	S_NLB_Hdr NLB_Hdr;
	CBookVar* pCurBookVar;
	CStringVar* pCurStrVar;
	CLink* pCurLnk;
	S_BookVarModification* pBVModCur;
	CModificationsList* pModListCur;
	POSITION posModList;	// Позиция в списке модифицируемых переменных
	int iModCount;			// Число элементов списка модифицируемых переменных
	int i, j;	
	CTriggeredString* pTSCur;
	POSITION posTSList;	// Позиция в списке Triggered Strings
	int iTSCount;		// Число элементов списка Triggered Strings
	int iTSCPCount;		// Число элементов списка Condition Pair для текущей Triggered String
	S_TS_ConditionPair* ptscp;	// Текущая Condition Pair для текущей Triggered String
	POSITION posTSCPList;	// Позиция в списке Condition Pairs

	NLB_Hdr.szSignature = new char[iSignatureLen];	// Здесь char, а не TCHAR, так как подпись будет ВСЕГДА постоянна!
	if (!NLB_Hdr.szSignature) ((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_OUT_OF_MEMORY);	

	if (ar.IsStoring())
	{
		// storing code 		
		iLnksCount = m_lstLnks.GetCount();
		iPagesCount = m_lstPages.GetCount();
		iBookVarsCount = m_lstBookVars.GetCount();
		iStrVarsCount = m_lstStrVars.GetCount();
		iProbVarsCount = m_lstProbVars.GetCount();
		iTSCount = m_lstTriggeredStrings.GetCount();

		// Сначала запишем заголовок файла:
		strcpy(NLB_Hdr.szSignature, SZ_NLB_SIGNATURE);	// strcpy, а не _tcscpy
		for (i = 0; i < iSignatureLen; i++) ar << NLB_Hdr.szSignature[i];
		NLB_Hdr.dwNLB_Ver = 1;
		ar << NLB_Hdr.dwNLB_Ver;
		NLB_Hdr.dwReserved1 = 
			NLB_Hdr.dwReserved2 =
			NLB_Hdr.dwReserved3 = 
			NLB_Hdr.dwReserved4 = 
			NLB_Hdr.dwReserved5 = 0;
		ar << NLB_Hdr.dwReserved1 << NLB_Hdr.dwReserved2 << NLB_Hdr.dwReserved3
				<< NLB_Hdr.dwReserved4 << NLB_Hdr.dwReserved5;

		ar << iLnksCount << iPagesCount << iBookVarsCount << iStrVarsCount << iProbVarsCount << iTSCount;
		ar << m_iSelectedPagesCount;
		ar << FindPageIndexByPointer(m_pStartPoint);		

		pos = m_lstLnks.GetHeadPosition();		
		for (i=0; i<iLnksCount; i++)
		{		
			// Запишем ссылки (номера страниц в списке)
			pCurLnk = m_lstLnks.GetAt(pos);
			ar << FindPageIndexByPointer(pCurLnk->Get_pPageFrom());
			ar << FindPageIndexByPointer(pCurLnk->Get_pPageTo());
			// Запишем информацию о модифицируемых переменных
			pModListCur = &pCurLnk->Get_BVModList();
			posModList = pModListCur->GetModificationsHeadPosition();
			iModCount = pModListCur->GetModificationsCount();
			ar << iModCount;
			for (int j = 0; j < iModCount; j++)
			{
				pBVModCur = pModListCur->GetNextModification(posModList);
				ar << FindBookVarIndexByPointer(pBVModCur->pModifiedBookVar);	// Для сохранения информации о модификациях переменных
				ar << FindBookVarIndexByPointer(pBVModCur->pModificationBody);	// Для сохранения информации о модификациях переменных
			}

			pCurLnk->Serialize(ar);
			m_lstLnks.GetNext(pos);
		}
				
		posTSList = m_lstTriggeredStrings.GetHeadPosition();
		for (i=0; i<iTSCount; i++)
		{
			pTSCur = m_lstTriggeredStrings.GetAt(posTSList);
			pTSCur->Serialize(ar);

			iTSCPCount = pTSCur->GetConditionPairsCount();
			posTSCPList = pTSCur->GetConditionPairsHeadPosition();
			ar << FindStrVarIndexByPointer(pTSCur->GetDefaultValue());
			ar << iTSCPCount;
			for (int j = 0; j < iTSCPCount; j++)
			{
				ptscp = pTSCur->GetConditionPairAt(posTSCPList);
				ar << FindBookVarIndexByPointer(ptscp->pBVCondition);
				ar << FindStrVarIndexByPointer(ptscp->pStrVar);
				pTSCur->GetNextConditionPair(posTSCPList);
			}
			
			m_lstTriggeredStrings.GetNext(posTSList);
		}

		pos = m_lstPages.GetHeadPosition();			
		for (i=0; i<iPagesCount; i++)
		{			
			// Запишем номер этой страницы в списке
			ar << FindPageIndexByPointer(m_lstPages.GetAt(pos));

			m_lstPages.GetAt(pos)->Serialize(ar);
			m_lstPages.GetNext(pos);
		}
		
		pos = m_lstBookVars.GetHeadPosition();			
		for (i=0; i<iBookVarsCount; i++)
		{				
			pCurBookVar = m_lstBookVars.GetAt(pos);
			ar << FindBookVarIndexByPointer(pCurBookVar);	// Для сохранения информации о модификациях переменных и Triggered Strings
			pCurBookVar->Serialize(ar);			
			m_lstBookVars.GetNext(pos);
		}

		pos = m_lstStrVars.GetHeadPosition();			
		for (i=0; i<iStrVarsCount; i++)
		{				
			pCurStrVar = m_lstStrVars.GetAt(pos);			
			ar << FindStrVarIndexByPointer(pCurStrVar);		// Для информации о Triggered Strings
			pCurStrVar->Serialize(ar);			
			m_lstStrVars.GetNext(pos);
		}

		pos = m_lstProbVars.GetHeadPosition();			
		for (i=0; i<iProbVarsCount; i++)
		{						
			m_lstProbVars.GetAt(pos)->Serialize(ar);			
			m_lstProbVars.GetNext(pos);
		}
		
		SetModifiedFlag(FALSE);
	}
	else
	{
		// loading code				
		CBookPage page;
		CBookVar BookVar;
		CProbVar ProbVar;
		CStringVar StrVar;
		CTriggeredString TSVar;
		CLink lnk;	
		POSITION pos_remov;
		int iStartPointPos;
		int iPositionPageFrom;
		int iPositionPageTo;
		CBookPage* pp_old;
		CBookPage* pp_new;
		int iPagePosition;
		BOOL bStartPointChanged = FALSE;
		int iSVDefaultValue;
		int iSVCondition;
		int iSVValue;
		S_TS_ConditionPair scpCur;

		// Сначала считаем заголовок файла		
		for (i = 0; i < iSignatureLen; i++) ar >> NLB_Hdr.szSignature[i];
		if (strcmp(NLB_Hdr.szSignature, SZ_NLB_SIGNATURE) != 0) 	// Чтение провалилось, так как подпись не совпала.		
		{			
			CString strCaption;
			CString strMessage;
			strMessage.LoadString(IDS_SIGNATURE_CORRUPT);
			strCaption.LoadString(IDS_ERROR_MB_CAPT);
			MessageBox(AfxGetMainWnd()->GetSafeHwnd(), strMessage, strCaption, MB_ICONEXCLAMATION);
			if (NLB_Hdr.szSignature) delete []NLB_Hdr.szSignature;
			AfxThrowArchiveException(CArchiveException::badIndex, NULL); 						
			return;
		}
		
		ar >> NLB_Hdr.dwNLB_Ver;
		if (NLB_Hdr.dwNLB_Ver != 1)	// Чтение провалилось, так как версия не совпала.				
		{					
			CString strCaption;
			CString strMessage;
			strMessage.Format(IDS_VERSION_MISMATCH, NLB_Hdr.dwNLB_Ver);
			strCaption.LoadString(IDS_ERROR_MB_CAPT);
			MessageBox(AfxGetMainWnd()->GetSafeHwnd(), strMessage, strCaption, MB_ICONEXCLAMATION);
			if (NLB_Hdr.szSignature) delete []NLB_Hdr.szSignature;
			AfxThrowArchiveException(CArchiveException::badSchema, NULL); 						
			return;
		}
		
		ar >> NLB_Hdr.dwReserved1 >> NLB_Hdr.dwReserved2 >> NLB_Hdr.dwReserved3
				>> NLB_Hdr.dwReserved4 >> NLB_Hdr.dwReserved5;
				
		ar >> iLnksCount >> iPagesCount >> iBookVarsCount >> iStrVarsCount >> iProbVarsCount >> iTSCount;
		
		ar >> m_iSelectedPagesCount;		
		ar >> iStartPointPos;		
		if (iStartPointPos == -1)
		{
			m_pStartPoint = NULL;
			bStartPointChanged = TRUE;
		}

		for (i=0; i<iLnksCount; i++)
		{				
			CLink* pLinkCur;
			S_BookVarModification bvm;
			int iTmp;

			// Считаем СТАРЫЕ позиции ссылок на страницы в списке ссылок
			ar >> iPositionPageFrom;
			ar >> iPositionPageTo;			

			if ((iPositionPageFrom == -1)||(iPositionPageTo == -1)) // Это - ошибка, такого быть не может!
				((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_FILE_CORRUPT);

			// Считаем информацию о модифицируемых переменных
			ar >> iModCount;
			pModListCur = &lnk.Get_BVModList();			
			for (int j = 0; j < iModCount; j++)
			{
				ar >> iTmp;
				if (iTmp == -1) ((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_FILE_CORRUPT);
				bvm.pModifiedBookVar = (CBookVar*) iTmp;
				ar >> iTmp;
				if (iTmp == -1) ((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_FILE_CORRUPT);
				bvm.pModificationBody = (CBookVar*) iTmp;
				pModListCur->AddBookVarModification(bvm);
			}

			lnk.Serialize(ar);
			lnk.Get_pPageFrom() = (CBookPage*) iPositionPageFrom;
			lnk.Get_pPageTo() = (CBookPage*) iPositionPageTo;
			if ((pLinkCur = new CLink(lnk))==NULL) ((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_OUT_OF_MEMORY);			
			m_lstLnks.AddHead(pLinkCur);
			pModListCur->EmptyModificationsList();	// Подготовим для следующей порции
		}
							
		for (i=0; i<iTSCount; i++)
		{
			TSVar.Serialize(ar);
			if ((pTSCur = new CTriggeredString(TSVar))==NULL) ((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_OUT_OF_MEMORY);
			m_lstTriggeredStrings.AddHead(pTSCur);

			ar >> iSVDefaultValue;
			pTSCur->SetDefaultValue((CStringVar*) iSVDefaultValue);
			ar >> iTSCPCount;
			for (int j = 0; j < iTSCPCount; j++)
			{
				ar >> iSVCondition;
				ar >> iSVValue;
				scpCur.pBVCondition = (CBookVar*) iSVCondition;
				scpCur.pStrVar = (CStringVar*) iSVValue;
				pTSCur->AddConditionPair(scpCur);
			}			
		}

		for (i=0; i<iPagesCount; i++)
		{		
			CBookPage* pPageCur;
			ar >> iPagePosition;

			if (iPagePosition == -1) ((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_FILE_CORRUPT);

			page.Serialize(ar);
			if ((pPageCur = new CBookPage(page))==NULL) ((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_OUT_OF_MEMORY);
			
			m_lstPages.AddHead(pPageCur);
			pp_old = (CBookPage*) iPagePosition;
			pp_new = m_lstPages.GetHead();

			if ((!bStartPointChanged)&&(iStartPointPos == iPagePosition))
			{
				m_pStartPoint = (CBookPage*) pp_new;
				bStartPointChanged = TRUE;
			}

			RelinkPointers(m_lstLnks, pp_old, pp_new);
		}	

		for (i=0; i<iBookVarsCount; i++)
		{			
			CBookVar* pVarCur;
			int iTmp;

			ar >> iTmp;
			BookVar.Serialize(ar);
			if ((pVarCur = new CBookVar(BookVar))==NULL) ((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_OUT_OF_MEMORY);			

			pos = m_lstLnks.GetHeadPosition();		
			for (j=0; j<iLnksCount; j++)
			{					
				pCurLnk = m_lstLnks.GetNext(pos);
				RelinkModPointers(&(pCurLnk->Get_BVModList()), 
					&(pCurLnk->m_lstModifiedBookVarChanged), 
					&(pCurLnk->m_lstModificationBodyChanged), 
					(CBookVar*) iTmp, pVarCur);				
			}
			
			posTSList = m_lstTriggeredStrings.GetHeadPosition();
			for (j=0; j<iTSCount; j++)
			{
				pTSCur = m_lstTriggeredStrings.GetNext(posTSList);				
				RelinkTSVarBVPointers(pTSCur, (CBookVar*) iTmp, pVarCur);
			}

			m_lstBookVars.AddHead(pVarCur);
		}

		for (i=0; i<iStrVarsCount; i++)
		{			
			CStringVar* pVarCur;
			int iTmp;

			ar >> iTmp;
			StrVar.Serialize(ar);
			if ((pVarCur = new CStringVar(StrVar))==NULL) ((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_OUT_OF_MEMORY);
			
			posTSList = m_lstTriggeredStrings.GetHeadPosition();
			for (int j=0; j<iTSCount; j++)
			{
				pTSCur = m_lstTriggeredStrings.GetNext(posTSList);			
				if (iTmp == ((int) pTSCur->GetDefaultValue())) pTSCur->SetDefaultValue(pVarCur);
				RelinkTSVarSVPointers(pTSCur, (CStringVar*) iTmp, pVarCur);
			}

			m_lstStrVars.AddHead(pVarCur);
		}

		posTSList = m_lstTriggeredStrings.GetHeadPosition();
		for (i=0; i<iTSCount; i++)
		{
			pTSCur = m_lstTriggeredStrings.GetNext(posTSList);
			if (((int) pTSCur->GetDefaultValue()) == -1) pTSCur->SetDefaultValue(NULL);
		}

		for (i=0; i<iProbVarsCount; i++)
		{			
			CProbVar* pProbVarCur;

			ProbVar.Serialize(ar);
			if ((pProbVarCur = new CProbVar(ProbVar))==NULL) ((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_OUT_OF_MEMORY);
			m_lstProbVars.AddHead(pProbVarCur);
		}
		
		// Очистим вспомогательные списки...
		pos = m_lstPosFromChanged.GetHeadPosition();
		while (!m_lstPosFromChanged.IsEmpty())
		{
			pos_remov = pos;
			m_lstPosFromChanged.GetNext(pos);
			m_lstPosFromChanged.RemoveAt(pos_remov);
		}

		pos = m_lstPosToChanged.GetHeadPosition();
		while (!m_lstPosToChanged.IsEmpty())
		{
			pos_remov = pos;
			m_lstPosToChanged.GetNext(pos);
			m_lstPosToChanged.RemoveAt(pos_remov);
		}		
		
		POSITION posBVC;
		pos = m_lstLnks.GetHeadPosition();		
		for (i=0; i<iLnksCount; i++)
		{					
			pCurLnk = m_lstLnks.GetNext(pos);
			posBVC = pCurLnk->m_lstModifiedBookVarChanged.GetHeadPosition();
			while (!pCurLnk->m_lstModifiedBookVarChanged.IsEmpty())
			{
				pos_remov = posBVC;
				pCurLnk->m_lstModifiedBookVarChanged.GetNext(posBVC);
				pCurLnk->m_lstModifiedBookVarChanged.RemoveAt(pos_remov);
			}					
			posBVC = pCurLnk->m_lstModificationBodyChanged.GetHeadPosition();
			while (!pCurLnk->m_lstModificationBodyChanged.IsEmpty())
			{
				pos_remov = posBVC;
				pCurLnk->m_lstModificationBodyChanged.GetNext(posBVC);
				pCurLnk->m_lstModificationBodyChanged.RemoveAt(pos_remov);
			}					
		}

		posTSList = m_lstTriggeredStrings.GetHeadPosition();
		for (i=0; i<iTSCount; i++)
		{
			pTSCur = m_lstTriggeredStrings.GetNext(posTSList);			
			posBVC = pTSCur->m_lstConditionBookVarChanged.GetHeadPosition();
			while (!pTSCur->m_lstConditionBookVarChanged.IsEmpty())
			{
				pos_remov = posBVC;
				pTSCur->m_lstConditionBookVarChanged.GetNext(posBVC);
				pTSCur->m_lstConditionBookVarChanged.RemoveAt(pos_remov);
			}
			posBVC = pTSCur->m_lstValueStrVarChanged.GetHeadPosition();
			while (!pTSCur->m_lstValueStrVarChanged.IsEmpty())
			{
				pos_remov = posBVC;
				pTSCur->m_lstValueStrVarChanged.GetNext(posBVC);
				pTSCur->m_lstValueStrVarChanged.RemoveAt(pos_remov);
			}
		}
		
		SetModifiedFlag(TRUE);
		UpdateAllViews(NULL);
	}

	if (NLB_Hdr.szSignature) delete []NLB_Hdr.szSignature;
}

VOID CNLB_BldrDoc::RelinkPointers(const CList<CLink*, CLink*&>& lstWhere, CBookPage* pp_old, CBookPage* pp_new)
{
	POSITION pos = lstWhere.GetHeadPosition();
	CBookPage* pFrom;
	CBookPage* pTo;
	int iLnksCount = lstWhere.GetCount();

	for (int i=0; i<iLnksCount; i++)
	{	
		pFrom = lstWhere.GetAt(pos)->Get_pPageFrom();
		pTo = lstWhere.GetAt(pos)->Get_pPageTo();

		if (pFrom == pp_old) 
		{
			if (!m_lstPosFromChanged.Find(pos))	// Т.е. не нашел такую => можем менять
			{
				lstWhere.GetAt(pos)->Get_pPageFrom() = pp_new;
				m_lstPosFromChanged.AddHead(pos);	// Теперь такая уже есть => оберегли себя от повторного изменения указателя
			}
		}
		if (pTo == pp_old) 
		{
			if (!m_lstPosToChanged.Find(pos))	// Т.е. не нашел такую => можем менять
			{
				lstWhere.GetAt(pos)->Get_pPageTo() = pp_new;
				m_lstPosToChanged.AddHead(pos);	// Теперь такая уже есть => оберегли себя от повторного изменения указателя
			}
		}

		lstWhere.GetNext(pos);
	}
}

VOID CNLB_BldrDoc::RelinkModPointers(CModificationsList* plstWhere, 
									 CList<POSITION, POSITION>* plstModifiedBookVarChanged, 
									 CList<POSITION, POSITION>* plstModificationBodyChanged, 
									 CBookVar* pv_old, 
									 CBookVar* pv_new)
{
	POSITION pos = plstWhere->GetModificationsHeadPosition();
	int iModCount = plstWhere->GetModificationsCount();
	S_BookVarModification* pModCur;

	for (int i=0; i<iModCount; i++)
	{	
		pModCur = plstWhere->GetModificationAt(pos);
		
		if (pModCur->pModifiedBookVar == pv_old) 
		{
			if (!plstModifiedBookVarChanged->Find(pos))	// Т.е. не нашел такую => можем менять
			{
				pModCur->pModifiedBookVar = pv_new;
				plstModifiedBookVarChanged->AddHead(pos);	// Теперь такая уже есть => оберегли себя от повторного изменения указателя
			}
		}
		if (pModCur->pModificationBody == pv_old) 
		{
			if (!plstModificationBodyChanged->Find(pos))	// Т.е. не нашел такую => можем менять
			{
				pModCur->pModificationBody = pv_new;
				plstModificationBodyChanged->AddHead(pos);	// Теперь такая уже есть => оберегли себя от повторного изменения указателя
			}
		}

		plstWhere->GetNextModification(pos);
	}
}

VOID CNLB_BldrDoc::RelinkTSVarBVPointers(CTriggeredString* pTSVarCur,
									 CBookVar* pv_old, 
									 CBookVar* pv_new)
{
	POSITION pos = pTSVarCur->GetConditionPairsHeadPosition();
	int iCPCount = pTSVarCur->GetConditionPairsCount();
	S_TS_ConditionPair* ptscpCur;

	for (int i=0; i<iCPCount; i++)
	{	
		ptscpCur = pTSVarCur->GetNextConditionPair(pos);
		
		if (ptscpCur->pBVCondition == pv_old) 
		{
			if (!pTSVarCur->m_lstConditionBookVarChanged.Find(pos))	// Т.е. не нашел такую => можем менять
			{
				ptscpCur->pBVCondition = pv_new;
				pTSVarCur->m_lstConditionBookVarChanged.AddHead(pos);	// Теперь такая уже есть => оберегли себя от повторного изменения указателя
			}
		}
	}
}

VOID CNLB_BldrDoc::RelinkTSVarSVPointers(CTriggeredString* pTSVarCur,
									 CStringVar* pv_old, 
									 CStringVar* pv_new)
{
	POSITION pos = pTSVarCur->GetConditionPairsHeadPosition();
	int iCPCount = pTSVarCur->GetConditionPairsCount();
	S_TS_ConditionPair* ptscpCur;

	for (int i=0; i<iCPCount; i++)
	{	
		ptscpCur = pTSVarCur->GetNextConditionPair(pos);
		
		if (ptscpCur->pStrVar == pv_old) 
		{
			if (!pTSVarCur->m_lstValueStrVarChanged.Find(pos))	// Т.е. не нашел такую => можем менять
			{
				ptscpCur->pStrVar = pv_new;
				pTSVarCur->m_lstValueStrVarChanged.AddHead(pos);	// Теперь такая уже есть => оберегли себя от повторного изменения указателя
			}
		}
	}
}

int CNLB_BldrDoc::FindPageIndexByPointer(const CBookPage* pPage) const
{
	POSITION pos;	// Позиция в списке
	int iPagePos = -1;	// Предполагаем, что такой страницы нет...
	int iPagesCount = m_lstPages.GetCount();
	CBookPage* pCurPage;

	if (pPage)	// Если pPage == NULL, то такой страницы в списке точно нет...
	{
		pos = m_lstPages.GetHeadPosition();			
		for (int i=0; i<iPagesCount; i++)
		{			
			pCurPage = m_lstPages.GetAt(pos);
			if (pCurPage == pPage)
			{
				iPagePos = i;
				break;
			}

			m_lstPages.GetNext(pos);
		}	
	}

	return iPagePos;
}

int CNLB_BldrDoc::FindPageIndexByVar(const CString& strPageVar) const
{
	POSITION pos;	// Позиция в списке
	int iPagePos = -1;	// Предполагаем, что такой страницы нет...
	int iPagesCount = m_lstPages.GetCount();
	CBookPage* pCurPage;

	if (strPageVar.GetLength() == 0) return -1;	// Считаем, что пустые имена недопустимы
	pos = m_lstPages.GetHeadPosition();			
	for (int i=0; i<iPagesCount; i++)
	{			
		pCurPage = m_lstPages.GetAt(pos);
		if (pCurPage->Get_strPageVar() == strPageVar)
		{
			iPagePos = i;
			break;
		}

		m_lstPages.GetNext(pos);
	}

	return iPagePos;
}

POSITION CNLB_BldrDoc::FindPagePositionByName(const CString& strPageVar) const
{
	POSITION pos;	// Позиция в списке	
	int iPagesCount = m_lstPages.GetCount();
	CBookPage* pCurPage;

	if (strPageVar.GetLength() == 0) return NULL;	// Считаем, что пустые имена недопустимы
	pos = m_lstPages.GetHeadPosition();			
	for (int i=0; i<iPagesCount; i++)
	{			
		pCurPage = m_lstPages.GetAt(pos);
		if (pCurPage->Get_strPageVar() == strPageVar) return pos;

		m_lstPages.GetNext(pos);
	}

	return NULL;
}

POSITION CNLB_BldrDoc::FindBookVarPositionByName(const CString& strBookVar) const
{
	POSITION pos = NULL;	// Позиция в списке	
	int iBookVarsCount = m_lstBookVars.GetCount();
	CBookVar* pCurBookVar;

	pos = m_lstBookVars.GetHeadPosition();			
	for (int i=0; i<iBookVarsCount; i++)
	{			
		pCurBookVar = m_lstBookVars.GetAt(pos);
		if (pCurBookVar->Get_strBookVarName() == strBookVar)
		{
			return pos;
			break;
		}

		m_lstBookVars.GetNext(pos);
	}

	return NULL;
}

int CNLB_BldrDoc::FindBookVarIndexByPointer(const CBookVar* pBVar) const
{
	POSITION pos = NULL;	// Позиция в списке	
	int iBookVarsCount = m_lstBookVars.GetCount();
	CBookVar* pCurBookVar;

	pos = m_lstBookVars.GetHeadPosition();			
	for (int i=0; i<iBookVarsCount; i++)
	{			
		pCurBookVar = m_lstBookVars.GetAt(pos);
		if (pCurBookVar == pBVar)
		{
			return i;
			break;
		}

		m_lstBookVars.GetNext(pos);
	}

	return -1;	// Типа ошибка
}

POSITION CNLB_BldrDoc::FindStrVarPositionByName(const CString& strStrVar) const
{
	POSITION pos = NULL;	// Позиция в списке	
	int iStrVarsCount = m_lstStrVars.GetCount();
	CStringVar* pCurStrVar;

	pos = m_lstStrVars.GetHeadPosition();			
	for (int i=0; i<iStrVarsCount; i++)
	{			
		pCurStrVar = m_lstStrVars.GetAt(pos);
		if (pCurStrVar->Get_strStrVarName() == strStrVar)
		{
			return pos;
			break;
		}

		m_lstStrVars.GetNext(pos);
	}

	return NULL;
}

POSITION CNLB_BldrDoc::FindTSVarPositionByName(const CString& strTSVar) const
{
	POSITION pos = NULL;	// Позиция в списке	
	int iTSVarsCount = m_lstTriggeredStrings.GetCount();
	CTriggeredString* pCurTSVar;

	pos = m_lstTriggeredStrings.GetHeadPosition();			
	for (int i=0; i<iTSVarsCount; i++)
	{			
		pCurTSVar = m_lstTriggeredStrings.GetAt(pos);
		if (pCurTSVar->Get_strTSName() == strTSVar)
		{
			return pos;
			break;
		}

		m_lstTriggeredStrings.GetNext(pos);
	}

	return NULL;
}

int CNLB_BldrDoc::FindStrVarIndexByPointer(const CStringVar* pBVar) const
{
	POSITION pos = NULL;	// Позиция в списке	
	int iStrVarsCount = m_lstStrVars.GetCount();
	CStringVar* pCurStrVar;

	pos = m_lstStrVars.GetHeadPosition();			
	for (int i=0; i<iStrVarsCount; i++)
	{			
		pCurStrVar = m_lstStrVars.GetAt(pos);
		if (pCurStrVar == pBVar)
		{
			return i;
			break;
		}

		m_lstStrVars.GetNext(pos);
	}

	return -1;	// Типа ошибка
}

POSITION CNLB_BldrDoc::FindProbVarPositionByName(const CString& strProbVar) const
{
	POSITION pos = NULL;	// Позиция в списке	
	int iProbVarsCount = m_lstProbVars.GetCount();
	CProbVar* pCurProbVar;

	pos = m_lstProbVars.GetHeadPosition();			
	for (int i=0; i<iProbVarsCount; i++)
	{			
		pCurProbVar = m_lstProbVars.GetAt(pos);
		if (pCurProbVar->Get_strProbVarName() == strProbVar)
		{
			return pos;
			break;
		}

		m_lstProbVars.GetNext(pos);
	}

	return NULL;
}

BOOL CNLB_BldrDoc::IsExistLink(const CLink& lnk)
{
	POSITION pos;	// Позиция в списке
	CLink* pCurLink;
	int iLnksCount = m_lstLnks.GetCount();

	pos = m_lstLnks.GetHeadPosition();			
	for (int i=0; i<iLnksCount; i++)
	{
		pCurLink = m_lstLnks.GetAt(pos);
		if (*pCurLink == lnk) return TRUE;
		m_lstLnks.GetNext(pos);
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CNLB_BldrDoc diagnostics

#ifdef _DEBUG
void CNLB_BldrDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNLB_BldrDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNLB_BldrDoc commands



void CNLB_BldrDoc::DeleteContents() 
{
	POSITION pos;
	POSITION pos_remov;
	CTriggeredString* pTSCur;

	// Очистим списки текущего документа
	pos = m_lstLnks.GetHeadPosition();
	while (!m_lstLnks.IsEmpty())
	{			
		pos_remov = pos;
		m_lstLnks.GetNext(pos);
		delete (m_lstLnks.GetAt(pos_remov));
		m_lstLnks.RemoveAt(pos_remov);
	}

	pos = m_lstPages.GetHeadPosition();
	while (!m_lstPages.IsEmpty())
	{
		pos_remov = pos;
		m_lstPages.GetNext(pos);
		delete (m_lstPages.GetAt(pos_remov));
		m_lstPages.RemoveAt(pos_remov);
	}
	m_pStartPoint = NULL;	// на всякий пожарный...	

	pos = m_lstBookVars.GetHeadPosition();
	while (!m_lstBookVars.IsEmpty())
	{
		pos_remov = pos;
		m_lstBookVars.GetNext(pos);
		delete (m_lstBookVars.GetAt(pos_remov));
		m_lstBookVars.RemoveAt(pos_remov);
	}

	pos = m_lstProbVars.GetHeadPosition();
	while (!m_lstProbVars.IsEmpty())
	{
		pos_remov = pos;
		m_lstProbVars.GetNext(pos);
		delete (m_lstProbVars.GetAt(pos_remov));
		m_lstProbVars.RemoveAt(pos_remov);
	}

	pos = m_lstStrVars.GetHeadPosition();
	while (!m_lstStrVars.IsEmpty())
	{
		pos_remov = pos;
		m_lstStrVars.GetNext(pos);
		delete (m_lstStrVars.GetAt(pos_remov));
		m_lstStrVars.RemoveAt(pos_remov);
	}

	pos = m_lstTriggeredStrings.GetHeadPosition();
	while (!m_lstTriggeredStrings.IsEmpty())
	{
		pos_remov = pos;
		m_lstTriggeredStrings.GetNext(pos);
		pTSCur = (CTriggeredString*) m_lstTriggeredStrings.GetAt(pos_remov);
		pTSCur->EmptyTSList();
		delete (pTSCur);
		m_lstTriggeredStrings.RemoveAt(pos_remov);
	}
	
	CDocument::DeleteContents();
}

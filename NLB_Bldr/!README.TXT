1) ���� �� 㭨�⮦��� ������, ᮧ������ LoadIcon?
�᫨ ��, � ���� ����ࠢ��� ��ࠡ��稪 Build->Compile

2) ������� ⠪�� ���窠 �� OnMouseMove:
 m_fIsResizingNow = FALSE; - ��� ������ ������! ����� ������� ������� (����������� ������ ��� ����������� PaintInMemory)

3) if (m_pDlgComperr) delete m_pDlgComperr;	
   if (m_pDlgSearchRes) delete m_pDlgSearchRes;
�����-� ��। �⨬ ���� �� �஢����, �� ᮮ⢥�����騥 �������
�������. ������, ��, ������, ᮢ��蠥��� ��⮬���᪨, ⠪ ��� 
�� ����⪥ �� OnDestroy ��� View �맢��� DestroyWindow ��� 
������� �ਢ���� � �஢���...

4) // (***) -- ����⨬ �� �� �� ����... ��������, ��� ����� ���� �஡����...
�஡���� ����� ���� � UNICODE

5) else if ((strCurPage[i]=='-') && _istspace((_TXCHAR) strCurPage[i+1]))
(�� � Convert_To_NLB_Engine_v10())
������ ���������, ����୮, ����� ����୮ ��ࠡ��뢠�� ⠪��:
aaaaaaaa -
aaaaa
(��᫥ �� - �� �஡��, � \r)
�� �த� �� �� �� ࠡ�⠥�. �� �����, �� _istspace �ࠢ��쭮 ��।����
\r ��� �஡��? � ��ࠢ��, �� �� ࠢ�� ����᭮...

6) ����� �� ��뢠�� TrimLeft() � TrimRight() ����� CString ��� ��ࠬ��஢?
7) �஡���� - ��᪮��筠� ४���� � �맮��� ��ப���� �㭪権. �������筮
InfiniteLoop � ��६����� �����, �� �� ��ࠧ�� ��㤭�� �஢����.
���� �� ����� �� �஢������, � ���짮��⥫� ������ ����� ���ந�� ��᪮�����
४���� � StringVariables.
IDS_STRERR_SELFREFERENCE_FOUND - ������ �������� � �� ��� �����
8) �᫨ �� �� ��ࠢ�� - ।���஢���� �訡���� String Variables � ⮬
��砥, �᫨ � ᮮ⢥�����饬 MessageBox � ConvertTo... ����� ������ Yes.
9) MoveUp - MoveDown � StringVariableProp-s
10) CList<...> lst;
lst.Serialize(ar);
��� ������ ������! ������ ���� ��������� �������������! ���������!
/*
xlat.cpp

XLat - �������������

*/

/* Revision: 1.00 22.12.2000 $ */

/*
Modify:
  22.12.2000 SVS
    + ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop
#include "internalheaders.hpp"


/* $ 05.09.2000 SVS
  XLat-�������������!
  �� ������ ������� EditSwap by SVS :-)))
*/
char* WINAPI Xlat(
   char *Line,                    // �������� ������
   int StartPos,                  // ������ �������������������
   int EndPos,                    // ����� �������������������
   struct CharTableSet *TableSet, // ���������������� ������� (����� ���� NULL)
   DWORD Flags)                   // ����� (��. enum XLATMODE)
{
  BYTE Chr,ChrOld;
  int J,I;
  int PreLang=2,CurLang=2; // uncnown
  int LangCount[2]={0,0};
  int IsChange=0;

  /* $ 08.09.2000 SVS
     �������� �������� :-)))
  */
  if(!Line || *Line == 0)
    return NULL;
  /* SVS $ */

  I=strlen(Line);

  if(EndPos > I)
    EndPos=I;

  if(StartPos < 0)
    StartPos=0;

  if(StartPos > EndPos || StartPos >= I)
    return Line;


//  OemToCharBuff(Opt.QWERTY.Table[0],Opt.QWERTY.Table[0],80);???
  if(!Opt.XLat.Table[0][0] || !Opt.XLat.Table[1][0])
    return Line;


  I=strlen((char *)Opt.XLat.Table[1]),
  J=strlen((char *)Opt.XLat.Table[0]);
  int MinLenTable=(I > J?J:I);

  if (TableSet)
    // �� ������� ��������� � OEM
    DecodeString(Line+StartPos,(LPBYTE)TableSet->DecodeTable,EndPos-StartPos+1);

  // ���� �� ���� ������
  for(J=StartPos; J < EndPos; J++)
  {
    ChrOld=Chr=(BYTE)Line[J];
    // ChrOld - ���� ������
    IsChange=0;
    // ���� �� ��������� Chr � ��������
    for(I=0; I < MinLenTable; ++I)
    {
      // ������ �� ��������?
      if(Chr == (BYTE)Opt.XLat.Table[1][I])
      {
        Chr=(char)Opt.XLat.Table[0][I];
        IsChange=1;
        CurLang=1; // pred - english
        LangCount[1]++;
        break;
      }
      // ������ �� �������?
      else if(Chr == (BYTE)Opt.XLat.Table[0][I])
      {
        Chr=(char)Opt.XLat.Table[1][I];
        CurLang=0; // pred - russian
        LangCount[0]++;
        IsChange=1;
        break;
      }
    }

    if(!IsChange) // ������ ������...
    {
      PreLang=CurLang;
      if(LangCount[0] > LangCount[1])
        CurLang=0;
      else
        CurLang=1;
      if(PreLang != CurLang)
        CurLang=PreLang;

        for(I=0; I < sizeof(Opt.XLat.Rules[0]) && Opt.XLat.Rules[0][I]; I+=2)
          if(ChrOld == (BYTE)Opt.XLat.Rules[CurLang][I])
          {
             Chr=(BYTE)Opt.XLat.Rules[CurLang][I+1];
             break;
          }
    }

    Line[J]=(char)Chr;
  }

  if (TableSet)
    // �� OEM � ������� ���������
    EncodeString(Line+StartPos,(LPBYTE)TableSet->EncodeTable,EndPos-StartPos+1);

  // ����������� ��������� ����������?
  //  � ��������� �� �������� ��� Win9x - ������� WinNT � ������������� :-)
  /* $ 20.09.2000 SVS
     ������� ������� ������� � ������� ���� ������ FAR.
  */
  if(hFarWnd && (Flags & XLAT_SWITCHKEYBLAYOUT))
  {
    PostMessage(hFarWnd,WM_INPUTLANGCHANGEREQUEST, 1, HKL_NEXT);
    /* $ 04.11.2000 SVS
       ������ �������� ������, ���� ����.
    */
    if(Flags & XLAT_SWITCHKEYBBEEP)
      MessageBeep(0);
    /* SVS $ */
  }
  /* SVS $ */

  return Line;
}
/* SVS $ */

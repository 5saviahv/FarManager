/*
farexcpt.cpp

��� ��� ����������

*/

/* Revision: 1.17 10.09.2002 $ */

/*
Modify:
  10.09.2002 SVS
    ! strcpy -> strncpy
  02.07.2002 SVS
    - BugZ#374 - ���������� ������� ��� ������ �� ����
      ��! � ������� �� ��� ����������. � � ��� ����� ������� ���������...
      � �����... ��� ����������, �� ������� ����� ;-)
  21.02.2002 SVS
    + � ����������� ���������� ���������� ���� �������� ��������� �����
      ������ ���������� (ProcessException) � ��������� ��������� �������.
  19.02.2002 SVS
    ! �� �������� ������� ��������. ���� �����������, �� � ���������
      (��������������) �� ����� ������ �� ������ �����.
      ��� �����, ��� ����� � ��� - ���� ����, ���� :-(
  25.01.2002 SVS
    - ����, � ����� ������ ��������� MSVC... :-((
  25.01.2002 SVS
    ! ��������� � �������� (WriteEvent) � ������ ��������� � ���������� �����.
    - ������ � farexcpt.cpp::GetLogicalAddress()
    ! ������ pXXXXXX �������� ������ ����� (FuncFlags)
    ! ���� ������ ��������� ;-)
  24.01.2002 SVS
    ! ��� �� ������� �������� (WriteEvent) - � ��� � ��� ���� �������� ��
      ������
  22.01.2002 SVS
    ! ��������� � PLUGINRECORD
    + System/UseFarevent:DWORD = 1 ������ � ����, �� ��������� ���������
  02.11.2001 SVS
    ! ������ �� ����������� ��������� ������!
    ! ��������, �� ������ ��������� ������� ����� �����, �������� �����
      WriteEvent()
  03.10.2001 SVS
    ! ������� ��������� ��������� ���������� �������� � "��������" �������!
  18.09.2001 SVS
    + EXCEPTION_FLT_* - ������� ��������� ��������� �����
  16.09.2001 SVS
    ! ����������� ����������
    ! ������� ������ "Debug" �� �������������
  10.07.2001 SVS
    + FARAREARECORD.ScrWH - ������� ������ - ������, ������
  26.06.2001 SVS
    + IsDebuggerPresent() - ������ ������� (������������������)
  06.06.2001 SVS
    ! Mix/Max
  17.05.2001 SVS
    -  local variable 'xpn' used without having been initialized
  16.05.2001 SVS
    ! ��������� ���������������� ������� EVENTPROC � ��������� WriteEvent
    ! ������ ������� PLUGINRECORD �������� � ��������� ������� WritePLUGINRECORD()
  16.05.2001 SVS
    + Created
    ! "farexcpt.dmp" ������������ � ����� ����� -  "farevent.dmp"
      � ���� ���� �������� ����������� � ���� ������, �� ������� ��,
      ��� �������� EventLog ��� ������ :-)))
    ! DumpExceptionInfo from syslog.cpp
    ! DumpExceptionInfo ������������ � WriteEvent
    ! xfilter from exception.cpp
    ! ��������� ��������� �����
*/

#include "headers.hpp"
#pragma hdrstop

#include "plugin.hpp"
#include "lang.hpp"
#include "global.hpp"
#include "fn.hpp"
#include "plugins.hpp"
#include "macro.hpp"
#include "filepanels.hpp"
#include "ctrlobj.hpp"
#include "manager.hpp"
#include "farexcpt.hpp"
#include "BlockExtKey.hpp"

#define MAX_DELTA_CODE  100

int WriteEvent(DWORD DumpType, // FLOG_*
               EXCEPTION_POINTERS *xp,
               struct PluginItem *Module,
               void *RawData,DWORD RawDataSize,
               DWORD RawDataFlags,DWORD RawType)
{
  return 0;
}

/* ************************************************************************
   $ 16.10.2000 SVS
   ����������� ���������� ����������.
*/
static char* xFromMSGTitle(int From)
{
  if(From == EXCEPT_SETSTARTUPINFO || From == EXCEPT_MINFARVERSION)
    return MSG(MExceptTitleLoad);
  else
    return MSG(MExceptTitle);
}

int xfilter(
    int From,                 // ������: 0 = OpenPlugin, 1 = OpenFilePlugin
    EXCEPTION_POINTERS *xp,   // ������ ��������
    struct PluginItem *Module,// ������, ��������� � ����������.
    DWORD Flags)              // �������������� ����� - ���� ������ ����
                              //        0x1 - ���������� ��� ��������?
{
   ProcessException=TRUE;
   BlockExtKey blockExtKey;

   struct __ECODE {
     DWORD Code;     // ��� ����������
     DWORD IdMsg;    // ID ��������� �� LNG-�����
     DWORD RetCode;  // ��� ������?
   } ECode[]={
     {EXCEPTION_ACCESS_VIOLATION, MExcRAccess, EXCEPTION_EXECUTE_HANDLER},
     {EXCEPTION_ARRAY_BOUNDS_EXCEEDED, MExcOutOfBounds, EXCEPTION_EXECUTE_HANDLER},
     {EXCEPTION_INT_DIVIDE_BY_ZERO,MExcDivideByZero, EXCEPTION_EXECUTE_HANDLER},
     {EXCEPTION_STACK_OVERFLOW,MExcStackOverflow, EXCEPTION_EXECUTE_HANDLER},
     {EXCEPTION_BREAKPOINT,MExcBreakPoint, EXCEPTION_EXECUTE_HANDLER},
     {EXCEPTION_FLT_DIVIDE_BY_ZERO,MExcFloatDivideByZero,EXCEPTION_EXECUTE_HANDLER},
     {EXCEPTION_FLT_OVERFLOW,MExcFloatOverflow,EXCEPTION_EXECUTE_HANDLER},
     {EXCEPTION_FLT_STACK_CHECK,MExcFloatStackOverflow,EXCEPTION_EXECUTE_HANDLER},
     {EXCEPTION_FLT_UNDERFLOW,MExcFloatUnderflow,EXCEPTION_EXECUTE_HANDLER},
     // ���� ���������.
   };
   // EXCEPTION_CONTINUE_EXECUTION  ??????
   char *pName;
   int  I, rc, Ret=1;
   char Buf[2][NM];
   char TruncFileName[2*NM];
   BOOL Unload = FALSE; // ���������� � ������, ���� ������ ����� ���������

   // ������� ������ ����������
   EXCEPTION_RECORD *xr = xp->ExceptionRecord;

   // ������� ���� ����� ������� ���������
   if (xr->ExceptionCode != STATUS_INVALIDFUNCTIONRESULT)
     WriteEvent(FLOG_ALL&(~FLOG_PLUGINSINFO),xp,Module,NULL,0);

   // CONTEXT ����� ������������ ��� ����������� ��� ������ � ���
   //         ����������� ���������...
   // CONTEXT *xc = xp->ContextRecord;

   rc = EXCEPTION_EXECUTE_HANDLER;

   /*$ 23.01.2001 skv
     ����������� ���������� �� ����� ������������.
   */
   pName=NULL;
   strncpy(TruncFileName,NullToEmpty(Module->ModuleName),sizeof(TruncFileName));

   /* $ 26.02.2001 VVM
       ! ��������� STATUS_INVALIDFUNCTIONRESULT */
   // ���� ����� ������������ � ������ �������, �.�. ��� �������� "�� ��������"
   if (From == EXCEPT_GETPLUGININFO_DATA || From == EXCEPT_GETOPENPLUGININFO_DATA)
   {
     I = 0;
     static const char *NameField[2][3]={
       {"DiskMenuStrings","PluginMenuStrings","PluginConfigStrings"},
       {"InfoLines","DescrFiles","PanelModesArray"},};
     switch(From)
     {
       case EXCEPT_GETPLUGININFO_DATA:
         pName = "PluginInfo";
         I = 0;
         break;
       case EXCEPT_GETOPENPLUGININFO_DATA:
         pName = "OpenPluginInfo";
         I = 1;
         break;
     }

     if(xr->ExceptionCode >= STATUS_STRUCTWRONGFILLED &&
        xr->ExceptionCode <= STATUS_STRUCTWRONGFILLED+2)
     {
       sprintf(Buf[0],
           MSG(MExcStructField),
           pName,
           NameField[I][xr->ExceptionCode-STATUS_STRUCTWRONGFILLED]);
     }
     else
       sprintf(Buf[0],MSG(MExcStructWrongFilled),pName);

     if(FrameManager && !FrameManager->ManagerIsDown())
       Message(MSG_WARNING,1,
            xFromMSGTitle(From),
            MSG(MExcTrappedException),
            MSG(MExcCheckOnLousys),
            TruncPathStr(TruncFileName,40),
            Buf[0],
            "\1",
            MSG(MExcUnloadYes),
            MSG(MOk));
   } /* EXCEPT_GETPLUGININFO_DATA && EXCEPT_GETOPENPLUGININFO_DATA */

   // ������ ���������� ���������� �� �������� 0 ������ INVALID_HANDLE_VALUE
   // �� Open*Plugin()
   else if (xr->ExceptionCode == STATUS_INVALIDFUNCTIONRESULT)
   {
     switch (From)
     {
       case EXCEPT_OPENPLUGIN:
         pName="OpenPlugin";
         break;
       case EXCEPT_OPENFILEPLUGIN:
         pName="OpenFilePlugin";
         break;
       case EXCEPT_OPENPLUGIN_FINDLIST:
         pName="OpenPlugin(OPEN_FINDLIST)";
         break;
       default:
         pName="???";
         break;
     }

     sprintf(Buf[0],MSG(MExcInvalidFuncResult),pName);
     if(FrameManager && !FrameManager->ManagerIsDown())
       Message(MSG_WARNING, 1,
                 xFromMSGTitle(From),
                 MSG(MExcTrappedException),
                 MSG(MExcCheckOnLousys),
                 TruncPathStr(TruncFileName,40),
                 Buf[0],
                 "\1",
                 MSG(MExcUnloadYes),
                 MSG(MOk));
   }

   else
   {
     // ���������� "��������" FAR`� ���������� � ����������...
     for(I=0; I < sizeof(ECode)/sizeof(ECode[0]); ++I)
       if(ECode[I].Code == xr->ExceptionCode)
       {
         pName=MSG(ECode[I].IdMsg);
         rc=ECode[I].RetCode;
         if(xr->ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
         {
           sprintf(Buf[1],MSG(xr->ExceptionInformation[0]+MExcRAccess),xr->ExceptionInformation[1]);
           pName=Buf[1];
         }
         break;
       }

     if (!pName) pName=MSG(MExcUnknown);

     sprintf(Buf[0],MSG(MExcAddress),xr->ExceptionAddress);
     if(FrameManager && !FrameManager->ManagerIsDown())
       Message(MSG_WARNING,1,
               xFromMSGTitle(From),
               MSG(MExcTrappedException),
               pName,
               Buf[0],
               TruncPathStr(TruncFileName,40),"\1",
               MSG(MExcUnloadYes),
               MSG(MOk));
   } /* else */

   rc = EXCEPTION_EXECUTE_HANDLER;
   /* VVM $ */

   if(xr->ExceptionFlags&EXCEPTION_NONCONTINUABLE)
     rc=EXCEPTION_CONTINUE_SEARCH; //?
   return rc;
}

/*
savefpos.cpp

class SaveFilePos

*/

/* Revision: 1.05 22.02.2002 $ */

/*
Modify:
  22.02.2002 SVS
    ! revert.  long -> int64 :-\
  06.05.2001 DJ
    ! �������� #include
  20.02.2001 SVS
    ! ��������� - � ������ ����!
  09.08.2000 svs
    ! revert.  int64 -> long
  08.08.2000 tran 1.01
    ! long -> int64
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "savefpos.hpp"
#include "fn.hpp"

SaveFilePos::SaveFilePos(FILE *SaveFile)
{
  SaveFilePos::SaveFile=SaveFile;
  SavePos=ftell64(SaveFile);
}


SaveFilePos::~SaveFilePos()
{
  fseek64(SaveFile,SavePos,SEEK_SET);
}

/*
savefpos.cpp

class SaveFilePos

*/

/* Revision: 1.03 20.02.2001 $ */

/*
Modify:
  20.02.2001 SVS
    ! ��������� - � ��饬� ����!
  09.08.2000 svs
    ! revert.  int64 -> long
  08.08.2000 tran 1.01
    ! long -> int64
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

#include "headers.hpp"
#pragma hdrstop
#include "internalheaders.hpp"

SaveFilePos::SaveFilePos(FILE *SaveFile)
{
  SaveFilePos::SaveFile=SaveFile;
  SavePos=ftell(SaveFile);
}


SaveFilePos::~SaveFilePos()
{
  fseek(SaveFile,SavePos,SEEK_SET);
}


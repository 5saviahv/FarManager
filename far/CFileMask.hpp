#ifndef __CFileMask_HPP
#define __CFileMask_HPP

/*
CFileMask.hpp

�������� ����� ��� ������ � ������� ������. ������������ ����� ������ ���.

*/

/* Revision: 1.01 02.07.2001 $ */

/*
Modify:
  02.07.2001 IS
    ! Free �������� � public
    + FMF_ADDASTERISK
  01.07.2001 IS
    + ������� � �����
*/

#include "BaseFileMask.hpp"

enum FM_FLAGS
{
  FMF_SILENT        = 0x00000001,
  FMF_FORBIDEXCLUDE = 0x00000002,
  FMF_ADDASTERISK   = 0x00000004
};

class CFileMask
{
private:
    BaseFileMask *FileMask;

public:
    CFileMask();
    ~CFileMask() { Free(); }

public:
    BOOL Set(const char *Masks, DWORD Flags);
    BOOL Compare(const char *Name);
    BOOL IsEmpty(void);
    void Free();

private:
  CFileMask& operator=(const CFileMask& rhs); /* ����� �� */
  CFileMask(const CFileMask& rhs); /* �������������� �� ��������� */

};

#endif // __CFileMask_HPP

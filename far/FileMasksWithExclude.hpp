#ifndef __FileMasksWithExclude_HPP
#define __FileMasksWithExclude_HPP

/*
FileMasksWithExclude.hpp

����� ��� ������ �� �������� ������� ������ (����������� ������� �����
����������).
*/

/* Revision: 1.01 02.07.2001 $ */

/*
Modify:
  02.07.2001 IS
    ! �� �������� Free �� �����������, ������ �������� �������
  01.07.2001 IS
    + ������� � �����
*/

#include "FileMasksProcessor.hpp"

class FileMasksWithExclude:public BaseFileMask
{
private:
    void Free();

public:
    FileMasksWithExclude();
    ~FileMasksWithExclude() {}

public:
    BOOL Set(const char *Masks, DWORD Flags);
    BOOL Compare(const char *Name);
    BOOL IsEmpty(void);

private:
    FileMasksProcessor Include, Exclude;

private:
  FileMasksWithExclude& operator=(const FileMasksWithExclude& rhs); /* ����� �� */
  FileMasksWithExclude(const FileMasksWithExclude& rhs); /* �������������� �� ��������� */
};

#endif // __FileMasksWithExclude_HPP

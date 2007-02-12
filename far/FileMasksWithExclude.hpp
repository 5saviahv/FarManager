#ifndef __FileMasksWithExclude_HPP
#define __FileMasksWithExclude_HPP

/*
FileMasksWithExclude.hpp

����� ��� ������ �� �������� ������� ������ (����������� ������� �����
����������).
*/

#include "FileMasksProcessor.hpp"

class FileMasksWithExclude:public BaseFileMask
{
private:
    void Free();

public:
    FileMasksWithExclude();
    virtual ~FileMasksWithExclude() {}

public:
    virtual BOOL Set(const char *Masks, DWORD Flags);
    virtual BOOL Compare(const char *Name);
    virtual BOOL IsEmpty(void);

private:
    FileMasksProcessor Include, Exclude;

private:
  FileMasksWithExclude& operator=(const FileMasksWithExclude& rhs); /* ����� �� */
  FileMasksWithExclude(const FileMasksWithExclude& rhs); /* �������������� �� ��������� */
};

#endif // __FileMasksWithExclude_HPP

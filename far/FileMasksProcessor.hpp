#ifndef __FileMasksProcessor_HPP
#define __FileMasksProcessor_HPP

/*
FileMasksProcessor.hpp

����� ��� ������ � �������� ������� ������ (�� ����������� ������� �����
����������).
*/

/* Revision: 1.00 01.07.2001 $ */

/*
Modify:
  01.07.2001 IS
    + ������� � �����
*/

#include "BaseFileMask.hpp"
#include  "udlist.hpp"

class FileMasksProcessor:public BaseFileMask
{
private:
    void Free();

public:
    FileMasksProcessor();
    ~FileMasksProcessor() { Free(); }

public:
    BOOL Set(const char *Masks, DWORD Flags);
    BOOL Compare(const char *Name);
    BOOL IsEmpty(void);

private:
    UserDefinedList Masks; // ������ ����� ������
    const char *MaskPtr;   // ��������� �� ������� ����� � ������

private:
  FileMasksProcessor& operator=(const FileMasksProcessor& rhs); /* ����� �� */
  FileMasksProcessor(const FileMasksProcessor& rhs); /* �������������� �� ��������� */

};

#endif // __FileMasksProcessor_HPP

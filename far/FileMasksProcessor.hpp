#ifndef __FileMasksProcessor_HPP
#define __FileMasksProcessor_HPP

/*
FileMasksProcessor.hpp

����� ��� ������ � �������� ������� ������ (�� ����������� ������� �����
����������).
*/

#include "BaseFileMask.hpp"
#include  "udlist.hpp"

enum FMP_FLAGS
{
  FMPF_ADDASTERISK = 0x00000001 // ��������� '*', ���� ����� �� ��������
                                // �� ������ �� ���������
                                // ��������: '*', '?', '.'
};

class FileMasksProcessor:public BaseFileMask
{
public:
    FileMasksProcessor();
    ~FileMasksProcessor() {}

public:
    BOOL Set(const char *Masks, DWORD Flags);
    BOOL Compare(const char *Name);
    BOOL IsEmpty(void);
    void Free();

private:
    UserDefinedList Masks; // ������ ����� ������
    const char *MaskPtr;   // ��������� �� ������� ����� � ������

private:
  FileMasksProcessor& operator=(const FileMasksProcessor& rhs); /* ����� �� */
  FileMasksProcessor(const FileMasksProcessor& rhs); /* �������������� �� ��������� */

};

#endif // __FileMasksProcessor_HPP

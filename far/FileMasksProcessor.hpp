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

class FileMasksProcessor : public BaseFileMask
{
public:
    FileMasksProcessor();
    virtual ~FileMasksProcessor() {}

public:
    virtual BOOL Set(const wchar_t *Masks, DWORD Flags);
    virtual BOOL Compare(const wchar_t *Name);
    virtual BOOL IsEmpty(void);
    void Free();

private:
    UserDefinedList Masks; // ������ ����� ������
    const wchar_t *MaskPtr;   // ��������� �� ������� ����� � ������

private:
  FileMasksProcessor& operator=(const FileMasksProcessor& rhs); /* ����� �� */
  FileMasksProcessor(const FileMasksProcessor& rhs); /* �������������� �� ��������� */

};

#endif // __FileMasksProcessor_HPP

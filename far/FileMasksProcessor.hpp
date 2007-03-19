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

class FileMasksProcessorW:public BaseFileMaskW
{
public:
    FileMasksProcessorW();
    virtual ~FileMasksProcessorW() {}

public:
    virtual BOOL Set(const wchar_t *Masks, DWORD Flags);
    virtual BOOL Compare(const wchar_t *Name);
    virtual BOOL IsEmpty(void);
    void Free();

private:
    UserDefinedListW Masks; // ������ ����� ������
    const wchar_t *MaskPtr;   // ��������� �� ������� ����� � ������

private:
  FileMasksProcessorW& operator=(const FileMasksProcessorW& rhs); /* ����� �� */
  FileMasksProcessorW(const FileMasksProcessorW& rhs); /* �������������� �� ��������� */

};

#endif // __FileMasksProcessor_HPP

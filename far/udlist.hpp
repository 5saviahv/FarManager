#ifndef __UserDefinedList_HPP__
#define __UserDefinedList_HPP__

/*
udlist.hpp

������ ����-����, �������������� ����� ������-�����������. ���� �����, �����
������� ������ �������� �����������, �� ���� ������� ������� ��������� �
�������. ���� ����� ����������� ������ ������ � ������ ���, �� ���������, ���
��� �� �����������, � ������� ������.

*/

/* Revision: 1.05 11.08.2001 $ */

/*
Modify:
  11.08.2001 IS
    + UDL_FLAGS
    ! SetSeparators -> SetParameters
  01.08.2001 IS
    + GetTotal
  02.07.2001 IS
    + AddAsterisk
    ! ����� Free ���� public
  12.06.2001 IS
    + ��������� ��������� � ���������� ���������� ������
    + ������� ��� �������� ������������ ������������.
  09.06.2001 IS
    + ���������� � ������ ������� �����������. ������ ������������ ���. ��
      ��������� ��� ����� ';' � ','
  02.06.2001 IS
    + ������� � �����
*/

#include "farconst.hpp"

enum UDL_FLAGS
{
  ULF_ADDASTERISK    =0x00000001, // ��������� '*' � ����� �������� ������,
                                  // ���� �� �� �������� '?', '*' � '.'
  ULF_PACKASTERISKS  =0x00000002, // ������ "*.*" � ������ �������� ������ "*"
  ULF_PROCESSBRACKETS=0x00000004, // ��������� ���������� ������ ��� �������
                                  // ������ �������������
};

class UserDefinedList
{
  private:
    DWORD Total;
    char *Data, *DataEnd, *DataCurrent;
    BYTE Separator1, Separator2;
    BOOL ProcessBrackets, AddAsterisk, PackAsterisks;

  private:
    BOOL CheckSeparators() const; // �������� ������������ �� ������������
    void SetDefaultSeparators();
    const char *Skip(const char *Str, int &Length, int &RealLength, BOOL &Error);

  private:
    UserDefinedList& operator=(const UserDefinedList& rhs); // ����� ��
    UserDefinedList(const UserDefinedList& rhs); // �������������� �� ���������

  public:
    // �� ��������� ������������ ��������� ';' � ',', �
    // ProcessBrackets=FALSE, AddAsterisk=FALSE, PackAsterisks=FALSE
    UserDefinedList();

    // ���� ����������� �����������. ��. �������� SetSeparators
    UserDefinedList(BYTE separator1, BYTE separator2, DWORD Flags);
    ~UserDefinedList() { Free(); }

  public:
    // ������� �������-����������� � ��������� ��� ��������� ���������
    // ���������� ������.
    // ���� ���� �� Separator* ����� 0x00, �� �� ������������ ��� ����������
    // (�.�. � Set)
    // ���� ��� ����������� ����� 0x00, �� ����������������� ����������� ��
    // ��������� (';' & ',').
    // ���� AddAsterisk ����� TRUE, �� � ����� �������� ������ �����
    // ����������� '*', ���� ���� ������� �� �������� '?', '*' � '.'
    // ���������� FALSE, ���� ���� �� ������������ �������� �������� ���
    // �������� ��������� ������ � ���� �� ������������ �������� ����������
    // �������.
    BOOL SetParameters(BYTE Separator1, BYTE Separator2, DWORD Flags);

    // �������������� ������. ��������� ������, ����������� �������������.
    // ���������� FALSE ��� �������.
    // ����: ���� List==NULL, �� ���������� ������������ ������� ����� ������
    BOOL Set(const char *List);

    // �������� ����� ������� ������ �� �������
    void Start(void);

    // ������ ��������� �� ��������� ������� ������ ��� NULL
    const char *GetNext(void);

    // ���������� ������� ������
    void Free();

    // TRUE, ���� ������ ��������� � ������ ���
    BOOL IsEmpty();

    // ������� ���������� ��������� � ������
    DWORD GetTotal () const { return Total; }
};

#endif // __UserDefinedList_HPP

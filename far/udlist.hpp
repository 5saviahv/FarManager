#ifndef __UserDefinedList_HPP__
#define __UserDefinedList_HPP__

/*
udlist.hpp

������ ����-����, �������������� ����� ������-�����������. ���� �����, �����
������� ������ �������� �����������, �� ���� ������� ������� ��������� �
�������. ���� ����� ����������� ������ ������ � ������ ���, �� ���������, ���
��� �� �����������, � ������� ������.

*/

/* Revision: 1.02 12.06.2001 $ */

/*
Modify:
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

class UserDefinedList
{
  private:
    char *Data, *DataEnd, *DataCurrent;
    BYTE Separator1, Separator2;
    BOOL ProcessBrackets;

  private:
    BOOL CheckSeparators() const; // �������� ������������ �� ������������
    void SetDefaultSeparators();
    void Free();
    const char *Skip(const char *Str, int &Length, int &RealLength, BOOL &Error);

  private:
    UserDefinedList& operator=(const UserDefinedList& rhs); // ����� ��
    UserDefinedList(const UserDefinedList& rhs); // �������������� �� ���������

  public:
    // �� ��������� ������������ ��������� ';' � ',', �
    // ProcessBrackets=FALSE
    UserDefinedList();

    // ���� ����������� �����������. ��. �������� SetSeparators
    UserDefinedList(BYTE separator1, BYTE separator2, BOOL ProcessBrackets);
    ~UserDefinedList() { Free(); }

  public:
    // ������� �������-����������� � ��������� ��� ��������� ���������
    // ���������� ������.
    // ���� ���� �� Separator* ����� 0x00, �� �� ������������ ��� ����������
    // (�.�. � Set)
    // ���� ��� ����������� ����� 0x00, �� ����������������� ����������� ��
    // ��������� (';' & ',').
    // ���������� FALSE, ���� ���� �� ������������ �������� �������� ���
    // �������� ��������� ������ � ���� �� ������������ �������� ����������
    // �������.
    BOOL SetSeparators(BYTE Separator1, BYTE Separator2, BOOL ProcessBrackets);

    // �������������� ������. ��������� ������, ����������� �������������.
    // ���������� FALSE ��� �������.
    // ����: ���� List==NULL, �� ���������� ������������ ������� ����� ������
    BOOL Set(const char *List);

    // �������� ����� ������� ������ �� �������
    void Start(void);

    // ������ ��������� �� ��������� ������� ������ ��� NULL
    const char *GetNext(void);

    // TRUE, ���� ������ ��������� � ������ ���
    BOOL IsEmpty();
};

#endif // __UserDefinedList_HPP

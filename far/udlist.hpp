#ifndef __UserDefinedList_HPP__
#define __UserDefinedList_HPP__

/*
udlist.hpp

������ ����-����, �������������� ����� ������-�����������. ���� �����, �����
������� ������ �������� �����������, �� ���� ������� ������� ��������� �
�������.

*/

/* Revision: 1.00 02.06.2001 $ */

/*
Modify:
  02.06.2001 IS
    + ������� � �����
*/

#include "farconst.hpp"

class UserDefinedList
{
  private:
    char *Data, *DataEnd, *DataCurrent;
    BYTE Separator;

  private:
    void Free();
    const char *Skip(const char *Str, int &Length, int &RealLength, BOOL &Error);

  private:
    UserDefinedList& operator=(const UserDefinedList& rhs); // ����� ��
    UserDefinedList(const UserDefinedList& rhs); // �������������� �� ���������

  public:
    UserDefinedList();               // �� ��������� ������������ ��������� ';'
    UserDefinedList(BYTE separator); // ���� ����������� �����������
    ~UserDefinedList() { Free(); }

  public:
    // ������� ������-�����������
    void SetSeparator(BYTE Separator);

    // �������������� ������. ��������� ������, ����������� �������� separator.
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

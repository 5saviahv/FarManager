#ifndef __UserDefinedList_HPP__
#define __UserDefinedList_HPP__

/*
udlist.hpp

������ ����-����, �������������� ����� ������-�����������. ���� �����, �����
������� ������ �������� �����������, �� ���� ������� ������� ��������� �
�������.

*/

/* Revision: 1.01 09.06.2001 $ */

/*
Modify:
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

  private:
    void SetDefaultSeparators();
    void Free();
    const char *Skip(const char *Str, int &Length, int &RealLength, BOOL &Error);

  private:
    UserDefinedList& operator=(const UserDefinedList& rhs); // ����� ��
    UserDefinedList(const UserDefinedList& rhs); // �������������� �� ���������

  public:
    UserDefinedList(); // �� ��������� ������������ ��������� ';' � ','
    UserDefinedList(BYTE separator1, BYTE separator2); // ���� ����������� �����������
    ~UserDefinedList() { Free(); }

  public:
    // ������� �������-�����������
    // ���� ���� �� Separator* ����� 0x00, �� �� ������������
    // ���� ��� ����������� ����� 0x00, �� ����������������� ����������� ��
    // ��������� (';' & ',')
    void SetSeparator(BYTE Separator1, BYTE Separator2);

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

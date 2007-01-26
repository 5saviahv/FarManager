#ifndef __UserDefinedList_HPP__
#define __UserDefinedList_HPP__

/*
udlist.hpp

������ ����-����, �������������� ����� ������-�����������. ���� �����, �����
������� ������ �������� �����������, �� ���� ������� ������� ��������� �
�������. ���� ����� ����������� ������ ������ � ������ ���, �� ���������, ���
��� �� �����������, � ������� ������.

*/

#include "farconst.hpp"
#include "array.hpp"

enum UDL_FLAGS
{
  ULF_ADDASTERISK    =0x00000001, // ��������� '*' � ����� �������� ������,
                                  // ���� �� �� �������� '?', '*' � '.'
  ULF_PACKASTERISKS  =0x00000002, // ������ "*.*" � ������ �������� ������ "*"
                                  // ������ "***" � ������ �������� ������ "*"
  ULF_PROCESSBRACKETS=0x00000004, // ��������� ���������� ������ ��� �������
                                  // ������ �������������
  ULF_UNIQUE         =0x00000010, // ������� ������������� ��������
  ULF_SORT           =0x00000020, // ������������� (� ������ ��������)
};

class UserDefinedListItem
{
  public:
   unsigned int index;
   char *Str;
   UserDefinedListItem():Str(NULL), index(0)
   {
   }
   bool operator==(const UserDefinedListItem &rhs) const;
   int operator<(const UserDefinedListItem &rhs) const;
   const UserDefinedListItem& operator=(const UserDefinedListItem &rhs);
   const UserDefinedListItem& operator=(const char *rhs);
   char *set(const char *Src, unsigned int size);
   ~UserDefinedListItem();
};

class UserDefinedList
{
  private:
    TArray<UserDefinedListItem> Array;
    unsigned int CurrentItem;
    BYTE Separator1, Separator2;
    BOOL ProcessBrackets, AddAsterisk, PackAsterisks, Unique, Sort;

  private:
    BOOL CheckSeparators() const; // �������� ������������ �� ������������
    void SetDefaultSeparators();
    const char *Skip(const char *Str, int &Length, int &RealLength, BOOL &Error);
    static int __cdecl CmpItems(const UserDefinedListItem **el1,
      const UserDefinedListItem **el2);

  private:
    UserDefinedList& operator=(const UserDefinedList& rhs); // ����� ��
    UserDefinedList(const UserDefinedList& rhs); // �������������� �� ���������

  public:
    // �� ��������� ������������ ��������� ';' � ',', �
    // ProcessBrackets=AddAsterisk=PackAsterisks=FALSE
    // Unique=Sort=FALSE
    UserDefinedList();

    // ���� ����������� �����������. ��. �������� SetParameters
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
    BOOL Set(const char *List, BOOL AddToList=FALSE);

    // ���������� � ��� ������������� ������
    // ����: ���� NewItem==NULL, �� ���������� ������������ ������� �����
    // ������
    BOOL AddItem(const char *NewItem)
    {
      return Set(NewItem,TRUE);
    }

    // �������� ����� ������� ������ �� �������
    void Reset(void);

    // ������ ��������� �� ��������� ������� ������ ��� NULL
    const char *GetNext(void);

    // ���������� ������
    void Free();

    // TRUE, ���� ������ ��������� � ������ ���
    BOOL IsEmpty();

    // ������� ���������� ��������� � ������
    DWORD GetTotal () const { return Array.getSize(); }
};

#endif // __UserDefinedList_HPP

#ifndef __BITFLAGS_HPP__
#define __BITFLAGS_HPP__
/*
bitflags.hpp

�����

*/

/* Revision: 1.00 08.11.2001 $ */

/*
Modify:
  08.11.2001 SVS
    + ������� ������ (�������� ����� ��������� ��, ��� ����)
*/

class BitFlags{
  public:
    DWORD Flags;

  public:
    BitFlags(){Flags=0;}
    ~BitFlags(){}

  public:
    // ���������� ����� ������
    DWORD Set(DWORD NewFlags){ Flags|=NewFlags;return Flags;}
    // �������� ����� ������
    DWORD Skip(DWORD NewFlags){ Flags&=~NewFlags;return Flags; }
    // ��������� ����� ������
    BOOL  Check(DWORD NewFlags){ return Flags&NewFlags; }
    // �������� ��������� ������ ������
    DWORD Change(DWORD NewFlags,BOOL Status){ if(Status) Flags|=NewFlags; else Flags&=~NewFlags; return Flags;}
};

#endif // __BITFLAGS_HPP__

#ifndef __BITFLAGS_HPP__
#define __BITFLAGS_HPP__
/*
bitflags.hpp

�����

*/

class BitFlags{
  public:
    DWORD Flags;

  public:
    BitFlags(){Flags=0;}
    BitFlags(DWORD Fl){Flags=Fl;}

    ~BitFlags(){}

  public:
    // ���������� ����� ������
    DWORD Set(DWORD NewFlags){ Flags|=NewFlags;return Flags;}
    // �������� ����� ������
    DWORD Clear(DWORD NewFlags){ Flags&=~NewFlags;return Flags; }
    // ��������� ����� ������
    BOOL  Check(DWORD NewFlags){ return Flags&NewFlags?TRUE:FALSE; }
    // �������� ��������� ������ ������ � ����������� �� Status
    DWORD Change(DWORD NewFlags,BOOL Status){ if(Status) Flags|=NewFlags; else Flags&=~NewFlags; return Flags;}
    // ������������� ��������� ������
    DWORD Swap(DWORD SwapedFlags){ if(Flags&SwapedFlags) Flags&=~SwapedFlags; else Flags|=SwapedFlags; return Flags;}
    //�������� ��� �����
    void ClearAll(){Flags=0;}
};

#endif // __BITFLAGS_HPP__

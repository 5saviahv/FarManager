#ifndef __BaseFileMask_HPP
#define __BaseFileMask_HPP

/*
BaseFileMask.hpp

����������� �����, ������� ��� �������� ������ � �������.
*/

/* Revision: 1.01 24.04.2005 $ */

/*
Modify:
  24.04.2005 AY
    ! GCC
  01.07.2001 IS
    + ������� � �����
*/

extern const int EXCLUDEMASKSEPARATOR;

class BaseFileMask
{
public:
    BaseFileMask() {}
    virtual ~BaseFileMask() {}

public:
    virtual BOOL Set(const char *Masks, DWORD Flags)=0;
    virtual BOOL Compare(const char *Name)=0;
    virtual BOOL IsEmpty(void) { return TRUE; }

private:
  BaseFileMask& operator=(const BaseFileMask& rhs); /* ����� �� */
  BaseFileMask(const BaseFileMask& rhs); /* �������������� �� ��������� */

};

#endif // __BaseFileMask_HPP

#ifndef __BaseFileMask_HPP
#define __BaseFileMask_HPP

/*
BaseFileMask.hpp

����������� �����, ������� ��� �������� ������ � �������.
*/

/* Revision: 1.00 01.07.2001 $ */

/*
Modify:
  01.07.2001 IS
    + ������� � �����
*/

extern const EXCLUDEMASKSEPARATOR;

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

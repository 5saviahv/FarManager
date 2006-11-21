#ifndef __FILEFILTER_HPP__
#define __FILEFILTER_HPP__
/*
filefilter.hpp

�������� ������

*/

/* Revision: 1.03 23.04.2005 $ */

/*
Modify:
  23.04.2005 KM
    ! ����������� ��� ������� ������������� �������� Directory �� �����������
  07.12.2003 SVS
    ! SIZE_COUNT ������ - ����� ������� FSIZE_IN_LAST �� farconst.hpp
  11.10.2003 KM
    + CFileMask FilterMask - �������� ���������������� ����� �����
      ��� ��������� �������� ���������� ������.
  04.10.2003 KM
    ! �������� � ����� ������� ��������. ������ ����� ��� :-)
*/

#include "plugin.hpp"
#include "struct.hpp"
#include "CFileMask.hpp"

#define DATE_COUNT  3

class FileFilter
{
  friend LONG_PTR WINAPI FilterDlgProc(HANDLE hDlg, int Msg,int Param1,LONG_PTR Param2);

  private:

    const char *FmtMask1;               // ����� ���� ��� �������� DD.MM.YYYY � MM.DD.YYYY
    const char *FmtMask2;               // ����� ���� ��� ������� YYYY.MM.DD
    const char *FmtMask3;               // ����� �������
    const char *DigitMask;              // ����� ��� ����� �������� �����
    const char *FilterMasksHistoryName; // ������� ��� ����� ������

    FarList SizeList;                   // ���� ��� ����������: ����� - ���������
    FarListItem *TableItemSize;
    FarList DateList;                   // ���� ��� ���������� ������� �����
    FarListItem *TableItemDate;

    // ����� ��� ������� ���������
    char DateMask[16],DateStrAfter[16],DateStrBefore[16];
    char TimeMask[16],TimeStrAfter[16],TimeStrBefore[16];

    int DateSeparator;                  // ����������� ����
    int TimeSeparator;                  // ����������� �������
    int DateFormat;                     // ������ ���� � �������

    FilterParams FF;                    // ���������� ��������� ���������� ������������
                                        // ��� ����, ����� �� ������ �������� Opt.OpFilter
                                        // �� "����".

    CFileMask FilterMask;               // ��������� ���������������� �����.

  private:

    // ���������� ���������
    static LONG_PTR WINAPI FilterDlgProc(HANDLE hDlg,int Msg,int Param1,LONG_PTR Param2);
    void GetFileDateAndTime(const char *Src,unsigned *Dst,int Separator);

    // ������������� ��������� ����� ���� � ������� � FILETIME
    FILETIME &StrToDateTime(const char *CDate,const char *CTime,FILETIME &ft);

  public:

    FileFilter(int DisableDirAttr=FALSE);
    ~FileFilter();

    // �������� ������� ��������� �������.
    FilterParams GetParams(){return FF;};

    // ������ ����� ���������� "�������" � ������ ��� �����������:
    // �������� �� ���� fd ��� ������� �������������� �������.
    // ���������� TRUE  - ��������;
    //            FALSE - �� ��������.
    int FileInFilter(WIN32_FIND_DATA *fd);

    // ������ ����� ���������� ��� ��������� ���������� �������.
    void Configure();
};

#endif  // __FINDFILES_HPP__

#ifndef __FILEFILTER_HPP__
#define __FILEFILTER_HPP__
/*
filefilter.hpp

�������� ������

*/

/* Revision: 1.00 04.10.2003 $ */

/*
Modify:
  04.10.2003 KM
    ! �������� � ����� ������� ��������. ������ ����� ��� :-)
*/

#include "plugin.hpp"
#include "struct.hpp"
#include "CFileMask.hpp"

#define SIZE_COUNT  2
#define DATE_COUNT  3

class FileFilter
{
  friend long WINAPI FilterDlgProc(HANDLE hDlg, int Msg,int Param1,long Param2);

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

  private:

    // ���������� ���������
    static long WINAPI FilterDlgProc(HANDLE hDlg,int Msg,int Param1,long Param2);
    void GetFileDateAndTime(const char *Src,unsigned *Dst,int Separator);

    // ������������� ��������� ����� ���� � ������� � FILETIME
    FILETIME &StrToDateTime(const char *CDate,const char *CTime,FILETIME &ft);

  public:

    FileFilter();
    ~FileFilter();

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

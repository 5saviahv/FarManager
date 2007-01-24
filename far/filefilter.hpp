#ifndef __FILEFILTER_HPP__
#define __FILEFILTER_HPP__
/*
filefilter.hpp

�������� ������

*/

#include "plugin.hpp"
#include "struct.hpp"
#include "CFileMask.hpp"
#include "bitflags.hpp"

#define DATE_COUNT  3

enum FileFilterFlags {
  FFF_RPANELINCLUDE = 1,
  FFF_RPANELEXCLUDE = 2,
  FFF_LPANELINCLUDE = 4,
  FFF_LPANELEXCLUDE = 8,
  FFF_FINDFILEINCLUDE = 16,
  FFF_FINDFILEEXCLUDE = 32,
  FFF_COPYINCLUDE = 64,
  FFF_COPYEXCLUDE = 128,
};

class FileFilterParams
{
  private:

    char m_Title[512];

    struct
    {
      DWORD Used;
      char Mask[512];
      CFileMask FilterMask; // ��������� ���������������� �����.
    } FMask;

    struct
    {
      DWORD Used;
      FDateType DateType;
      FILETIME DateAfter;
      FILETIME DateBefore;
    } FDate;

    struct
    {
      DWORD Used;
      FSizeType SizeType;
      __int64 SizeAbove; // ����� ������ ����� ������ � SizeType ��� -1 ��� �����
      __int64 SizeBelow; // ����� ������ ����� ������ � SizeType ��� -1 ��� �����
      unsigned __int64 SizeAboveReal; // ����� ������ ����� ������ � ������
      unsigned __int64 SizeBelowReal; // ����� ������ ����� ������ � ������
    } FSize;

    struct
    {
      DWORD Used;
      DWORD AttrSet;
      DWORD AttrClear;
    } FAttr;

  public:

    BitFlags Flags; // ����� �������

  public:

    FileFilterParams();

    void SetTitle(const char *Title);
    void SetMask(DWORD Used, const char *Mask);
    void SetDate(DWORD Used, DWORD DateType, FILETIME DateAfter, FILETIME DateBefore);
    void SetSize(DWORD Used, DWORD SizeType, __int64 SizeAbove, __int64 SizeBelow);
    void SetAttr(DWORD Used, DWORD AttrSet, DWORD AttrClear);

    const char *GetTitle();
    DWORD GetMask(const char **Mask);
    DWORD GetDate(DWORD *DateType, FILETIME *DateAfter, FILETIME *DateBefore);
    DWORD GetSize(DWORD *SizeType, __int64 *SizeAbove, __int64 *SizeBelow);
    DWORD GetAttr(DWORD *AttrSet, DWORD *AttrClear);

    // ������ ����� ���������� "�������" � ������ ��� �����������:
    // �������� �� ���� fd ��� ������� �������������� �������.
    // ���������� true  - ��������;
    //            false - �� ��������.
    bool FileInFilter(WIN32_FIND_DATA *fd);
};

class VMenu;
class Panel;

enum enumFileFilterType {
  FFT_PANEL = 0,
  FFT_FINDFILE,
  FFT_COPY,
};

class FileFilter
{
  private:
    Panel *m_HostPanel;
    enumFileFilterType m_FilterType;

    int  ParseAndAddMasks(char **ExtPtr,const char *FileName,DWORD FileAttr,int& ExtCount,int Check);
    void ProcessSelection(VMenu *FilterList);
    void GetIncludeExcludeFlags(DWORD &Inc, DWORD &Exc);
    int  GetCheck(FileFilterParams *FFP);

  public:
    FileFilter(Panel *HostPanel, enumFileFilterType FilterType);
    ~FileFilter();

    void FilterEdit();
    bool FileInFilter(WIN32_FIND_DATA *fd);
    bool IsEnabledOnPanel();

    static void InitFilter();
    static void CloseFilter();
    static void SwapFilter();
    static void SaveFilters(bool SaveAll=true);
};

bool FileFilterConfig(FileFilterParams *FF);

#endif  // __FINDFILES_HPP__

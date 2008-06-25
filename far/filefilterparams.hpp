#ifndef __FILEFILTERPARAMS_HPP__
#define __FILEFILTERPARAMS_HPP__
/*
filefilterparams.hpp

��������� ��������� �������

*/

#include "plugin.hpp"
#include "struct.hpp"
#include "CFileMask.hpp"
#include "bitflags.hpp"

struct FileListItem;

#define FILEFILTER_MASK_SIZE 2048
#define FILEFILTER_SIZE_SIZE 32

enum enumFileFilterFlagsType
{
  FFFT_FIRST = 0, //������ ���� ������

  FFFT_LEFTPANEL = FFFT_FIRST,
  FFFT_RIGHTPANEL,
  FFFT_FINDFILE,
  FFFT_COPY,
  FFFT_SELECT,

  FFFT_COUNT, //������ ���� ���������
};

enum enumFileFilterFlags
{
  FFF_NONE    = 0x00000000,
  FFF_INCLUDE = 0x00000001,
  FFF_EXCLUDE = 0x00000002,
  FFF_STRONG  = 0x10000000
};

enum enumFDateType
{
  FDATE_MODIFIED=0,
  FDATE_CREATED,
  FDATE_OPENED,

  FDATE_COUNT, // ������ ��������� !!!
};

class FileFilterParams
{
  private:

    char m_Title[512];

    struct
    {
      bool Used;
      char Mask[FILEFILTER_MASK_SIZE];
      CFileMask FilterMask; // ��������� ���������������� �����.
    } FMask;

    struct
    {
      bool Used;
      enumFDateType DateType;
      FILETIME DateAfter;
      FILETIME DateBefore;
      bool bRelative;
    } FDate;

    struct
    {
      bool Used;
      char SizeAbove[FILEFILTER_SIZE_SIZE]; // ����� ������ ����� ������ ��� ��� ��� ����
      char SizeBelow[FILEFILTER_SIZE_SIZE]; // ����� ������ ����� ������ ��� ��� ��� ����
      unsigned __int64 SizeAboveReal; // ����� ������ ����� ������ � ������
      unsigned __int64 SizeBelowReal; // ����� ������ ����� ������ � ������
    } FSize;

    struct
    {
      bool Used;
      DWORD AttrSet;
      DWORD AttrClear;
    } FAttr;

    struct
    {
      HighlightDataColor Colors;
      int SortGroup;
      bool bContinueProcessing;
    } FHighlight;

    DWORD FFlags[FFFT_COUNT];

  public:

    FileFilterParams();

    const FileFilterParams &operator=(const FileFilterParams &FF);

    void SetTitle(const char *Title);
    void SetMask(bool Used, const char *Mask);
    void SetDate(bool Used, DWORD DateType, FILETIME DateAfter, FILETIME DateBefore, bool bRelative);
    void SetSize(bool Used, const char *SizeAbove, const char *SizeBelow);
    void SetAttr(bool Used, DWORD AttrSet, DWORD AttrClear);
    void SetColors(HighlightDataColor *Colors);
    void SetSortGroup(int SortGroup) { FHighlight.SortGroup = SortGroup; }
    void SetContinueProcessing(bool bContinueProcessing) { FHighlight.bContinueProcessing = bContinueProcessing; }
    void SetFlags(enumFileFilterFlagsType FType, DWORD Flags) { FFlags[FType] = Flags; }
    void ClearAllFlags() { memset(FFlags,0,sizeof(FFlags)); }

    const char *GetTitle() const;
    bool  GetMask(const char **Mask) const;
    bool  GetDate(DWORD *DateType, FILETIME *DateAfter, FILETIME *DateBefore, bool *bRelative) const;
    bool  GetSize(const char **SizeAbove, const char **SizeBelow) const;
    bool  GetAttr(DWORD *AttrSet, DWORD *AttrClear) const;
    void  GetColors(HighlightDataColor *Colors) const;
    int   GetMarkChar() const;
    int   GetSortGroup() const { return FHighlight.SortGroup; }
    bool  GetContinueProcessing() const { return FHighlight.bContinueProcessing; }
    DWORD GetFlags(enumFileFilterFlagsType FType) const { return FFlags[FType]; }

    // ������ ����� ���������� "�������" � ������ ��� �����������:
    // �������� �� ���� fd ��� ������� �������������� �������.
    // ���������� true  - ��������;
    //            false - �� ��������.
    bool FileInFilter(WIN32_FIND_DATA *fd, unsigned __int64 CurrentTime);
    bool FileInFilter(FileListItem *fli, unsigned __int64 CurrentTime);
};

bool FileFilterConfig(FileFilterParams *FF, bool ColorConfig=false);

//���������������� ������� ��� �������� ����� ���� ��������� ��������.
void MenuString(char *dest, FileFilterParams *FF, bool bHighlightType=false, bool bPanelType=false, const char *FMask=NULL, const char *Title=NULL);

#endif //__FILEFILTERPARAMS_HPP__

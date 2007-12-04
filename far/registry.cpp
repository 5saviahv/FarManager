/*
registry.cpp

������ � registry

*/

#include "headers.hpp"
#pragma hdrstop

#include "fn.hpp"
#include "global.hpp"
#include "array.hpp"

static LONG CloseRegKey(HKEY hKey);
int CopyKeyTree(const char *Src,const char *Dest,const char *Skip=NULL);
void DeleteFullKeyTree(const char *KeyName);
static void DeleteKeyTreePart(const char *KeyName);
static int DeleteCount;

static HKEY hRegRootKey=HKEY_CURRENT_USER;
static HKEY hRegCurrentKey=NULL;
static int RequestSameKey=FALSE;

void SetRegRootKey(HKEY hRootKey)
{
  hRegRootKey=hRootKey;
}


void UseSameRegKey()
{
  CloseSameRegKey();
  RequestSameKey=TRUE;
}


void CloseSameRegKey()
{
  if (hRegCurrentKey!=NULL)
  {
    RegCloseKey(hRegCurrentKey);
    hRegCurrentKey=NULL;
  }
  RequestSameKey=FALSE;
}


LONG SetRegKey(const char *Key,const char *ValueName,const char * const ValueData, int SizeData, DWORD Type)
{
  HKEY hKey;
  LONG Ret=ERROR_SUCCESS;
  if((hKey=CreateRegKey(Key)) != NULL)
    Ret=RegSetValueEx(hKey,ValueName,0,Type,(unsigned char *)ValueData,(DWORD)SizeData);
  CloseRegKey(hKey);
  return Ret;
}


LONG SetRegKey(const char *Key,const char *ValueName,const char * const ValueData)
{
  HKEY hKey;
  LONG Ret=ERROR_SUCCESS;
  if((hKey=CreateRegKey(Key)) != NULL)
    Ret=RegSetValueEx(hKey,ValueName,0,REG_SZ,(unsigned char *)ValueData,(DWORD)strlen(ValueData)+1);
  CloseRegKey(hKey);
  return Ret;
}


LONG SetRegKey(const char *Key,const char *ValueName,DWORD ValueData)
{
  HKEY hKey;
  LONG Ret=ERROR_SUCCESS;
  if((hKey=CreateRegKey(Key)) != NULL)
    Ret=RegSetValueEx(hKey,ValueName,0,REG_DWORD,(BYTE *)&ValueData,sizeof(ValueData));
  CloseRegKey(hKey);
  return Ret;
}

LONG SetRegKey64(const char *Key,const char *ValueName,unsigned __int64 ValueData)
{
  HKEY hKey;
  LONG Ret=ERROR_SUCCESS;
  if((hKey=CreateRegKey(Key)) != NULL)
    Ret=RegSetValueEx(hKey,ValueName,0,REG_QWORD,(BYTE *)&ValueData,sizeof(ValueData));
  CloseRegKey(hKey);
  return Ret;
}


LONG SetRegKey(const char *Key,const char *ValueName,const BYTE *ValueData,DWORD ValueSize)
{
  HKEY hKey;
  LONG Ret=ERROR_SUCCESS;
  if((hKey=CreateRegKey(Key)) != NULL)
    Ret=RegSetValueEx(hKey,ValueName,0,REG_BINARY,ValueData,ValueSize);
  CloseRegKey(hKey);
  return Ret;
}


int GetRegKeySize(const char *Key,const char *ValueName)
{
  HKEY hKey=OpenRegKey(Key);
  DWORD QueryDataSize=GetRegKeySize(hKey,ValueName);
  CloseRegKey(hKey);
  return QueryDataSize;
}

int GetRegKeySize(HKEY hKey,const char *ValueName)
{
  if(hKey)
  {
    BYTE Buffer;
    DWORD Type,QueryDataSize=sizeof(Buffer);
    int ExitCode=RegQueryValueEx(hKey,ValueName,0,&Type,(unsigned char *)&Buffer,&QueryDataSize);
    if(ExitCode==ERROR_SUCCESS || ExitCode == ERROR_MORE_DATA)
      return QueryDataSize;
  }
  return 0;
}

/* $ 22.02.2001 SVS
  ��� ��������� ������ (GetRegKey) ���������� �������� � ERROR_MORE_DATA
  ���� ����� �������� ����������� - ������� ������� ���� � ����� ������
*/
int GetRegKey(const char *Key,const char *ValueName,char *ValueData,const char *Default,DWORD DataSize,DWORD *pType)
{
  int ExitCode;
  HKEY hKey=OpenRegKey(Key);
  if(hKey) // ������� ���������!
  {
    DWORD Type,QueryDataSize=DataSize;
    ExitCode=RegQueryValueEx(hKey,ValueName,0,&Type,(unsigned char *)ValueData,&QueryDataSize);
    if(ExitCode == ERROR_MORE_DATA) // ���� ������ �� ����������...
    {
      char *TempBuffer=new char[QueryDataSize+1]; // ...�� ������� ������� ����
      if(TempBuffer) // ���� � ������� ��� ���������...
      {
        if((ExitCode=RegQueryValueEx(hKey,ValueName,0,&Type,(unsigned char *)TempBuffer,&QueryDataSize)) == ERROR_SUCCESS)
          xstrncpy(ValueData,TempBuffer,DataSize-1); // ��������� ������� ����.
        delete[] TempBuffer;
      }
    }
    if(pType)
      *pType=Type;
    CloseRegKey(hKey);
  }
  if (hKey==NULL || ExitCode!=ERROR_SUCCESS)
  {
    xstrncpy (ValueData,Default, DataSize-1);
    return(FALSE);
  }
  return(TRUE);
}
/* SVS $ */


int GetRegKey(const char *Key,const char *ValueName,int &ValueData,DWORD Default)
{
  int ExitCode;
  HKEY hKey=OpenRegKey(Key);
  if(hKey)
  {
    DWORD Type,Size=sizeof(ValueData);
    ExitCode=RegQueryValueEx(hKey,ValueName,0,&Type,(BYTE *)&ValueData,&Size);
    CloseRegKey(hKey);
  }
  if (hKey==NULL || ExitCode!=ERROR_SUCCESS)
  {
    ValueData=Default;
    return(FALSE);
  }
  return(TRUE);
}


int GetRegKey(const char *Key,const char *ValueName,DWORD Default)
{
  int ValueData;
  GetRegKey(Key,ValueName,ValueData,Default);
  return(ValueData);
}

int GetRegKey64(const char *Key,const char *ValueName,__int64 &ValueData,unsigned __int64 Default)
{
  int ExitCode;
  HKEY hKey=OpenRegKey(Key);
  if(hKey)
  {
    DWORD Type,Size=sizeof(ValueData);
    ExitCode=RegQueryValueEx(hKey,ValueName,0,&Type,(BYTE *)&ValueData,&Size);
    CloseRegKey(hKey);
  }
  if (hKey==NULL || ExitCode!=ERROR_SUCCESS)
  {
    ValueData=Default;
    return(FALSE);
  }
  return(TRUE);
}


__int64 GetRegKey64(const char *Key,const char *ValueName,unsigned __int64 Default)
{
  __int64 ValueData;
  GetRegKey64(Key,ValueName,ValueData,Default);
  return(ValueData);
}


int GetRegKey(const char *Key,const char *ValueName,BYTE *ValueData,const BYTE *Default,DWORD DataSize,DWORD *pType)
{
  int ExitCode;
  HKEY hKey=OpenRegKey(Key);
  DWORD Required=DataSize;
  if(hKey)
  {
    DWORD Type;
    ExitCode=RegQueryValueEx(hKey,ValueName,0,&Type,ValueData,&Required);
    if(ExitCode == ERROR_MORE_DATA) // ���� ������ �� ����������...
    {
      char *TempBuffer=new char[Required+1]; // ...�� ������� ������� ����
      if(TempBuffer) // ���� � ������� ��� ���������...
      {
        if((ExitCode=RegQueryValueEx(hKey,ValueName,0,&Type,(unsigned char *)TempBuffer,&Required)) == ERROR_SUCCESS)
          memcpy(ValueData,TempBuffer,DataSize);  // ��������� ������� ����.
        delete[] TempBuffer;
      }
    }
    if(pType)
      *pType=Type;
    CloseRegKey(hKey);
  }
  if (hKey==NULL || ExitCode!=ERROR_SUCCESS)
  {
    if (Default!=NULL)
      memcpy(ValueData,Default,DataSize);
    else
      memset(ValueData,0,DataSize);
    return(0);
  }
  return(Required);
}

static char *MkKeyName(const char *Key, char *Dest, int DestSize)
{
  char FullKeyName[2048];
  xstrncpy(FullKeyName,Opt.RegRoot,sizeof(FullKeyName)-1);
  if(*Key)
  {
    strncat(FullKeyName,"\\",sizeof(FullKeyName)-1);
    strncat(FullKeyName,Key,sizeof(FullKeyName)-1);
  }
  xstrncpy(Dest,FullKeyName,DestSize);
  return Dest;
}

HKEY CreateRegKey(const char *Key)
{
  if (hRegCurrentKey)
    return(hRegCurrentKey);
  HKEY hKey;
  DWORD Disposition;
  char FullKeyName[512];
  MkKeyName(Key,FullKeyName,sizeof(FullKeyName)-1);
  if(RegCreateKeyEx(hRegRootKey,FullKeyName,0,NULL,0,KEY_WRITE,NULL,
                 &hKey,&Disposition) != ERROR_SUCCESS)
    hKey=NULL;
  if (RequestSameKey)
  {
    RequestSameKey=FALSE;
    hRegCurrentKey=hKey;
  }
  return(hKey);
}


HKEY OpenRegKey(const char *Key)
{
  if (hRegCurrentKey)
    return(hRegCurrentKey);
  HKEY hKey;
  char FullKeyName[512];
  MkKeyName(Key,FullKeyName,sizeof(FullKeyName)-1);
  if (RegOpenKeyEx(hRegRootKey,FullKeyName,0,KEY_QUERY_VALUE|KEY_ENUMERATE_SUB_KEYS,&hKey)!=ERROR_SUCCESS)
  {
    CloseSameRegKey();
    return(NULL);
  }
  if (RequestSameKey)
  {
    RequestSameKey=FALSE;
    hRegCurrentKey=hKey;
  }
  return(hKey);
}


void DeleteRegKey(const char *Key)
{
  char FullKeyName[512];
  MkKeyName(Key,FullKeyName,sizeof(FullKeyName)-1);
  RegDeleteKey(hRegRootKey,FullKeyName);
}


void DeleteRegValue(const char *Key,const char *Value)
{
  HKEY hKey;
  char FullKeyName[512];
  MkKeyName(Key,FullKeyName,sizeof(FullKeyName)-1);
  if (RegOpenKeyEx(hRegRootKey,FullKeyName,0,KEY_WRITE,&hKey)==ERROR_SUCCESS)
  {
    RegDeleteValue(hKey,Value);
    CloseRegKey(hKey);
  }
}

void DeleteKeyRecord(const char *KeyMask,int Position)
{
  char FullKeyName[512],NextFullKeyName[512],MaskKeyName[512];
  MkKeyName(KeyMask,MaskKeyName,sizeof(MaskKeyName)-1);
  while (1)
  {
    sprintf(FullKeyName,MaskKeyName,Position++);
    sprintf(NextFullKeyName,MaskKeyName,Position);
    if (!CopyKeyTree(NextFullKeyName,FullKeyName))
    {
      DeleteFullKeyTree(FullKeyName);
      break;
    }
  }
}


void InsertKeyRecord(const char *KeyMask,int Position,int TotalKeys)
{
  char FullKeyName[512],PrevFullKeyName[512],MaskKeyName[512];
  MkKeyName(KeyMask,MaskKeyName,sizeof(MaskKeyName)-1);
  for (int CurPos=TotalKeys;CurPos>Position;CurPos--)
  {
    sprintf(FullKeyName,MaskKeyName,CurPos);
    sprintf(PrevFullKeyName,MaskKeyName,CurPos-1);
    if (!CopyKeyTree(PrevFullKeyName,FullKeyName))
      break;
  }
  sprintf(FullKeyName,MaskKeyName,Position);
  DeleteFullKeyTree(FullKeyName);
}

class KeyRecordItem
{
  public:
   int ItemIdx;
   KeyRecordItem() { ItemIdx=0; }
   bool operator==(const KeyRecordItem &rhs) const{
     return ItemIdx == rhs.ItemIdx;
   };
   int operator<(const KeyRecordItem &rhs) const{
     return ItemIdx < rhs.ItemIdx;
   };
   const KeyRecordItem& operator=(const KeyRecordItem &rhs)
   {
     ItemIdx = rhs.ItemIdx;
     return *this;
   };

   ~KeyRecordItem()
   {
   }
};

void RenumKeyRecord(const char *KeyRoot,const char *KeyMask,const char *KeyMask0)
{
  TArray<KeyRecordItem> KAItems;
  KeyRecordItem KItem;
  int CurPos;
  char RegKey[80];
  char FullKeyName[512],PrevFullKeyName[512],MaskKeyName[512];
  BOOL Processed=FALSE;

  // ���� ������
  for (CurPos=0;;CurPos++)
  {
    if(!EnumRegKey(KeyRoot,CurPos,RegKey,sizeof(RegKey)))
      break;
    KItem.ItemIdx=atoi(RegKey+strlen(KeyMask0));
    if(KItem.ItemIdx != CurPos)
      Processed=TRUE;
    KAItems.addItem(KItem);
  }

  if(Processed)
  {
    KAItems.Sort();

    MkKeyName(KeyMask,MaskKeyName,sizeof(MaskKeyName)-1);
    for(int CurPos=0;;++CurPos)
    {
      KeyRecordItem *Item=KAItems.getItem(CurPos);
      if(!Item)
        break;

      // �������� ������������� CurPos
      sprintf(FullKeyName,KeyMask,CurPos);
      if(!CheckRegKey(FullKeyName))
      {
        sprintf(FullKeyName,MaskKeyName,CurPos);
        sprintf(PrevFullKeyName,MaskKeyName,Item->ItemIdx);
        if (!CopyKeyTree(PrevFullKeyName,FullKeyName))
          break;
        DeleteFullKeyTree(PrevFullKeyName);
      }
    }
  }
}


int CopyKeyTree(const char *Src,const char *Dest,const char *Skip)
{
  HKEY hSrcKey,hDestKey;
  if (RegOpenKeyEx(hRegRootKey,Src,0,KEY_READ,&hSrcKey)!=ERROR_SUCCESS)
    return(FALSE);
  DeleteFullKeyTree(Dest);
  DWORD Disposition;
  if (RegCreateKeyEx(hRegRootKey,Dest,0,NULL,0,KEY_WRITE,NULL,&hDestKey,&Disposition)!=ERROR_SUCCESS)
  {
    CloseRegKey(hSrcKey);
    return(FALSE);
  }

  int I;
  for (I=0;;I++)
  {
    char ValueName[200],ValueData[1000];
    DWORD Type,NameSize=sizeof(ValueName),DataSize=sizeof(ValueData);
    if (RegEnumValue(hSrcKey,I,ValueName,&NameSize,NULL,&Type,(BYTE *)ValueData,&DataSize)!=ERROR_SUCCESS)
      break;
    RegSetValueEx(hDestKey,ValueName,0,Type,(BYTE *)ValueData,DataSize);
  }
  CloseRegKey(hDestKey);

  for (I=0;;I++)
  {
    char SubkeyName[200],SrcKeyName[512],DestKeyName[512];
    DWORD NameSize=sizeof(SubkeyName);
    FILETIME LastWrite;
    if (RegEnumKeyEx(hSrcKey,I,SubkeyName,&NameSize,NULL,NULL,NULL,&LastWrite)!=ERROR_SUCCESS)
      break;
    xstrncpy(SrcKeyName,Src,sizeof(SrcKeyName)-1);
    strncat(SrcKeyName,"\\",sizeof(SrcKeyName)-1);
    strncat(SrcKeyName,SubkeyName,sizeof(SrcKeyName)-1);
    if (Skip!=NULL)
    {
      bool Found=false;
      const char *SkipName=Skip;
      while (!Found && *SkipName)
        if (LocalStricmp(SrcKeyName,SkipName)==0)
          Found=true;
        else
          SkipName+=strlen(SkipName)+1;
      if (Found)
        continue;
    }
    xstrncpy(DestKeyName,Dest,sizeof(DestKeyName)-1);
    strncat(DestKeyName,"\\",sizeof(DestKeyName)-1);
    strncat(DestKeyName,SubkeyName,sizeof(DestKeyName)-1);
    if (RegCreateKeyEx(hRegRootKey,DestKeyName,0,NULL,0,KEY_WRITE,NULL,&hDestKey,&Disposition)!=ERROR_SUCCESS)
      break;
    CloseRegKey(hDestKey);
    CopyKeyTree(SrcKeyName,DestKeyName);
  }

  CloseRegKey(hSrcKey);
  return(TRUE);
}


void DeleteKeyTree(const char *KeyName)
{
  char FullKeyName[200];
  MkKeyName(KeyName,FullKeyName,sizeof(FullKeyName)-1);
  if (WinVer.dwPlatformId!=VER_PLATFORM_WIN32_WINDOWS ||
      RegDeleteKey(hRegRootKey,FullKeyName)!=ERROR_SUCCESS)
    DeleteFullKeyTree(FullKeyName);
}


void DeleteFullKeyTree(const char *KeyName)
{
  do
  {
    DeleteCount=0;
    DeleteKeyTreePart(KeyName);
  } while (DeleteCount!=0);
}


void DeleteKeyTreePart(const char *KeyName)
{
  HKEY hKey;
  if (RegOpenKeyEx(hRegRootKey,KeyName,0,KEY_READ,&hKey)!=ERROR_SUCCESS)
    return;
  for (int I=0;;I++)
  {
    char SubkeyName[200],FullKeyName[512];
    DWORD NameSize=sizeof(SubkeyName);
    FILETIME LastWrite;
    if (RegEnumKeyEx(hKey,I,SubkeyName,&NameSize,NULL,NULL,NULL,&LastWrite)!=ERROR_SUCCESS)
      break;

    xstrncpy(FullKeyName,KeyName,sizeof(FullKeyName)-1);
    strncat(FullKeyName,"\\",sizeof(FullKeyName)-1);
    strncat(FullKeyName,SubkeyName,sizeof(FullKeyName)-1);
    DeleteKeyTreePart(FullKeyName);
  }
  CloseRegKey(hKey);
  if (RegDeleteKey(hRegRootKey,KeyName)==ERROR_SUCCESS)
    DeleteCount++;
}


/* 07.03.2001 IS
   �������� ������� ����� � ��� ������, ���� �� �� �������� ������� ����������
   � ���������. ���������� TRUE ��� ������.
*/
int DeleteEmptyKey(HKEY hRoot, const char *FullKeyName)
{
  HKEY hKey;
  int Exist=RegOpenKeyEx(hRoot,FullKeyName,0,KEY_ALL_ACCESS,&hKey)==ERROR_SUCCESS;
  if(Exist)
  {
     int RetCode=FALSE;
     if(hKey)
     {
        FILETIME LastWriteTime;
        char SubName[512];
        DWORD SubSize=sizeof(SubName);

        LONG ExitCode=RegEnumKeyEx(hKey,0,SubName,&SubSize,NULL,NULL,NULL,
                                   &LastWriteTime);

        if(ExitCode!=ERROR_SUCCESS)
           ExitCode=RegEnumValue(hKey,0,SubName,&SubSize,NULL,NULL,NULL, NULL);

        CloseRegKey(hKey);

        if(ExitCode!=ERROR_SUCCESS)
          {
            char KeyName[512], *pSubKey;
            xstrncpy(KeyName, FullKeyName, sizeof(KeyName)-1);
            if(NULL!=(pSubKey=strrchr(KeyName,'\\')))
              {
                 *pSubKey=0;
                 pSubKey++;
                 Exist=RegOpenKeyEx(hRoot,KeyName,0,KEY_ALL_ACCESS,&hKey)==ERROR_SUCCESS;
                 if(Exist && hKey)
                 {
                   RetCode=RegDeleteKey(hKey, pSubKey)==ERROR_SUCCESS;
                   CloseRegKey(hKey);
                 }
              }
          }
     }
     return RetCode;
  }
  return TRUE;
}
/* IS $ */


int CheckRegKey(const char *Key)
{
  HKEY hKey;
  char FullKeyName[512];
  MkKeyName(Key,FullKeyName,sizeof(FullKeyName)-1);
  int Exist=RegOpenKeyEx(hRegRootKey,FullKeyName,0,KEY_QUERY_VALUE,&hKey)==ERROR_SUCCESS;
  CloseRegKey(hKey);
  return(Exist);
}

/* 15.09.2000 IS
   ���������� FALSE, ���� ��������� ���������� �� �������� ������
   ��� ������ ������ ����� ����.
*/
int CheckRegValue(const char *Key,const char *ValueName)
{
  int ExitCode;
  DWORD DataSize=0;
  HKEY hKey=OpenRegKey(Key);
  if(hKey)
  {
    DWORD Type;
    ExitCode=RegQueryValueEx(hKey,ValueName,0,&Type,NULL,&DataSize);
    CloseRegKey(hKey);
  }
  if (hKey==NULL || ExitCode!=ERROR_SUCCESS || !DataSize)
    return(FALSE);
  return(TRUE);
}
/* IS $ */


int EnumRegKey(const char *Key,DWORD Index,char *DestName,DWORD DestSize)
{
  HKEY hKey=OpenRegKey(Key);
  if(hKey)
  {
    FILETIME LastWriteTime;
    char SubName[512];
    DWORD SubSize=sizeof(SubName);
    int ExitCode=RegEnumKeyEx(hKey,Index,SubName,&SubSize,NULL,NULL,NULL,&LastWriteTime);
    CloseRegKey(hKey);
    if (ExitCode==ERROR_SUCCESS)
    {
      char TempName[512];
      xstrncpy(TempName,Key,sizeof(TempName)-1);
      if (*TempName)
        AddEndSlash(TempName);
      strncat(TempName,SubName,sizeof(TempName)-1);
      xstrncpy(DestName,TempName,DestSize-1);
      return(TRUE);
    }
  }
  return(FALSE);
}

int EnumRegValue(const char *Key,DWORD Index,char *DestName,DWORD DestSize,LPBYTE SData,DWORD SDataSize,LPDWORD IData,__int64* IData64)
{
  HKEY hKey=OpenRegKey(Key);
  int RetCode=REG_NONE;

  if(hKey)
  {
    char ValueName[512];

    while( TRUE )
    {
      DWORD ValSize=sizeof(ValueName);
      DWORD Type=(DWORD)-1;

      if (RegEnumValue(hKey,Index,ValueName,&ValSize,NULL,&Type,SData,&SDataSize) != ERROR_SUCCESS)
        break;

      RetCode=Type;
      if(DestName)
        xstrncpy(DestName,ValueName,DestSize-1);

      if(Type == REG_SZ)
        break;
      else if(Type == REG_DWORD)
      {
        if(IData)
          *IData=*(DWORD*)SData;
        break;
      }
      else if(Type == REG_QWORD)
      {
        if(IData64)
          *IData64=*(__int64*)SData;
        break;
      }
    }

    CloseRegKey(hKey);
  }
  return RetCode;
}


LONG CloseRegKey(HKEY hKey)
{
  if (hRegCurrentKey || !hKey)
    return ERROR_SUCCESS;
  return(RegCloseKey(hKey));
}

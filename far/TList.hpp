#ifndef __TLIST_HPP__
#define __TLIST_HPP__

/*  TList.hpp
    ������� ������ ������ � ���������� ������� / by Spinoza /
    Object ������ ����� ����������� �� ��������� � ��������� ���������:
      const Object& operator=(const Object &)
*/

#if defined(__BORLANDC__)
  #pragma warn -inl
#endif

template <class Object>
class TList
{
  protected:
    struct OneItem
    {
      Object   Item;
      OneItem *Prev, *Next;
    };

    OneItem *First, *Last, *Current, *Tmp, *SavedPos;

    DWORD Size;

  public:
    TList(): First(NULL), Last(NULL), Current(NULL), SavedPos(0), Size(0)
      {}
    ~TList() { clear(); }

  public:
    // �������� ���������� ��������� � ������
    DWORD size() const { return Size; }

    // ���������� TRUE, ���� ������ ����
    bool empty() const { return !Size; }

    //  �������� ��������� �� ������� ������� ������ (���������� NULL ��� �������)
    const Object *getItem() { return Current?&Current->Item:NULL; }

    // ���������� TRUE, ���� ������� ������� ����������� �� ������ �������
    bool isBegin() const { return Current==First; }

    // ���������� TRUE, ���� ������� ������� ����������� �� ��������� �������
    bool isEnd() const { return Current==Last; }

    // ���� � ������ ������ � ������� ��������� �� ������ �������
    // ������������ NULL, ���� ������ ����
    const Object *toBegin()
    {
      Current=First;
      return Current?&Current->Item:NULL;
    }

    // ���� � ����� ������ � ������� ��������� �� ��������� �������
    // ������������ NULL, ���� ������ ����
    const Object *toEnd()
    {
      Current=Last;
      return Current?&Current->Item:NULL;
    }

    // ���� � ���������� �������� ������ � ������� ��������� �� ����
    // (���������� NULL, ���� ��������� ����� ������ ��� ������� �������
    // �� ��� ���������)
    const Object *toNext()
    {
      return (Current && NULL!=(Current=Current->Next))?&Current->Item:NULL;
    }

    // ���� � ����������� �������� ������ � ������� ��������� �� ����
    // (���������� NULL, ���� ���������� ������ ������ ��� ������� �������
    // �� ��� ���������)
    const Object *toPrev()
    {
      return (Current && NULL!=(Current=Current->Prev))?&Current->Item:NULL;
    }

    // �������� ������� � ������ ������
    // ������� ������� ��� ������ ��������������� �� ���� �������
    // ��� ������� ������������ FALSE
    bool push_front(const Object &Source)
    {
      Tmp=new OneItem;
      if (Tmp) // ���������, ���� �� ����� ����������
      {
        Tmp->Item=Source;
        Tmp->Prev=NULL;
        if (First)
          First->Prev=Tmp;
        Tmp->Next=First;
        First=Current=Tmp;
        if (!Last) // ������ �� �������� ��� ����
          Last=First;
        ++Size;
        return true;
      }
      return false;
    }

    // �������� ������� � ����� ������
    // ������� ������� ��� ������ ��������������� �� ���� �������
    // ��� ������� ������������ FALSE
    bool push_back(const Object &Source)
    {
      Tmp=new OneItem;
      if (Tmp) // ���������, ���� �� ����� ����������
      {
        Tmp->Item=Source;
        if(Last)
          Last->Next=Tmp;
        Tmp->Prev=Last;
        Tmp->Next=NULL;
        Last=Current=Tmp;
        if (!First) // ������ �� �������� ��� ����
          First=Last;
        ++Size;
        return true;
      }
      return false;
    }

    // �������� ������� ����� ������� ������� � ������
    // ���� ������� ������� �� ����������, �� ������� ����������� � ����� ������ (=push_back)
    // ������� ������� ��� ������ ��������������� �� ����� �������
    // ��� ������� ������������ FALSE
    bool insert(const Object &Source)
    {
      if (!Current)
        return push_back(Source);
      Tmp=new OneItem;
      if (Tmp) // ���������, ���� �� ����� ����������
      {
        if (isEnd())
          Last=Tmp;
        Tmp->Item=Source;
        Tmp->Next=Current->Next;
        Tmp->Prev=Current;
        Current->Next=Tmp;
        if (Tmp->Next)
          Tmp->Next->Prev=Tmp;
        Current=Tmp;
        ++Size;
        return true;
      }
      return false;
    }

    // ������� �������, ������� ������� ��������������� �� ��������� �������
    // ���� ������� ������� �� �������� �� ����������, �� ������������ FALSE
    bool removeToEnd()
    {
      SavedPos=NULL;
      if (Current)
      {
        if (isEnd())
          Last=Last->Prev;
        if (isBegin())
          First=First->Next;
        Tmp=Current->Next;
        if (Current->Next)
          Current->Next->Prev = Current->Prev;
        if (Current->Prev)
          Current->Prev->Next = Current->Next;
        delete Current;
        Current=Tmp;
        --Size;
        if (!Size)
          First=Last=NULL;
        return true;
      }
      return false;
    }

    // ������� removeToEnd
    bool erase() { return removeToEnd(); }

    // ������� �������, ������� ������� ��������������� �� ���������� �������
    // ���� ������� ������� �� �������� �� ����������, �� ������������ FALSE
    bool removeToBegin()
    {
      SavedPos=NULL;
      if (Current)
      {
        if (isEnd())
          Last=Last->Prev;
        if (isBegin())
          First=First->Next;
        Tmp=Current->Prev;
        if (Current->Next)
          Current->Next->Prev = Current->Prev;
        if (Current->Prev)
          Current->Prev->Next = Current->Next;
        delete Current;
        Current=Tmp;
        --Size;
        if (!Size)
          First=Last=NULL;
        return true;
      }
      return false;
    }

    // �������� ������ �� ���������� ������ lst � ��������
    void swap(TList<Object> &lst)
    {
      OneItem *newFirst=lst.First, *newLast=lst.Last,
              *newCurrent=lst.Current, *newSavedPos=lst.SavedPos;
      lst.First=First;
      lst.Last=Last;
      lst.Current=Current;
      lst.SavedPos=SavedPos;
      First=newFirst;
      Last=newLast;
      Current=newCurrent;
      SavedPos=newSavedPos;
    }

    // ��������� ������� ������� (��. restorePosition) ��� ������������
    // ��������������. ������������ FALSE, ���� ������� ������� ����
    // ��������������
    bool storePosition()
    {
      SavedPos=Current;
      return SavedPos!=NULL;
    }

    // ������������ ������� ������� (��. storePosition) ��� ������������
    // ��������������. ������������ FALSE, ���� ������� ������� �����
    // ��������������
    bool restorePosition()
    {
      Current=SavedPos;
      return Current!=NULL;
    }

    // �������� ������
    void clear()
    {
      toBegin();
      while(erase())
        ;
      SavedPos=NULL;
    }


  private:
    TList& operator=(const TList& rhs); /* ����� �� �������������� */
    TList(const TList& rhs);            /* �� ���������            */
};

#if defined(__BORLANDC__)
  #pragma warn .inl
#endif

#endif // __TLIST_HPP__

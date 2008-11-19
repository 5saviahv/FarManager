#ifndef __TPREREDRAWFUNC_HPP__
#define __TPREREDRAWFUNC_HPP__
/*
TPreRedrawFunc.hpp

������ ������

*/

typedef void (*PREREDRAWFUNC)(void);

struct PreRedrawParamStruct
{
  DWORD Flags;
  void *Param1;
  const void *Param2;
  const void *Param3;
  void *Param4;
  __int64 Param5;
};

struct PreRedrawItem
{
  PREREDRAWFUNC PreRedrawFunc;
  PreRedrawParamStruct Param;
};

class TPreRedrawFunc
{
  private:
    unsigned int Total;

    struct OneItem
    {
      PreRedrawItem Item;
      OneItem *Next;

      OneItem(struct PreRedrawItem NewItem,OneItem *NextItem) : Item(NewItem), Next(NextItem) {}
    };

    struct OneItem *Top, *current;

  public:
    static struct PreRedrawItem errorStack;

  public:
    TPreRedrawFunc() : Total(0), Top(NULL) {};
    ~TPreRedrawFunc() { Free(); }

  public:
    // ������ ������⢮ ����⮢ �� �⥪�
    unsigned int Size() const { return Total; }

    // ����� ����� � �⥪�
    PreRedrawItem Pop();

    // ����� ����� � �⥪� ��� ��������� �⥪�
    PreRedrawItem Peek();

    // �������� ����� �� �⥪
    PreRedrawItem Push(const PreRedrawItem &Source);
    PreRedrawItem Push(PREREDRAWFUNC Func,PreRedrawParamStruct *Param=NULL);

    PreRedrawItem SetParam(PreRedrawParamStruct Param);

    // ������ �⥪
    void Free();

    bool isEmpty() const {return Total==0;};


  private:
    //TPreRedrawFunc& operator=(const TPreRedrawFunc&){return *this;}; /* �⮡� �� �����஢����� */
    //TPreRedrawFunc(const TPreRedrawFunc&){};            /* �� 㬮�砭��            */

    //PREREDRAWFUNC Set(PREREDRAWFUNC fn);

};

extern TPreRedrawFunc PreRedraw;


class TPreRedrawFuncGuard
{
  public:
    TPreRedrawFuncGuard(PREREDRAWFUNC Func);
    ~TPreRedrawFuncGuard();
};


#endif  // __TPREREDRAWFUNC_HPP__

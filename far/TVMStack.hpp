#ifndef __TVMSTACK_HPP__
#define __TVMSTACK_HPP__

/*  VMStack.hpp
    ���� (LIFO) ��� ����������� ������ ��������
*/

#include "tvar.hpp"

class TVMStack
{
  private:
    unsigned int Total;
    struct OneItem
    {
      TVar  Item;
      OneItem *Next;

      OneItem(TVar NewVar,OneItem *NextItem) : Item(NewVar), Next(NextItem) {}
    };

    struct OneItem *Top, *current;

  public:
    static TVar errorStack;

  public:
    TVMStack():Total(0), Top(NULL) {};
    ~TVMStack() { Free(); }

  public:
    // ������� ���������� ��������� �� �����
    unsigned int Size() const { return Total; }

    // ����� ������� �� �����
    TVar Pop();

    // ����� ������� �� ����� ��� ��������� �����
    TVar Peek();

    // �������� ������� �� ����
    TVar Push(const TVar &Source);

    // �������� ����
    void Free();

    bool isEmpty() const {return Total==0;};

  private:
    TVMStack& operator=(const TVMStack& rhs); /* ����� �� �������������� */
    TVMStack(const TVMStack& rhs);            /* �� ���������            */
};

#endif // __TVMSTACK_HPP__

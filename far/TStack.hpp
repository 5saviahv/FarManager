#ifndef _TSTACK_HPP_
#define _TSTACK_HPP_

/*  TStack.hpp
    ������ ������ �� ������ (LIFO)

    TStack<Object> Stack;
    // Object ������ ����� ����������� �� ��������� � ��������
    // const Object& operator=(const Object &)
*/

template <class Object>
class TStack
{
	private:
		struct OneItem
		{
			Object  Item;
			OneItem *Next;
		};

		struct OneItem *Top;

		DWORD Size;

	public:
		TStack():Size(0), Top(NULL) {};
		~TStack() { Free(); }

	public:
		// ������� ���������� ��������� �� �����
		DWORD size() const { return Size; }

		// ���������� TRUE, ���� ������ ����
		bool empty() const { return !Size; }

		// ����� ������� �� �����
		// ��� ����� �������� ����� Destination, ����� - NULL
		Object *Pop(Object &Destination)
		{
			if (Top)
			{
				--Size;
				Destination=Top->Item;
				struct OneItem *Temp=Top->Next;
				delete Top;
				Top=Temp;
				return &Destination;
			}

			return NULL;
		}

		// ����� ������� �� ����� ��� ��������� �����
		// ��� ����� �������� ����� Destination, ����� - NULL
		Object *Peek(/*Object &Destination*/)
		{
			if (Top)
			{
				//Destination=Top->Item;
				//return &Destination;
				return &Top->Item;
			}

			return NULL;
		}

		// �������� ������� �� ����
		// ��� ����� �������� ����� �������� �� �����, ����� - NULL
		Object *Push(const Object &Source)
		{
			struct OneItem *Temp=new OneItem;

			if (Temp)
			{
				Temp->Next=Top;
				Temp->Item=Source;
				Top=Temp;
				++Size;
				return &Top->Item;
			}

			return NULL;
		}

		// �������� ����
		void Free()
		{
			while (Top)
			{
				struct OneItem *Temp=Top->Next;
				delete Top;
				Top=Temp;
			}

			Size=0;
		}

	private:
		TStack& operator=(const TStack& rhs); /* ����� �� �������������� */
		TStack(const TStack& rhs);            /* �� ���������            */
};

#endif // _TSTACK_HPP_

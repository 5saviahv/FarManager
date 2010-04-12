#ifndef __BLOCKEXTKEY_HPP__
#define __BLOCKEXTKEY_HPP__
/*
BlockExtKey.hpp

������� � ����������� - ���������� �������� ��������

���� ����� � �������� ������������ ������ � ����������� � ���
�������� �������� ��� ������ ����. ���� - �������������
��������� ���������� ������� (���� Alt-F9 � ��� � ���), �.�.
� ��� ����� �������� ��������� ������ � �����������, ��������������
� ������� ����������...

*/

#include "global.hpp"

class BlockExtKey
{
	private:
		int OldIsProcessAssignMacroKey, OldIsProcessVE_FindFile;

	public:
		BlockExtKey()
		{
			OldIsProcessAssignMacroKey=IsProcessAssignMacroKey;
			IsProcessAssignMacroKey=1;
			OldIsProcessVE_FindFile=IsProcessVE_FindFile;
			IsProcessVE_FindFile=0;
		}
		~BlockExtKey() {IsProcessAssignMacroKey=OldIsProcessAssignMacroKey; IsProcessVE_FindFile=OldIsProcessVE_FindFile;}
};

#endif // __BLOCKEXTKEY_HPP__

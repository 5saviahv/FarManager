#ifndef __SAVESCREEN_HPP__
#define __SAVESCREEN_HPP__
/*
savescr.hpp

���࠭塞 � ����⠭��������� �࠭ ��᪠�� � 楫����

*/

/* Revision: 1.00 25.06.2000 $ */

/*
Modify:
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

class SaveScreen
{
  private:
    void SaveArea(int X1,int Y1,int X2,int Y2);
    char *ScreenBuf;
    int CurPosX,CurPosY,CurVisible,CurSize;
    int X1,Y1,X2,Y2;
    int RealScreen;
  public:
    SaveScreen();
    SaveScreen(int RealScreen);
    SaveScreen(int X1,int Y1,int X2,int Y2,int RealScreen=FALSE);
    ~SaveScreen();
    void RestoreArea(int RestoreCursor=TRUE);
    void Discard();
    void AppendArea(SaveScreen *NewArea);
    CHAR_INFO* GetBufferAddress() {return((CHAR_INFO *)ScreenBuf);};
};

#endif	// __SAVESCREEN_HPP__


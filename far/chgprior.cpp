/*
chgprior.cpp

class ChangePriority

*/

/* Revision: 1.00 25.06.2000 $ */

/*
Modify:
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

#define STRICT

#if !defined(_INC_WINDOWS) && !defined(_WINDOWS_)
#include <windows.h>
#endif

#ifndef __CHANGEPRIORITY_HPP__
#include "chgprior.hpp"
#endif


ChangePriority::ChangePriority(int NewPriority)
{
  SavePriority=GetThreadPriority(GetCurrentThread());
  SetThreadPriority(GetCurrentThread(),NewPriority);
}


ChangePriority::~ChangePriority()
{
  SetThreadPriority(GetCurrentThread(),SavePriority);
}



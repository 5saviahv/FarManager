#
# ��������� FAR Manager
# Modify: 25.06.2000 SVS
# Modify: 27.06.2000 tran (precompiled headers)
# Modify: 29.06.2000 SVS (������� +BccW32.cfg -
#                         � �������� �� � ������� ��� ��ப�)
#                        + 䠩� FAR.EXE ᮧ������ � ⥪�饬 ��⠫���
#                          ��᫥ 祣� ��������� � final
# Modify: 04.07.2000 SVS +����஢���� hlf&lng 䠩��� � final
#                        +��⠢�� ��८�।������� �㭪橨 ����� �����
#
# Modify: 12.07.2000 SVS +$(FARCMEM) - ��� ����祭�� � ��⮢�� 楫��
#                         ����� cmem
# Modify: 06.12.2000 SVS + $(OBJPATH)\mix.obj: mix.cpp cc.bat
#              �⮡� ��窠�� �� 㤠���� mix.obj, �᫨ ���⠢��� 䫠� SYSLOG
#
# Modify: 30.12.2000 SVS + fileattr
#
# make -fmkfar.mak [options]
#   -DALLOC - ��⠢��� ��८�।������ �㭪樨 ࠡ��� � �������
# ����, ���� �� �㤥� ��४�������� �� ��������� H*-䠩���
.AUTODEPEND

# ������� �� ��६����� �।� (env)
LIBPATH=$(FARLIB)
INCLUDEPATH=$(FARINC)
BCCPATH=$(FARBCC)
# bccpath �㦥� ��� ⮣�, �⮡� ��אַ �ᯮ�짮���� ��� � exe 䠩���
# set bccpath=e:\bc5, ���� � path ����� ᨤ��� ᮢᥬ ��㣮� ��ૠ��


# ���� � ��⠫��� � �६���묨 䠩���� - obj, etc
OBJPATH=OBJ

# � �㤥� ����饭 १����
FINALPATH=final

.path.obj = $(OBJPATH)
.path.cpp = .
.path.exe = $(FINALPATH)

!ifdef TRY
OPTEXT=-x
!else
OPTEXT=-x-
!endif

!ifdef DEBUG
OPTDEBUG=-v
CSMFILE=fard.csm
!else
OPTDEBUG=-v-
CSMFILE=Far.csm
!endif


!ifdef PRECOMP
PRECOMPOPT=-H=$(OBJPATH)\$(CSMFILE)
!else
PRECOMPOPT=-H-
!endif

#
# Borland C++ tools
#
BCC32   = $(BCCPATH)\bin\Bcc32 +BccW32.cfg
!ifdef ILINK
TLINK32 = $(BCCPATH)\bin\ilink32
BRC32   = $(BCCPATH)\bin\Brcc32
!else
TLINK32 = $(BCCPATH)\bin\Tlink32
BRC32   = $(BCCPATH)\bin\Brc32
!endif
# TLIB    = $(BCCPATH)\TLib - � ��祬 �� ��� �㦥�?
# IMPLIB  = $(BCCPATH)\bin\Implib - � ��� ⮦�?


#
# Options
#
LINKFLAGS =  -L$(LIBPATH) -Tpe -ap -c $(OPTDEBUG) -s

# � ��祬 �㡫�஢��� ��樨 ��� � � bccw32.cfg?
#CCFLAGS = -WC -WM -K -d -f- $(OPTDEBUG) -R- -k- -x- -RT -Og -Ot -Z -O -Oe -Ol -Ob -Om -Op -Ov -w-csu $(PRECOMPOPT) -I$(INCLUDEPATH)
# SVS: ����! �.�. �஡���� � ������ ���. ��ப� ��� ��ᤠ��
#      ���� ��⠢�� ���⮩
CCFLAGS =

# !!! -d - merge duplicate string - �।��� ����, ������ ᨫ쭮 ��蠥�

#
# Dependency List
#
Dep_Far = \
   $(FINALPATH)\Far.exe

Far : BccW32.cfg $(Dep_Far)
  echo MakeNode

# �� �� ����ᨬ��� �� �㦭�
# ���� ����뢠�� ���� �ࠢ��� � �� %)

.cpp.obj:
  @settitle "{$.} - Compiling..."
  @$(BCC32) -c -o$@ {$. }

$(OBJPATH)\ctrlobj.obj: ctrlobj.cpp ctrlobj.hpp copyright.inc
$(OBJPATH)\syslog.obj: syslog.cpp cc.bat
$(OBJPATH)\global.obj: global.cpp global.hpp farversion.inc

FAROBJ=\
   $(OBJPATH)\modal.obj\
   $(OBJPATH)\help.obj\
   $(OBJPATH)\checkver.obj\
   $(OBJPATH)\plugapi.obj\
   $(OBJPATH)\language.obj\
   $(OBJPATH)\setcolor.obj\
   $(OBJPATH)\palette.obj\
   $(OBJPATH)\mkdir.obj\
   $(OBJPATH)\plugins.obj\
   $(OBJPATH)\manager.obj\
   $(OBJPATH)\poscache.obj\
   $(OBJPATH)\grabber.obj\
   $(OBJPATH)\macro.obj\
   $(OBJPATH)\scrbuf.obj\
   $(OBJPATH)\scrsaver.obj\
   $(OBJPATH)\keybar.obj\
   $(OBJPATH)\print.obj\
   $(OBJPATH)\iswind.obj\
   $(OBJPATH)\global.obj\
   $(OBJPATH)\history.obj\
   $(OBJPATH)\registry.obj\
   $(OBJPATH)\cmdline.obj\
   $(OBJPATH)\namelist.obj\
   $(OBJPATH)\dizlist.obj\
   $(OBJPATH)\grpsort.obj\
   $(OBJPATH)\gettable.obj\
   $(OBJPATH)\int64.obj\
   $(OBJPATH)\infolist.obj\
   $(OBJPATH)\hmenu.obj\
   $(OBJPATH)\vmenu.obj\
   $(OBJPATH)\qview.obj\
   $(OBJPATH)\editor.obj\
   $(OBJPATH)\options.obj\
   $(OBJPATH)\edit.obj\
   $(OBJPATH)\viewer.obj\
   $(OBJPATH)\filetype.obj\
   $(OBJPATH)\dialog.obj\
   $(OBJPATH)\findfile.obj\
   $(OBJPATH)\menubar.obj\
   $(OBJPATH)\interf.obj\
   $(OBJPATH)\usermenu.obj\
   $(OBJPATH)\chgmmode.obj\
   $(OBJPATH)\rdrwdsk.obj\
   $(OBJPATH)\copy.obj\
   $(OBJPATH)\panel.obj\
   $(OBJPATH)\scrobj.obj\
   $(OBJPATH)\savescr.obj\
   $(OBJPATH)\delete.obj\
   $(OBJPATH)\flupdate.obj\
   $(OBJPATH)\flplugin.obj\
   $(OBJPATH)\scantree.obj\
   $(OBJPATH)\keyboard.obj\
   $(OBJPATH)\clipboard.obj\
   $(OBJPATH)\eject.obj\
   $(OBJPATH)\xlat.obj\
   $(OBJPATH)\mix.obj\
   $(OBJPATH)\plist.obj\
   $(OBJPATH)\hilight.obj\
   $(OBJPATH)\config.obj\
   $(OBJPATH)\message.obj\
   $(OBJPATH)\setattr.obj\
   $(OBJPATH)\plognmn.obj\
   $(OBJPATH)\filestr.obj\
   $(OBJPATH)\local.obj\
   $(OBJPATH)\filter.obj\
   $(OBJPATH)\lockscrn.obj\
   $(OBJPATH)\fileedit.obj\
   $(OBJPATH)\fileview.obj\
   $(OBJPATH)\filelist.obj\
   $(OBJPATH)\treelist.obj\
   $(OBJPATH)\savefpos.obj\
   $(OBJPATH)\chgprior.obj\
   $(OBJPATH)\foldtree.obj\
   $(OBJPATH)\ffolders.obj\
   $(OBJPATH)\ctrlobj.obj\
   $(OBJPATH)\flmodes.obj\
   $(OBJPATH)\flshow.obj\
   $(OBJPATH)\farrtl.obj\
   $(OBJPATH)\syslog.obj\
   $(OBJPATH)\fileattr.obj\
   $(OBJPATH)\main.obj

Dep_fardexe = BccW32.cfg\
   Far.def\
   $(OBJPATH)\Far.res \
   $(FAROBJ)


!ifdef ILINK
$(FINALPATH)\Far.exe : $(Dep_fardexe)
  @settitle "Linking..."
  @$(TLINK32)  $(LINKFLAGS) @&&|
$(LIBPATH)\c0x32.obj $(FAROBJ)
$<,$*
$(LIBPATH)\import32.lib $(LIBPATH)\cw32mt.lib
Far.def
$(OBJPATH)\Far.res
|
!else
$(FINALPATH)\Far.exe : $(Dep_fardexe)
  @settitle "Linking..."
  @$(TLINK32)  $(LINKFLAGS) @&&|
$(LIBPATH)\c0x32.obj $(FAROBJ)
$<,$*
$(LIBPATH)\import32.lib $(LIBPATH)\cw32mt.lib
Far.def
|
   @copy Far.map $(FINALPATH)\Far.map
   @$(BRC32) $(OBJPATH)\Far.res $(OBJPATH)\Far.res $<
!endif

# ��易⥫쭮! �� �� � ����� �� ������...
   @del $(FINALPATH)\FarEng.hlf  >nul
   @del $(FINALPATH)\FarRus.hlf  >nul
   @del $(FINALPATH)\FarEng.lng  >nul
   @del $(FINALPATH)\FarRus.lng  >nul
!ifdef ILINK
   @del $(FINALPATH)\Far.ilc  >nul
   @del $(FINALPATH)\Far.ild  >nul
   @del $(FINALPATH)\Far.ilf  >nul
   @del $(FINALPATH)\Far.ils  >nul
   @del $(FINALPATH)\Far.tds  >nul
!endif
   @copy FarEng.hlf $(FINALPATH)\FarEng.hlf >nul
   @copy FarRus.hlf $(FINALPATH)\FarRus.hlf >nul
   @copy FarEng.lng $(FINALPATH)\FarEng.lng >nul
   @copy FarRus.lng $(FINALPATH)\FarRus.lng >nul

$(OBJPATH)\Far.res :  Far.rc
  @settitle "Compiling resource..."
  $(BRC32) -R @&&|
 $(RESFLAGS)  -FO$@ Far.rc
|

# Compiler configuration file
# ��� ��, � ���� ����� ���. ��ப� ��࠭�祭�
BccW32.cfg : mkfar.mak cc.bat
   Copy &&|
-WC
-WM
-K
-d
-f-
-R-
-k-
-RT
-Og
-Ot
-Z
-a8
-O
-Oe
-Ol
-Ob
-Om
-Op
-Ov
-w-csu
-I$(INCLUDEPATH)
$(FARCMEM)
$(FARALLOC)
$(PRECOMPOPT)
$(OPTDEBUG)
$(OPTEXT)
$(FARSYSLOG)
| $@

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

!ifdef DEBUG
OPTDEBUG=-v
CSMFILE=fard.csm
!else
OPTDEBUG=-v-
CSMFILE=far.csm
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
#BCC32   = $(BCCPATH)\bin\Bcc32
TLINK32 = $(BCCPATH)\bin\TLink32
BRC32   = $(BCCPATH)\bin\Brc32
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
   $(FINALPATH)\far.exe

Far : BccW32.cfg $(Dep_Far)
  echo MakeNode

# �� �� ����ᨬ��� �� �㦭�
# ���� ����뢠�� ���� �ࠢ��� � �� %)

.cpp.obj:
  $(BCC32) -c -o$@ {$. }

$(OBJPATH)\ctrlobj.obj: ctrlobj.cpp ctrlobj.hpp copyright.inc

!ifdef ALLOC
Dep_fardexe = BccW32.cfg\
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
   $(OBJPATH)\main.obj\
   $(OBJPATH)\farrtl.obj\
   far.def\
   $(OBJPATH)\far.res
!else
Dep_fardexe = BccW32.cfg\
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
   $(OBJPATH)\main.obj\
   far.def\
   $(OBJPATH)\far.res
!endif

!ifdef ALLOC
$(FINALPATH)\far.exe : $(Dep_fardexe)
  $(TLINK32) @&&|
 $(LINKFLAGS) +
$(LIBPATH)\c0x32.obj+
$(OBJPATH)\farrtl.obj+
$(OBJPATH)\modal.obj+
$(OBJPATH)\flplugin.obj+
$(OBJPATH)\help.obj+
$(OBJPATH)\rdrwdsk.obj+
$(OBJPATH)\checkver.obj+
$(OBJPATH)\setcolor.obj+
$(OBJPATH)\mkdir.obj+
$(OBJPATH)\language.obj+
$(OBJPATH)\print.obj+
$(OBJPATH)\palette.obj+
$(OBJPATH)\plugapi.obj+
$(OBJPATH)\interf.obj+
$(OBJPATH)\dizlist.obj+
$(OBJPATH)\edit.obj+
$(OBJPATH)\plugins.obj+
$(OBJPATH)\filestr.obj+
$(OBJPATH)\mix.obj+
$(OBJPATH)\delete.obj+
$(OBJPATH)\manager.obj+
$(OBJPATH)\poscache.obj+
$(OBJPATH)\options.obj+
$(OBJPATH)\grabber.obj+
$(OBJPATH)\macro.obj+
$(OBJPATH)\scrbuf.obj+
$(OBJPATH)\scrobj.obj+
$(OBJPATH)\flshow.obj+
$(OBJPATH)\ffolders.obj+
$(OBJPATH)\setattr.obj+
$(OBJPATH)\panel.obj+
$(OBJPATH)\local.obj+
$(OBJPATH)\iswind.obj+
$(OBJPATH)\config.obj+
$(OBJPATH)\message.obj+
$(OBJPATH)\flupdate.obj+
$(OBJPATH)\ctrlobj.obj+
$(OBJPATH)\keybar.obj+
$(OBJPATH)\scrsaver.obj+
$(OBJPATH)\chgmmode.obj+
$(OBJPATH)\registry.obj+
$(OBJPATH)\global.obj+
$(OBJPATH)\grpsort.obj+
$(OBJPATH)\gettable.obj+
$(OBJPATH)\hmenu.obj+
$(OBJPATH)\vmenu.obj+
$(OBJPATH)\history.obj+
$(OBJPATH)\namelist.obj+
$(OBJPATH)\int64.obj+
$(OBJPATH)\infolist.obj+
$(OBJPATH)\plognmn.obj+
$(OBJPATH)\lockscrn.obj+
$(OBJPATH)\qview.obj+
$(OBJPATH)\editor.obj+
$(OBJPATH)\menubar.obj+
$(OBJPATH)\viewer.obj+
$(OBJPATH)\scantree.obj+
$(OBJPATH)\filetype.obj+
$(OBJPATH)\savescr.obj+
$(OBJPATH)\dialog.obj+
$(OBJPATH)\savefpos.obj+
$(OBJPATH)\chgprior.obj+
$(OBJPATH)\flmodes.obj+
$(OBJPATH)\findfile.obj+
$(OBJPATH)\usermenu.obj+
$(OBJPATH)\fileedit.obj+
$(OBJPATH)\fileview.obj+
$(OBJPATH)\foldtree.obj+
$(OBJPATH)\copy.obj+
$(OBJPATH)\plist.obj+
$(OBJPATH)\hilight.obj+
$(OBJPATH)\filter.obj+
$(OBJPATH)\filelist.obj+
$(OBJPATH)\treelist.obj+
$(OBJPATH)\cmdline.obj+
$(OBJPATH)\main.obj
$<,$*
$(LIBPATH)\import32.lib+
$(LIBPATH)\cw32mt.lib
far.def
|
!else
$(FINALPATH)\far.exe : $(Dep_fardexe)
  $(TLINK32) @&&|
 $(LINKFLAGS) +
$(LIBPATH)\c0x32.obj+
$(OBJPATH)\modal.obj+
$(OBJPATH)\flplugin.obj+
$(OBJPATH)\help.obj+
$(OBJPATH)\rdrwdsk.obj+
$(OBJPATH)\checkver.obj+
$(OBJPATH)\setcolor.obj+
$(OBJPATH)\mkdir.obj+
$(OBJPATH)\language.obj+
$(OBJPATH)\print.obj+
$(OBJPATH)\palette.obj+
$(OBJPATH)\plugapi.obj+
$(OBJPATH)\interf.obj+
$(OBJPATH)\dizlist.obj+
$(OBJPATH)\edit.obj+
$(OBJPATH)\plugins.obj+
$(OBJPATH)\filestr.obj+
$(OBJPATH)\mix.obj+
$(OBJPATH)\delete.obj+
$(OBJPATH)\manager.obj+
$(OBJPATH)\poscache.obj+
$(OBJPATH)\options.obj+
$(OBJPATH)\grabber.obj+
$(OBJPATH)\macro.obj+
$(OBJPATH)\scrbuf.obj+
$(OBJPATH)\scrobj.obj+
$(OBJPATH)\flshow.obj+
$(OBJPATH)\ffolders.obj+
$(OBJPATH)\setattr.obj+
$(OBJPATH)\panel.obj+
$(OBJPATH)\local.obj+
$(OBJPATH)\iswind.obj+
$(OBJPATH)\config.obj+
$(OBJPATH)\message.obj+
$(OBJPATH)\flupdate.obj+
$(OBJPATH)\ctrlobj.obj+
$(OBJPATH)\keybar.obj+
$(OBJPATH)\scrsaver.obj+
$(OBJPATH)\chgmmode.obj+
$(OBJPATH)\registry.obj+
$(OBJPATH)\global.obj+
$(OBJPATH)\grpsort.obj+
$(OBJPATH)\gettable.obj+
$(OBJPATH)\hmenu.obj+
$(OBJPATH)\vmenu.obj+
$(OBJPATH)\history.obj+
$(OBJPATH)\namelist.obj+
$(OBJPATH)\int64.obj+
$(OBJPATH)\infolist.obj+
$(OBJPATH)\plognmn.obj+
$(OBJPATH)\lockscrn.obj+
$(OBJPATH)\qview.obj+
$(OBJPATH)\editor.obj+
$(OBJPATH)\menubar.obj+
$(OBJPATH)\viewer.obj+
$(OBJPATH)\scantree.obj+
$(OBJPATH)\filetype.obj+
$(OBJPATH)\savescr.obj+
$(OBJPATH)\dialog.obj+
$(OBJPATH)\savefpos.obj+
$(OBJPATH)\chgprior.obj+
$(OBJPATH)\flmodes.obj+
$(OBJPATH)\findfile.obj+
$(OBJPATH)\usermenu.obj+
$(OBJPATH)\fileedit.obj+
$(OBJPATH)\fileview.obj+
$(OBJPATH)\foldtree.obj+
$(OBJPATH)\copy.obj+
$(OBJPATH)\plist.obj+
$(OBJPATH)\hilight.obj+
$(OBJPATH)\filter.obj+
$(OBJPATH)\filelist.obj+
$(OBJPATH)\treelist.obj+
$(OBJPATH)\cmdline.obj+
$(OBJPATH)\main.obj
$<,$*
$(LIBPATH)\import32.lib+
$(LIBPATH)\cw32mt.lib
far.def
|
!endif

   $(BRC32) $(OBJPATH)\far.res $(OBJPATH)\far.res $<
# ��易⥫쭮! �� �� � ����� �� ������...
   @del $(FINALPATH)\FarEng.hlf
   @del $(FINALPATH)\FarRus.hlf
   @del $(FINALPATH)\FarEng.lng
   @del $(FINALPATH)\FarRus.lng
   @copy FarEng.hlf $(FINALPATH)\FarEng.hlf
   @copy FarRus.hlf $(FINALPATH)\FarRus.hlf
   @copy FarEng.lng $(FINALPATH)\FarEng.lng
   @copy FarRus.lng $(FINALPATH)\FarRus.lng

$(OBJPATH)\far.res :  far.rc
  $(BRC32) -R @&&|
 $(RESFLAGS)  -FO$@ far.rc
|

# Compiler configuration file
# ��� ��, � ���� ����� ���. ��ப� ��࠭�祭�
BccW32.cfg :
   Copy &&|
-WC
-WM
-K
-d
-f-
$(OPTDEBUG)
-R-
-k-
-x-
-RT
-Og
-Ot
-Z
-O
-Oe
-Ol
-Ob
-Om
-Op
-Ov
-w-csu
$(PRECOMPOPT)
-I$(INCLUDEPATH)
| $@

#ifndef __FARLANG_HPP__
#define __FARLANG_HPP__
/*
lang.hpp

�������������� ��������� �����

�������� - �� ������� ����������� ������ enum!
           � lngedit �������� ������ � ������� ���������

           ����� � lng ����� �� ������� ����� 2� ������������
           ������.

  MListEval - ������ ���� ������ ��������� - �� ��� �� ���� �� ������

*/

/* Revision: 1.111 20.09.2002 $ */

/*
Modify:
  20.09.2002 SVS
    - BugZ#645 - �� ������ ����������� ��������� ������
  04.09.2002 SVS
    + MEditAskSaveExt, MEditBtnSaveAs
  06.07.2002 VVM
    + MComspecNotFound
  22.05.2002 SVS
    + MConfigCloseCDGate
  29.04.2002 SVS
    + MShortcutPlugin
  27.04.2002 SVS
    + MSaveThisShortcut
  02.04.2002 KM
    + MFindFileDrive,MSearchFromRootOfDrive
  28.03.2002 SVS
    + MListGb, MListTb
    ! MListMb ����� ����� MListKb
  27.03.2002 SVS
    + MErrorFullPathNameLong
  22.03.2002 SVS
    ! ����������� MNeedNearPath
  21.03.2002 SVS
    + MNeedNearPath
  20.03.2002 DJ
    + MAssocNeedMask
  19.03.2002 SVS
    + MEditSaveMac
  12.03.2002 VVM
    + MDoYouWantToStopWork2
  02.03.2002 KM
  	+ MCopySkipAll
  21.02.2002 SVS
    + MChangeWaitingLoadDisk, MPlgRequired2
  23.01.2002 SVS
    + MEditSavedChangedNonFile2, MEditNewPath?
  10.01.2002 SVS
    + MEditSavedChangedNonFile
  03.01.2002 IS
    ! MEditDisableROFileModification -> MEditLockROFileModification
      MMacroSettingsDisableOutput -> MMacroSettingsEnableOutput
  03.01.2002 SVS
    ! �������� ������ � ��� ������ ��� $Date
  28.12.2001 SVS
    + � ��������: MHYes, MHNo, MHOk, MHCancel, MHRetry, MHSkip, MHAbort,
                  MHIgnore, MHDelete, MHSplit
  26.12.2001 SVS
    + MCannotUpdateRODiz, MCfgDizROUpdate
  24.12.2001 SVS
    + MHelpSearch* - ��� �������� ������ � �����.
  17.12.2001 IS
    + MConfigMousePanelMClickRule - ����� ��� ������� ������ ���� � �������
  11.12.2001 SVS
    + MKBTree*F*
  07.12.2001 IS
    + MMultiMakeDir - ����� ��� ������� �������� ��������
  20.11.2001 SVS
    + MConfigCopyTimeRule, MConfigAutoUpdateLimit, MConfigAutoUpdateLimit2
    ! ������ MConfigSubstPluginPrefix, MConfigShowMenuScrollbar
  09.11.2001 IS
    + MPanelBracketsForLongName
  02.11.2001 SVS
    + MHelpType, MVMenuType
  26.10.2001 KM
    + MFindFolders
  22.10.2001 SVS
    + �� ������ ���������� � Win2K
  16.10.2001 SVS
    + MCopyStreamN
  12.10.2001 SVS
    + MSetAttrOriginal
  05.10.2001 SVS
    + MCannotCreateListWrite, MCannotCreateListTemp - ������� - ��-�� ����
      �� ������ ������� ���� ������ - ������ ��� �������� �� ��������� :-(
  03.10.2001 SVS
    ! ������ MExcUnload �� �������������
  18.09.2001 SVS
    + MExcFloat*,MCMLTargetTO
  11.09.2001 SVS
    + MQuickViewVolMount, MSetAttrVolMount
  08.09.2001 VVM
    + MConfigDialogsEditBlock - ��� ���������� ������ � ������� �����
  29.08.2001 VVM
    + MUserMenuInvalidFormat
  08.08.2001 KM
    + MFindFileCodePage
    ! MFindFileAllTables
  06.08.2001 SVS
    + MAskDeleteSubMenuItem
  03.08.2001 IS
    + ��� ������� �����������/�����������: MCopyMultiActions
  02.08.2001 IS
    + ����� ������� ��� ������� �������������� ���������� ������:
      MFileAssocAltExec, MFileAssocAltView, MFileAssocAltEdit
  01.08.2001 SVS
    ! ���������, ��������� � ������ ���������� � ���� �����!
    + MDocumentsHelpTitle
  24.07.2001 SVS
    + MConfigPgUpChangeDisk, MSetConfirmHistoryClear
  23.07.2001 VVM
    + MHistoryClear
    + MClear
  22.07.2001 SVS
    + MNewOpen, MSetConfirmAllowReedit
    + MFindFileResultTitle
  18.07.2001 VVM
    + MCopyCannotRenameFolder
    + MCopyIgnore
  01.07.2001 IS
    + MIncorrectMask
  22.06.2001 SVS
    + MStrFTime*
  17.06.2001 SVS
    + MListSymLink
  07.06.2001 SVS
    + MSetColorClockNormalEditor, MSetColorClockNormalViewer
  05.06.2001 IS
    + MEditCanNotEditDirectory
  04.06.2001 IS
    + MIncorrectDirList - ��������� � ������������ ������ ���������
  02.06.2001 IS
    + MCopyIncorrectTargetList - ��������� � ������������ ������ ����� ���
      ����������� ��� �����������
  30.05.2001 SVS
    + MCMLItems* - ��� ��������� ��� ������� �����������
    + MCopyCannotCreateJunction, MCopyFolderNotEmpty, MCopyCannotCreateVolMount,
      MCopyRetrVolFailed, MCopyMountVolFailed, MCopyMountVolFailed2,
      MCopyCannotSupportVolMount, MCopyMountName,
  24.05.2001 SVS
    ! ������� ������� ��������� � EditorConfig ���������� ���� (��� �����
      ��������� :-)
  21.05.2001 DJ
    + MDialogType
  21.05.2001 SVS
    + MSetColorMenuDisabled, MSetColorHMenuDisabled
  20.05.2001 IS
    + MEditDisableROFileModification, MEditWarningBeforeOpenROFile
  30.04.2001 DJ
    + MInfo*F1-F12, MQView*F1-F12
  25.04.2001 SVS
    ! ��������� ��������� MMacroSettings* � ����� � "����������" �������
      ���������� �������.
  12.04.2001 SVS
    + MSetAttrUnknownJunction
  08.04.2001 SVS
    + MCopyNotSupportLinkX
  06.04.2001 SVS
    + MCopySymLink - ������ �� ������� (�� ����� ����� � H-������ ������ :-)
    + MEditSaveAsFormatTitle
  04.04.2001 VVM
    + MSetAttrBlank
  20.03.2001 tran
    + MCheckingFileInPlugin
  20.03.2001 SVS
    + MEditFileLong2
  16.03.2001 SVS
    + MNetUserName, MNetUserPassword,
    ! � �������-������� �������� Exit ������ �� ������ ������ ���������
    + MChangeDriveDisconnectMapped
  15.03.2001 SVS
    + �� ������ �������� ������������ ������:
      MChangeDriveDisconnectTitle, MChangeDriveDisconnectQuestion,
      MChangeDriveDisconnectReconnect, MSetConfirmRemoveConnection
  15.03.2001 IS
    + MAdditionalHotKey - ������ ��������� �� ������ 11 �������������� �������
      ��� ���� ������ �����. ��������� ������������ ������ ������� � ������ ��
      48 (�.�. �� '0').
  13.03.2001 SVS
    + MAskDeleteLink, MDeleteLinkDelete, MDeleteLinkUnlink
  26.02.2001 VVM
    + MExcInvalidFuncResult
  22.02.2001 SVS
    + MMacroDis* - �� ������ ����������� ��������
  09.02.2001 IS
    + MSetConfirmEsc
    + MKeyESCWasPressed, MDoYouWantToStopWork
  01.02.2001 SVS
    + MQuickViewJunction
  30.01.2001 VVM
    + MCopyTimeInfo
  28.01.2001 SVS
    + MAbort,  MIgnore
  25.01.2001 SVS
    + MExcStructWrongFilled, MExcStructField
  23.01.2001 skv
    + MExcBreakPoint, MExcUnknown
  19.01.2001 SVS
    ! ������ � MRegTitle �� MListEval ���������� � ����� ������.
      MListEval - ������ ���� ������ ��������� - �� ��� �� ���� �� ������
  17.01.2001 SVS
    + MMacroReDefinedKey, MMacroReDefinedKey2, MMacroDeleteKey, MMacroDeleteKey2,
  14.01.2001 SVS
    + MHighlightJunction, MSetAttrJunction
  05.01.2001 SVS
    + MAskDeleteItems0,   MAskDeleteItemsA,   MAskDeleteItemsS,
  05.01.2001 IS
    + MAskDeleteFile, MAskDeleteRecycleFile, MAskWipeFile
    ! MAskDeleteFiles -> MAskDeleteItems
  05.01.2001 SVS
    + MInfoSUBST, MChangeDriveSUBST, MChangeDriveCannotDelSubst
  04.01.2001 SVS
    + MMacroSettingsIgnoreFileFolders,
    + MMacroSettingsFolders,
    + MMacroSettingsFiles,
  03.01.2001 SVS
    + MSetAttrSelectedObjects
    -   MSetAttrChange,
    -   MSetAttrSetClear,
  30.12.2000 SVS
    + MHelp*F1-12
  21.12.2000 SVS
    + MMacroSettingsIgnorePanels
  21.12.2000 SVS
    + MMacroSettingsFilePanels, MMacroSettingsPluginPanels
  14.12.2000 SVS
    + MChangeVolumeInUse, MChangeVolumeInUse2,
      MChangeCouldNotUnlockMedia, MChangeCouldNotEjectMedia
  13.12.2000
    + MEditInputSize
  04.12.2000 SVS
    + MSetColorDialog*Disabled, MSetColorWarning*Disabled, MCheckBox2State
  29.11.2000 SVS
    + MEditROOpen, MEditRSH, MEditFileLong
  29.11.2000 SVS
    + MViewF9: � lng ����� ������ �� ��� ����� ��� F9 �� ������ -
      ��������� ������ :))
  27.11.2000 SVS
    + MExcDebugger
  22.11.2000 SVS
    + MSetColorDialogMenuScrollBar - ������ ��������� ��� ������
  02.11.2000 OT
    + MBuffSizeTooSmall_1, MBuffSizeTooSmall_2
 23.10.2000 SVS
    + MExcCheckOnLousys :-)
  20.10.2000 SVS
    + MHighlightEncrypted, MSetAttrEncrypted, MSetAttrEncryptedCannotFor,
    ! ������ MSetAttrNTFSOnly �� �������������
  17.10.2000 SVS
    + ��� ��������� MExc*
  16.10.2000 SVS
    + MExc* - ��� ����������
  27.09.2000 SVS
    + MHelpActivatorFormat
    + MHelpActivatorURL
    + MHelpActivatorQ
  24.09.2000 SVS
    + MViewConfigSaveShortPos
    + MEditConfigSaveShortPos
  20.09.2000 SVS
    + MConfigSubstPluginPrefix
  15.09.2000 IS
    + MDistributionTableWasNotFound, MAutoDetectWillNotWork
  13.09.2000 tran 1.16
    + MSetColorCommandLinePrefix
  12.09.2000 SVS
    ! MViewF2WWrap ������� � ���������� �� MViewShiftF2
  04.08.2000 SVS
    + MCopyOnlyNewerFiles
    - MEditConfigWordDiv
  03.08.2000 tran
    + ����� ��������� ��� "������" MPlgBadVers, MPlgRequired
  03.08.2000 KM 1.12
    + ����� ��������� MFindFileWholeWords,MViewSearchWholeWords,
      MEditSearchWholeWords ��� ������ �� ����� ������ � Find file (Alt-F7),
      ������ �� ������� � ���������.
  03.08.2000 SVS
    + MEditConfigWordDiv - ����������� ���� � ���������� ���������
  02.08.2000 SVS
    + CtrlAlt*, AltShift*, CtrlShift*
  26.07.2000 SVS
    + ��������� MConfigAutoComplete
  24.07.2000 VVM
    + ����� ��������� � ����� � ���������m usermenu.cpp
      MMainMenuFAR � MMainMenuREG.
  18.07.2000 tran
    + ����� ��������� � ����� � ScrollBar in Viewer
  15.07.2000 SVS
    + ��������� MConfigPersonalPath
  12.07.2000 SVS
    + ��������� MViewF2WWrap
  07.07.2000 IS
    + ������ ��� ��������� � ������������� �������������� ��������� ������ ��
      ���������: MHighlightWarning, MHighlightAskRestore,
    ! � lng-������ ���������� ������ MHighlightBottom - ������� ���������� �
      "Ctrl+R"
  06.07.2000 tran
    ! �������� ���� ������
  06.07.2000 SVS
    + �������
      MSetColorDialogMenuHighLight,
      MSetColorDialogMenuSelectedHighLight,
  04.07.2000 SVS
    ! Scroll bar � ���� �������� �� Options|Panel settings
      � Options|Interface settings
  29.06.2000 tran
    + �������� ��� ���� �������������� ������ � viewer & editor
  29.06.2000 SVS
    + ����� ����� ��� Options|Panel settings
      [ ] Show scrollbar into Menus
    + ����� ����� ��� ��������� ������ Menu ��� Menu Scrollbar
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

enum
{

  MShareware=0,
  MRegistered,
  MYes,
  MNo,
  MOk,

  MHYes,
  MHNo,
  MHOk,

  MCancel,
  MRetry,
  MSkip,
  MAbort,
  MIgnore,
  MDelete,
  MSplit,

  MHCancel,
  MHRetry,
  MHSkip,
  MHAbort,
  MHIgnore,
  MHDelete,
  MHSplit,

  MWarning,
  MError,

  MQuit,
  MAskQuit,

  MF1,
  MF2,
  MF3,
  MF4,
  MF5,
  MF6,
  MF7,
  MF8,
  MF9,
  MF10,
  MF11,
  MF12,

  MAltF1,
  MAltF2,
  MAltF3,
  MAltF4,
  MAltF5,
  MAltF6,
  MAltF7,
  MAltF8,
  MAltF9,
  MAltF10,
  MAltF11,
  MAltF12,

  MCtrlF1,
  MCtrlF2,
  MCtrlF3,
  MCtrlF4,
  MCtrlF5,
  MCtrlF6,
  MCtrlF7,
  MCtrlF8,
  MCtrlF9,
  MCtrlF10,
  MCtrlF11,
  MCtrlF12,

  MShiftF1,
  MShiftF2,
  MShiftF3,
  MShiftF4,
  MShiftF5,
  MShiftF6,
  MShiftF7,
  MShiftF8,
  MShiftF9,
  MShiftF10,
  MShiftF11,
  MShiftF12,

  MAltShiftF1,
  MAltShiftF2,
  MAltShiftF3,
  MAltShiftF4,
  MAltShiftF5,
  MAltShiftF6,
  MAltShiftF7,
  MAltShiftF8,
  MAltShiftF9,
  MAltShiftF10,
  MAltShiftF11,
  MAltShiftF12,

  MCtrlShiftF1,
  MCtrlShiftF2,
  MCtrlShiftF3,
  MCtrlShiftF4,
  MCtrlShiftF5,
  MCtrlShiftF6,
  MCtrlShiftF7,
  MCtrlShiftF8,
  MCtrlShiftF9,
  MCtrlShiftF10,
  MCtrlShiftF11,
  MCtrlShiftF12,

  MCtrlAltF1,
  MCtrlAltF2,
  MCtrlAltF3,
  MCtrlAltF4,
  MCtrlAltF5,
  MCtrlAltF6,
  MCtrlAltF7,
  MCtrlAltF8,
  MCtrlAltF9,
  MCtrlAltF10,
  MCtrlAltF11,
  MCtrlAltF12,

  MHistoryTitle,
  MFolderHistoryTitle,
  MViewHistoryTitle,

  MHistoryView,
  MHistoryEdit,
  MHistoryExt,

  MHistoryClear,
  MClear,

  MConfigSystemTitle,
  MConfigRO,
  MConfigRecycleBin,
  MConfigSystemCopy,
  MConfigCopySharing,
  MConfigCreateUppercaseFolders,
  MConfigInactivity,
  MConfigInactivityMinutes,
  MConfigSaveHistory,
  MConfigSaveFoldersHistory,
  MConfigSaveViewHistory,
  MConfigRegisteredTypes,
  MConfigCloseCDGate,
  MConfigPersonalPath,
  MConfigAutoSave,

  MConfigPanelTitle,
  MConfigHidden,
  MConfigHighlight,
  MConfigAutoChange,
  MConfigSelectFolders,
  MConfigReverseSort,
  MConfigAutoUpdateLimit,
  MConfigAutoUpdateLimit2,
  MConfigShowColumns,
  MConfigShowStatus,
  MConfigShowTotal,
  MConfigShowFree,
  MConfigShowScrollbar,
  MConfigShowScreensNumber,
  MConfigShowSortMode,

  MConfigInterfaceTitle,
  MConfigClock,
  MConfigViewerEditorClock,
  MConfigMouse,
  MConfigMousePanelMClickRule,
  MConfigKeyBar,
  MConfigMenuBar,
  MConfigSaver,
  MConfigSaverMinutes,
  MConfigDialogsEditHistory,
  MConfigDialogsEditBlock,
  MConfigUsePromptFormat,
  MConfigAltGr,
  MConfigCopyTotal,
  MConfigCopyTimeRule,
  MConfigAutoComplete,
  MConfigPgUpChangeDisk,

  MViewConfigTitle,
  MViewConfigExternal,
  MViewConfigExternalF3,
  MViewConfigExternalAltF3,
  MViewConfigExternalCommand,
  MViewConfigInternal,
  MViewConfigSavePos,
  MViewConfigSaveShortPos,
  MViewAutoDetectTable,
  MViewConfigTabSize,
  MViewConfigScrollbar,
  MViewConfigArrows,

  MEditConfigTitle,
  MEditConfigExternal,
  MEditConfigEditorF4,
  MEditConfigEditorAltF4,
  MEditConfigEditorCommand,
  MEditConfigInternal,
  MEditConfigTabsToSpaces,
  MEditConfigPersistentBlocks,
  MEditConfigDelRemovesBlocks,
  MEditConfigAutoIndent,
  MEditConfigSavePos,
  MEditConfigSaveShortPos,
  MEditAutoDetectTable,
  MEditCursorBeyondEnd,
  MEditLockROFileModification,
  MEditWarningBeforeOpenROFile,
  MEditConfigTabSize,

  MDistributionTableWasNotFound,
  MAutoDetectWillNotWork,

  MSaveSetupTitle,
  MSaveSetupAsk1,
  MSaveSetupAsk2,
  MSaveSetup,

  MCopyDlgTitle,
  MMoveDlgTitle,
  MLinkDlgTitle,
  MCopySecurity,
  MCopyOnlyNewerFiles,
  MCopyMultiActions,
  MCopySymLink,
  MCopyDlgCopy,
  MCopyDlgTree,
  MCopyDlgCancel,
  MCopyDlgRename,
  MCopyDlgLink,
  MCopyDlgTotal,
  MCopyScanning,

  MCopyFile,
  MMoveFile,
  MLinkFile,
  MCopyFiles,
  MMoveFiles,
  MLinkFiles,
  MCMLTargetTO,
  MCMLItems0,
  MCMLItemsA,
  MCMLItemsS,

  MCopyIncorrectTargetList,

  MCopyNotSupportLink1,
  MCopyNotSupportLink2,

  MCopyCopyingTitle,
  MCopyMovingTitle,

  MCopyCannotFind,

  MCannotCopyFolderToItself1,
  MCannotCopyFolderToItself2,

  MCopyCannotCreateFolder,
  MCopyCannotRenameFolder,
  MCopyIgnore,
  MCopyRetry,
  MCopySkip,
  MCopySkipAll,
  MCopyCancel,

  MCopyCannotCreateJunction,
  MCopyFolderNotEmpty,

  MCopyCannotCreateVolMount,
  MCopyRetrVolFailed,
  MCopyMountVolFailed,
  MCopyMountVolFailed2,
  MCopyCannotSupportVolMount,
  MCopyMountName,

  MCannotCopyFileToItself1,
  MCannotCopyFileToItself2,

  MCopyStream1,
  MCopyStream2,
  MCopyStream3,
  MCopyStream4,

  MCopyFileExist,
  MCopySource,
  MCopyDest,
  MCopyOverwrite,
  MCopyOverwriteAll,
  MCopySkipOvr,
  MCopySkipAllOvr,
  MCopyAppend,
  MCopyResume,
  MCopyCancelOvr,

  MCopyFileRO,
  MCopyAskDelete,
  MCopyDeleteRO,
  MCopyDeleteAllRO,
  MCopySkipRO,
  MCopySkipAllRO,
  MCopyCancelRO,

  MCannotCopy,
  MCannotMove,
  MCannotLink,
  MCannotCopyTo,

  MCopyReadError,
  MCopyWriteError,

  MCopyProcessed,
  MCopyMoving,
  MCopyCopying,
  MCopyTo,

  MCopyErrorDiskFull,

  MDeleteTitle,
  MAskDeleteFolder,
  MAskDeleteFile,
  MAskDelete,
  MAskDeleteRecycleFolder,
  MAskDeleteRecycleFile,
  MAskDeleteRecycle,
  MAskWipeFolder,
  MAskWipeFile,
  MAskWipe,
  MAskDeleteLink,
  MAskDeleteItems,
  MAskDeleteItems0,
  MAskDeleteItemsA,
  MAskDeleteItemsS,

  MDeleteFolderTitle,
  MDeleteFilesTitle,
  MDeleteFolderConfirm,
  MDeleteFileDelete,
  MDeleteFileAll,
  MDeleteFileSkip,
  MDeleteFileSkipAll,
  MDeleteFileCancel,

  MDeleteLinkDelete,
  MDeleteLinkUnlink,

  MDeletingTitle,

  MDeleting,

  MDeleteRO,
  MAskDeleteRO,

  MCannotDeleteFile,
  MCannotDeleteFolder,
  MDeleteRetry,
  MDeleteSkip,
  MDeleteCancel,

  MCannotGetSecurity,
  MCannotSetSecurity,

  MEditTitle,
  MAskReload,
  MCurrent,
  MReload,
  MNewOpen,
  MEditCannotOpen,
  MEditReading,
  MEditAskSave,
  MEditAskSaveExt,

  MEditSave,
  MEditNotSave,
  MEditContinue,
  MEditBtnSaveAs,

  MEditRO,
  MEditExists,
  MEditOvr,
  MEditSaving,
  MEditStatusLine,
  MEditStatusCol,

  MEditRSH,
  MEditFileLong,
  MEditFileLong2,
  MEditROOpen,

  MEditCanNotEditDirectory,

  MEditSearchTitle,
  MEditSearchFor,
  MEditSearchCase,
  MEditSearchWholeWords,
  MEditSearchReverse,
  MEditSearchSearch,
  MEditSearchCancel,

  MEditReplaceTitle,
  MEditReplaceWith,
  MEditReplaceReplace,

  MEditSearchingFor,
  MEditNotFound,

  MEditAskReplace,
  MEditAskReplaceWith,
  MEditReplace,
  MEditReplaceAll,
  MEditSkip,
  MEditCancel,

  MEditGoToLine,

  MFolderShortcutsTitle,
  MFolderShortcutBottom,
  MShortcutNone,
  MShortcutPlugin,
  MEnterShortcut,
  MNeedNearPath,
  MSaveThisShortcut,

  MEditF1,
  MEditF2,
  MEditF3,
  MEditF4,
  MEditF5,
  MEditF6,
  MEditF7,
  MEditF8,
  MEditF8DOS,
  MEditF9,
  MEditF10,
  MEditF11,
  MEditF12,

  MEditShiftF1,
  MEditShiftF2,
  MEditShiftF3,
  MEditShiftF4,
  MEditShiftF5,
  MEditShiftF6,
  MEditShiftF7,
  MEditShiftF8,
  MEditShiftF9,
  MEditShiftF10,
  MEditShiftF11,
  MEditShiftF12,

  MEditAltF1,
  MEditAltF2,
  MEditAltF3,
  MEditAltF4,
  MEditAltF5,
  MEditAltF6,
  MEditAltF7,
  MEditAltF8,
  MEditAltF9,
  MEditAltF10,
  MEditAltF11,
  MEditAltF12,

  MEditCtrlF1,
  MEditCtrlF2,
  MEditCtrlF3,
  MEditCtrlF4,
  MEditCtrlF5,
  MEditCtrlF6,
  MEditCtrlF7,
  MEditCtrlF8,
  MEditCtrlF9,
  MEditCtrlF10,
  MEditCtrlF11,
  MEditCtrlF12,

  MEditAltShiftF1,
  MEditAltShiftF2,
  MEditAltShiftF3,
  MEditAltShiftF4,
  MEditAltShiftF5,
  MEditAltShiftF6,
  MEditAltShiftF7,
  MEditAltShiftF8,
  MEditAltShiftF9,
  MEditAltShiftF10,
  MEditAltShiftF11,
  MEditAltShiftF12,

  MEditCtrlShiftF1,
  MEditCtrlShiftF2,
  MEditCtrlShiftF3,
  MEditCtrlShiftF4,
  MEditCtrlShiftF5,
  MEditCtrlShiftF6,
  MEditCtrlShiftF7,
  MEditCtrlShiftF8,
  MEditCtrlShiftF9,
  MEditCtrlShiftF10,
  MEditCtrlShiftF11,
  MEditCtrlShiftF12,

  MEditCtrlAltF1,
  MEditCtrlAltF2,
  MEditCtrlAltF3,
  MEditCtrlAltF4,
  MEditCtrlAltF5,
  MEditCtrlAltF6,
  MEditCtrlAltF7,
  MEditCtrlAltF8,
  MEditCtrlAltF9,
  MEditCtrlAltF10,
  MEditCtrlAltF11,
  MEditCtrlAltF12,

  MEditSaveAs,
  MEditSaveAsFormatTitle,
  MEditSaveOriginal,
  MEditSaveDOS,
  MEditSaveUnix,
  MEditSaveMac,
  MEditCannotSave,
  MEditSavedChangedNonFile,
  MEditSavedChangedNonFile2,
  MEditNewPath1,
  MEditNewPath2,
  MEditNewPath3,

  MColumnName,
  MColumnSize,
  MColumnPacked,
  MColumnDate,
  MColumnTime,
  MColumnModified,
  MColumnCreated,
  MColumnAccessed,
  MColumnAttr,
  MColumnDescription,
  MColumnOwner,
  MColumnMumLinks,

  MListUp,
  MListFolder,
  MListSymLink,
  MListKb,
  MListMb,
  MListGb,
  MListTb,
  MListFileSize,
  MListFilesSize,
  MListFreeSize,

  MDirInfoViewTitle,
  MFileToEdit,

  MUnselectTitle,
  MSelectTitle,

  MCompareTitle,
  MCompareFilePanelsRequired1,
  MCompareFilePanelsRequired2,
  MCompareSameFolders1,
  MCompareSameFolders2,

  MSelectAssocTitle,

  MAssocTitle,
  MAssocBottom,
  MAskDelAssoc,
  MAssocNeedMask,

  MFileAssocTitle,
  MFileAssocMasks,
  MFileAssocDescr,
  MFileAssocExec,
  MFileAssocAltExec,
  MFileAssocView,
  MFileAssocAltView,
  MFileAssocEdit,
  MFileAssocAltEdit,

  MViewF1,
  MViewF2,
  MViewF2Unwrap,
  MViewF3,
  MViewF4,
  MViewF4Text,
  MViewF5,
  MViewF6,
  MViewF7,
  MViewF8,
  MViewF8DOS,
  MViewF9,
  MViewF10,
  MViewF11,
  MViewF12,

  MViewShiftF1,
  MViewShiftF2,
  MViewShiftF3,
  MViewShiftF4,
  MViewShiftF5,
  MViewShiftF6,
  MViewShiftF7,
  MViewShiftF8,
  MViewShiftF9,
  MViewShiftF10,
  MViewShiftF11,
  MViewShiftF12,

  MViewAltF1,
  MViewAltF2,
  MViewAltF3,
  MViewAltF4,
  MViewAltF5,
  MViewAltF6,
  MViewAltF7,
  MViewAltF8,
  MViewAltF9,
  MViewAltF10,
  MViewAltF11,
  MViewAltF12,

  MViewCtrlF1,
  MViewCtrlF2,
  MViewCtrlF3,
  MViewCtrlF4,
  MViewCtrlF5,
  MViewCtrlF6,
  MViewCtrlF7,
  MViewCtrlF8,
  MViewCtrlF9,
  MViewCtrlF10,
  MViewCtrlF11,
  MViewCtrlF12,

  MViewAltShiftF1,
  MViewAltShiftF2,
  MViewAltShiftF3,
  MViewAltShiftF4,
  MViewAltShiftF5,
  MViewAltShiftF6,
  MViewAltShiftF7,
  MViewAltShiftF8,
  MViewAltShiftF9,
  MViewAltShiftF10,
  MViewAltShiftF11,
  MViewAltShiftF12,

  MViewCtrlShiftF1,
  MViewCtrlShiftF2,
  MViewCtrlShiftF3,
  MViewCtrlShiftF4,
  MViewCtrlShiftF5,
  MViewCtrlShiftF6,
  MViewCtrlShiftF7,
  MViewCtrlShiftF8,
  MViewCtrlShiftF9,
  MViewCtrlShiftF10,
  MViewCtrlShiftF11,
  MViewCtrlShiftF12,

  MViewCtrlAltF1,
  MViewCtrlAltF2,
  MViewCtrlAltF3,
  MViewCtrlAltF4,
  MViewCtrlAltF5,
  MViewCtrlAltF6,
  MViewCtrlAltF7,
  MViewCtrlAltF8,
  MViewCtrlAltF9,
  MViewCtrlAltF10,
  MViewCtrlAltF11,
  MViewCtrlAltF12,

  MInViewer,
  MInEditor,

  MFilterTitle,
  MFilterBottom,
  MNoCustomFilters,
  MPanelFileType,
  MCanEditCustomFilterOnly,
  MAskDeleteFilter,
  MCanDeleteCustomFilterOnly,

  MEnterFilterTitle,
  MFilterMasks,

  MFindFileTitle,
  MFindFileResultTitle,
  MFindFileMasks,
  MFindFileText,
  MFindFileCodePage,
  MFindFileCase,
  MFindFileWholeWords,
  MFindFileAllTables,
  MFindArchives,
  MFindFolders,
  MSearchAllDisks,
  MSearchAllButNetwork,
  MSearchFromRoot,
  MSearchFromRootOfDrive,
  MSearchFromCurrent,
  MSearchInCurrent,
  MSearchInSelected,
  MFindFileFind,
  MFindFileDrive,
  MFindFileTable,
  MFindSearchingIn,
  MFindNewSearch,
  MFindGoTo,
  MFindView,
  MFindPanel,
  MFindStop,

  MFindDone,
  MFindCancel,

  MFindFound,

  MFindFileFolder,

  MFoldTreeSearch,

  MGetTableTitle,
  MGetTableNormalText,

  MHighlightTitle,
  MHighlightBottom,
  MHighlightAskDel,
  MHighlightWarning,
  MHighlightAskRestore,

  MHighlightEditTitle,
  MHighlightMasks,
  MHighlightIncludeAttr,
  MHighlightRO,
  MHighlightHidden,
  MHighlightSystem,
  MHighlightArchive,
  MHighlightCompressed,
  MHighlightEncrypted,
  MHighlightFolder,
  MHighlightJunction,
  MHighlightExcludeAttr,
  MHighlightColors,
  MHighlightNormal,
  MHighlightSelected,
  MHighlightCursor,
  MHighlightSelectedCursor,
  MHighlightMarkChar,

  MInfoTitle,
  MInfoCompName,
  MInfoUserName,
  MInfoRemovable,
  MInfoFixed,
  MInfoNetwork,
  MInfoCDROM,
  MInfoRAM,
  MInfoSUBST,
  MInfoDisk,
  MInfoDiskTotal,
  MInfoDiskFree,
  MInfoDiskLabel,
  MInfoDiskNumber,
  MInfoMemory,
  MInfoMemoryLoad,
  MInfoMemoryTotal,
  MInfoMemoryFree,
  MInfoVirtualTotal,
  MInfoVirtualFree,
  MInfoDizAbsent,

  MErrorInvalidFunction,
  MErrorBadCommand,
  MErrorFileNotFound,
  MErrorPathNotFound,
  MErrorTooManyOpenFiles,
  MErrorAccessDenied,
  MErrorNotEnoughMemory,
  MErrorDiskRO,
  MErrorDeviceNotReady,
  MErrorCannotAccessDisk,
  MErrorSectorNotFound,
  MErrorOutOfPaper,
  MErrorWrite,
  MErrorRead,
  MErrorDeviceGeneral,
  MErrorFileSharing,
  MErrorNetworkPathNotFound,
  MErrorNetworkBusy,
  MErrorNetworkAccessDenied,
  MErrorNetworkWrite,
  MErrorDiskLocked,
  MErrorFileExists,
  MErrorInvalidName,
  MErrorInsufficientDiskSpace,
  MErrorFolderNotEmpty,
  MErrorIncorrectUserName,
  MErrorIncorrectPassword,
  MErrorLoginFailure,
  MErrorConnectionAborted,
  MErrorCancelled,
  MErrorNetAbsent,
  MErrorNetDeviceInUse,
  MErrorNetOpenFiles,
  MErrorAlreadyAssigned,
  MErrorAlreadyRemebered,
  MErrorNotLoggedOn,
  MErrorInvalidPassword,
  MErrorNoRecoveryPolicy,
  MErrorEncryptionFailed,
  MErrorDecryptionFailed,
  MErrorFileNotEncrypted,

  MErrorFullPathNameLong,

  MCannotExecute,
  MScanningFolder,

  MMakeFolderTitle,
  MCreateFolder,
  MMultiMakeDir,
  MIncorrectDirList,
  MCannotCreateFolder,

  MMenuBriefView,
  MMenuMediumView,
  MMenuFullView,
  MMenuWideView,
  MMenuDetailedView,
  MMenuDizView,
  MMenuLongDizView,
  MMenuOwnersView,
  MMenuLinksView,
  MMenuAlternativeView,

  MMenuInfoPanel,
  MMenuTreePanel,
  MMenuQuickView,
  MMenuSortModes,
  MMenuLongNames,
  MMenuTogglePanel,
  MMenuReread,
  MMenuChangeDrive,

  MMenuView,
  MMenuEdit,
  MMenuCopy,
  MMenuMove,
  MMenuCreateFolder,
  MMenuDelete,
  MMenuAdd,
  MMenuExtract,
  MMenuArchiveCommands,
  MMenuAttributes,
  MMenuApplyCommand,
  MMenuDescribe,
  MMenuSelectGroup,
  MMenuUnselectGroup,
  MMenuInvertSelection,
  MMenuRestoreSelection,

  MMenuFindFile,
  MMenuHistory,
  MMenuVideoMode,
  MMenuFindFolder,
  MMenuViewHistory,
  MMenuFoldersHistory,
  MMenuSwapPanels,
  MMenuTogglePanels,
  MMenuCompareFolders,
  MMenuUserMenu,
  MMenuFileAssociations,
  MMenuFolderShortcuts,
  MMenuEditSortGroups,
  MMenuFilter,
  MMenuPluginCommands,
  MMenuWindowsList,
  MMenuProcessList,

  MMenuSystemSettings,
  MMenuPanelSettings,
  MMenuInterface,
  MMenuLanguages,
  MMenuPluginsConfig,
  MMenuConfirmation,
  MMenuFilePanelModes,
  MMenuFileDescriptions,
  MMenuFolderInfoFiles,
  MMenuViewer,
  MMenuEditor,
  MMenuColors,
  MMenuFilesHighlighting,
  MMenuSaveSetup,

  MMenuTogglePanelRight,
  MMenuChangeDriveRight,

  MMenuLeftTitle,
  MMenuFilesTitle,
  MMenuCommandsTitle,
  MMenuOptionsTitle,
  MMenuRightTitle,

  MMenuSortTitle,
  MMenuSortByName,
  MMenuSortByExt,
  MMenuSortByModification,
  MMenuSortBySize,
  MMenuUnsorted,
  MMenuSortByCreation,
  MMenuSortByAccess,
  MMenuSortByDiz,
  MMenuSortByOwner,
  MMenuSortByCompressedSize,
  MMenuSortByNumLinks,
  MMenuSortUseGroups,
  MMenuSortSelectedFirst,

  MChangeDriveTitle,
  MChangeDriveRemovable,
  MChangeDriveFixed,
  MChangeDriveNetwork,
  MChangeDriveCDROM,
  MChangeDriveRAM,
  MChangeDriveSUBST,
  MChangeDriveLabelAbsent,
  MChangeDriveMb,
  MChangeDriveCannotReadDisk,
  MChangeDriveCannotDisconnect,
  MChangeDriveCannotDelSubst,
  MChangeDriveOpenFiles,

  MChangeDriveDisconnectTitle,
  MChangeDriveDisconnectQuestion,
  MChangeDriveDisconnectMapped,
  MChangeDriveDisconnectReconnect,

  MChangeDriveAskDisconnect,
  MChangeVolumeInUse,
  MChangeVolumeInUse2,
  MChangeWaitingLoadDisk,
  MChangeCouldNotUnlockMedia,
  MChangeCouldNotEjectMedia,
  MAdditionalHotKey,

  MSearchFileTitle,
  MCannotCreateListFile,
  MCannotCreateListTemp,
  MCannotCreateListWrite,

  MDragFiles,
  MDragMove,
  MDragCopy,

  MProcessListTitle,
  MKillProcessTitle,
  MAskKillProcess,
  MKillProcessWarning,
  MKillProcessKill,
  MCannotKillProcess,

  MQuickViewTitle,
  MQuickViewFolder,
  MQuickViewJunction,
  MQuickViewVolMount,
  MQuickViewContains,
  MQuickViewFolders,
  MQuickViewFiles,
  MQuickViewBytes,
  MQuickViewCompressed,
  MQuickViewCluster,
  MQuickViewRealSize,
  MQuickViewSlack,

  MSetAttrTitle,
  MSetAttrFor,
  MSetAttrSelectedObjects,
  MSetAttrJunction,
  MSetAttrVolMount,
  MSetAttrUnknownJunction,
  MSetAttrRO,
  MSetAttrArchive,
  MSetAttrHidden,
  MSetAttrSystem,
  MSetAttrCompressed,
  MSetAttrEncrypted,
  MSetAttrSubfolders,
  MSetAttrFileTime,
  MSetAttrModification,
  MSetAttrCreation,
  MSetAttrLastAccess,
  MSetAttrOriginal,
  MSetAttrCurrent,
  MSetAttrBlank,
  MSetAttrSet,

  MSetAttrTimeTitle1,
  MSetAttrTimeTitle2,
  MSetAttrTimeTitle3,

  MSetAttrSetting,
  MSetAttrCannotFor,
  MSetAttrCompressedCannotFor,
  MSetAttrEncryptedCannotFor,
  MSetAttrTimeCannotFor,

  MSetColorPanel,
  MSetColorDialog,
  MSetColorWarning,
  MSetColorMenu,
  MSetColorHMenu,
  MSetColorKeyBar,
  MSetColorCommandLine,
  MSetColorClock,
  MSetColorViewer,
  MSetColorEditor,
  MSetColorHelp,
  MSetDefaultColors,
  MSetBW,

  MSetColorPanelNormal,
  MSetColorPanelSelected,
  MSetColorPanelHighlightedInfo,
  MSetColorPanelDragging,
  MSetColorPanelBox,
  MSetColorPanelNormalCursor,
  MSetColorPanelSelectedCursor,
  MSetColorPanelNormalTitle,
  MSetColorPanelSelectedTitle,
  MSetColorPanelColumnTitle,
  MSetColorPanelTotalInfo,
  MSetColorPanelSelectedInfo,
  MSetColorPanelScrollbar,
  MSetColorPanelScreensNumber,

  MSetColorDialogNormal,
  MSetColorDialogHighlighted,
  MSetColorDialogDisabled,
  MSetColorDialogBox,
  MSetColorDialogBoxTitle,
  MSetColorDialogHighlightedBoxTitle,
  MSetColorDialogTextInput,
  MSetColorDialogUnchangedTextInput,
  MSetColorDialogSelectedTextInput,
  MSetColorDialogEditDisabled,
  MSetColorDialogButtons,
  MSetColorDialogSelectedButtons,
  MSetColorDialogHighlightedButtons,
  MSetColorDialogSelectedHighlightedButtons,
  MSetColorDialogListText,
  MSetColorDialogSelectedListText,
  MSetColorDialogMenuHighLight,
  MSetColorDialogMenuSelectedHighLight,
  MSetColorDialogListDisabled,
  MSetColorDialogMenuScrollBar,

  MSetColorWarningNormal,
  MSetColorWarningHighlighted,
  MSetColorWarningDisabled,
  MSetColorWarningBox,
  MSetColorWarningBoxTitle,
  MSetColorWarningHighlightedBoxTitle,
  MSetColorWarningTextInput,
  MSetColorWarningEditDisabled,
  MSetColorWarningButtons,
  MSetColorWarningSelectedButtons,
  MSetColorWarningHighlightedButtons,
  MSetColorWarningSelectedHighlightedButtons,
  MSetColorWarningListDisabled,

  MSetColorMenuNormal,
  MSetColorMenuSelected,
  MSetColorMenuHighlighted,
  MSetColorMenuSelectedHighlighted,
  MSetColorMenuDisabled,
  MSetColorMenuBox,
  MSetColorMenuTitle,
  MSetColorMenuScrollBar,

  MSetColorHMenuNormal,
  MSetColorHMenuSelected,
  MSetColorHMenuHighlighted,
  MSetColorHMenuSelectedHighlighted,

  MSetColorKeyBarNumbers,
  MSetColorKeyBarNames,
  MSetColorKeyBarBackground,

  MSetColorCommandLineNormal,
  MSetColorCommandLineSelected,
  MSetColorCommandLinePrefix,

  MSetColorClockNormal,
  MSetColorClockNormalEditor,
  MSetColorClockNormalViewer,

  MSetColorViewerNormal,
  MSetColorViewerSelected,
  MSetColorViewerStatus,
  MSetColorViewerArrows,
  MSetColorViewerScrollbar,

  MSetColorEditorNormal,
  MSetColorEditorSelected,
  MSetColorEditorStatus,

  MSetColorHelpNormal,
  MSetColorHelpHighlighted,
  MSetColorHelpReference,
  MSetColorHelpSelectedReference,
  MSetColorHelpBox,
  MSetColorHelpBoxTitle,
  MSetColorHelpScrollbar,

  MSetColorGroupsTitle,
  MSetColorItemsTitle,

  MSetColorTitle,
  MSetColorForeground,
  MSetColorBackground,
  MSetColorSample,
  MSetColorSet,
  MSetColorCancel,

  MSetConfirmTitle,
  MSetConfirmCopy,
  MSetConfirmMove,
  MSetConfirmDrag,
  MSetConfirmDelete,
  MSetConfirmDeleteFolders,
  MSetConfirmEsc,
  MSetConfirmRemoveConnection,
  MSetConfirmAllowReedit,
  MSetConfirmHistoryClear,
  MSetConfirmExit,

  MFindFolderTitle,
  MTreeTitle,
  MCannotSaveTree,
  MReadingTree,

  MUserMenuTitle,
  MChooseMenuType,
  MChooseMenuMain,
  MChooseMenuLocal,
  MMainMenuTitle,
  MMainMenuFAR,
  MMainMenuREG,
  MLocalMenuTitle,
  MAskDeleteMenuItem,
  MAskDeleteSubMenuItem,
  MUserMenuInvalidInput,

  MEditMenuTitle,
  MEditMenuHotKey,
  MEditMenuLabel,
  MEditMenuCommands,

  MAskInsertMenuOrCommand,
  MMenuInsertCommand,
  MMenuInsertMenu,

  MEditSubmenuTitle,
  MEditSubmenuHotKey,
  MEditSubmenuLabel,

  MViewerTitle,
  MViewerCannotOpenFile,
  MViewerStatusCol,

  MViewSearchTitle,
  MViewSearchFor,
  MViewSearchForText,
  MViewSearchForHex,
  MViewSearchCase,
  MViewSearchWholeWords,
  MViewSearchReverse,
  MViewSearchSearch,
  MViewSearchCancel,

  MViewSearchingFor,
  MViewSearchCannotFind,

  MPrintTitle,
  MPrintTo,
  MPrintFilesTo,
  MPreparingForPrinting,
  MJobs,
  MCannotOpenPrinter,
  MCannotPrint,

  MSortGroupsTitle,
  MSortGroupsBottom,

  MSortGroupsAskDel,
  MSortGroupsEnter,

  MDescribeFiles,
  MEnterDescription,

  MReadingDiz,
  MCannotUpdateDiz,
  MCannotUpdateRODiz,

  MCfgDizTitle,
  MCfgDizListNames,
  MCfgDizSetHidden,
  MCfgDizROUpdate,
  MCfgDizStartPos,
  MCfgDizNotUpdate,
  MCfgDizUpdateIfDisplayed,
  MCfgDizAlwaysUpdate,

  MReadingFiles,

  MUserBreakTitle,
  MOperationNotCompleted,

  MEditPanelModes,

  MEditPanelModesBrief,
  MEditPanelModesMedium,
  MEditPanelModesFull,
  MEditPanelModesWide,
  MEditPanelModesDetailed,
  MEditPanelModesDiz,
  MEditPanelModesLongDiz,
  MEditPanelModesOwners,
  MEditPanelModesLinks,
  MEditPanelModesAlternative,

  MEditPanelModeTypes,
  MEditPanelModeWidths,
  MEditPanelModeStatusTypes,
  MEditPanelModeStatusWidths,
  MEditPanelModeFullscreen,
  MEditPanelModeAlignExtensions,
  MEditPanelModeFoldersUpperCase,
  MEditPanelModeFilesLowerCase,
  MEditPanelModeUpperToLowerCase,
  MEditPanelModeCaseSensitiveSort,
  MEditPanelReadHelp,

  MSetFolderInfoTitle,
  MSetFolderInfoNames,

  MScreensTitle,
  MScreensPanels,
  MScreensView,
  MScreensEdit,

  MAskApplyCommandTitle,
  MAskApplyCommand,

  MPluginConfigTitle,
  MPluginCommandsMenuTitle,

  MPreparingList,

  MLangTitle,
  MHelpLangTitle,

  MDefineMacroTitle,
  MDefineMacro,
  MMacroReDefinedKey,
  MMacroDeleteAssign,
  MMacroDeleteKey,
  MMacroReDefinedKey2,
  MMacroDeleteKey2,
  MMacroDisDisabledKey,
  MMacroDisOverwrite,
  MMacroDisAnotherKey,

  MMacroSettingsTitle,
  MMacroSettingsEnableOutput,
  MMacroSettingsRunAfterStart,
  MMacroSettingsCommandLine,
  MMacroSettingsPluginPanel,
  MMacroSettingsFolders,
  MMacroSettingsSelectionPresent,

  MCannotSaveFile,
  MTextSavedToTemp,

  MMonthJan,
  MMonthFeb,
  MMonthMar,
  MMonthApr,
  MMonthMay,
  MMonthJun,
  MMonthJul,
  MMonthAug,
  MMonthSep,
  MMonthOct,
  MMonthNov,
  MMonthDec,

  MPluginHotKeyTitle,
  MPluginHotKey,
  MPluginHotKeyBottom,

  MRightCtrl,

  MViewerGoTo,
  MGoToPercent,
  MGoToHex,
  MGoToDecimal,

  MExceptTitleLoad,
  MExceptTitle,
  MExcTrappedException,
  MExcCheckOnLousys,
  MExcStructWrongFilled,
  MExcStructField,
  MExcInvalidFuncResult,
  MExcAddress,
  MExcUnloadYes,
  MExcRAccess,
  MExcWAccess,
  MExcOutOfBounds,
  MExcDivideByZero,
  MExcStackOverflow,
  MExcBreakPoint,
  MExcFloatDivideByZero,
  MExcFloatOverflow,
  MExcFloatStackOverflow,
  MExcFloatUnderflow,
  MExcUnknown,
  MExcDebugger,

  MNetUserName,
  MNetUserPassword,

  MReadFolderError,

  MPlgBadVers,
  MPlgRequired,
  MPlgRequired2,

  MBuffSizeTooSmall_1,
  MBuffSizeTooSmall_2,

  MCheckBox2State,
  MEditInputSize,

  MHelpTitle,
  MHelpActivatorURL,
  MHelpActivatorFormat,
  MHelpActivatorQ,
  MCannotOpenHelp,
  MHelpTopicNotFound,

  MPluginsHelpTitle,
  MDocumentsHelpTitle,

  MHelpSearchTitle,
  MHelpSearchingFor,
  MHelpSearchCannotFind,

  MHelpF1,
  MHelpF2,
  MHelpF3,
  MHelpF4,
  MHelpF5,
  MHelpF6,
  MHelpF7,
  MHelpF8,
  MHelpF9,
  MHelpF10,
  MHelpF11,
  MHelpF12,

  MHelpShiftF1,
  MHelpShiftF2,
  MHelpShiftF3,
  MHelpShiftF4,
  MHelpShiftF5,
  MHelpShiftF6,
  MHelpShiftF7,
  MHelpShiftF8,
  MHelpShiftF9,
  MHelpShiftF10,
  MHelpShiftF11,
  MHelpShiftF12,

  MHelpAltF1,
  MHelpAltF2,
  MHelpAltF3,
  MHelpAltF4,
  MHelpAltF5,
  MHelpAltF6,
  MHelpAltF7,
  MHelpAltF8,
  MHelpAltF9,
  MHelpAltF10,
  MHelpAltF11,
  MHelpAltF12,

  MHelpCtrlF1,
  MHelpCtrlF2,
  MHelpCtrlF3,
  MHelpCtrlF4,
  MHelpCtrlF5,
  MHelpCtrlF6,
  MHelpCtrlF7,
  MHelpCtrlF8,
  MHelpCtrlF9,
  MHelpCtrlF10,
  MHelpCtrlF11,
  MHelpCtrlF12,

  MInfoF1,
  MInfoF2,
  MInfoF3,
  MInfoF4,
  MInfoF5,
  MInfoF6,
  MInfoF7,
  MInfoF8,
  MInfoF9,
  MInfoF10,
  MInfoF11,
  MInfoF12,

  MInfoShiftF1,
  MInfoShiftF2,
  MInfoShiftF3,
  MInfoShiftF4,
  MInfoShiftF5,
  MInfoShiftF6,
  MInfoShiftF7,
  MInfoShiftF8,
  MInfoShiftF9,
  MInfoShiftF10,
  MInfoShiftF11,
  MInfoShiftF12,

  MInfoAltF1,
  MInfoAltF2,
  MInfoAltF3,
  MInfoAltF4,
  MInfoAltF5,
  MInfoAltF6,
  MInfoAltF7,
  MInfoAltF8,
  MInfoAltF9,
  MInfoAltF10,
  MInfoAltF11,
  MInfoAltF12,

  MInfoCtrlF1,
  MInfoCtrlF2,
  MInfoCtrlF3,
  MInfoCtrlF4,
  MInfoCtrlF5,
  MInfoCtrlF6,
  MInfoCtrlF7,
  MInfoCtrlF8,
  MInfoCtrlF9,
  MInfoCtrlF10,
  MInfoCtrlF11,
  MInfoCtrlF12,

  MQViewF1,
  MQViewF2,
  MQViewF3,
  MQViewF4,
  MQViewF5,
  MQViewF6,
  MQViewF7,
  MQViewF8,
  MQViewF9,
  MQViewF10,
  MQViewF11,
  MQViewF12,

  MQViewShiftF1,
  MQViewShiftF2,
  MQViewShiftF3,
  MQViewShiftF4,
  MQViewShiftF5,
  MQViewShiftF6,
  MQViewShiftF7,
  MQViewShiftF8,
  MQViewShiftF9,
  MQViewShiftF10,
  MQViewShiftF11,
  MQViewShiftF12,

  MQViewAltF1,
  MQViewAltF2,
  MQViewAltF3,
  MQViewAltF4,
  MQViewAltF5,
  MQViewAltF6,
  MQViewAltF7,
  MQViewAltF8,
  MQViewAltF9,
  MQViewAltF10,
  MQViewAltF11,
  MQViewAltF12,

  MQViewCtrlF1,
  MQViewCtrlF2,
  MQViewCtrlF3,
  MQViewCtrlF4,
  MQViewCtrlF5,
  MQViewCtrlF6,
  MQViewCtrlF7,
  MQViewCtrlF8,
  MQViewCtrlF9,
  MQViewCtrlF10,
  MQViewCtrlF11,
  MQViewCtrlF12,

  MKBTreeF1,
  MKBTreeF2,
  MKBTreeF3,
  MKBTreeF4,
  MKBTreeF5,
  MKBTreeF6,
  MKBTreeF7,
  MKBTreeF8,
  MKBTreeF9,
  MKBTreeF10,
  MKBTreeF11,
  MKBTreeF12,

  MKBTreeShiftF1,
  MKBTreeShiftF2,
  MKBTreeShiftF3,
  MKBTreeShiftF4,
  MKBTreeShiftF5,
  MKBTreeShiftF6,
  MKBTreeShiftF7,
  MKBTreeShiftF8,
  MKBTreeShiftF9,
  MKBTreeShiftF10,
  MKBTreeShiftF11,
  MKBTreeShiftF12,

  MKBTreeAltF1,
  MKBTreeAltF2,
  MKBTreeAltF3,
  MKBTreeAltF4,
  MKBTreeAltF5,
  MKBTreeAltF6,
  MKBTreeAltF7,
  MKBTreeAltF8,
  MKBTreeAltF9,
  MKBTreeAltF10,
  MKBTreeAltF11,
  MKBTreeAltF12,

  MKBTreeCtrlF1,
  MKBTreeCtrlF2,
  MKBTreeCtrlF3,
  MKBTreeCtrlF4,
  MKBTreeCtrlF5,
  MKBTreeCtrlF6,
  MKBTreeCtrlF7,
  MKBTreeCtrlF8,
  MKBTreeCtrlF9,
  MKBTreeCtrlF10,
  MKBTreeCtrlF11,
  MKBTreeCtrlF12,

  MRegTitle,
  MRegUser,
  MRegCode,
  MRegFailed,
  MRegThanks,
  MRegOnly,
  MRegOnlyShort,

  MCopyTimeInfo,

  MKeyESCWasPressed,
  MDoYouWantToStopWork,
  MDoYouWantToStopWork2,

  MCheckingFileInPlugin,

  MDialogType,
  MHelpType,
  MVMenuType,

  MIncorrectMask,

  MPanelBracketsForLongName,

  MComspecNotFound,

  MListEval
};

#endif  // __FARLANG_HPP__

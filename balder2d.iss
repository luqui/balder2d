#define MyAppName "Balder2D"
#define MyAppVerName "Balder2D 0.9"
#define MyAppVersion "0.9"
#define MyAppPublisher "Bjørn Hansen"
#define MyAppExeName "balder2d.exe"
#define MyAppIconName "balder2d.ico"

[Setup]
AppName={#MyAppName}
AppVerName={#MyAppVerName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
LicenseFile=C:\balder2d installer\LEGAL.TXT
InfoAfterFile=C:\balder2d installer\README.TXT
OutputDir=C:\
OutputBaseFilename=balder2d_0_9
Compression=lzma
SolidCompression=yes

[Languages]
Name: english; MessagesFile: compiler:Default.isl

[Tasks]
Name: desktopicon; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: 

[Files]
Source: C:\balder2d installer\bin\balder2d.exe; DestDir: {app}; Flags: ignoreversion
Source: C:\balder2d installer\*; DestDir: {app}; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: {group}\{#MyAppName}; Filename: {app}\bin\{#MyAppExeName}; WorkingDir: {app}\bin\; IconFilename: {app}\bin\{#MyAppIconName}
Name: {group}\{cm:UninstallProgram,{#MyAppName}}; Filename: {uninstallexe}
Name: {userdesktop}\{#MyAppName}; Filename: {app}\bin\{#MyAppExeName}; WorkingDir: {app}\bin\; Tasks: desktopicon; IconFilename: {app}\bin\{#MyAppIconName}

[Components]
Name: Python_2_4; Description: Download & Install Python 2.4 (Required); Types: full compact; Languages: ; ExtraDiskSpaceRequired: 16700000
Name: Psyco_1_5; Description: Download & Install Psyco 1.5; Types: full; Languages: ; ExtraDiskSpaceRequired: 1000000

[_ISToolDownload]
Source: http://www.python.org/ftp/python/2.4.2/python-2.4.2.msi; DestDir: {tmp}; DestName: python-2.4.2.msi; Components: Python_2_4; Languages: 
Source: http://easynews.dl.sourceforge.net/sourceforge/psyco/psyco-1.5.win32-py2.4.exe; DestDir: {tmp}; DestName: psyco-1.5.win32-py2.4.exe; Components: Psyco_1_5; Languages: 

[Code]
function NextButtonClick(CurPage: Integer): Boolean;
begin
	Result := istool_download(CurPage);
end;

[Run]
Filename: {sys}\msiexec.exe; WorkingDir: {tmp}; Components: Python_2_4; Languages: ; Parameters: "/i ""{tmp}\python-2.4.2.msi"""
Filename: {tmp}\psyco-1.5.win32-py2.4.exe; WorkingDir: {tmp}; Components: Psyco_1_5; Languages: 

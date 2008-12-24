Balder2D Win32 Development Environment Setup Instructions

Author: Reuben Lord
Last Revised: 11/14/06
==================================================================================
Please note:
	1. The build and install process described below has only been tested in the order written from top to bottom.
	2. Path names for libraries and development tools that have been extracted from packages with different version numbers than the ones described below may very.

==================================================================================
Installing MinGW
-----------------
	1. Download the installer "MinGW-5.0.0.exe" or later from: http://www.mingw.org
	2. Double click the installer to run it.
	3. Click "Next >" at the Welcome screen.
	4. Click "I Agree" at the EULA.
	5. Select a mirror that is physically closest to you.
	6. Make sure "Download as needed and install" is selected.
	7. Click the "Next >" button.
	8. Select the "Current" MinGW package.
	9. Click "Next >".
	10. Under the list of components to install, check "MinGW base tools" and "g++ compiler".
	11. Click "Next >".
	12. Select an install path for MinGW. Default: "c:\MinGW"
	13. Click "Next >".
	14. Select a folder in the Start Menu for you MinGW shortcuts to go in. Default: "MinGW"
	15. Click "Install".
	16. Watch MinGW download and install.
	17. Click "Finish".

==================================================================================
Installing MSYS
----------------
	1. Download the installer "MSYS-1.0.11-2004.04.30-1.exe" or later from: http://www.mingw.org
	2. Double click the installer to run it.
	3. You will be asked "Do you really want to install Minimal SYStem?"  Click "Yes".
	4. Click "Next >" at the Welcome screen.
	5. Click "Yes" at the EULA.
	6. Click "Next >" at the Information screen.
	7. Select an install path. Default: "C:\msys\1.0"
	8. Under component selection, select "Installation for i386 based CPUs (Works for all x86 based CPUs >= i386)".
	9. Click "Next >".
	10. Select a folder in the Start Menu for you MinGW shortcuts to go in. Default: "MinGW"
	11. Click "Next >".
	12. Review setup settings.
	13. Click "Install".
	14. Watch it install MSYS.
	15. A command prompt with the MSYS post installer setup tool in it should appear after the installer finishes installing MSYS.
	16. Press the "y" key on you keyboard and hit "Enter" to continue with the post installer.
	17. Press "y" and hit "Enter" if you have MinGW installed.
	18. Enter the location of your MinGW installation in the following format. Default: "c:\\MinGW"
	19. It will perform some checks and then ask you to "Press any key to continue..."
	20. The post installer tool will now close and bring you back to the primary installer.
	21. Uncheck all check boxes.
	22. Click "Finish".
	23. If you want to change the colors for the MSYS interface they are defined in the file "C:\msys\1.0\msys.bat".

==================================================================================
Building & Installing SDL (Simple Directmedia Layer) Libraries
---------------------------------------------------------------
	1. Download the source code "SDL-1.2.9.zip" or later from: http://www.libsdl.org
	2. Extract all of the files to "C:\tmp\".
	3. Run the MSYS shell.
	4. Type: cd /c/tmp/SDL-1.2.9/
	5. Type: ./configure && make && make install
	6. MSYS will now start building and installing the primary SDL libraries.
	7. For the DLL file, refer to the section below named "Installing DLL Files".

==================================================================================
Installing SDL_image Libraries
-------------------------------
	1. Download the source code "SDL_image-devel-1.2.4-VC6.zip" or later from: http://www.libsdl.org/projects/SDL_image/
	2. Extract all of the files to "C:\tmp\".
	3. Go to: C:\tmp\SDL_image-1.2.4\
	4. Copy "include\SDL_image.h" to "C:\msys\1.0\local\include\SDL".
	5. Copy "lib\SDL_image.lib" to "C:\msys\1.0\local\lib".
	6. For the DLL files, refer to the section below named "Installing DLL Files".

==================================================================================
Installing SDL_mixer Libraries
-------------------------------
	1. Download the source code "SDL_mixer-devel-1.2.6-VC6.zip" or later from: http://www.libsdl.org/projects/SDL_mixer/
	2. Extract all of the files to "C:\tmp\".
	3. Go to: C:\tmp\SDL_mixer-1.2.6\
	4. Copy "include\SDL_mixer.h" to "C:\msys\1.0\local\include\SDL".
	5. Copy "lib\SDL_mixer.lib" to "C:\msys\1.0\local\lib".
	6. For the DLL files, refer to the section below named "Installing DLL Files".

==================================================================================
Building & Installing SDL_gfx Libraries
----------------------------------------
	1. Download the source code "SDL_gfx-2.0.13.zip" or later from: http://www.ferzkopp.net/joomla/content/view/19/14/
	2. Extract all of the files to "C:\tmp\".
	3. Make the modifications listed in step 5 to the following header files:
		a. C:\Tmp\SDL_gfx-2.0.13\SDL_framerate.h
		b. C:\Tmp\SDL_gfx-2.0.13\SDL_gfxPrimitives.h
		c. C:\Tmp\SDL_gfx-2.0.13\SDL_imageFilter.h
		d. C:\Tmp\SDL_gfx-2.0.13\SDL_rotozoom.h
	4. The following is the original code block in 4 of the SDL_gfx header files:
		#ifdef WIN32
		#ifdef BUILD_DLL
		#define DLLINTERFACE __declspec(dllexport)
		#else
		#define DLLINTERFACE __declspec(dllimport)
		#endif
		#else
		#define DLLINTERFACE
		#endif
	5. Make changes in all 4 header files listed above by REM'ing out lines 1, 2, 3, 4, 5, 6, 7 and 9 of the following code block:
		//#ifdef WIN32
		//#ifdef BUILD_DLL
		//#define DLLINTERFACE __declspec(dllexport)
		//#else
		//#define DLLINTERFACE __declspec(dllimport)
		//#endif
		//#else
		#define DLLINTERFACE
		//#endif
	6. Run the MSYS shell.
	7. Type: cd /c/tmp/sdl_gfx-2.0.13
	8. The "README" file in the dir
	8. Type: ./autogen.sh
	9. Type: ./configure
	10. Type: ./nodebug.sh
	11. Type: make
	12. Type: make install
	13. Type (this doesn't work): ldconfig
	14. There is no DLL file for this library.
	
==================================================================================
Building & Installing PhysFS Libraries
---------------------------------------
	1. CMakeCache.txt
    line 137: CMAKE_USE_RELATIVE_PATHS:BOOL=ON

    2. platforms/windows.c
    rem out lines 334 - 340

==================================================================================
Installing Guichan Libraries
-----------------------------
	1. Download "guichan-0.4.0-mingw32.zip" or later from: http://guichan.sourceforge.net/
	2. Extract all of the files to "C:\tmp\".
	3. Copy all files from "C:\tmp\guichan-0.4.0-mingw32\include\" to "C:\msys\1.0\local\include\".
	4. Copy all "*.a" files from "C:\tmp\guichan-0.4.0-mingw32\lib\" to "C:\msys\1.0\local\lib\".
	5. For the DLL files, refer to the section below named "Installing DLL Files".
	
==================================================================================
Installing Python Libraries
----------------------------
	1. Download "python-2.4.2.msi" or later from: http://www.python.org/
	2. Run the Python installer.
	3. On the user selection screen select "Install for all users".
	4. Click "Next >>".
	5. On the Select Destination Directory screen make sure the destination is set to: C:\Python24\
	6. Click "Next >>".
	7. On the Customize Python 2.4.2 screen select all components.
	8. Click "Next >>".
	9. Watch the installer install Python 2.4.2.
	10. On the Completed installation screen click "Finish".
	11. For the DLL files, refer to the section below named "Installing DLL Files".
	
==================================================================================
Installing Psyco Libraries
---------------------------
	1. Download "psyco-1.5.win32-py2.4.exe" or later from: http://psyco.sourceforge.net/
	2. Run the Psyco installer.
	3. On the Welcome screen click "Next >".
	4. On the Python installation verification screen make sure it located the version of Python you currently have installed.
	5. Click "Next >".
	6. On the Ready to Install screen click "Next >".
	7. Watch Psyco install.
	8. On the last screen just click "Finish".
	9. For the DLL files, refer to the section below named "Installing DLL Files".

==================================================================================
Installing TortoiseSVN-1.4.0.7501-win32-svn-1.4.0.msi SVN Tool
---------------------------------------------------------------
	1. Download "TortoiseSVN-1.4.0.7501-win32-svn-1.4.0.msi" or later from: http://tortoisesvn.net/
	2. Run the Tortoise SVN installer.
	3. On the welcome screen of the installer click "Next >".
	4. Mark Accept for the EULA.
	5. Click "Next >".
	6. Make sure all components are selected on the Custom Setup screen.
	7. Pick an installation path. Default: C:\Program Files\TortoiseSVN\
	8. Click "Next >".
	9. On the Ready to Install screen click "Install".
	10. It will now begin installing.
	11. On the completion screen un-check "Show Changelog".
	12. Click "Finish".
	13. It will now ask you to restart you system, click "Yes".
	14. Your system should restart now.
	15. Open your C:\ drive.
	16. Create a new directory named "Balder2D".
	17. Right click on the directory "C:\Balder2D\" directory that you just created to get the context menu.
	18. Click on the "SVN Checkout..." command.
	19. The Checkout dialog box will now appear.
	20. In the "URL of repository" field enter: https://svn.sourceforge.net/svnroot/balder
	21. Make sure the "Checkout directory:" says: C:\Balder2D
	22. Click "OK".
	23. It should now start checking everything out of the SVN and list them out as it does check them out.
	24. When it's done just click the "OK" button.
	25. You should have the latest copy of the Balder2D source code all downloaded into the "C:\Balder2D\" directory at this point.

==================================================================================
Installing DLL Files
---------------------
- SDL DLL
	1. Copy from "C:\msys\1.0\local\bin\SDL.dll" to "C:\Balder2D\bin\".
- Guichan DLL's
	2. Copy from "C:\Tmp\guichan-0.4.0-mingw32\bin\guichan.dll" to "C:\Balder2D\bin\".
	3. Copy from "C:\Tmp\guichan-0.4.0-mingw32\bin\guichan_sdl.dll" to "C:\Balder2D\bin\".
- SDL_image DLL's
	4. (DON'T NEED) Copy from "C:\Tmp\SDL_image-1.2.5\lib\jpeg.dll" to "C:\Balder2D\bin\".
	5. Copy from "C:\Tmp\SDL_image-1.2.5\lib\libpng12.dll" to "C:\Balder2D\bin\".
	6. Copy from "C:\Tmp\SDL_image-1.2.5\lib\SDL_image.dll" to "C:\Balder2D\bin\".
	7. Copy from "C:\Tmp\SDL_image-1.2.5\lib\zlib1.dll" to "C:\Balder2D\bin\".
- SDL_mixer version 1.2.6.0 DLL
	8. Copy from "C:\Tmp\SDL_mixer-1.2.6\lib\SDL_mixer.dll" to "C:\Balder2D\bin\".
- SDL_mixer version 1.2.7.0 DLL or later
	9. Copy from "C:\Tmp\SDL_mixer-1.2.7\lib\ogg.dll" to "C:\Balder2D\bin\".
	10. Copy from "C:\Tmp\SDL_mixer-1.2.7\lib\SDL_mixer.dll" to "C:\Balder2D\bin\".
	11. Copy from "C:\Tmp\SDL_mixer-1.2.7\lib\vorbis.dll" to "C:\Balder2D\bin\".
	12. Copy from "C:\Tmp\SDL_mixer-1.2.7\lib\vorbisfile.dll" to "C:\Balder2D\bin\".
- Python 2.4 DLL
	13. When Python 2.4 is installed it installs the DLL in the %PATH% directory.
- Psyco 1.5
	14. If it has any DLL's it installs them for you.

==================================================================================
Installing Code::Blocks IDE
----------------------------
	1. Download "codeblocks-1.0rc2.exe" or later from: http://www.codeblocks.org/
	2. Run the installer.
	3. At the Welcome screen click "Next >".
	4. On the EULA screen select the accept option.
	5. Click "Next >".
	6. On the Select Destination Location screen choose your installation path. Default: C:\Program Files\CodeBlocks
	7. Click "Next >".
	8. On the Select Components screen select "Full installation".
	9. Click "Next >".
	10. On the Select Start Menu Folder screen select a folder to store it in or leave the default.
	11. Click "Next >".
	12. On the Select Additional Tasks screen select the icons that you want it to make.
	13. Click "Next >".
	14. On the Ready to Install screen review your installation configuration.
	15. Click "Install".
	16. Watch it install Code::Blocks for you.
	17. On the Information screen click "Next >".
	18. On the Complete screen click "Finish".
	19. Code::Blocks will now load for you.
	20. The Compilers auto-detection screen will appear.
	21. Select the compiler: GNU GCC Compiler
	22. The Status for the "GNU GCC Compiler" should equal: Detected
	23. Click the "Set as default" button.
	24. Make sure at the bottom of the window you see the string: Default compiler: GNU GCC Compiler
	25. Click "Close".
	26. Code::Blocks finished loading up.
	27. If the "Tip of the Day" shows up uncheck "Show tips at startup" and click "Close" on the Tip dialog box.
	28. To open the project file click on the "File" menu.
	29. Then click the "Open" menu option.
	30. Open "C:\Balder2D\Balder2D.cbp".
	31. Now you should be able to build Balder2D with in Code::Blocks.
	32. To build Balder2D in Code::Blocks click on the "Build" menu.
	33. Then click the "Build" menu option (or press Ctrl+F9 to build).
	34. It will then build Balder2D and put the executable file in the following path: C:\Balder2D\bin\Balder2D.exe
	
==================================================================================
Creating a Code::Blocks IDE project file
-----------------------------------------
	1. Open Code::Blocks.
	2. If you have any projects open, then close them.
		a. On the main menu click on the "File" menu.
		b. Then click on the "Close project" menu option.
	3. On the main menu click on the "Project" menu.
	4. Then click on the "New project..." menu option.
	5. The "New from template" dialog box will appear.
	6. Select the "Console Application" icon.
	7. Check the "Do not create any files" check box.
	8. Set Project options: Console Application
	9. Set File(s) options: C++ source
	10. Click the "Create" button.
	11. The "Save project" dialog box will appear.
	12. Save it to: C:\Balder2D\Balder2D.cbp
	13. On the right hand side of the IDE is a tabbed window named "Management" (in the main menu it is "View>Manager" (Shift+F2)).  This will be referred to as the "Management Bar".
	14. Select the "Projects" tab on the Management Bar.
	15. Right click on the "Console application" node.
	16. Click on the "Properties" menu option.
	17. The "Project/targets options" dialog will appear.
	18. Select the "Project" tab.
	19. Set Title: Balder2D
	20. Select the "Targets" tab.
	21. Set Output filename: bin\Balder2D.exe
	22. Set Execution working dir: bin\
	23. Select the "Project" tab.
	24. Click on the "Project's build options..." button.
	25. This will make the "Project's Build options" dialog box appear.
		a. You can also reach this dialog box by right clicking on the project's node in the Management Bar.
		b. Click on the "Build options" menu option.
	26. Select the "Linker" tab.
	27. Add all of the libraries for the linker to link to the project.  I'm not sure if it requires a certain order, but if it does this order works.
		a. mingw32
		b. guichan
		c. guichan_sdl
		d. SDLmain
		e. SDL_gfx
		f. SDL
		g. SDL_image
		h. SDL_mixer
		i. C:\msys\1.0\local\lib\libboost_filesystem-mgw-1_33_1.lib
		j. python24
	28. Click on the "Add" button.
	29. Set File: [LIBRARY NAME]
	30. Click "OK".
	31. Repeat until all libraries have been added.
	32. Under the "Linker" tab there is a text window titled "Other linker options:", add this line: -mwindows
	33. Select the "Directories" tab.
	34. Under the "Directories" tab select the "Compiler" tab.
	35. Add all of the #include directories for the compiler to look in for the different library header files.  Please note when entering directories here do not put an ending "\" because it confuses the compiler.
		a. C:\MinGW\include
		b. C:\msys\1.0\local\include
		c. C:\msys\1.0\local\include\SDL
		d. C:\Python24\include
	36. Click on the "Add" button.
	37. Set Directory: [#INCLUDE PATH]
	38. Click "OK".
	39. Repeat until all #include directories have been added.
	40. Select the "Directories" tab.
	41. Under the "Directories" tab select the "Linker" tab.
	42. Add all of the library directories for the compiler to look in for the different library files.  Please note when entering directories here do not put an ending "\" because it confuses the compiler.
		a. C:\MinGW\lib
		b. C:\msys\1.0\local\lib
		c. C:\Python24\libs
	43. Click on the "Add" button.
	44. Set Directory: [LIBRARY PATH]
	45. Click "OK".
	46. Repeat until all library directories have been added.
	47. Click the "OK" button on the "Project's Build options" dialog box.
	48. Click the "OK" button on the "Project/targets options" dialog box.
	49. Right click on the project node "Balder2D" in the Management Bar.
	50. Click on the "Add files recursively..." menu option.
	51. Select: C:\Balder2D\src\
	52. Click the "OK" button.
	53. The "Multiple selection" dialog box will appear.
	54. Make sure all files are selected.  Make sure you see Selected: 40
	55. Click the "OK" button.
	56. Right click on the project node "Balder2D" in the Management Bar.
	57. Click on the "Add files recursively..." menu option.
	58. Select: C:\Balder2D\include\
	59. Click the "OK" button.
	60. The "Multiple selection" dialog box will appear.
	61. Make sure all files are selected.  Make sure you see Selected: 31
	62. Click the "OK" button.
	63. Click "File" on the main menu.
	64. Click the "Save project" menu option.
	65. This project file is ready to compile.
Welcome to libpqxx, the C++ API to the PostgreSQL database management system.

This file documents building and installation of libpqxx on Windows systems.
Please see the regular README for general instructions on building, installing,
and using the library.  If you are using Cygwin, MSYS, or another Unix-like
environment please try the Unix configuration & build procedure instead of the
one described here.  Some help for the MSYS/MinGW combination is given below,
but in general, things are a bit more complicated for Windows than they are for
other environments.

The only Windows compilers currently documented here are:

	Visual C++ (7.0 or above)
	MinGW

If you are using a different compiler, you should still be able to get it to
work.  If you do, or if you fail, please report your results so any problems can
be resolved, and so that we can add support for your compiler in the standard
libpqxx distribution.

We'll be using the command line throughout this document.


Build Environment
-----------------
Before you start, make sure your development environment is reasonably up to
date.  If you use MSYS, be sure your "grep" program is up to date and that you
have the "mktemp" program installed (these can be downloaded from
http://gnuwin32.sf.net/).  If you use Visual C++, make sure you have at least
version 7.0 or you'll run into serious compilation trouble.

The latest build has been tested against Visual Studio 8, 9.0 and 10.0, so
if you build with Visual C++ 7.0 you might have to make some adjustments to 
these instructions and scripts for the build to succeeed.

Obtaining libpq
---------------
Next, make sure you have a working installation of libpq, the C-level client
library included with PostgreSQL.

There are two ways to make libpq available on a Windows system: Install it or
build it from sources.

Installing libpq
----------------
The easiest way to get libpq on a Windows system, is to install PostgreSQL. The
PostgreSQL installer from Windows can be obtained from

	// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#ifndef __STDAFX_H
#define __STDAFX_H
#if _MSC_VER > 1000
   #pragma once
#endif
#ifdef __BORLANDC__
#define _countof(x) sizeof(x) / sizeof(x[0])
#endif

#define STRICT
#pragma warning(disable: 4100)  // unreferenced formal parameter
//#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#define _SECURE_SCL 1
// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif

//#ifndef _WIN32_WINDOWS	// Allow use of features specific to Windows 98 or later.
//#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
//#endif

#ifndef _WIN32_IE		// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif
//#define _SECURE_SCL 1
//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

//#include <Wingdi.h>
#include <windowsx.h>
#include <commctrl.h>
//#include <richedit.h>
#include <Commdlg.h>
#include <prsht.h>
#include <tchar.h>
#include <shlobj.h>
//#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include "SciLexer.h"
#include "Scintilla.h"

//#include <algorithm>
//#include <process.h>
//#define NDEBUG //directive ("no debugging") in the source code before the #include <assert.h> directive, the effect is to comment out the assert statement.
//#include <cstring>
//#include <cstdio>
#include <cassert>
#include <exception>
//#include <strstream.h>
#include <sstream>
using namespace std;

enum { CONSOLEAPP = 1, WINDOWAPP, DLLAPP,STATICLIBAPP,WXWIDGETSAPP };
enum { iSource = 1, iHeader, iResource, iOthers };
enum { CPLUSPLUS =1, CLANGUAGE};
enum { SDIAPP = 1, MDIAPP, DLGAPP};
enum { WXDLL = 1, WXSTATIC, WXMONOLITHIC};

typedef struct tagWizdata
{
    int iType; //Console =1 Windows = 2
    int iLanguage; //C++ = 1, C = 2
    BOOL bEmpty;
    int WinSubType; //SDI, MDI, DLGBASE
    int WxSubType;  // WXDLL = 1, WXSTATIC, WXMONOLITHIC};
} WIZDATA, *LPWIZDATA;


class AppData
  {
  public:
    AppData();
    void RebuildDirectories(void);
    string CPPEXT;
    string CEXT;
    string RESEXT;
    string PROJECTEXT;
    string EXEEXT;
    string HEADEREXT;
    string TEXTEXT;
    string DLLEXT;
    string LIBEXT;
    BOOL bManager;
    BOOL bMessages;
    BOOL bAutoHideMessages;
    RECT rc;
    RECT rec;
    HINSTANCE hInst;
    int cxClient;      //initialized at WM_SIZE
    int cyClient;
    int iStatusHeight; //height of status bar
    int iToolHeight;   //height of Rebar window
//initialized at WM_SIZE
    int g_tabsize;
    int splitsize;//width of splitter windows
    string szProjectFile; //path and filename of project file
    string CurTitle;  //hols the name without path of file in editor window
    string szExecutableFile;

    string szCompilerPath;// = TEXT("c:\\Borland\\bcc55\\bin\\bcc32.exe"); //Path to compilerstatic
    string szResourcePath;// = TEXT("c:\\Borland\\bcc55\\bin\\brcc32.exe");// Path to resource compiler
    string szLinkerPath;// = TEXT("c:\\Borland\\bcc55\\bin\\ilink32.exe"); // path to linker
    string psdkPath;//     = TEXT("c:\\Borland\\bcc55\\lib\\psdk");        //lib path
    string IncludePath;//  = TEXT("c:\\Borland\\bcc55\\include");           //include path
    string LibPath;//      = TEXT("c:\\Borland\\bcc55\\lib");
    string CompilerArgs; //these are the strings passed to the compiler, linker and resource compiler
    string ResourceArgs;
    string LinkerArgs;
    string g_CurrentDir; // for holding the current directory or project file directory
    string addlibraries; //place to add aditional libraries to the linker
    string szProjectName; //name of project without path
    BOOL bCancelWizard; //TRUE if user cancel the Wizard for creating a project
    BOOL bProjectModified; //if you add files to a project this global become true
    BOOL bModify;
    BOOL g_bCompilerStatus; //compiler strings have been or not initialized
    BOOL bColorsModify;
    int iErrors;
    int iWarnings;
    BOOL brunning;
    BOOL bCompiled;
    char g_szAppName[80]; //Title name for main Window
    HWND hActiveEditor; //Main Edit Window
    HWND hwndTV; //Lower TreeView Control Window (Handles the project files)
    HWND hTabWnd; //Tab Control window
    HWND hwndOpenTV;  //Upper TreeView Control handle the open files (allocated memory)
    HWND hTabDebug;//Compiler Error Messages Tab Window
    HWND hDebug; //Compiler Error Messages readonly Editor window
    HWND hwndProgressDlg;
    HWND hListView;
    HWND hComboFunctions;
    HWND hwndMain; //Main Window process the main window messages
    HWND hWndStatusbar;
    HWND hTreeSplitter;  //Horizontal Splitter that divides the TreeView Controls
    HWND hHorzSplitter; //horizontal splitter that divides tab controls
    HWND hVertSplitter; //Vertical splitter window that divides the TreeView Controls and Tab Control
    HWND hwndSelected;
    vector <string> VectorSource; //holds source files names(*.cpp or *.c) //holds source files names(*.cpp or *.c)
    vector <string> VectorHeader; // holds headers files (*.h) //
    vector <string> VectorOther;  // holds others files actually only text files (*.txt)
    vector <string> VectorResource;
    map<string,HWND > Mem_map;
    map <string, char> file_map;
    map <string, string> dir_map;
    map <string,int> proto_map;
    HTREEITEM hRoot;
    HTREEITEM hSource;
    HTREEITEM hHeader;
    HTREEITEM hResource;
    HTREEITEM hOther;//root and childs of hwndTV Control
   };
//using std::goodbit;



// C RunTime Header Files
//#include <cstdlib>
//#include <process.h>
//#include <malloc.h>
//#include <memory>
//#include <tchar.h>


// TODO: reference additional headers your program requires here

#endif

	
This build procedure has been tested against all the versions of the One click
installer up to the current version 9.1.1-1. If you select the pgInstaller, you
will have to make some adjustments to these instructions and scripts for the 
build to succeeed.

After installing PostgreSQL, you will be able to find libpq under

	C:\Program Files\PostgreSQL\{Version}\lib

Building libpq from sources
---------------------------
If you prefer to build libpq from sources, you can compile a recent version of 
PostgreSQL.  Its source tree should then contain libpq in binary form, as well
as the corresponding headers.  Look for these in src/interfaces/libpq. Visual 
C++ will generate separate Debug and Release versions in subdirectories called 
Debug and Release, respectively, at that location.  Note these locations; they
will become important in the next section.

The source code for PostgreSQL can be obtained from

	http://postgresql.org/download/

Select a recent version of postgres and download the corrresponding .tar.gz or
.tar.bz2 archive for that version.  Unpack the sources to some directory on your
computer.

Select "Visual Studio Command Prompt" from the Start menu. Alternatively, 
open a Command Prompt window and run:

    VCVARS32.BAT

to prepare the environment.

From the Command Prompt window, "cd" into the postgres source tree directory and
run:

	nmake /f win32.mak

You'll also want to build a Debug-flavour libpq, so then run:

	nmake /f win32.mak DEBUG=1

The libpq binaries will be produced in src/interfaces/libpq/Release and
src/interface/libpq/Debug, respecitively, and the Debug versions will have an
extra "D" in their names.

(Instructions for building PostgreSQL with MinGW are given in the MSYS section
below).

Preparing to Compile libpqxx
----------------------------
The rest of this text will assume that you have the libpq library in its binary
form(s), as well as its header files.  Now, to work on building libpqxx!

Before you can build libpqxx you need to provide certain common settings:

  i) Copy win32/common-sample to win32/common, and edit the latter to reflect
    the paths to your PostgreSQL includes and library files:
    - If you compiled PostgreSQL for yourself to obtain libpq, simply set PGSQLSRC
      in win32/common to point to the "src" subdirectory in your PostgreSQL source
      tree.
    - If you installed libpq using the One Click Installer, verify that PGSQLSRC 
      points to your specific installation of PostgreSQL. Then follow the 
      instructions in the win32/common file to comment several lines out and
      uncomment several others.
    - If you obtained libpq in any other way, you may be able to build libpqxx by
      making the appropriate adjustments to the 'common' file.

 ii) Create the configuration headers: include/pqxx/config-*-*.h.  The easiest 
    start for this is to copy from available samples.  In config/sample-headers,
	find the directories most accurately describing your compiler and libpq 
	version, respectively.  Take the config-*-*.h headers from those directories
	and copy them into the include/pqxx/ directory.
    On a Unix-like system these headers would be generated automatically by the
    configure script; in the Windows case they may need some editing.  How these
    files work is described in more detail below.

iii) If you are building libpqxx from a development source tree rather than from
    a release distribution, you may not have the makefiles for your compiler.
    These makefiles should be in the win32 directory and have names ending in
    ".mak": "mingw.mak," "vc-libpqxx.mak," "vc-test.mak," and so on.  If you
    don't have these files but you do have a python interpreter, you can
    generate them using the template2mak.py script in the tools directory:

      ./tools/template2mak.py win32/mingw.mak.template win32/mingw.mak
      ./tools/template2mak.py win32/vc-libpqxx.mak.template win32/vc-libpqxx.mak
      ./tools/template2mak.py win32/vc-test.mak.template win32/vc-test.mak
      ./tools/template2mak.py win32/vc-test-unit.mak.template win32/vc-test-unit.mak


Building libpqxx
----------------
The rest of the procedure depends on your compiler.

If you're using Visual C++:

1) Select Visual Studio Command Prompt from the Start menu. Alternatively, 
   open a Command Prompt window and run 
   
       VCVARS32.BAT

   to prepare the environment.
   Then CD into the main directory of the libpqxx source tree.  Among other
   things it will contain the files "configure" and "Makefile.am," and 
   subdirectories "src," "include," "win32" etc.

2) Run:

       "nmake /f win32/vc-libpqxx.mak ALL"
	   
   to compile all dynamic, static, debug and release libraries.  Once completed,
   you will find them in these libraries in the lib directory. The build script
   also copies the libpq dlls and stub libraries to the lib directory. You will
   have to link with these files when building applications that use libpqxx
   clases and methods.

3) Your version of the libpq DLL may require other DLLs in order to execute. If
   you installed libpq with the one click installer, these DLLs can be found in
   the same folder as libpq itself. The exact set of DLLs varies depending on the
   version of PostgreSQL that you installed. For the current versions of the One
   Click Installer, they are:
   - Version 8.3.16-1 - comerr32.dll, gssapi32.dll, k5sprt32.dll, krb5_32.dll
                        libiconv-2.dll and libintl-8.dll
   - Version 8.4.9-1  - comerr32.dll, gssapi32.dll, k5sprt32.dll, krb5_32.dll
                        libiconv-2.dll and libintl-8.dll
   - Version 9.0.5-1  - libiconv-2.dll and libintl-8.dll
   - Version 9.1.1-1  - libeay32.dll, libiconv-2.dll, libintl-8.dll and 
                        ssleay32.dll
						
4) Once you have identified any additional DLLs, you should copy them to another
   location (perhaps the libpqxx lib folder.
   
5) At this point, you will no longer need to keep the postgres sources or binaries
   on your computer to build applications with libpqxx.  Feel free to remove them
   from your system if you wish to do so.

If you're using MinGW (but without a Unix-like environment, see above):

1) Run "make -f win32/MinGW.mak"

2) Consider installing a Unix-like environment like MSYS to automate all this!


Running the test suite
----------------------
After building libpqxx, it is recommended that you compile and run the self-test
programs included in the package.  Unix, Cygwin, or MSYS users simply type 
"make check" to build and run the entire regression test.  For Visual C++ users, 
the following steps should accomplish the same:

 1) To build all of the self-test configurations, run:
        nmake /f win32\vc-test.mak ALL
        nmake /f win32\vc-test.mak ALL DEBUG=1
        nmake /f win32\vc-test.mak ALL DLL=1
        nmake /f win32\vc-test.mak ALL DLL=1 DEBUG=1
    This will create four new directories: TestStaticDebug, TestStaticRelease,
    TestDllDebug, and TestDllRelease.  Each of these folders will contain the
    program "runner.exe," as well as any required DLLs.
	
 2) Make sure a PostgreSQL database is running and accessible, and set up the
    environment variables PGDATABASE, PGHOST, PGPORT, and PGUSER as described
    in the libpqxx README file so the test program can connect without needing
    to pass a connection string.
	
 3) Run runner.exe from any of the test directories.  When runner.exe runs,
    it executes each of the test programs in order.  Some of the tests will
    produce lots of output, some won't produce any.  Error messages that are
    expected in the course of a normal test run are prefixed with "(Expected)"
    to show that they are not harmful.  All tests should complete successfully.


Running the unit tests
----------------------
You may also want to compile and run the unit test programs included in the
package.  The procedure for doing so for Visual Studio users is nearly 
identical to the procedure for running the self-tests, except for the name of
the .mak file, which is win32\vc-unit.mak. The output build directories are 
UnitStaticDebug, UnitStaticRelease, UnitDllDebug, and UnitDllRelease. The test
application name is again runner.exe. All of these tests should complete
successfully as well.


Setting Up a Visual Studio Project
----------------------------------
Here's how to set up a Visual C++ project file for your own program using
libpqxx (based on description by Alexandre Hanft, 2006-03-06):

 1) Create a new project in the Visual C++ development environment.
 2) Add your first C++ file and provide some initial code, e.g. by copying the
    example from the libpqxx home page.
 3) Edit Project Properties, C++: Additional Include Directories.  Add the
    location of the "include" directory found in your libpqxx source tree, e.g.
    C:\src\libpqxx\include if you keep libpqxx in C:\src\libpqxx.
 4) Edit Project Properties, Linker: Additional Library Directories.  Add the
    directory where your libpq and libpqxx libraries are, for example:
    C:\src\libpqxx\lib
 5) Edit Project Properties, Linker, Input: Additional Dependencies.  Add:
    libpqxx.lib.
    That is for your Release build.  Do the same for your Debug build, but
    replace that "libpqxx.lib" in there with "libpqxxD.lib".
 6) Your program should build successfully now.  It may not be ready to run yet,
    because some dynamically loaded libraries are needed when starting your
    program.  Copy the necessary libpq and libpqxx DLLs from the lib directory
    in your libpqxx tree, directly into your project's build directory so the
    system can find them when your development environment launches your
    program.
 7) Your program should now be able to run.  However, you may need to tell it
    how to connect to a database.  If you set the environment variables
    PGDATABASE, PGHOST, PGPORT, and PGUSER as described in the libpqxx README
    file, your program should be able to connect without passing further
    parameters.
 8) Once your program gets to the point where users should be able to configure
    it properly, change it to pass a connection string where it connects to any
    databases, and include your user's configuration settings.  A typical
    connection string setting all parameters named above might look like:
	"dbname=testbase user=testuser host=my.dbserver.somewhere.com"
 9) For any other problems, please check the project homepage as listed in the
    main libpqxx README file.


Getting Started with MinGW and MSYS
-----------------------------------
(based on contribution by Michael J. Pedersen, 2004-08-17)

MSYS is a Unix-like environment for Windows.  Once you have this installed,
building libpqxx should be a lot more like it is under Unix except that the
locations of libraries and such aren't sufficiently standardized.

Packages to download if you don't have them already:
1) MinGW (http://www.mingw.org/), plus any updates.
2) MSYS (http://www.mingw.org/), again with any updates.
3) w32api (http://www.mingw.org/).
4) PostgreSQL (http://www.postgresql.org/).
5) zlib (http://www.zlib.org/).

It is generally recommended to get the latest versions of these packages.

Compiling and installing PostgreSQL before you get to libpqxx:
1) Install MinGW (install to c:\mingw)
2) Install MSYS--but not into the MinGW directory tree!
3) Run MSYS (Start -> Programs -> MinGW -> MSYS -> msys)
4) Extract, compile, and install zlib
    # From the main source directory of zlib:
    ./configure --prefix=c:/mingw/local && make && make install
5) Extract, compile, and install postgres
    # From the main source directory of PostgreSQL:
    ./configure --prefix=c:/mingw/local --with-includes=c:/mingw/local/include --with-libs=c:/mingw/local/lib
    make && make install
6) Extract, compile, and install libpqxx
    export lDFLAGS=-lws2_32
    ./configure --prefix=c:/mingw/local --enable-static
    make && make install

If you don't have and don't want to install MSYS, you won't be able to perform
the "./configure" step.  You can build libpqxx without it, but it takes a bit
more work and doesn't build the self-test.  Use the special makefile MinGW.mak
in the win32 directory instead of the regular Makefile, and follow the procedure
described below to use the prefabricated configuration headers enclosed in the
source archive.

Building libpqxx without MSYS
-----------------------------
	# (first set up configuration headers, see below)
	export LDFLAGS=-lws2_32
	make -f win32/MinGW.mak
	# (install library, headers and pqxx-config manually on your system)

Having installed libpqxx, when compiling your own programs, use the following
steps (examples only here):

g++ -c -o myfile.o myfile.cpp `pqxx-config --cflags`
g++ -o myexec myfile.o `pqxx-config --libs` -lws2_32

Note that the linker step (g++ -o myexec) will give a spurious warning
about not knowing how to use the -R flag, but the executable will still
be generated, and work just fine.

The last argument to that linker step MUST be -lws2_32.  Without it, the link
will provide errors about undefined references to select(). If it's not the last
argument, you will get the same results.  Make sure it's there, and make sure
it's last. Trust me, you'll be happier.


Setting up the configuration headers
------------------------------------
Several headers, normally generated by the configure script, define certain
aspects of your compilation environment so that the library can work around
compiler shortcomings, functions missing in older libpq versions, etc.  (Samples
for various compiler and libpq versions can be found in the subdirectory
config/sample-headers, so if you find you need to provide these headers
yourself, look there first.)

Each of the aspects mentioned below is represented by a configuration item--a
preprocessor macro that is either #define'd to some value or not defined at all.
Most of the items are of a Boolean nature, i.e.  some feature or workaround is
enabled if the corresponding macro is defined or disabled if the macro is left
undefined.  Note that defining these macros to anything at all, even 0, means
that the corresponding option is enabled.  They are normally defined to 1, but
that value seems to have been chosen more for convention than anything else.

A few of the items require a textual value however, e.g. PGSTD needs to be
defined to the name of the standard namespace.  You'll typically want to have
this defined to "std" (leave out the quotes though!), but you may want to change
that if you use a standard library other than the default one shipped with your
compiler.

At the time of writing, there are 3 of these configuration headers that you
need to concern yourself about, all of which should be created in the
include/pqxx subdirectory:

	config-internal-compiler.h
	config-internal-libpq.h
	config-public-compiler.h


The names contain two variables:

1. Internal or public.  Configuration items that affect the compilation of
your client program are located in the config-public-*.h files (currently only
one file); ones that affect only the compilation of the library itself are kept
in the config-internal-*.h ones and are not included at all when compiling
client programs.  Public configuration items (except PGSTD) are prefixed with
PQXX_ to avoid clashes with macros defined by other code.

2. Autotools-related, compiler-related, or libpq-related.  The autotools items
are of no real interest, but the compiler and libpq ones are different.  The
compiler-related ones describe features and shortcomings of the compiler
environment, such as the availability of the <unistd.h> header or the ability
to compile certain complex C++ language constructs.  The libpq items describe
what features are available in the version of libpq you're compiling against.
At the time of writing, the latter all take the HAVE_... form; defining such a
macro indicates that the corresponding feature is available.

A complete list of configuration items and their categorizations can be found in
the configitems file.  The normal configure script reads the list of items from
include/pqxx/config.h.in and writes them to include/pqxx/config.h, then splits
them up into the five smaller files as directed by the configitems database.

Again, sample versions of these headers for various compilers and libpq versions
can be found in the config/sample-headers/ directory.  Try copying the three
headers for the compiler and libpq version most resembling your own environment
to include/pqxx/ and, if you have any problems compiling with those, editing
them by hand to enable or disable features that appear to be giving you trouble.


Shared and Static Libraries, or .DLL vs. .LIB
---------------------------------------------
There are two ways to build the libpqxx library: as a shared library (known
in the Unix world as a Shared Object or "so," but in the Windows world as a
Dynamically Loaded Library or DLL) or as a static library (whose name would
end in ".a" on Unix systems but in ".LIB" on Windows systems).  Linking your
own applications to these types of libraries is quite similar, except with a
DLL the system will also have to be able to find the library when your program
is loaded--not just when you are compiling it.

Unlike the situation in the Unix world, however, your choice on this point must
be known as you compile your application code.  With libpqxx you do this by
defining the preprocessor macro PQXX_SHARED when compiling your code for use
with a libpqxx DLL.  Leave this definition out if you intend to link to the
static version of the library.

(Internally, when compiling libpqxx, this variable will also be set according to
which kind of library is being built.  It has a slightly different meaning in
that case, but this should really only matter to you if you are helping debug
the library)

Similarly, you may choose to link to either the static or the shared version of
libpq.  The default is to use the dynamic library, which makes more sense there
because libpq's API and ABI are much simpler and more stable.  To build a
libpqxx for use with a static libpq, however, define the preprocessor symbol
PQXX_PQ_STATIC both when compiling libpqxx and while building your own
application.


Troubleshooting: config headers
-------------------------------
The biggest typical problem for Windows/VC++ users is the need to edit the
configuration headers manually, resulting usually in compile errors, e.g. for
missing header files.  Thus, if you get a compile error, the first places to
look are the config-*-*.h and config-*-*.h files in include/pqxx.

If, for instance, the error states that the header file <sys/select.h> is
missing, you will find a configuration variable called PQXX_HAVE_SYS_SELECT_H,
which should not be set if your system doesn't have sys/select.h.  Remove the
line defining this variable from the configuration header where it is defined
(see the configitems file for a list of locations) and the error should
disappear on the next compilation attempt.

If you have to edit your configuration headers, please send your results to the
author or to the libpqxx-general mailing list so that headers corresponding to
your environment can be included in later libpqxx releases.  Be sure to give a
usable personal name so you can be properly credited in the changelog!

CAUTION: DO NOT try to unset yes/no configuration items by defining their macros
to zero.  A value of zero counts as "yes."  If a macro is defined at all, that
means "yes" (or "true," or "enabled" etc.) regardless of what actual value you
set.  To disable an item, make sure it is not defined at all.


Troubleshooting: linking
------------------------
When you link your application to libpqxx on Windows, you may also need to link
to ws2_32.lib.  On Visual C++, add these in the compiler user interface under:

Project Properties::Linker::Input::Additional Dependencies

We recommend that you specify libpqxx_static.lib and ws2_32.lib there, in that
order.


Visual C++: min() and max()
---------------------------
This has been giving people no end of trouble.  The Visual C++ compiler, by
default, defines min() and max() as preprocessor macros--meaning that a lot of
correct, standard-conforming code will not compile properly.

The libpqxx headers disable this feature by defining the NOMINMAX preprocessor
macro, which tells the Visual C++ headers not to #define min() and max().
However this seems to cause some of Visual C++'s own headers to fail to compile.

One way around this is always to include libpqxx headers before system headers,
and #undef NOMINMAX between the two sets of #includes.

Another workaround, suggested by Trigve Siver, is to include first the system
headers and then the libpqxx headers, and finally:

    #include <algorithm>
    #define max(x,y) std::max(x,y)
    #define min(x,y) std::min(x,y)

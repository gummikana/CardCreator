#include "file_dialog.h"

#include <poro/poro.h>
#include <poro/platform_defs.h>


// open dialog
#ifdef PORO_PLAT_WINDOWS
	#include <windows.h>
#elif PORO_PLAT_MAC
	#import <Cocoa/Cocoa.h>
#elif PORO_PLAT_LINUX
    #ifndef NO_QT
        #include <QApplication>
        #include <QFileDialog>
        #include <QTimer>
    #endif
#endif

#include <fstream>

///////////////////////////////////////////////////////////////////////////////

std::string LoadFileDialog( const std::string& directory )
{
	// ceng::logger_debug << "OpenFileDialog" << std::endl;

    std::string initial_dir = directory;

    //exit fullscreen because modal windows dont work in fullscreen
    std::string file = "";

#ifdef PORO_PLAT_WINDOWS
	// printf("declaring...\n");

	// Declare
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[_MAX_PATH] = {0};
	// HWND hwnd;              // owner window
	// HANDLE hf;              // file handle

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof( ofn ) );
	ofn.lStructSize = sizeof( ofn );
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrFile = szFile;

	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not
	// use the contents of szFile to initialize itself.
	//
	// printf("setting...\n");
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
	ofn.lpstrDefExt = "xml";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	// ceng::logger << "my documents: " << initial_dir << std::endl;
	ofn.lpstrInitialDir = initial_dir.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;


	// Display the Open dialog box.
	if( GetOpenFileName(&ofn) == TRUE )
	{
		file = std::string( szFile );
	}

#endif
#ifdef CENG_PLATFORM_MACOSX
	NSOpenPanel* openDlg = [NSOpenPanel openPanel];
	NSArray* ftypes = [NSArray arrayWithObjects: @"png", nil];
	NSString *path = [[NSString alloc] initWithCString:initial_dir.c_str() encoding:NSMacOSRomanStringEncoding];

	[openDlg setAllowsMultipleSelection:NO];
	[openDlg setCanChooseFiles:YES];
	[openDlg setCanChooseDirectories:NO];
	[openDlg setAllowedFileTypes:ftypes];
	if ([openDlg runModalForDirectory:path file:nil] == NSOKButton)

	{
		NSArray* files = [openDlg filenames];
		file = std::string([[files objectAtIndex:0] cStringUsingEncoding:NSUTF8StringEncoding]);
	}
#endif
#if defined(CENG_PLATFORM_LINUX) && !defined(NO_QT)
    QString default_open_dir = QDir::homePath()+"/Crayon Physics Deluxe/My Levels";
    QString file_name = QFileDialog::getOpenFileName(0, "Open File", default_open_dir, "Crayon Physics Level (*.png);;All Files (*.*)");
    //Run a qt loop to clean up stuff and destory the file dialog window
    QTimer::singleShot(200, QApplication::instance(), SLOT(quit()));
    QApplication::instance()->exec();
    if(file_name.isEmpty() == false){
        file = file_name.toUtf8().data();
    }
#endif

   
	return file;
}

// ============================================================================

std::string SaveFileDialog( const std::string& directory )
{
   //  ceng::logger_debug << "SaveFileDialog" << std::endl;
    std::string path = directory;

    //exit fullscreen because modal windows dont work in fullscreen
    std::string file = "";

#ifdef PORO_PLAT_WINDOWS
	// printf("declaring...\n");

	// Declare
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[_MAX_PATH] = {0};
	// HWND hwnd;              // owner window
	// HANDLE hf;              // file handle

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof( ofn ) );
	ofn.lStructSize = sizeof( ofn );
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrFile = szFile;

	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not
	// use the contents of szFile to initialize itself.
	//
	// printf("setting...\n");
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Level (*.xml)\0*.xml\0";
	ofn.lpstrDefExt = "xml";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = path.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;


	// Display the Open dialog box.
	if( GetSaveFileName( &ofn ) == TRUE )
	{
		file = std::string( szFile );
	}
#endif
#if defined(CENG_PLATFORM_MACOSX)


    NSSavePanel *spanel = [NSSavePanel savePanel];
    NSString *nspath = [[NSString alloc] initWithCString:path.c_str() encoding:NSMacOSRomanStringEncoding];
    [spanel setDirectory:[nspath stringByExpandingTildeInPath]];
    //[spanel setPrompt:NSLocalizedString(@"Save",nil)];
    [spanel setRequiredFileType:@"png"];
    if ([spanel runModalForDirectory:nspath file:nil] == NSOKButton)
    {
        file = std::string([[spanel filename] cStringUsingEncoding:NSUTF8StringEncoding]);
    }

#endif
#if defined(CENG_PLATFORM_LINUX) && !defined(NO_QT)

    //Create default dirs if they don't exist
    QDir().mkdir( QDir::homePath()+"/Crayon Physics Deluxe" );
    QDir().mkdir( QDir::homePath()+"/Crayon Physics Deluxe/My Levels" );
    QString default_save_file = QDir::homePath()+"/Crayon Physics Deluxe/My Levels/untitled.png";
    QString file_name = QFileDialog::getSaveFileName(0, "Save File", default_save_file, "Crayon Physics Level (*.png)");
    //Run a qt loop to clean up stuff and destory the file dialog window
    QTimer::singleShot(200, QApplication::instance(), SLOT(quit()));
    QApplication::instance()->exec();
    
    if(file_name.isEmpty() == false){
        file = file_name.toUtf8().data();

        if( ceng::GetFileExtension( file ) == "" )
            file = file + ".png";
    }

#endif

	return file;
}

//-----------------------------------------------------------------------------
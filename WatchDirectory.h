#pragma once

/*
* WatchDirectory - Allows you to get a callback when the contents of a directory has changed.
* Currently implemented for windows platforms only. To be used in debug builds to get a callback so you can hot reload data that has changed.
* On non-windows platforms the implementation does nothing but does NOT require you to "#ifdef" out your code for those platforms.
*
* How to Use:
*
* //implement a DirectoryWatcher interface
* class MyClass : public DirectoryWatcher
* {
* public:
*	virtual void OnDirChanged(const char* path)
*	{
*		//The Directory was changed
*	};
* };
*
* //Make a WatchDirectory and tell it which dir to watch
* WatchDirectory watchDir;
* watchDir.Watch("C:\\Users\\davep\\Dropbox\\Public");
*
* //Set who is the listener for changes
* MyClass watcher;
* watchDir.SetCallBack(watcher);
*
* //call process once per frame or at interval you would like to monitor for changes
* while ( true )
* {
*	watchDir.Process();
*	Sleep(2000);
* }
*/

/*! \brief Inteface to an object that can recieve a callback when a directory has changed */
class DirectoryWatcher
{
public:
	virtual ~DirectoryWatcher() = 0 {}
	virtual void OnDirChanged(const char* path) = 0;
};

/*! \brief default implementation of directory watcher that does nothing. */
class WatchDirectoryImplNull
{
public:
	void SetCallBack(DirectoryWatcher& callback) {}
	void Watch(const char* pathName) {}
	void Process() {}
};

#if defined(WIN32)

////////////////// Windows Start ////////////////////////////

//see https://msdn.microsoft.com/en-us/library/windows/desktop/aa365261(v=vs.85).aspx - Obtaining Directory Change Notifications

// Basic include required for windows builds
#ifndef UNICODE
#define UNICODE
#endif 
#include <SDKDDKVer.h>	// Including SDKDDKVer.h defines the highest available Windows platform.
#include <windows.h>
#include <stdio.h>		// printf

/*! \brief Windows 32/64 implentation of the directory watcher */
class WatchDirectoryImplWin32
{
public:
	WatchDirectoryImplWin32();
	void SetCallBack(DirectoryWatcher& callback);
	void Watch(const char* pathName);
	void Process();

	HANDLE m_changeHndl;			///< The handle to the directory being watched
	wchar_t m_path[MAX_PATH];		///< The path being watched
	DirectoryWatcher* m_watcher;	///< This object gets the call back when the folder has changed
};

WatchDirectoryImplWin32::WatchDirectoryImplWin32()
	: m_changeHndl(INVALID_HANDLE_VALUE)
	, m_watcher(NULL)
{

}

void WatchDirectoryImplWin32::SetCallBack(DirectoryWatcher& watcher)
{
	m_watcher = &watcher;
}

void WatchDirectoryImplWin32::Watch(const char* pathName)
{
	const bool watchSubtree = false;
	size_t pReturnValue;
	mbstowcs_s(&pReturnValue, m_path, MAX_PATH, pathName, MAX_PATH);
	HANDLE changeHndl = FindFirstChangeNotification(m_path, watchSubtree, FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_FILE_NAME);
	if (changeHndl != INVALID_HANDLE_VALUE)
	{
		m_changeHndl = changeHndl;
	}
	else
	{
		printf("\n ERROR: FindFirstChangeNotification function failed %d.\n", GetLastError());
	}
}

void WatchDirectoryImplWin32::Process()
{
	if (m_changeHndl == INVALID_HANDLE_VALUE)
	{
		return;	//nothing to wait for
	}

	// Wait for notification.

	printf("\nWaiting for notification to: '%ls'\n", m_path);

	const DWORD waitTimeMs = 0;
	const DWORD dwWaitStatus = WaitForMultipleObjects(1, &m_changeHndl, FALSE, waitTimeMs);

	switch (dwWaitStatus)
	{
	case WAIT_OBJECT_0:
		// A file was created, renamed, or deleted in the directory.
		printf("file was changed in the watch folder");
		if (m_watcher)
		{
			m_watcher->OnDirChanged("PATH");
		}

		if (FindNextChangeNotification(m_changeHndl) == FALSE)
		{
			printf("\n ERROR: FindNextChangeNotification function failed %d.\n", GetLastError());
		}
		break;
	}

}

////////////////// Windows End ////////////////////////////
#endif

/*! \brief Used to watch the a directory for changes and recieve a notification when anything in that directory has been changed */
class WatchDirectory
{
public:
	/*! \brief Set the object that will recieve a call back if the directory has changed. */
	inline void SetCallBack(DirectoryWatcher& watcher) { m_wd.SetCallBack(watcher); }

	/*! \brief Cancels any watch on an existing directory and watches the directoyr given by #pathName parameter */
	inline void Watch(const char* pathName) { m_wd.Watch(pathName); }

	/*! \brief Call once per frame to watch for any directory change and issue a call back */
	inline void Process() { m_wd.Process(); }
private:

	//Pmpl for the actual WatchDirectory depending on the platform.
#if defined(WIN32)
	WatchDirectoryImplWin32 m_wd;
#else
	WatchDirectoryImplNull m_wd;			///< default impl that does nothing.
#endif
};
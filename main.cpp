// WatchFolder.cpp : Defines the entry point for the console application.
//

#include "WatchDirectory.h"

int main()
{
	//implements a DirectoryWatcher interface
	class MyClass : public DirectoryWatcher
	{
	public:
		virtual void OnDirChanged(const char* path) 
		{
			//The Directory was changed
		};
	};

	//Make a WatchDirectory and tell it which dir to watch
	WatchDirectory watchDir;
	watchDir.Watch("C:\\Users\\davep\\Dropbox\\Public");
	//Set who is the listener for changes
	MyClass watcher;
	watchDir.SetCallBack(watcher);

	//call process once per frame or at interval you would like to monitor for changes
	while ( true )
	{
		watchDir.Process();
		Sleep(2000);
	}

    return 0;
}


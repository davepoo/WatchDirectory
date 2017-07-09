# WatchDirectory.h
Allows you to get a callback when the contents of a directory has changed.

Currently implemented for windows platforms only. To be used in debug builds to get a callback so you can hot reload data that has changed.
On non-windows platforms the implementation does nothing but does NOT require you to "#ifdef" out your code for those platforms.

# How to Use:

```c++
#include "WatchDirectory.h"

//implement a DirectoryWatcher interface
class MyClass : public DirectoryWatcher
{
public:
	virtual void OnDirChanged(const charpath)
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
'''
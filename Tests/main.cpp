#include <stdio.h>

#include <Folder.h>

int main()
{
	v::Folder folder("/home/nim");
	v::FolderIndex index = folder.GetIndex(true);
	int numFiles = index.GetFileCount();
	int numDirs = index.GetDirCount();
	printf("%d files, %d dirs\n", numFiles, numDirs);
	return 0;
}

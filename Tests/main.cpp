#include <stdio.h>

#include <Folder.h>

int main()
{
	Folder folder(".");
	FolderIndex index = folder.GetIndex();
	printf("%d files, %d dirs\n", index.GetFileCount(), index.GetDirCount());
	return 0;
}

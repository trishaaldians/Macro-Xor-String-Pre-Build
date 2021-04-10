#include "Utils.h"
#include "resource.h"

int main(int argc, char * args[])
{
	printf("\n--------------- Xor Encryption HeRo Corporation ---------------\n\n");

	char CurrentPath[FILENAME_MAX];
	if (!_getcwd(CurrentPath, sizeof(CurrentPath)))
	{
		return 1;
	}

	std::string Directory = CurrentPath;
	std::string Filter = Directory + "\\*.*";

	printf("Current Path: %s\n", Directory);

	WIN32_FIND_DATA FindAllFiles;
	HANDLE hFindFiles = FindFirstFile(Filter.c_str(), &FindAllFiles);
	if (hFindFiles != INVALID_HANDLE_VALUE) 
	{
		while (FindNextFile(hFindFiles, &FindAllFiles) != 0)
		{
			ParseFile(FindAllFiles.cFileName, Directory);
		}
		FindClose(hFindFiles);
	}

	printf("\n-------------------- Macro Pre-Build Event --------------------\n\n");
}

#include "../../Common/Main.h"
#include "../../Common/String.h"

BYTE RandByte()
{
	return rand() % 256;
}

std::string GenerateXor(std::string Text, BOOL & dwStatus)
{
	std::string OriginalText = Text;
	if (OriginalText.find("http://") != -1)
		OriginalText = "http: //" + OriginalText.substr(OriginalText.find("http://") + 7); //just add space from http:// to http: // ( remove blue line )

	std::string Result;
	Result = "";
	dwStatus = TRUE;

	BYTE Start = RandByte();
	std::string End = "0x" + String::Dec2Hex(RandByte()) + String::Dec2Hex(RandByte()) + String::Dec2Hex(RandByte()) + String::Dec2Hex(RandByte());
	int Len = Text.length() + 1;
	std::string Hex = "\"";
	BYTE Value = Start;
	for (unsigned int i = 0; i < Text.length(); i++)
	{
		BYTE ch = Text.substr(i, 1).c_str()[0];
		BYTE chval;
		if (ch == '\\') {
			i++;
			ch = Text.substr(i, 1).c_str()[0];
			if (ch == '0') {
				chval = 0;
			}
			else if (ch == '\\') {
				chval = '\\';
			}
			else if (ch == 'a') {
				chval = 7;
			}
			else if (ch == 'b') {
				chval = 8;
			}
			else if (ch == 't') {
				chval = 9;
			}
			else if (ch == 'n') {
				chval = 10;
			}
			else if (ch == 'v') {
				chval = 11;
			}
			else if (ch == 'f') {
				chval = 12;
			}
			else if (ch == 'r') {
				chval = 13;
			}
			else {
				dwStatus = FALSE;
				Result = std::string("XorGen: invalid control sequence: \\") + (char)ch;
				return Result;
			}
			--Len;
		}
		else if (ch == '|') {
			chval = 0;
		}
		else {
			chval = ch;
			if (!(chval >= 32 && chval <= 126))
			{
				dwStatus = FALSE;
				Result = std::string("XorGen: invalid control sequence: \\") + (char)ch;
				return Result;
			}
		}
		chval ^= Value;
		Value += 1;
		Value %= 256;
		Hex += "\\x" + String::Dec2Hex(chval);
	}
	Hex += '"';

	std::stringstream sstream;

	sstream << "/*" << OriginalText << "*/XorStr<0x" << String::Dec2Hex(Start) << "," << Len << "," << End << ">(";
	std::string Str = sstream.str();
	Str += Hex + "+" + End + ").decode";
	Result = Str;

	return Result;
}

std::string ParseXORContents(std::string & TempLine, std::string setOrigFile, std::string setTempFile, std::string & getOrigFile, std::string & getTempFile)
{
	int StartXor = 0;
	int EndXor = 0;
	int StartQuote = 0;
	int EndQuote = 0;
	int StartSearch = 0;
	bool NeedsEnd = false;

	int Erase = 0;
	int Insert = 0;
	std::string TotalLine = TempLine;

	if (TempLine.length() <= 0)return TempLine;

	while ((unsigned)(EndQuote) < TempLine.length())
	{
		if (!NeedsEnd)
		{
			StartXor = TempLine.find("XOR(", StartSearch);
			if (StartXor == std::string::npos)return TotalLine;

			NeedsEnd = true;
			StartXor += 4;

			StartQuote = TempLine.find("\"", StartXor);
			if (StartQuote == std::string::npos)return TempLine;

			StartQuote += 1;
		}
		else
		{
			EndQuote = TempLine.find("\"", StartQuote);
			if (EndQuote == std::string::npos)return TempLine;

			bool FoundUnEscapedEnd = false;
			int EscapedFixup = 0;
			while (!FoundUnEscapedEnd)
			{
				if (TempLine.at(EndQuote - 1) != '\\')FoundUnEscapedEnd = true;
				else
				{
					EscapedFixup += 1;
					EndQuote = TempLine.find("\"", StartQuote + EscapedFixup);
					if (EndQuote == std::string::npos)return TempLine;
				}
			}

			NeedsEnd = false;

			StartSearch = EndQuote;

			std::string XORContents = TempLine.substr(StartQuote, EndQuote - StartQuote);
			printf("Content: %s\n", XORContents.c_str());

			BOOL Status;
			std::string EncryptedSubString = GenerateXor(XORContents, Status);

			if (!Status)return TempLine;

			EndXor = TempLine.find(")", EndQuote);
			if (EndXor == std::string::npos)return TempLine;

			EndXor += 1;
			StartXor -= 4;

			TotalLine.erase(StartXor - Erase + Insert, EndXor - StartXor);
			TotalLine.insert(StartXor - Erase + Insert, EncryptedSubString);

			Insert += EncryptedSubString.length();
			Erase += (EndXor - StartXor);

			getOrigFile = setOrigFile;
			getTempFile = setTempFile;
		}
	}
	return TempLine;
}

BOOL ParseFile(std::string FileName, std::string Directory)
{
	int StartXOR = 0;
	int EndXOR = 0;
	int LineNumber = 0;
	std::string getLine, getTempFile, getOrigFile;
	char * tempName = _tempnam(NULL, "tmp");
	char szLine[2048];

	if (FileName.find("\\") == std::string::npos && FileName.find(":") == std::string::npos || FileName.find("resource.h") == std::string::npos)
	{
		if (FileName.find(".cpp") != std::string::npos ||
			FileName.find(".c") != std::string::npos ||
			FileName.find(".cxx") != std::string::npos ||
			FileName.find(".h") != std::string::npos ||
			FileName.find(".hpp") != std::string::npos ||
			FileName.find(".hxx") != std::string::npos)
		{
			std::string setOrigFile = Directory + "\\" + FileName;
			std::string setTempFile = tempName;

			//printf("%s\n", setOrigFile.c_str());

			FILE * pFile = NULL;
			FILE * pTempFile = NULL;

			pFile = fopen(setOrigFile.c_str(), "r");
			if (pFile != NULL)
			{
				pTempFile = fopen(setTempFile.c_str(), "wb");
				if (pTempFile != NULL)
				{
					while (feof(pFile) != EOF)
					{
						getLine = "";
						while (feof(pFile) != EOF)
						{
							szLine[0] = '\0';
							fgets(szLine, sizeof(szLine), pFile);

							if (strlen(szLine) > 0)
							{
								getLine += szLine;
								if (szLine[strlen(szLine) - 1] == '\n')break;
							}
							else{
								break;
								return FALSE;
							}
						}

						if (getLine.length() > 0)
						{
							std::string tempLine = ParseXORContents(getLine, setOrigFile, setTempFile, getOrigFile, getTempFile);
							fputs(tempLine.c_str(), pTempFile);
						}
						else{
							break;
							return FALSE;
						}
					}
					fclose(pTempFile);
				}
				fclose(pFile);

				CopyFile(getTempFile.c_str(), getOrigFile.c_str(), false);
				//Sleep(100);
				remove(getTempFile.c_str());
			}
		}
	}
	return TRUE;
}
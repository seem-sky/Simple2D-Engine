#include "FileInput.h"

CFileInput::CFileInput(void)
{
    m_sFileDirectory = "";
    DeleteFileData();
}

CFileInput::~CFileInput(void)
{
}

bool CFileInput::ReadFile(string sFileName)
{
    //Datei wird geöffnet
    FILE *Data;
    fopen_s(&Data, sFileName.c_str(), "rt");
    if(!Data)
    {
        return false;
    }
    while(!feof(Data))
    {
        fgets(m_caData, READ_DATA_MAX_LETTER_IN_ROW - 1, Data);
    }
    InterpretFile();
    DeleteFileData();
    return true;
}

void CFileInput::DeleteFileData()
{
    for (int i = 0; i < READ_DATA_MAX_LETTER_IN_ROW; i++)
        m_caData[i] = 0;
}

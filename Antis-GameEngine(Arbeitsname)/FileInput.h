#ifndef FILE_INPUT_H
#define FILE_INPUT_H

#include <iostream>
#include <fstream>
#include "Logfile.h"
#include "StringAdditions.h"

const unsigned READ_DATA_MAX_LETTER_IN_ROW = 256;

template <class T>
class TFileInput
{
public:
    TFileInput(void)
    {
        m_sFileDirectory = "";
        m_sLogLocationName = LOGFILE_ENGINE_LOG_NAME + "TFileInput : ";
        DeleteFileData();
        m_pLogfile = CLogfile::Get();
    }
    // open, store and interpret file
    bool ReadFile(string sFileName)
    {
        FILE *Data;
        fopen_s(&Data, (m_sFileDirectory + sFileName).c_str(), "rt");
        if(!Data)
        {
            ERROR_LOG(m_sLogLocationName + "Unable to open " + m_sFileDirectory + sFileName + ". No such file or directory.");
            return false;
        }
        if (char *pFileData = GetFileData())
        {
            unsigned int i = 0;
            while(!feof(Data) && i < GetFileDataRows())
            {
                fgets(pFileData, READ_DATA_MAX_LETTER_IN_ROW - 1, Data);
                for (unsigned int j = 0; j < READ_DATA_MAX_LETTER_IN_ROW; j++)
                    ++pFileData;
                i++;
            }
            fclose(Data);
            InterpretFile();
            DeleteFileData();
        }
        else
        {
            ERROR_LOG(m_sLogLocationName + "pFileData is not a valid pointer.");
            fclose(Data);
            return false;
        }
        return true;
    }
protected:
    virtual void InterpretFile() { }
    virtual char* GetFileData() { return NULL; }
    virtual unsigned int GetFileDataRows() { return NULL; }

    string m_sFileDirectory;
    string m_sLogLocationName;
    CLogfile *m_pLogfile;
private:
    void DeleteFileData()
    {
        if (char *pFileData = GetFileData())
        {
            for (unsigned int i = 0; i < GetFileDataRows(); i++)
            {
                char *pTempData = pFileData;
                for (unsigned int j = 0; j < READ_DATA_MAX_LETTER_IN_ROW; j++)
                {
                    pTempData = 0;
                    ++pTempData;
                }
                for (unsigned int j = 0; j < READ_DATA_MAX_LETTER_IN_ROW; j++)
                    ++pFileData;
            }
        }
    }
};
#endif;
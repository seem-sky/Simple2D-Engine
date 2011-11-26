#ifndef FILE_INPUT_H
#define FILE_INPUT_H

#include <fstream>
#include <vector>
#include "Logfile.h"
#include "StringAdditions.h"

template <class T>
class TFileInput
{
public:
    TFileInput(void)
    {
        m_sFileDirectory = "";
        m_sLogLocationName = LOGFILE_ENGINE_LOG_NAME + "TFileInput : ";
        DeleteFileData();
    
    }
    // open, store and interpret file
    bool ReadFile(string sFileName)
    {
        ifstream Data((m_sFileDirectory + sFileName).c_str());
        if(!Data)
        {
            ERROR_LOG(m_sLogLocationName + "Unable to open " + m_sFileDirectory + sFileName + ". No such file or directory.");
            return false;
        }
        if (std::vector<std::string> *pFileData = GetFileData())
        {
            if (!pFileData->empty())
                pFileData->clear();

            std::string DataLine;

            unsigned int i = 0;
            while(std::getline(Data,DataLine))
                pFileData->push_back(DataLine);

            InterpretFile();
            DeleteFileData();
        }
        else
        {
            ERROR_LOG(m_sLogLocationName + "pFileData is not a valid pointer.");
            return false;
        }
        return true;
    }
protected:
    virtual void InterpretFile() { }
    virtual std::vector<std::string>* GetFileData() { return NULL; }
    virtual unsigned int GetFileDataRows() { return NULL; }

    string m_sFileDirectory;
    string m_sLogLocationName;
    CLogfile *m_pLogfile;
private:
    void DeleteFileData()
    {
        if (std::vector<std::string> *pFileData = GetFileData())
        {
            for (unsigned int i = 0; i < pFileData->size(); i++)
                pFileData->at(i).clear();
        }
    }
};
#endif;
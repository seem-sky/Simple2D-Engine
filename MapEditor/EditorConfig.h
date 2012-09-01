#ifndef EDITOR_CONFIG_H
#define EDITOR_CONFIG_H

#include <Singleton.h>

class EditorConfig : public TSingleton<EditorConfig>
{
public:
    EditorConfig(void);
    ~EditorConfig(void);

    std::string GetProjectDirectory() const { return m_sProjectDirectory; }
    void SetProjectDirectory(std::string p_sProjectDir) { m_sProjectDirectory = p_sProjectDir; }

private:
    std::string m_sProjectDirectory;
};
#endif
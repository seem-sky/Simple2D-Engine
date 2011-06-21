#pragma once
#include "fileinput.h"

class MapInput : public TFileInput<MapInput>
{
public:
    MapInput(void);
    ~MapInput(void);
    bool CreateMapFromFile();
};

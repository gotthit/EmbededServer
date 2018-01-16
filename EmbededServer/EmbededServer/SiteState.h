#pragma once

#include "ISiteState.h"

class SiteState : public ISiteState
{
public:
    SiteState();

    virtual Folder * GetAllFolders(int& numberOfFolders);

    virtual Field * GetAllFields(char * formName, int& numberOfFields);

    virtual char * GetURLForRequests();

    virtual bool SetValueInField(char * fieldName, char * value);

    ~SiteState();
};


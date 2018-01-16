#pragma once

#include "Folder.h"
#include "Field.h"

class ISiteState
{
public:

    virtual  Folder * GetAllFolders(int& numberOfFolders) = 0;

    virtual  Field * GetAllFields(char * formName, int& numberOfFields) = 0;

    virtual  char * GetURLForRequests() = 0;

    virtual  bool SetValueInField(char * fieldName, char * value) = 0;
};
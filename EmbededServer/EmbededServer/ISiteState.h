#pragma once

#include "Folder.h"
#include "Field.h"

class ISiteState
{
public:

    Folder * GetAllFolders(int& numberOfFolders);

    Field * GetAllFields(char * formName, int& numberOfFields);

    char * GetURLForRequests();

    void SetValueInField(char * fieldName, char * value);
};
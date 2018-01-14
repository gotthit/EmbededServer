#include "Folder.h"


Folder::Folder(Folder * subfolders, int subfoldersLength, char * name, char * formId)
{
    this->subfolders = subfolders;
    this->subfoldersLength = subfoldersLength;
    this->name = name;
    this->formId = formId;
}

Folder::~Folder()
{
    delete[] subfolders;
    delete[] name;
    delete[] formId;
}

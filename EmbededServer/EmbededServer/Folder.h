#pragma once

class Folder
{
public:

    int subfoldersLength;
    Folder * subfolders;
    char * name;
    char * formId;

    Folder(Folder * subfolders, int subfoldersLength, char * name, char * formId);

    ~Folder();
};


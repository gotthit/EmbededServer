#pragma once

class Field
{
public:

    char * fieldId;
    char * name;
    char * description;
    char * value;
    char * pattern;
    bool readonly;

    Field(char * fieldId, char * name, char * description, char * value, char * pattern, bool readonly);

    ~Field();
};


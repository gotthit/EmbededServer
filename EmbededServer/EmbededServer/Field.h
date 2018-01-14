#pragma once

class Field
{
public:

    char * name;
    char * description;
    char * value;
    char * pattern;
    bool readonly;

    Field(char * name, char * description, char * value, char * pattern, bool readonly);

    ~Field();
};


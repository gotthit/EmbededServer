#include "Field.h"



Field::Field(char * fieldId, char * name, char * description, char * value, char * pattern, bool readonly)
{
    this->fieldId = fieldId;
    this->name = name;
    this->description = description;
    this->value = value;
    this->pattern = pattern != '\0' ? pattern : ".*";
    this->readonly = readonly;
}


Field::~Field()
{
    delete[] name;
    delete[] description;
    delete[] value;
    delete[] pattern;
}

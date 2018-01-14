#pragma once

#include "ISiteState.h"

class Controller
{
private:

    const char* beforeForlders = "\0";
    const char* beforeFrom = "\0";
    const char* beforeOpenedFolderId = "\0";
    const char* endOfHtml = "\0";

    const char* formNameBegin = "\0";
    const char* formNameMiddle = "\0";
    const char* formNameEnd = "\0";

    const char* beforeFolderName = "\0";
    const char* beforeFolderBody = "\0";
    const char* endOfFolder = "\0";

    const char* beforeURL = "\0";
    const char* beforeFieldName = "\0";
    const char* beforeFieldDescription = "\0";
    const char* beforeHiddenFieldName = "\0";
    const char* beforeFieldPattern = "\0";
    const char* beforeFieldValue = "\0";
    const char* endOfField = "\0";

    const char* beforeReadonlyFieldName = "\0";
    const char* beforeReadonlyFieldDescription = "\0";
    const char* beforeReadonlyFieldValue = "\0";
    const char* endOfReadonlyField = "\0";

    
    int beforeForldersLength = lengthOfString(beforeForlders);
    int beforeFromLength = lengthOfString(beforeFrom);
    int beforeOpenedFolderIdLength = lengthOfString(beforeOpenedFolderId);
    int endOfHtmlLength = lengthOfString(endOfHtml);

    int formNameBeginLength = lengthOfString(formNameBegin);
    int formNameMiddleLength = lengthOfString(formNameMiddle);
    int formNameEndLength = lengthOfString(formNameEnd);

    int beforeFolderNameLength = lengthOfString(beforeFolderName);
    int beforeFolderBodyLength = lengthOfString(beforeFolderBody);
    int endOfFolderLength = lengthOfString(endOfFolder);

    int beforeURLLength = lengthOfString(beforeURL);
    int beforeFieldNameLength = lengthOfString(beforeFieldName);
    int beforeFieldDescriptionLength = lengthOfString(beforeFieldDescription);
    int beforeHiddenFieldNameLength = lengthOfString(beforeHiddenFieldName);
    int beforeFieldPatternLength = lengthOfString(beforeFieldPattern);
    int beforeFieldValueLength = lengthOfString(beforeFieldValue);
    int endOfFieldLength = lengthOfString(endOfField);

    int beforeReadonlyFieldNameLength = lengthOfString(beforeReadonlyFieldName);
    int beforeReadonlyFieldDescriptionLength = lengthOfString(beforeReadonlyFieldDescription);
    int beforeReadonlyFieldValueLength = lengthOfString(beforeReadonlyFieldValue);
    int endOfReadonlyFieldLength = lengthOfString(endOfReadonlyField);


    const char * formIdParameter = "form-id";
    const char * fieldNameParameter = "field-name";
    const char * fieldValueParameter = "field-value";

    ISiteState* state;


    int lengthOfString(const char * str);
    void appendString(char * str, int& index, const char * toAppend);
    char * getParameterValue(const char* requestBody, const char * parameterName);


    void addFolderPartLength(Folder * allFolders, int numberOfFolders, int& totalLength);
    void addFormPartLength(Field * allFields, int numberOfFields, char * formId, int& totalLength);

    int determineResultLength(Folder * allFolders, int numberOfFolders, Field * allFields, char * formId, int numberOfFields);


    void createFolderPart(Folder * allFolders, int numberOfFolders, char * result, int& index);
    void createFormPart(Field * allFields, int numberOfFields, char * formId, char * result, int& index);

    char* createRusult(int resultLength, Folder * allFolders, int numberOfFolders, Field * allFields, char * formId, int numberOfFields);

public:

    Controller(ISiteState* state);

    char * HandleRequest(const char* requestBody);

    ~Controller();
};
#include "Controller.h"

Controller::Controller(ISiteState* state)
{
    this->state = state;
}

int Controller::lengthOfString(const char * str)
{
    int length = 0;
    while (str[length] != '\0') ++length;
    return length;
}

void Controller::appendString(char * str, int& index, const char * toAppend)
{
    for (int j = 0; toAppend[j] != '\0'; ++j, ++index)
    {
        str[index] = toAppend[j];
    }
}

char * Controller::getParameterValue(const char* requestBody, const char * parameterName)
{
    int start = -1;
    int end = -1;
    bool found;
    for (int i = 0; requestBody[i] != '\0'; ++i)
    {
        found = true;
        int j = 0;
        for (; parameterName[j] != '\0'; ++j)
        {
            if (requestBody[i + j] != parameterName[j])
            {
                found = false;
                break;
            }
        }
        if (found) 
        {
            start = i + j;
            break;
        }
    }
    if (found && requestBody[start] != '\0')
    {
        ++start;
        end = start;
        while (requestBody[end] != '\0' || requestBody[end] != '&')
        {
            ++end;
        }
        char * result = new char[end - start + 1];
        for (int i = start; i < end; ++i)
        {
            result[i - start] = requestBody[i];
        }
        result[end] = '\0';
        return result;
    }
    else
    {
        return '\0';
    }
}

void Controller::addFolderPartLength(Folder * allFolders, int numberOfFolders, int& totalLength)
{
    for (int i = 0; i < numberOfFolders; ++i)
    {
        if (allFolders[i].subfoldersLength == 0)
        {
            totalLength += formNameBeginLength;
            totalLength += formNameMiddleLength;
            totalLength += formNameEndLength;

            totalLength += 2 * lengthOfString(allFolders[i].formId);
        }
        else
        {
            totalLength += beforeFolderNameLength;
            totalLength += beforeFolderBodyLength;
            totalLength += endOfFolderLength;

            totalLength += lengthOfString(allFolders[i].name);

            addFolderPartLength(allFolders[i].subfolders, allFolders[i].subfoldersLength, totalLength);
        }
    }
}

void Controller::addFormPartLength(Field * allFields, int numberOfFields, char * formId, int& totalLength)
{
    int urlLength = lengthOfString(state->GetURLForRequests());
    int formNameLength = lengthOfString(formId);
    for (int i = 0; i < numberOfFields; ++i)
    {
        if (allFields[i].readonly)
        {
            totalLength += beforeReadonlyFieldNameLength;
            totalLength += beforeReadonlyFieldDescriptionLength;
            totalLength += beforeReadonlyFieldValueLength;
            totalLength += endOfReadonlyFieldLength;

            totalLength += lengthOfString(allFields[i].name);
            totalLength += lengthOfString(allFields[i].description);
            totalLength += lengthOfString(allFields[i].value);
        }
        else
        {
            totalLength += beforeURLLength;
            totalLength += beforeFieldNameLength;
            totalLength += beforeFieldDescriptionLength;
            totalLength += beforeHiddenFieldNameLength;
            totalLength += beforeFieldPatternLength;
            totalLength += beforeFieldValueLength;
            totalLength += endOfFieldLength;

            totalLength += urlLength;
            totalLength += formNameLength;

            totalLength += 2 * lengthOfString(allFields[i].name);
            totalLength += lengthOfString(allFields[i].description);
            totalLength += lengthOfString(allFields[i].pattern);
            totalLength += lengthOfString(allFields[i].value);
        }
    }
}

int Controller::determineResultLength(Folder * allFolders, int numberOfFolders, Field * allFields, char * formId, int numberOfFields)
{
    int resultLength = 0;
    resultLength += beforeForldersLength;
    resultLength += beforeFromLength;
    resultLength += beforeOpenedFolderIdLength;
    resultLength += endOfHtmlLength;

    addFolderPartLength(allFolders, numberOfFolders, resultLength);
    addFormPartLength(allFields, numberOfFields, formId, resultLength);

    return resultLength;
}

void Controller::createFolderPart(Folder * allFolders, int numberOfFolders, char * result, int& index)
{
    for (int i = 0; i < numberOfFolders; ++i)
    {
        if (allFolders[i].subfoldersLength == 0)
        {
            appendString(result, index, formNameBegin);
            appendString(result, index, allFolders[i].formId);

            appendString(result, index, formNameMiddle);
            appendString(result, index, allFolders[i].formId);

            appendString(result, index, formNameEnd);
        }
        else
        {
            appendString(result, index, beforeFolderName);
            appendString(result, index, allFolders[i].name);

            appendString(result, index, beforeFolderBody);

            createFolderPart(allFolders[i].subfolders, allFolders[i].subfoldersLength, result, index);

            appendString(result, index, endOfFolder);
        }
    }
}

void Controller::createFormPart(Field * allFields, int numberOfFields, char * formId, char * result, int& index)
{
    for (int i = 0; i < numberOfFields; ++i)
    {
        if (allFields[i].readonly)
        {
            appendString(result, index, beforeReadonlyFieldName);
            appendString(result, index, allFields[i].name);

            appendString(result, index, beforeReadonlyFieldDescription);
            appendString(result, index, allFields[i].description);

            appendString(result, index, beforeReadonlyFieldValue);
            appendString(result, index, allFields[i].value);

            appendString(result, index, endOfReadonlyField);
        }
        else
        {
            appendString(result, index, beforeURL);
            appendString(result, index, state->GetURLForRequests());

            appendString(result, index, beforeFieldName);
            appendString(result, index, allFields[i].name);

            appendString(result, index, beforeFieldDescription);
            appendString(result, index, allFields[i].description);

            appendString(result, index, beforeHiddenFieldName);
            appendString(result, index, allFields[i].name);

            appendString(result, index, beforeFieldPattern);
            appendString(result, index, allFields[i].pattern);

            appendString(result, index, beforeFieldValue);
            appendString(result, index, allFields[i].value);

            appendString(result, index, endOfField);
        }
    }
}

char* Controller::createRusult(int resultLength, Folder * allFolders, int numberOfFolders, Field * allFields, char * formId, int numberOfFields)
{
    char * result = new char[resultLength + 1];
    int index = 0;

    appendString(result, index, beforeForlders);
    createFolderPart(allFolders, numberOfFolders, result, index);

    appendString(result, index, beforeFrom);
    createFormPart(allFields, numberOfFields, formId, result, index);

    appendString(result, index, beforeOpenedFolderId);
    appendString(result, index, formId);

    appendString(result, index, endOfHtml);

    result[resultLength] = '\0';

    return result;
}

char * Controller::HandleRequest(const char* requestBody)
{
    char * formId = getParameterValue(requestBody, formIdParameter);
    char * fieldName = getParameterValue(requestBody, fieldNameParameter);
    char * fieldValue = getParameterValue(requestBody, fieldValueParameter);

    int numberOfFolders = 0;
    int numberOfFields = 0;
    Folder * allFolders = state->GetAllFolders(numberOfFolders);
    Field * allFields = nullptr;

    if (fieldName != '\0')
    {
        state->SetValueInField(fieldName, fieldValue);
    }
    if (formId != '\0')
    {
        allFields = state->GetAllFields(formId, numberOfFields);
    }

    int resultLength = determineResultLength(allFolders, numberOfFolders, allFields, formId, numberOfFields);
    char * result = createRusult(resultLength, allFolders, numberOfFolders, allFields, formId, numberOfFields);

    delete[] formId;
    delete[] fieldName;
    delete[] fieldValue;

    return result;
}

Controller::~Controller()
{
    delete state;
}
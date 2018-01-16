#include "Controller.h"

Controller::Controller(ISiteState* state)
{
    this->state = state;
}

int Controller::LengthOfString(const char * str)
{
    int length = 0;
    while (str[length] != '\0') ++length;
    return length;
}

void Controller::appendString(char * appendTo, int& index, const char * toAppend)
{
    for (int j = 0; toAppend[j] != '\0'; ++j, ++index)
    {
        appendTo[index] = toAppend[j];
    }
}

int Controller::getHexadecimalDigitToDecimal(char ch)
{
    if ('0' <= ch && ch <= '9')
    {
        return ch - '0';
    }
    return ch - 'A' + 10;
}

int Controller::getCodeInDecimal(const char * source, int index)
{
    return getHexadecimalDigitToDecimal(source[index]) * 16 + getHexadecimalDigitToDecimal(source[index + 1]);
}

Controller::specialCode Controller::getSpecialCode(const char * source, int& index)
{
    if (source[index] == codeChar)
    {
        int code = getCodeInDecimal(source, index + 1);
        if (code == specialCode::StartOfParamName)
        {
            index += 3;
            return specialCode::StartOfParamName;
        }
        if (code == specialCode::EndOfParamValue)
        {
            index += 3;
            return specialCode::EndOfParamValue;
        }
        if (code == specialCode::PasteMark)
        {
            index += 3;
            return specialCode::PasteMark;
        }
    }
    return specialCode::NotASpecialCode;
}

char Controller::getNextChar(const char * source, int& index)
{
    char result;
    if (source[index] == codeChar)
    {
        result = (char)getCodeInDecimal(source, index + 1);
        index += 3;
        return result;
    }
    result = source[index];
    ++index;
    return result;
}

Controller::pasteType Controller::getPasteType(const char * source, int& index)
{
    pasteType result = (pasteType)getCodeInDecimal(source, index);
    index += 2;
    return result;
}

char * Controller::getParameterValue(const char* requestBody, const char * parameterName)
{
    int startOfValue = -1;
    bool found = false;

    int requestIndex = 0;
    while (true)
    {
        char symbol;
        specialCode code = getSpecialCode(requestBody, requestIndex);
        if (code == specialCode::StartOfParamName)
        {
            found = true;
            for (int paramIndex = 0; parameterName[paramIndex] != '\0'; ++paramIndex)
            {
                if (parameterName[paramIndex] != getNextChar(requestBody, requestIndex))
                {
                    found = false;
                    break;
                }
            }
            if (found)
            {
                break;
            }
        }
        else if (code == specialCode::NotASpecialCode)
        {
            symbol = getNextChar(requestBody, requestIndex);
            if (symbol == '\0')
            {
                break;
            }
        }
        else
        {
            // Never goes here
        }
    }

    if (found)
    {
        ++requestIndex;
        startOfValue = requestIndex;

        int lengthOfValue = 0;
        // remember lazy calculations of Boolean expressions
        while (getSpecialCode(requestBody, requestIndex) != specialCode::EndOfParamValue && getNextChar(requestBody, requestIndex) != '\0')
        {
            ++lengthOfValue;
        }
        ++lengthOfValue;
        char * result = new char[lengthOfValue];

        requestIndex = startOfValue;

        for (int i = 0; i < lengthOfValue; ++i)
        {
            result[i] = getNextChar(requestBody, requestIndex);
        }
        result[lengthOfValue - 1] = '\0';
        return result;
    }
    else
    {
        return new char[1] {'\0'};
    }
}

bool Controller::checkRequestType(const char * requestBody, const char * type)
{
    bool equals = true;
    for (int i = 0; type[i] != '\0'; ++i)
    {
        if (requestBody[i] == '\0' || requestBody[i] != type[i])
        {
            equals = false;
            break;
        }
    }
    return equals;
}

// You can leave some parameters as null if you are sure, that they will never be pasted
void Controller::addNumberOfSymbolsInFilledPattern(
    int& totalLength,
    const char * pattern,
    Folder * allFolders,
    int numberOfFolders,
    Field * allFields,
    int numberOfFields,
    const char * formId,
    Folder * currentFolder,
    Field * currentField)
{
    int patternIndex = 0;
    char currentSymbol = '\0';
    specialCode currentSpecialCode;
    pasteType currentPasteType;

    while (true)
    {
        currentSpecialCode = getSpecialCode(pattern, patternIndex);

        if (currentSpecialCode == specialCode::PasteMark)
        {
            currentPasteType = getPasteType(pattern, patternIndex);

            switch (currentPasteType)
            {
            case PasteFolders:

                addFolderPartLength(allFolders, numberOfFolders, totalLength);
                break;
            case PasteFields:

                addFieldsPartLength(allFields, numberOfFields, totalLength);
                break;
            case PasteFieldId:

                totalLength += LengthOfString(currentField->fieldId);
                break;
            case PasteFieldName:

                totalLength += LengthOfString(currentField->name);
                break;
            case PasteFieldDescription:

                totalLength += LengthOfString(currentField->description);
                break;
            case PasteFieldValue:

                totalLength += LengthOfString(currentField->value);
                break;
            case PasteFieldPattern:

                totalLength += LengthOfString(currentField->pattern);
                break;
            case PasteFolderName:

                totalLength += LengthOfString(currentFolder->name);
                break;
            case PasteFormId:

                totalLength += LengthOfString(currentFolder->formId);
                break;
            case PasteOpenedFormId:

                totalLength += LengthOfString(formId);
                break;
            case PasteServerUrl:

                totalLength += LengthOfString(state->GetURLForRequests());
                break;
            default:
                // Never go here. Add more cases if needed
                break;
            }
        }
        else
        {
            if (pattern[patternIndex] == '\0')
            {
                break;
            }
            ++patternIndex;

            ++totalLength;
        }
    }
}

// You can leave some parameters as null if you are sure, that they will never be pasted
void Controller::appendFilledPattern(
    char * appendTo,
    int& index,
    const char * pattern,
    Folder * allFolders,
    int numberOfFolders,
    Field * allFields,
    int numberOfFields,
    const char * formId,
    Folder * currentFolder,
    Field * currentField)
{
    int patternIndex = 0;
    char currentSymbol = '\0';
    specialCode currentSpecialCode;
    pasteType currentPasteType;

    while (true)
    {
        currentSpecialCode = getSpecialCode(pattern, patternIndex);

        if (currentSpecialCode == specialCode::PasteMark)
        {
            currentPasteType = getPasteType(pattern, patternIndex);

            switch (currentPasteType)
            {
            case PasteFolders:

                createFolderPart(allFolders, numberOfFolders, appendTo, index);
                break;
            case PasteFields:

                createFieldsPart(allFields, numberOfFields, appendTo, index);
                break;
            case PasteFieldId:

                appendString(appendTo, index, currentField->fieldId);
                break;
            case PasteFieldName:

                appendString(appendTo, index, currentField->name);
                break;
            case PasteFieldDescription:

                appendString(appendTo, index, currentField->description);
                break;
            case PasteFieldValue:

                appendString(appendTo, index, currentField->value);
                break;
            case PasteFieldPattern:

                appendString(appendTo, index, currentField->pattern);
                break;
            case PasteFolderName:

                appendString(appendTo, index, currentFolder->name);
                break;
            case PasteFormId:

                appendString(appendTo, index, currentFolder->formId);
                break;
            case PasteOpenedFormId:

                appendString(appendTo, index, formId);
                break;
            case PasteServerUrl:

                appendString(appendTo, index, state->GetURLForRequests());
                break;
            default:
                // Never go here. Add more cases if needed
                break;
            }
        }
        else
        {
            currentSymbol = pattern[patternIndex];

            if (currentSymbol == '\0')
            {
                break;
            }
            appendTo[index] = currentSymbol;

            ++index;
            ++patternIndex;
        }
    }
}

void Controller::addFolderPartLength(Folder * allFolders, int numberOfFolders, int& totalLength)
{
    for (int i = 0; i < numberOfFolders; ++i)
    {
        if (allFolders[i].subfoldersLength == 0)
        {
            addNumberOfSymbolsInFilledPattern(totalLength, formInFolderHtml, nullptr, 0, nullptr, 0, nullptr, &allFolders[i], nullptr);
        }
        else
        {
            addNumberOfSymbolsInFilledPattern(totalLength, folderHtml, allFolders[i].subfolders, allFolders[i].subfoldersLength,
                nullptr, 0, nullptr, &allFolders[i], nullptr);
        }
    }
}

void Controller::addFieldsPartLength(Field * allFields, int numberOfFields, int& totalLength)
{
    for (int i = 0; i < numberOfFields; ++i)
    {
        int patternIndex = 0;

        if (allFields[i].readonly)
        {
            addNumberOfSymbolsInFilledPattern(totalLength, readonlyFieldHtml, nullptr, 0, nullptr, 0, nullptr, nullptr, &allFields[i]);
        }
        else
        {
            addNumberOfSymbolsInFilledPattern(totalLength, fieldHtml, nullptr, 0, nullptr, 0, nullptr, nullptr, &allFields[i]);
        }
    }
}

int Controller::determineResultLength(Folder * allFolders, int numberOfFolders, Field * allFields, int numberOfFields, char * formId)
{
    int totalLength = 0;

    addNumberOfSymbolsInFilledPattern(totalLength, mainHtml, allFolders, numberOfFolders, allFields, numberOfFields, formId, nullptr, nullptr);

    return totalLength;
}

void Controller::createFolderPart(Folder * allFolders, int numberOfFolders, char * result, int& index)
{
    for (int i = 0; i < numberOfFolders; ++i)
    {
        if (allFolders[i].subfoldersLength == 0)
        {
            appendFilledPattern(result, index, formInFolderHtml, nullptr, 0, nullptr, 0, nullptr, &allFolders[i], nullptr);
        }
        else
        {   
            appendFilledPattern(result, index, folderHtml, allFolders[i].subfolders, allFolders[i].subfoldersLength,
                nullptr, 0, nullptr, &allFolders[i], nullptr);
        }
    }
}

void Controller::createFieldsPart(Field * allFields, int numberOfFields, char * result, int& index)
{
    for (int i = 0; i < numberOfFields; ++i)
    {
        int patternIndex = 0;

        if (allFields[i].readonly)
        {
            appendFilledPattern(result, index, readonlyFieldHtml, nullptr, 0, nullptr, 0, nullptr, nullptr, &allFields[i]);
        }
        else
        {
            appendFilledPattern(result, index, fieldHtml, nullptr, 0, nullptr, 0, nullptr, nullptr, &allFields[i]);
        }
    }
}

char* Controller::createRusult(int resultLength, Folder * allFolders, int numberOfFolders, Field * allFields, int numberOfFields, char * formId)
{
    char * result = new char[resultLength + 1];
    int resultIndex = 0;

    appendFilledPattern(result, resultIndex, mainHtml, allFolders, numberOfFolders, allFields, numberOfFields, formId, nullptr, nullptr);

    result[resultLength] = '\0';

    return result;
}

char * Controller::HandleRequest(const char* requestBody)
{
    char * result = nullptr;

    char * formId = getParameterValue(requestBody, formIdParameter);
    char * fieldName = getParameterValue(requestBody, fieldNameParameter);
    char * fieldValue = getParameterValue(requestBody, fieldValueParameter);

    if (checkRequestType(requestBody, "POST"))
    {
        bool isSucceeded = state->SetValueInField(fieldName, fieldValue);
        if (isSucceeded)
        {
            result = new char[3] {'O', 'K', '\0'};
        }
        else
        {
            result = new char[5] {'F', 'A', 'I', 'L', '\0' };
        }
    }
    else
    {

        int numberOfFolders = 0;
        int numberOfFields = 0;
        Folder * allFolders = state->GetAllFolders(numberOfFolders);
        Field * allFields = nullptr;
        if (Controller::LengthOfString(formId) != 0)
        {
            allFields = state->GetAllFields(formId, numberOfFields);
        }

        int resultLength = determineResultLength(allFolders, numberOfFolders, allFields, numberOfFields, formId);
        result = createRusult(resultLength, allFolders, numberOfFolders, allFields, numberOfFields, formId);
    }

    delete[] formId;
    delete[] fieldName;
    delete[] fieldValue;

    return result;
}

Controller::~Controller()
{
    delete state;
}
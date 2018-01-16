#pragma once

#include "ISiteState.h"

class Controller
{
private:

    // length of all codes equals 2
    // all codes written in hexadecimal notation

    const char codeChar = '%';

    enum specialCode {NotASpecialCode = 0, StartOfParamName = 65, EndOfParamValue = 66, PasteMark = 67};
    enum pasteType {
        PasteFolders = 1, PasteFields = 2, PasteFieldId = 3, PasteFieldName = 4, PasteFieldDescription = 5, PasteFieldValue = 6,
        PasteFieldPattern = 7, PasteFolderName = 8, PasteFormId = 9, PasteOpenedFormId = 10, PasteServerUrl = 11       
    };
    
    const char * mainHtml = "<head> <title>Page name</title> <meta charset='utf-8'> <style rel='stylesheet' type='text/css'> html{min-width: 500px;}.main{margin-top: 50px;}.hidden{display: none;}.nav-bar{width: 250px; float: left;}.nav-bar *{overflow-y: hidden;}.folder-name, .form-name{border-width: 2px; border-style: solid; padding: 5px; padding-left: 13px;}.folder-name:hover, .form-name:hover{cursor: pointer;}.folder-body{padding-left: 20px; height: 0px;}.content{margin-left: 290px;}.parameter{margin-top: 50px;}.parameter-text{display: inline-block; max-width: 500px;}.parameter-value, .parameter-button{margin: 10px; display: inline-block; vertical-align: top;}.parameter-text p{margin: 5px;}.parameter-name{font-weight: bold;}.parameter-description{font-size: 18px;}</style></head><body> <div class='main'> <div class='nav-bar'> %4301 </div><div class='content'> %4302 </div></div><script>function initInteraction(){var serverUrl='%430B'; var prevCodeSymbolReplacement='%' + '%'.charCodeAt(0).toString(16).toUpperCase(); var startOfParamNameSymbol='%' + (65).toString(16).toUpperCase(); var endOfParamValueSymbol='%' + (66).toString(16).toUpperCase(); var folderNames=document.getElementsByClassName('folder-name'); var lastOpenedFolder=null; for (var i=0; i < folderNames.length; ++i){folderNames[i].onclick=function(){this.parentElement.lastElementChild.style.height='auto'; if (lastOpenedFolder !=null){while (lastOpenedFolder.className=='folder'){lastOpenedFolder.lastElementChild.style.height='0px'; if (lastOpenedFolder==this.parentElement){lastOpenedFolder=lastOpenedFolder.parentElement.parentElement; if (lastOpenedFolder.className !='folder'){lastOpenedFolder=null;}return;}lastOpenedFolder=lastOpenedFolder.parentElement.parentElement;}}var currentOpenedFolder=this.parentElement; while (currentOpenedFolder.className=='folder'){currentOpenedFolder.lastElementChild.style.height='auto'; currentOpenedFolder=currentOpenedFolder.parentElement.parentElement;}lastOpenedFolder=this.parentElement;};}var parameterNames=document.getElementsByClassName('parameter-name'); var maxWidth=-1; for (var i=0; i < parameterNames.length; ++i){maxWidth=Math.max(maxWidth, parameterNames[i].clientWidth);}for (var i=0; i < parameterNames.length; ++i){parameterNames[i].style.width=maxWidth + 'px';}var formNames=document.getElementsByClassName('form-name'); for (var i=0; i < formNames.length; ++i){formNames[i].onclick=function(){document.location.href=(serverUrl + '?').replace(/[%]/g, prevCodeSymbolReplacement) + startOfParamNameSymbol + ('form-id=' + this.id).replace(/[%]/g, prevCodeSymbolReplacement) + endOfParamValueSymbol;};}var parameterButtons=document.getElementsByClassName('parameter-button'); for (var i=0; i < parameterButtons.length; ++i){parameterButtons[i].onclick=function(){var parameterId=this.parentElement.getAttribute('data-id'); var parameterValue=this.parentElement.getElementsByClassName('parameter-value')[0].value; var xhr=new XMLHttpRequest(); xhr.open('POST', serverUrl, false); xhr.send(startOfParamNameSymbol + ('field-id=' + parameterId).replace(/[%]/g, prevCodeSymbolReplacement) + endOfParamValueSymbol + startOfParamNameSymbol +('field-value=' + parameterValue).replace(/[%]/g, prevCodeSymbolReplacement) + endOfParamValueSymbol); if (xhr.status==200){if (xhr.responseText=='OK'){alert('The value of field successfully setted');}else{alert('Can not set the value of the field')}}else{alert(xhr.status + ': ' + xhr.statusText);}};}var openedForm=document.getElementById('%430A'); if (openedForm !=null){openedForm.parentElement.parentElement.parentElement.firstElementChild.onclick();}}initInteraction(); </script></body>";
    const char * folderHtml = "<div class='folder'> <div class='folder-name'>%4308</div><div class='folder-body'>%4301 </div></div>\0";
    const char * formInFolderHtml = "<div class='folder'> <div class='form-name' id='%4309'>%4308</div></div>\0";
    const char * fieldHtml = "<div class='parameter' data-id='%4303'> <div class='parameter-text'> <p class='parameter-name'>%4304</p><p class='parameter-description'>%4305</p></div><input class='parameter-value' type='text' pattern='%4307' value='%4306'> <button class='parameter-button'>Set property</button> </div>";
    const char * readonlyFieldHtml = "<div class='parameter'> <div class='parameter-text'> <p class='parameter-name'>%4304</p><p class='parameter-description'>%4305</p></div><input class='parameter-value' type='text' readonly value='%4306'> </div>";

    const char * formIdParameter = "form-id";
    const char * fieldNameParameter = "field-id";
    const char * fieldValueParameter = "field-value";

    ISiteState* state;

    int getHexadecimalDigitToDecimal(char ch);
    int getCodeInDecimal(const char * source, int index);
    Controller::specialCode getSpecialCode(const char * source, int& index);
    Controller::pasteType getPasteType(const char * source, int& index);
    char getNextChar(const char * source, int& index);

    void appendString(char * str, int& index, const char * toAppend);
    char * getParameterValue(const char* requestBody, const char * parameterName);

    bool checkRequestType(const char * requestBody, const char * type);

    void addNumberOfSymbolsInFilledPattern(int& totalLength, const char * pattern, Folder * allFolders, int numberOfFolders, Field * allFields, int numberOfFields,
        const char * formId, Folder * currentFolder, Field * currentField);

    void appendFilledPattern(char * appendTo, int& index, const char * pattern, Folder * allFolders, int numberOfFolders, Field * allFields, int numberOfFields,
        const char * formId, Folder * currentFolder, Field * currentField);

    void addFolderPartLength(Folder * allFolders, int numberOfFolders, int& totalLength);
    void addFieldsPartLength(Field * allFields, int numberOfFields, int& totalLength);
    int determineResultLength(Folder * allFolders, int numberOfFolders, Field * allFields, int numberOfFields, char * formId);

    void createFolderPart(Folder * allFolders, int numberOfFolders, char * result, int& index);
    void createFieldsPart(Field * allFields, int numberOfFields, char * result, int& index);
    char* createRusult(int resultLength, Folder * allFolders, int numberOfFolders, Field * allFields, int numberOfFields, char * formId);

public:

    static int LengthOfString(const char * str);

    Controller(ISiteState* state);

    char * HandleRequest(const char* requestBody);

    ~Controller();
};
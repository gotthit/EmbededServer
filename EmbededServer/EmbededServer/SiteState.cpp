#include "SiteState.h"


SiteState::SiteState()
{
}

Folder * SiteState::GetAllFolders(int& numberOfFolders)
{
    numberOfFolders = 2;

    return new Folder[2]{
        Folder(new Folder[2]{
                Folder(new Folder[2]{
                    Folder(nullptr, 0, "end ", "asdd"),
                    Folder(nullptr, 0, "��� ��� ", "125555")
                }, 2, "Open it", "Open it __ q"),
            Folder(nullptr, 0, "% some ��� LUL", "1yur sd")
        }, 2, "Open it", "Open it __ q"),
        Folder(new Folder[2]{
            Folder(nullptr, 0, " end   tooooo    ", "124 ��� gsd"),
            Folder(nullptr, 0, "end", "15 as ")
        }, 2, "Open it", "Open it __ q")
    };
}

Field * SiteState::GetAllFields(char * formName, int& numberOfFields)
{
    numberOfFields = 3;

    return new Field[3]{
        Field("id 1 asd", "NAME_1", "blablabla bla lbab labllab", "123,2", "", false),
        Field("2", "2 ���", "blaa wd srfbl sefa b", "1%a sd", "", true),
        Field("%3%", "3 NAME", "blabl abllab", "������ ����", "", false)
    };
}

char * SiteState::GetURLForRequests()
{
    return "http://localhost";
}

bool SiteState::SetValueInField(char * fieldName, char * value)
{
    return true;
}

SiteState::~SiteState()
{
}

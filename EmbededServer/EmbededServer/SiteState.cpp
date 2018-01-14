#include "SiteState.h"


SiteState::SiteState()
{
}

Folder * ISiteState::GetAllFolders(int& numberOfFolders)
{
    return 0;
}

Field * ISiteState::GetAllFields(char * formName, int& numberOfFields)
{
    return 0;
}

char * ISiteState::GetURLForRequests()
{
    return 0;
}

void ISiteState::SetValueInField(char * fieldName, char * value)
{

}

SiteState::~SiteState()
{
}

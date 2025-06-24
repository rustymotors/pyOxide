/**
 * @file NPSLoginDll.cpp
 * @brief NPSLoginDll API implementation
 *
 * This file contains the implementation of the NPSLoginDll API.
 *
 * @ingroup MCO
 * @ingroup NPS
 * @ingroup NPSLoginDll
 *
 * @version 1.0
 *
 * @date 2020-04-01
 *
 * @note This file is part of the Motor City Online project and was created by reverse engineering the debug client.
 *
 * @see NPSLoginDll.h
 *
*/
#include "NPSLoginDll.h"

NPSSTATUS cNPSLoginAPI::NPSUserLogin(const char *UserName, const char *Password, const char *aaiServiceId, unsigned int crc, NPS_UserStatus &UserStatus, char errTxt[512], char url[512], tfIdleCallBack IdleCallBack, void *Context)
{
    NPSSTATUS result;

    result = NPS_NOT_IMPLEMENTED;

    // This function is not implemented yet

    return result;
};

cNPSLoginAPI * NPSLoginAPI_GetInterface(const char *AuthLoginDllpath, const char *inAuthLoginBaseService, const char *inAuthLoginServer)
{
    return NPSLoginDllExport new cNPSLoginAPI();
}

void NPSLoginAPI_ReleaseInterface(void)
{
    return;
};

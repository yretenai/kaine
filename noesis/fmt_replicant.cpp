//
// Created by Lilith on 2021-05-12.
//

#include "support/pluginshare.h"
#include "XAPFile.hpp"

const char* g_pPluginName = "fmt_replicant";
const char* g_pPluginDesc = "handlers for NieR:Replicant";

using namespace fmt_replicant;

bool __cdecl NPAPI_InitLocal() {
    int handle = g_nfn->NPAPI_Register(const_cast<char *>("NieR Replicant Model XAP"), const_cast<char *>(".xap"));
    if (handle < 0) {
        return false;
    }
    g_nfn->NPAPI_SetTypeHandler_TypeCheck(handle, XAPFile::check_model);
    g_nfn->NPAPI_SetTypeHandler_LoadModel(handle, XAPFile::load_model);


    handle = g_nfn->NPAPI_Register(const_cast<char *>("NieR Replicant Texture XAP"), const_cast<char *>(".xap"));
    if (handle < 0) {
        return false;
    }
    g_nfn->NPAPI_SetTypeHandler_TypeCheck(handle, XAPFile::check_tex);
    g_nfn->NPAPI_SetTypeHandler_LoadRGBA(handle, XAPFile::load_tex);

    return true;
}

void __cdecl NPAPI_ShutdownLocal() {
}

#include "pch.h"
#include "XAPFile.h"

bool NPAPI_InitLocal(void) {
    int handle = g_nfn->NPAPI_Register(const_cast<char *>("NieR Replicant Model XAP"), const_cast<char *>("*.xap;*"));
    if (handle < 0) {
        return false;
    }
    g_nfn->NPAPI_SetTypeHandler_TypeCheck(handle, XAPFile::check);
    g_nfn->NPAPI_SetTypeHandler_LoadModel(handle, XAPFile::load_model);
    
    
    handle = g_nfn->NPAPI_Register(const_cast<char *>("NieR Replicant Texture XAP"), const_cast<char *>("*.xap;*"));
    if (handle < 0) {
        return false;
    }
    g_nfn->NPAPI_SetTypeHandler_TypeCheck(handle, XAPFile::check);
    g_nfn->NPAPI_SetTypeHandler_LoadRGBA(handle, XAPFile::load_tex);
}

void NPAPI_ShutdownLocal() {
    return;
}

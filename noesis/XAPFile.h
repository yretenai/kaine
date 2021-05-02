#pragma once

#include "pch.h"

class XAPFile {
public:
    static bool check(BYTE *buffer, int buffer_length, noeRAPI_t *rapi);
    static noesisModel_t *load_model(BYTE *buffer, int buffer_length, int &num_mdl, noeRAPI_t *rapi);
    static bool load_tex(BYTE *buffer, int buffer_length, CArrayList<noesisTex_t *> &noe_tex, noeRAPI_t *rapi);
    static inline bool load_tex(std::filesystem::path &xap_path, CArrayList<noesisTex_t *> &noe_tex, const noeRAPI_t *rapi);
};

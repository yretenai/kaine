//
// Created by Lilith on 2021-05-12.
//

#pragma once

#include "support/pluginshare.h"

#include <filesystem>

namespace fmt_replicant {
    class XAPFile {
    public:
        static bool check_model(BYTE *buffer, int buffer_length, noeRAPI_t *rapi);
        static noesisModel_t *load_model(BYTE *buffer, int buffer_length, int &num_mdl, noeRAPI_t *rapi);
        static bool check_tex(BYTE *buffer, int buffer_length, noeRAPI_t *rapi);
        static bool load_tex(BYTE *buffer, int buffer_length, CArrayList<noesisTex_t *> &noe_tex, noeRAPI_t *rapi);
    };
}

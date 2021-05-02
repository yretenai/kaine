#include "pch.h"
#include "XAPFile.h"

bool XAPFile::check(BYTE *buffer, int buffer_length, noeRAPI_t *rapi) {
    return false;
}

noesisModel_t* XAPFile::load_model(BYTE *buffer, int buffer_length, int &num_mdl, noeRAPI_t *rapi) {
    return false;
}

bool XAPFile::load_tex(BYTE *buffer, int buffer_length, CArrayList<noesisTex_t *> &noe_tex, noeRAPI_t *rapi) {
    return false;
}

inline bool XAPFile::load_tex(std::filesystem::path &xap_path, CArrayList<noesisTex_t *> &noe_tex, const noeRAPI_t *rapi) {
    return false;
}

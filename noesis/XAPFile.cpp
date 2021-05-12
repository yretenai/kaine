//
// Created by Lilith on 2021-05-12.
//

#include "XAPFile.hpp"

#include <kaine/bxon_types.hpp>
#include <kaine/kaine.hpp>

namespace fmt_replicant {
    bool XAPFile::check_model(BYTE *buffer, int buffer_length, noeRAPI_t *rapi) {
        return false;
    }

    noesisModel_t *XAPFile::load_model(BYTE *buffer, int buffer_length, int &num_mdl, noeRAPI_t *rapi) {
        return nullptr;
    }

    bool XAPFile::check_tex(BYTE *buffer, int buffer_length, noeRAPI_t *rapi) {
        auto array = dragon::Array<uint8_t>(buffer, buffer_length, true);
        auto pack = kaine::pack(array);
        for (const auto &file : pack.files) {
            auto file_name = pack.file_names[file.id];
            if (std::filesystem::path(file_name).extension() != ".rtex") {
                continue;
            }

            auto bxon = kaine::bxon(*pack.file_data[file.id]);

            if (bxon.name == kaine::bxon_types::tp_gx_tex_head::bxon_name) {
                return true;
            }
        }
        return false;
    }

    bool XAPFile::load_tex(BYTE *buffer, int buffer_length, CArrayList<noesisTex_t *> &noe_tex, noeRAPI_t *rapi) {
        auto array = dragon::Array<uint8_t>(buffer, buffer_length, true);
        auto pack = kaine::pack(array);
        uintptr_t offset = 0;
        for (const auto &file : pack.files) {
            auto file_name = pack.file_names[file.id];
            if (std::filesystem::path(file_name).extension() != ".rtex") {
                continue;
            }

            auto bxon = kaine::bxon(*pack.file_data[file.id]);

            if (bxon.name == kaine::bxon_types::tp_gx_tex_head::bxon_name) {
                // rly lazy lol
                auto tex_head = bxon.get_instance<kaine::bxon_types::tp_gx_tex_head>();
                auto dds = tex_head->generate_dds(pack.resource, &offset);
                auto tex = rapi->Noesis_LoadTexByHandler(dds->data(), dds->size(), const_cast<char *>(".dds"));
                noe_tex.Append(tex);
            }
        }

        return true;
    }
}// namespace fmt_replicant

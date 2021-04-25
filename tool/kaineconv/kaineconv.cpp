//
// Created by Lilith on 2021-04-25.
//

#include <kaine/bxon_types.hpp>
#include <kaine/kaine.hpp>
#include <string>

typedef std::shared_ptr<dragon::Array<uint8_t>> (*convert_bxon)(kaine::bxon &bxon, std::shared_ptr<dragon::Array<uint8_t>> &resources, uintptr_t *offset);

std::shared_ptr<dragon::Array<uint8_t>> convert_gxtex(kaine::bxon &bxon, std::shared_ptr<dragon::Array<uint8_t>> &resources, uintptr_t *offset) {
    auto tex_head = bxon.get_instance<kaine::bxon_types::tp_gx_tex_head>();
    return tex_head->generate_dds(resources, offset);
}

typedef struct KAINE_CONV_HANDLER {
    const char *name;
    const char *extension;
    convert_bxon convert;
} KaineConverterInfo;

const KaineConverterInfo Handlers[] = {
        {kaine::bxon_types::tp_gx_tex_head::bxon_name, ".dds", convert_gxtex},
        {nullptr, nullptr, nullptr}};

std::shared_ptr<dragon::Array<uint8_t>> convert(kaine::bxon &bxon, std::shared_ptr<dragon::Array<uint8_t>> &resources, uintptr_t *offset, std::filesystem::path &path) {
    auto i = 0;
    while (true) {
        if (Handlers[i].convert == nullptr) {
            break;
        }

        if (Handlers[i].name == bxon.name) {
            auto result = Handlers[i].convert(bxon, resources, offset);
            if (result == nullptr) {
                continue;
            }
            if (Handlers[i].extension != nullptr) {
                path = path.replace_extension(Handlers[i].extension);
            }
            return result;
        }
    }
    std::cerr << "cannot convert type " << bxon.name << "!" << std::endl;
    return nullptr;
}

int main(int argc, char **argv) {
    for (auto i = 1; i < argc; ++i) {
        auto path = std::filesystem::path(argv[i]);
        if (!std::filesystem::exists(path)) {
            std::cerr << argv[i] << " does not exist" << std::endl;
            continue;
        }
        try {
            auto pack_data = dragon::read_file(path);
            auto pack = kaine::pack(pack_data);
            path = path.replace_extension();
            uintptr_t offset = 0;
            for (const auto &file : pack.files) {
                auto file_name = pack.file_names[file.id];
                auto file_path = path / file_name;
                try {
                    auto bxon = kaine::bxon(*pack.file_data[file.id]);
                    std::cout << file_name << std::endl;
                    auto converted_data = convert(bxon, pack.resource, &offset, file_path);
                    if (converted_data == nullptr) {
                        continue;
                    }
                    auto file_dir = file_path.parent_path();
                    if (!std::filesystem::exists(file_dir)) {
                        std::filesystem::create_directories(file_dir);
                    }
                    dragon::write_file(file_path, *converted_data);
                    offset = dragon::Align<uintptr_t>(offset, 0x40);
                } catch (std::exception &exception) {
                    std::cerr << "can't convert " << file_name << " - error: " << exception.what() << std::endl;
                }
            }
        } catch (std::exception &exception) {
            std::cerr << "can't convert " << argv[i] << " - error: " << exception.what() << std::endl;
        }
    }
}
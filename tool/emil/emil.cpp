//
// Created by Lilith on 2021-04-24.
//

#include <kaine/bxon/types/tp_archive_file_param.hpp>
#include <kaine/kaine.hpp>

void process_dir(const std::filesystem::path &data_dir, const std::filesystem::path &target_dir) {
    auto info_path = data_dir / "info.arc";
    if (!std::filesystem::exists(info_path)) return;

    std::shared_ptr<std::istream> file = std::make_shared<std::ifstream>(info_path, std::ios::binary | std::ios::in);
    auto info_arc = kaine::arc(file, true);
    auto info_data = info_arc.get_file(0, 0);
    auto info_bxon = kaine::bxon(*info_data);
    auto info = info_bxon.get_instance<kaine::bxon_types::tp_archive_file_param>();

    std::vector<kaine::arc> archives;
    archives.reserve(info->archive_count);
    for (auto i = 0; i < info->archive_count; ++i) {
        std::cout << "opening archive " << info->archive_names[i] << std::endl;
        archives.emplace_back(info->open_archive(i, data_dir));
    }

    for (auto &param : info->files) {
        auto dest = target_dir / info->file_names[param.hash];
        auto archive = archives[param.index];
        auto data = info->read_file(archive, param);
        if (data == nullptr) {
            continue;
        }

        auto xap = kaine::pack(*data);

        for (const auto &xap_header : xap.headers) {
            auto header_dest = dest / xap.header_names[xap_header.id];
            auto dest_path = header_dest.parent_path();

            if (!std::filesystem::exists(dest_path)) {
                std::filesystem::create_directories(dest_path);
            }

            dragon::write_file(header_dest, *xap.header_data[xap_header.id]);
        }

        for (const auto &xap_file : xap.files) {
            auto file_dest = dest / xap.file_names[xap_file.id];
            auto dest_path = file_dest.parent_path();

            if (!std::filesystem::exists(dest_path)) {
                std::filesystem::create_directories(dest_path);
            }

            std::cout << info->file_names[param.hash] << "/" << xap.file_names[xap_file.id] << std::endl;

            dragon::write_file(file_dest, *xap.file_data[xap_file.id]);
        }

        if(xap.resource != nullptr) {
            auto file_dest = dest / xap.header_names[xap.headers[0].id];
            file_dest.replace_extension(".xap.asset");
            auto dest_path = file_dest.parent_path();

            if (!std::filesystem::exists(dest_path)) {
                std::filesystem::create_directories(dest_path);
            }

            dragon::write_file(file_dest, *xap.resource);
        }
    }
}

int main(int argv, char **argc) {
    if (argv < 3) {
        std::cerr << "usage: emil install_dir output_dir";
        return -1;
    }
    auto install_dir = std::filesystem::path(argc[1]);
    auto target_dir = std::filesystem::path(argc[2]);
    process_dir(install_dir / "data", target_dir);
    for (const auto &dlc_dir : std::filesystem::directory_iterator(install_dir / "dlc")) {
        if (!std::filesystem::is_directory(dlc_dir)) continue;
        process_dir(dlc_dir, target_dir);
    }
    return 0;
}

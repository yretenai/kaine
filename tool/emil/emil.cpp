//
// Created by Lilith on 2021-04-24.
//

// emil extracts xap files from arc files.

#include <ProgramOptions.hxx>
#include <kaine/bxon_types/tp_archive_file_param.hpp>
#include <kaine/kaine.hpp>

void process_dir(const std::filesystem::path &data_dir, const std::filesystem::path &target_dir, bool unpack_xap, bool keep_xap, bool xap_ext) {
    auto info_path = data_dir / "info.arc";
    if (!std::filesystem::exists(info_path)) return;

    std::shared_ptr<std::istream> file = std::make_shared<std::ifstream>(info_path, std::ios::binary | std::ios::in);
    auto info_arc = kaine::arc(file, true);
    auto info_data = info_arc.get_file(0, 0);
    auto info_name = data_dir.filename().string() + "_info.bxon";
    auto info_bxon = kaine::bxon(*info_data);
    auto info = info_bxon.get_instance<kaine::bxon_types::tp_archive_file_param>();

    std::cout << "saving " << info_name << std::endl;
    if (!std::filesystem::exists(target_dir)) {
        std::filesystem::create_directories(target_dir);
    }
    dragon::write_file(target_dir / info_name, *info_data);

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

        if (!unpack_xap || keep_xap) {
            std::cout << "saving " << info->file_names[param.hash] << std::endl;
            auto dest_path = dest.parent_path();
            if (!std::filesystem::exists(dest_path)) {
                std::filesystem::create_directories(dest_path);
            }

            auto dest_xap = dest;
            if (xap_ext) {
                dest_xap = dest.string() + ".xap";
            } else {
                dest = dest.string() + "_";
            }
            dragon::write_file(dest_xap, *data);
        }

        if (unpack_xap) {
            std::cout << "processing " << info->file_names[param.hash] << std::endl;

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

            if (xap.resource != nullptr) {
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
}

int main(int argc, char **argv) {
    std::cout << kaine::get_version_str() << std::endl;
    std::cout << "Emil version 1.0.3" << std::endl;

    if (kaine::get_version() != KAINE_VERSION) {
        std::cout << "warn: Kaine version is " << kaine::get_version() << " expected version " << KAINE_VERSION << " (" << KAINE_VERSION_S << ")! behavior is undefined!" << std::endl;
    }

    po::parser parser;

    auto &target_dir = parser["output"]
                               .abbreviation('o')
                               .description("Output folder, if unset defaults to install_dir/build_assets/rom/pc")
                               .type(po::string);

    auto &install_dir = parser["install"]
                                .abbreviation('i')
                                .description("Game folder")
                                .type(po::string)
                                .no_fallback();

    auto &unpack_xap = parser["unpack"]
                               .abbreviation('x')
                               .description("Unpack XAP files");

    auto &keep_xap = parser["keep"]
                             .abbreviation('X')
                             .description("Keep unpacked XAP files");

    auto &xap_extension = parser["ext"]
                                  .abbreviation('e')
                                  .description("Use AP Extension");

    auto &help = parser["help"]
                         .abbreviation('?')
                         .description("print this help screen")
                         .callback([&] { std::cout << parser << '\n'; });

    if (!parser(argc, argv)) {
        std::cerr << "errors occurred; aborting" << std::endl;
        return -1;
    }

    if (help.was_set())
        return 0;

    if (!install_dir.was_set())
        return 0;

    auto install_dir_actual = std::filesystem::path(install_dir.get().string);
    auto target_dir_actual = install_dir_actual.parent_path() / "build_assets" / "rom" / "pc";
    if (target_dir.was_set()) {
        target_dir_actual = std::filesystem::path(target_dir.get().string);
    }
    process_dir(install_dir_actual / "data", target_dir_actual, unpack_xap.was_set(), keep_xap.was_set(), xap_extension.was_set());
    for (const auto &dlc_dir : std::filesystem::directory_iterator(install_dir_actual / "dlc")) {
        if (!std::filesystem::is_directory(dlc_dir)) continue;
        process_dir(dlc_dir, target_dir_actual, unpack_xap.was_set(), keep_xap.was_set(), xap_extension.was_set());
    }

    return 0;
}

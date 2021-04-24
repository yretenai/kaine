//
// Created by Lilith on 2021-04-24.
//

#pragma once

#include <kaine/arc/arc.hpp>
#include <kaine/bxon/types/abstract_bxon.hpp>
#include <kaine/kaine_export.h>

#include <standard_dragon/dragon.hpp>

#include <cstdint>
#include <map>
#include <string>

namespace kaine::bxon_types {
    class KAINE_EXPORT tp_archive_file_param : public abstract_bxon {
    private:
        static constexpr uintptr_t EXPECTED_DATA_SIZE = 0x10;
        static constexpr uint64_t FILE_NAME_OFFSET = 0x4;
        static constexpr uint64_t ARC_NAME_OFFSET = 0;
        static constexpr uint64_t FILE_PARAM_OFFSET = 0xC;
        static constexpr uint64_t ARC_PARAM_OFFSET = 0x4;

    public:
        explicit tp_archive_file_param(std::shared_ptr<dragon::Array<uint8_t>> &buffer);
        ~tp_archive_file_param() override = default;

#pragma pack(push, 4)
        typedef struct ARCHIVE_FILE_PARAM {
            uint32_t hash;
            uint32_t rel_offset_name;
            uint32_t archive_offset;
            uint32_t csize;
            uint32_t size;
            uint32_t buffer_size;
            uint8_t index;
            bool is_compressed;
        } ArchiveFileParam;
#pragma pack(pop)
        DRAGON_ASSERT(sizeof(ARCHIVE_FILE_PARAM) == 0x1C, "tpArchiveFileParam has an invalid size");

#pragma pack(push, 4)
        typedef struct ARCHIVE_ARC_PARAM {
            uint32_t rel_offset_name;
            uint32_t unknown;
            bool is_streamed;
        } ArchiveArcParam;
#pragma pack(pop)
        DRAGON_ASSERT(sizeof(ARCHIVE_FILE_PARAM) == 0x1C, "tpArchiveFileParam has an invalid size");

#pragma pack(push, 4)
        struct {
            uint32_t archive_count = 0;
            uint32_t rel_offset_archive_list = 0;
            uint32_t file_count = 0;
            uint32_t rel_offset_file_list = 0;
        };
#pragma pack(pop)

        std::vector<std::string> archive_names;
        dragon::Array<ArchiveArcParam> archives;
        std::map<uint32_t, std::string> file_names;
        dragon::Array<ArchiveFileParam> files;

        kaine::arc open_archive(int32_t index, const std::filesystem::path &data_dir);
        std::shared_ptr<dragon::Array<uint8_t>> read_file(kaine::arc &archive, uint32_t hash);
        static std::shared_ptr<dragon::Array<uint8_t>> read_file(kaine::arc &archive, ArchiveFileParam &param);

        static std::string get_name() { return "tpArchiveFileParam"; }
    };
} // namespace kaine::bxon_types
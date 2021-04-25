//
// Created by Lilith on 2021-04-25.
//

#pragma once

#include <kaine/kaine_export.h>

#include <map>
#include <memory>
#include <string>

#include <standard_dragon/Array.hpp>
#include <standard_dragon/dragon.hpp>

namespace kaine {
    class KAINE_EXPORT pack {
    private:
        static constexpr uint32_t FOURCC = DRAGON_MAGIC32('P', 'A', 'C', 'K');
        static constexpr uintptr_t EXPECTED_DATA_SIZE = 0x2C;
        static constexpr uint32_t PRELOAD_OFFSET = 0x18;
        static constexpr uint32_t HEADER_OFFSET = 0x20;
        static constexpr uint32_t FILE_OFFSET = 0x28;
        static constexpr uint32_t PRELOAD_NAME_OFFSET = 0x4;
        static constexpr uint32_t FILE_NAME_OFFSET = 0x4;
        static constexpr uint32_t FILE_DATA_OFFSET = 0xC;
        static constexpr uint32_t FILE_STR_OFFSET = 0x10;// unused

    public:
        explicit pack(dragon::Array<uint8_t> &buffer);
        ~pack() = default;

#pragma pack(push, 4)
        typedef struct XAP_DEPENDENCY {
            uint32_t id;
            uint32_t rel_offset_name;
            uint32_t unknown;
        } XapDependency;
        DRAGON_ASSERT(sizeof(XAP_DEPENDENCY) == 0xC, "XAP_DEPENDENCY has an invalid size");

        typedef struct XAP_FILE {
            uint32_t id;
            uint32_t rel_offset_name;
            uint32_t size;
            uint32_t rel_offset_data;
            uint32_t rel_offset_str;
        } XapFile;
        DRAGON_ASSERT(sizeof(XAP_FILE) == 0x14, "XAP_FILE has an invalid size");

        struct {
            uint32_t magic = FOURCC;
            uint32_t version = 0;
            uint32_t total_size = 0;
            uint32_t serialized_size = 0;
            uint32_t resource_size = 0;
            uint32_t preload_count = 0;
            uint32_t rel_offset_preload = 0;
            uint32_t xap_header_count = 0;
            uint32_t rel_offset_xap_header = 0;
            uint32_t file_count = 0;
            uint32_t rel_offset_file = 0;
        };
#pragma pack(pop)

        dragon::Array<XapDependency> dependencies;
        std::map<uint32_t, std::string> dependency_names;

        dragon::Array<XapFile> headers;
        std::map<uint32_t, std::string> header_names;
        std::map<uint32_t, std::shared_ptr<dragon::Array<uint8_t>>> header_data;

        dragon::Array<XapFile> files;
        std::map<uint32_t, std::string> file_names;
        std::map<uint32_t, std::shared_ptr<dragon::Array<uint8_t>>> file_data;

        std::shared_ptr<dragon::Array<uint8_t>> resource;
    };
}// namespace kaine

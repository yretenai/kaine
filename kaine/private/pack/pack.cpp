//
// Created by Lilith on 2021-04-25.
//

#include <kaine/pack/pack.hpp>

#include <cassert>

#include <standard_dragon/exception/invalid_data.hpp>
#include <standard_dragon/exception/not_implemented.hpp>

kaine::pack::pack(dragon::Array<uint8_t> &buffer) {
    auto data_start = reinterpret_cast<uintptr_t>(&magic);
#ifndef NDEBUG
    auto data_end = reinterpret_cast<uintptr_t>(&rel_offset_file) + sizeof(uint32_t);
    assert(data_end - data_start == EXPECTED_DATA_SIZE);
#endif

    if (buffer.cast<uint32_t>(0) != FOURCC || buffer.size() < EXPECTED_DATA_SIZE) {
        throw dragon::exception::invalid_data("Buffer passed to pack() is not a valid PACK buffer.");
    }

    buffer.copy(data_start, 0, EXPECTED_DATA_SIZE);

    if (version != 4) {
        throw dragon::exception::not_implemented("PACK is not version 4");
    }

    auto ptr = buffer.data();
    uint32_t offset;
    if (rel_offset_preload > 0) {
        offset = rel_offset_preload + PRELOAD_OFFSET;
        preload = dragon::Array<XapPreload>(reinterpret_cast<XapPreload *>(ptr + offset), preload_count, true);
        for (auto i = 0; i < preload_count; ++i) {
            auto param = preload[i];
            if (param.rel_offset_name > 0) {
                auto name_offset = offset + sizeof(XapPreload) * i + param.rel_offset_name + PRELOAD_NAME_OFFSET;
                preload_names[param.id] = std::string(reinterpret_cast<const char *>(ptr + name_offset));
            }
        }
    }

    if (rel_offset_xap_header > 0) {
        offset = rel_offset_xap_header + HEADER_OFFSET;
        headers = dragon::Array<XapFile>(reinterpret_cast<XapFile *>(ptr + offset), xap_header_count, true);
        for (auto i = 0; i < xap_header_count; ++i) {
            auto param = headers[i];
            if (param.rel_offset_name > 0) {
                auto name_offset = offset + sizeof(XapFile) * i + param.rel_offset_name + FILE_NAME_OFFSET;
                header_names[param.id] = std::string(reinterpret_cast<const char *>(ptr + name_offset));
            }

            if (param.rel_offset_data > 0) {
                auto data_offset = offset + sizeof(XapFile) * i + param.rel_offset_data + FILE_DATA_OFFSET;
                header_data[param.id] = std::make_shared<dragon::Array<uint8_t>>(ptr + data_offset, param.size, true);
            }
        }
    }

    if (rel_offset_file > 0) {
        offset = rel_offset_file + FILE_OFFSET;
        files = dragon::Array<XapFile>(reinterpret_cast<XapFile *>(ptr + offset), file_count, true);
        for (auto i = 0; i < file_count; ++i) {
            auto param = files[i];
            if (param.rel_offset_name > 0) {
                auto name_offset = offset + sizeof(XapFile) * i + param.rel_offset_name + FILE_NAME_OFFSET;
                file_names[param.id] = std::string(reinterpret_cast<const char *>(ptr + name_offset));
            }

            if (param.rel_offset_data > 0) {
                auto data_offset = offset + sizeof(XapFile) * i + param.rel_offset_data + FILE_DATA_OFFSET;
                file_data[param.id] = std::make_shared<dragon::Array<uint8_t>>(ptr + data_offset, param.size, true);
            }
        }
    }

    if(resource_size > 0) {
        resource = std::make_shared<dragon::Array<uint8_t>>(ptr + serialized_size, resource_size, true);
    }
}

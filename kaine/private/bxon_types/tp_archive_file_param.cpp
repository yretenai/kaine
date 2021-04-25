//
// Created by Lilith on 2021-04-24.
//

#include <kaine/bxon_types/tp_archive_file_param.hpp>

#include <cassert>
#include <iostream>

#include <standard_dragon/exception/invalid_data.hpp>

kaine::bxon_types::tp_archive_file_param::tp_archive_file_param(std::shared_ptr<dragon::Array<uint8_t>> &buffer) {
    auto data_start = reinterpret_cast<uintptr_t>(&archive_count);
#ifndef NDEBUG
    auto data_end = reinterpret_cast<uintptr_t>(&rel_offset_file_list) + sizeof(uint32_t);
    assert(data_end - data_start == EXPECTED_DATA_SIZE);
#endif

    if (buffer->size() < EXPECTED_DATA_SIZE) { throw dragon::exception::invalid_data("Buffer passed to tp_archive_file_param is not a valid tp_archive_file_param buffer."); }

    buffer->copy(data_start, 0, EXPECTED_DATA_SIZE);

    uint8_t *ptr = buffer->data();
    uint32_t offset;

    if (rel_offset_archive_list > 0) {
        offset = rel_offset_archive_list + ARC_PARAM_OFFSET;
        archives = dragon::Array<ArchiveArcParam>(reinterpret_cast<ArchiveArcParam *>(ptr + offset), archive_count,
                                                  true);
        archive_names.reserve(archive_count);
        for (auto i = 0; i < archive_count; ++i) {
            auto param = archives[i];
            if (param.rel_offset_name > 0) {
                auto name_offset = offset + sizeof(ArchiveArcParam) * i + param.rel_offset_name + ARC_NAME_OFFSET;
                archive_names.emplace_back(std::string(reinterpret_cast<const char *>(ptr + name_offset)));
            }
        }
    }

    if (rel_offset_file_list > 0) {
        offset = rel_offset_file_list + FILE_PARAM_OFFSET;
        files = dragon::Array<ArchiveFileParam>(reinterpret_cast<ArchiveFileParam *>(ptr + offset), file_count, true);
        for (auto i = 0; i < file_count; ++i) {
            auto file = files[i];
            if (file.rel_offset_name > 0) {
                auto name_offset = offset + sizeof(ArchiveFileParam) * i + file.rel_offset_name + FILE_NAME_OFFSET;
                file_names[file.hash] = std::string(reinterpret_cast<const char *>(ptr + name_offset));
            }
        }
    }
}

kaine::arc kaine::bxon_types::tp_archive_file_param::open_archive(int32_t index, const std::filesystem::path &data_dir) {
    if (index >= archive_count) {
        throw dragon::exception::invalid_data();
    }
    auto data_file = data_dir / archive_names[index];
    std::shared_ptr<std::istream> file = std::make_shared<std::ifstream>(data_file, std::ios::binary | std::ios::in);
    return kaine::arc(file, !archives[index].is_streamed);
}

std::shared_ptr<dragon::Array<uint8_t>> kaine::bxon_types::tp_archive_file_param::read_file(kaine::arc &archive, uint32_t hash) {
    for (auto file : files) {
        if (file.hash == hash) { return read_file(archive, file); }
    }

    return nullptr;
}

std::shared_ptr<dragon::Array<uint8_t>> kaine::bxon_types::tp_archive_file_param::read_file(kaine::arc &archive, kaine::bxon_types::tp_archive_file_param::ArchiveFileParam &param) {
    auto size = param.buffer_size + param.size;
    auto offset = static_cast<uint64_t>(param.archive_offset) << 4;
    if (param.is_compressed) { return archive.decompress_file(offset, param.csize, size); }

    return archive.get_file(offset, size);
}

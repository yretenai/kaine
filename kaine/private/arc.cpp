//
// Created by Lilith on 2021-04-24.
//

#include <kaine/arc.hpp>
#include <zstd.h>

#include <iostream>

kaine::arc::arc(std::shared_ptr<std::istream> &stream, bool is_full_compressed) {
    data = stream;
    stream->seekg(0, std::ios::end);
    length = static_cast<uint64_t>(stream->tellg());

    if (is_full_compressed) {
        decompressed = decompress_file(0, length, 0);
        data = nullptr;
        length = decompressed->size();
    }
}

std::shared_ptr<dragon::Array<uint8_t>> kaine::arc::get_file(uint64_t offset, uint64_t size) {
    if (decompressed != nullptr) {
        if (size == 0) {
            size = decompressed->size() - offset;
        }
        return std::make_shared<dragon::Array<uint8_t>>(decompressed->data() + offset, static_cast<size_t>(size), true);
    } else {
        data->seekg(static_cast<int64_t>(offset), std::ios::beg);
        auto buffer = std::make_shared<dragon::Array<uint8_t>>(static_cast<size_t>(size), nullptr);
        data->read(reinterpret_cast<char *>(buffer->data()), static_cast<std::streamsize>(size));
        return buffer;
    }
}

std::shared_ptr<dragon::Array<uint8_t>> kaine::arc::decompress_file(uint64_t offset, uint64_t csize, uint64_t size) {
    auto cbuffer = get_file(offset, csize);
    if (size == 0) {
        size = ZSTD_getFrameContentSize(cbuffer->data(), cbuffer->size());
        if (size == ZSTD_CONTENTSIZE_UNKNOWN || size == ZSTD_CONTENTSIZE_ERROR) {
            return nullptr;
        }
    }
    auto buffer = std::make_shared<dragon::Array<uint8_t>>(static_cast<size_t>(size), nullptr);
    auto read = ZSTD_decompress(buffer->data(), size, cbuffer->data(), csize);
    if (ZSTD_isError(read)) {
#if !NDEBUG
        std::cout << " " << ZSTD_getErrorName(read) << std::endl;
#endif
        return nullptr;
    }
    return buffer;
}

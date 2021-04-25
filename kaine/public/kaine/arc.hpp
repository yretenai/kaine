//
// Created by Lilith on 2021-04-24.
//

#pragma once

#include <istream>
#include <kaine/kaine_export.h>
#include <memory>
#include <standard_dragon/Array.hpp>

namespace kaine {
    class KAINE_EXPORT arc {
    private:
        std::shared_ptr<std::istream> data;
        std::shared_ptr<dragon::Array<uint8_t>> decompressed = nullptr;

    public:
        explicit arc(std::shared_ptr<std::istream> &stream, bool is_full_compressed);

        uint64_t length;

        std::shared_ptr<dragon::Array<uint8_t>> get_file(uint64_t offset, uint64_t size);
        std::shared_ptr<dragon::Array<uint8_t>> decompress_file(uint64_t offset, uint64_t csize, uint64_t size);
    };
}// namespace kaine

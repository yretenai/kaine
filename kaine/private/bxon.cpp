//
// Created by Lilith on 2021-04-24.
//

#include <kaine/bxon.hpp>

#include <cassert>
#include <standard_dragon/exception/not_implemented.hpp>

kaine::bxon::bxon(dragon::Array<uint8_t> &buffer) {
    auto data_start = reinterpret_cast<uintptr_t>(&magic);
#ifndef NDEBUG
    auto data_end = reinterpret_cast<uintptr_t>(&rel_offset_data) + sizeof(uint32_t);
    assert(data_end - data_start == EXPECTED_DATA_SIZE);
#endif

    if (buffer.cast<uint32_t>(0) != FOURCC || buffer.size() < EXPECTED_DATA_SIZE) {
        throw dragon::exception::invalid_data("Buffer passed to bxon() is not a valid BXON buffer.");
    }

    buffer.copy(data_start, 0, EXPECTED_DATA_SIZE);

    if (version != 3) {
        throw dragon::exception::not_implemented("BXON is not version 3");
    }

    if (rel_offset_typename > 0) {
        name = std::string(reinterpret_cast<const char *const>(buffer.data() + rel_offset_typename + TYPENAME_OFFSET));
    }
    if (rel_offset_data > 0) {
        data = std::make_shared<dragon::Array<uint8_t>>(buffer.data() + rel_offset_data + DATA_OFFSET, buffer.size() - rel_offset_data - DATA_OFFSET, true);
    }
}

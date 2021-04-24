//
// Created by Lilith on 2021-04-24.
//

#pragma once

#include <kaine/bxon/types/abstract_bxon.hpp>
#include <kaine/kaine.hpp>

#include <standard_dragon/dragon.hpp>
#include <standard_dragon/exception/invalid_data.hpp>

namespace kaine {
    class KAINE_EXPORT bxon {
    private:
        static constexpr uint32_t FOURCC = DRAGON_MAGIC32('B', 'X', 'O', 'N');
        static constexpr uintptr_t EXPECTED_DATA_SIZE = 0x10;
        static constexpr uint32_t TYPENAME_OFFSET = 0x8 + 0x4;
        static constexpr uint32_t DATA_OFFSET = 0xC + 0x4;

    public:
        explicit bxon(dragon::Array<uint8_t> &buffer);
        ~bxon() = default;

        std::shared_ptr<dragon::Array<uint8_t>> data;

#pragma pack(push, 4)
        struct {
            uint32_t version = 0;
            uint32_t project_id = 0;
            uint32_t rel_offset_typename = 0;
            uint32_t rel_offset_data = 0;
        };
#pragma pack(pop)

        std::string name;

        template<class T>
        typename std::enable_if<std::is_base_of<kaine::bxon_types::abstract_bxon, T>::value, std::shared_ptr<T>>::type
        get_instance() {
            if (T::get_name() != name) {
                throw dragon::exception::invalid_data("expected type to be " + name + " instead got " + T::get_name());
            }
            return std::make_shared<T>(data);
        }
    };
} // namespace kaine
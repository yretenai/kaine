//
// Created by Lilith on 2021-04-25.
//

#pragma once

#include <kaine/bxon_types/abstract_bxon.hpp>
#include <kaine/kaine_export.h>

#include <standard_dragon/Array.hpp>
#include <standard_dragon/dragon.hpp>

#include <cstdint>

namespace kaine::bxon_types {
    class KAINE_EXPORT tp_gx_tex_head : public abstract_bxon {
    private:
        static constexpr uintptr_t EXPECTED_DATA_SIZE = 0x24;
        static constexpr uintptr_t MIP_SURFACE_OFFSET = 0x20;

    public:
        explicit tp_gx_tex_head(std::shared_ptr<dragon::Array<uint8_t>> &buffer);
        ~tp_gx_tex_head() override = default;

#pragma pack(push, 4)
        typedef struct TEX_HEAD_MIP_SURFACE {
            uint32_t offset;
            uint32_t unknown_1;
            uint32_t unknown_2;
            uint32_t unknown_3;
            uint32_t size;
            uint32_t unknown_5;
            uint32_t width;
            uint32_t height;
            uint32_t unknown_8;
            uint32_t unknown_9;
        } TexHeadMip;
        DRAGON_ASSERT(sizeof(TEX_HEAD_MIP_SURFACE) == 0x28, "ArchiveArcParam has an invalid size");

        enum class XonSurfaceFormat : uint32_t {
            UNKNOWN = 0,
            R8G8B8A8_UNORM = 0x00010800,
            R8G8B8A8_UNORM_SRGB = 0x00010B00,
            BC1_UNORM = 0x00010F00,
            BC1_UNORM_SRGB = 0x00011000,
            BC2_UNORM = 0x00011100,
            BC2_UNORM_SRGB = 0x00011200,
            BC3_UNORM = 0x00011300,
            BC3_UNORM_SRGB = 0x00011400,
            BC4_UNORM = 0x00011500,
            BC5_UNORM = 0x00011600,
            R32G32B32A32_FLOAT = 0x00030000,
        };

        struct {
            uint32_t width = 0;
            uint32_t height = 0;
            uint32_t surfaces = 0;
            uint32_t unknown_1 = 0;
            uint32_t total_image_size = 0;
            uint32_t unknown_2 = 0;
            XonSurfaceFormat format = XonSurfaceFormat::UNKNOWN;
            uint32_t mip_surface_count = 0;
            uint32_t rel_offset_mip_surface = 0;
        };
#pragma pack(pop)

        dragon::Array<TexHeadMip> mip_surfaces;

        std::shared_ptr<dragon::Array<uint8_t>> generate_dds(std::shared_ptr<dragon::Array<uint8_t>> &resource, uintptr_t *offset) const;

        static constexpr const char *bxon_name = "tpGxTexHead";
    };
}// namespace kaine::bxon_types

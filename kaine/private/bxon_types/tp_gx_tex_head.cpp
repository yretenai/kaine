//
// Created by Lilith on 2021-04-25.
//

#include <kaine/bxon_types/tp_gx_tex_head.hpp>

#include <cassert>

#include <standard_dragon/dds_support.hpp>
#include <standard_dragon/exception/invalid_data.hpp>

kaine::bxon_types::tp_gx_tex_head::tp_gx_tex_head(std::shared_ptr<dragon::Array<uint8_t>> &buffer) {
    auto data_start = reinterpret_cast<uintptr_t>(&width);
#ifndef NDEBUG
    auto data_end = reinterpret_cast<uintptr_t>(&rel_offset_mip_surface) + sizeof(uint32_t);
    assert(data_end - data_start == EXPECTED_DATA_SIZE);
#endif

    if (buffer->size() < EXPECTED_DATA_SIZE) { throw dragon::exception::invalid_data("Buffer passed to tp_gx_tex_head is not a valid tp_gx_tex_head buffer."); }

    buffer->copy(data_start, 0, EXPECTED_DATA_SIZE);

    uint8_t *ptr = buffer->data();
    uint32_t offset;

    if (rel_offset_mip_surface > 0) {
        offset = rel_offset_mip_surface + MIP_SURFACE_OFFSET;
        mip_surfaces = dragon::Array<TexHeadMip>(reinterpret_cast<TexHeadMip *>(ptr + offset), mip_surface_count, true);
    }
}

std::shared_ptr<dragon::Array<uint8_t>> kaine::bxon_types::tp_gx_tex_head::generate_dds(std::shared_ptr<dragon::Array<uint8_t>> &resource, uintptr_t *offset) const {
    auto dds = standard_dragon::support::DDS{};
    dds.dx9.linear_size = total_image_size;
    dds.dx9.width = width;
    dds.dx9.height = height;
    dds.dx9.mip_count = mip_surface_count;
    dds.dx10.array_size = surfaces;
    switch (format) {
        default:
        case XonSurfaceFormat::UNKNOWN:
            std::cerr << "warn: unrecognized XonSurfaceFormat 0x" << HEXLOG32 << static_cast<uint32_t>(format) << " defaulting to BC3_UNORM" << std::endl;
            dds.dx10.format = standard_dragon::support::DXGIFormat::BC3_UNORM;
            break;
        case XonSurfaceFormat::R8G8B8A8_UNORM:
            dds.dx10.format = standard_dragon::support::DXGIFormat::R8G8B8A8_UNORM;
            break;
        case XonSurfaceFormat::R8G8B8A8_UNORM_SRGB:
            dds.dx10.format = standard_dragon::support::DXGIFormat::R8G8B8A8_UNORM_SRGB;
            break;
        case XonSurfaceFormat::BC1_UNORM:
            dds.dx10.format = standard_dragon::support::DXGIFormat::BC1_UNORM;
            break;
        case XonSurfaceFormat::BC1_UNORM_SRGB:
            dds.dx10.format = standard_dragon::support::DXGIFormat::BC1_UNORM_SRGB;
            break;
        case XonSurfaceFormat::BC2_UNORM:
            dds.dx10.format = standard_dragon::support::DXGIFormat::BC2_UNORM;
            break;
        case XonSurfaceFormat::BC2_UNORM_SRGB:
            dds.dx10.format = standard_dragon::support::DXGIFormat::BC2_UNORM_SRGB;
            break;
        case XonSurfaceFormat::BC3_UNORM:
            dds.dx10.format = standard_dragon::support::DXGIFormat::BC3_UNORM;
            break;
        case XonSurfaceFormat::BC3_UNORM_SRGB:
            dds.dx10.format = standard_dragon::support::DXGIFormat::BC3_UNORM_SRGB;
            break;
        case XonSurfaceFormat::BC4_UNORM:
            dds.dx10.format = standard_dragon::support::DXGIFormat::BC4_UNORM;
            break;
        case XonSurfaceFormat::BC5_UNORM:
            dds.dx10.format = standard_dragon::support::DXGIFormat::BC5_UNORM;
            break;
        case XonSurfaceFormat::R32G32B32A32_FLOAT:
            dds.dx10.format = standard_dragon::support::DXGIFormat::R32G32B32A32_FLOAT;
            break;
    }

    auto buffer = std::make_shared<dragon::Array<uint8_t>>(total_image_size + sizeof(standard_dragon::support::DDS), nullptr);
    buffer->paste(reinterpret_cast<uintptr_t>(&dds), 0, sizeof(standard_dragon::support::DDS));
    auto ptr = reinterpret_cast<uintptr_t>(buffer->data() + sizeof(standard_dragon::support::DDS));
    resource->lpcopy(&ptr, offset, total_image_size);
    return buffer;
}

//
// Created by Lilith on 2021-04-24.
//

#pragma once

#define KAINE_VERSION 1
#define KAINE_VERSION_M 0
#define KAINE_VERSION_m 0
#define KAINE_VERSION_i 1
#define KAINE_VERSION_S "Kaine version 0.0.1"

#include <kaine/kaine_export.h>

#include <kaine/arc/arc.hpp>
#include <kaine/bxon/bxon.hpp>
#include <kaine/pack/pack.hpp>

#include <string>

namespace kaine {
    KAINE_EXPORT std::string get_version_str();
    KAINE_EXPORT int get_version();
} // namespace kaine

//
// Created by Lilith on 2021-04-24.
//

#pragma once

#ifdef _WIN32
#ifdef _WIN32_CLANG
#include <kaine/kaine_export_clang.a.h>
#else
#include <kaine/kaine_export_win.a.h>
#endif
#else
#include <kaine/kaine_export_linux.a.h>
#endif

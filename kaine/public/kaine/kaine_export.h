//
// Created by Lilith on 2021-04-24.
//

#pragma once

#ifdef _WIN32
#include <kaine/generated/kaine_export_win.a.h>
#else
#include <kaine/generated/kaine_export_linux.a.h>
#endif
#ifdef KAINE_STATIC
#undef KAINE_EXPORT
#define KAINE_EXPORT
#endif

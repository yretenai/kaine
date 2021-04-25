
#ifndef KAINE_EXPORT_H
#define KAINE_EXPORT_H

#ifdef KAINE_STATIC_DEFINE
#  define KAINE_EXPORT
#  define KAINE_NO_EXPORT
#else
#  ifndef KAINE_EXPORT
#    ifdef kaine_EXPORTS
        /* We are building this library */
#      define KAINE_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define KAINE_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef KAINE_NO_EXPORT
#    define KAINE_NO_EXPORT 
#  endif
#endif

#ifndef KAINE_DEPRECATED
#  define KAINE_DEPRECATED __declspec(deprecated)
#endif

#ifndef KAINE_DEPRECATED_EXPORT
#  define KAINE_DEPRECATED_EXPORT KAINE_EXPORT KAINE_DEPRECATED
#endif

#ifndef KAINE_DEPRECATED_NO_EXPORT
#  define KAINE_DEPRECATED_NO_EXPORT KAINE_NO_EXPORT KAINE_DEPRECATED
#endif

#if 1 /* DEFINE_NO_DEPRECATED */
#  ifndef KAINE_NO_DEPRECATED
#    define KAINE_NO_DEPRECATED
#  endif
#endif

#endif /* KAINE_EXPORT_H */

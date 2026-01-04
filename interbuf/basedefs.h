#ifndef _INTERBUF_BASE_BASEDEFS_H_
#define _INTERBUF_BASE_BASEDEFS_H_

#include <peff/base/basedefs.h>

#if defined(_MSC_VER)
	#define INTERBUF_DLLEXPORT __declspec(dllexport)
	#define INTERBUF_DLLIMPORT __declspec(dllimport)
#elif defined(__GNUC__) || defined(__clang__)
	#define INTERBUF_DLLEXPORT __attribute__((__visibility__("default")))
	#define INTERBUF_DLLIMPORT __attribute__((__visibility__("default")))
#endif

#define INTERBUF_FORCEINLINE PEFF_FORCEINLINE

#if defined(_MSC_VER)
	#define INTERBUF_DECL_EXPLICIT_INSTANTIATED_CLASS(apiModifier, name, ...) extern template class apiModifier name<__VA_ARGS__>
	#define INTERBUF_DEF_EXPLICIT_INSTANTIATED_CLASS(apiModifier, name, ...) template class apiModifier name<__VA_ARGS__>
#elif defined(__GNUC__) || defined(__clang__)
	#define INTERBUF_DECL_EXPLICIT_INSTANTIATED_CLASS(apiModifier, name, ...) extern template class apiModifier name<__VA_ARGS__>
	#define INTERBUF_DEF_EXPLICIT_INSTANTIATED_CLASS(apiModifier, name, ...) template class name<__VA_ARGS__>
#else
	#define INTERBUF_DECL_EXPLICIT_INSTANTIATED_CLASS(apiModifier, name, ...)
	#define INTERBUF_DEF_EXPLICIT_INSTANTIATED_CLASS(apiModifier, name, ...)
#endif

#if !INTERBUF_STATIC_LINK
	#if IS_INTERBUF_BASE_BUILDING
		#define INTERBUF_API INTERBUF_DLLEXPORT
	#else
		#define INTERBUF_API INTERBUF_DLLIMPORT
	#endif
#else
	#define INTERBUF_API
#endif

#endif

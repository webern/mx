#pragma once

#ifdef _MSC_VER
    #define noexcept
	#if _MSC_VER >= 1900
		#undef noexcept
	#endif
#endif

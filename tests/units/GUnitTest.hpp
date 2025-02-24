#ifndef AREG_TESTS_UNITS_GUNITTESTS_HPP
/************************************************************************
 * \file        tests/units/GUnitTest.hpp
 * \ingroup     platform dependent files
 * \author      Artak Avetyan
 * \brief       AREG Framework, platform dependent includes
 *
 ************************************************************************/
 /************************************************************************
  * Include files.
  ************************************************************************/
#if !defined(IMP_AREG_DLL)          && \
    !defined(_IMP_AREG_DLL)         && \
    !defined(IMPORT_SHARED_SYMBOLS) && \
    !defined(_IMPORT_SHARED_SYMBOLS)&& \
    !defined(IMP_AREG_LIB)          && \
    !defined(_IMP_AREG_LIB)         && \
    !defined(IMPORT_STATIC_SYMBOLS) && \
    !defined(_IMPORT_STATIC_SYMBOLS)

    #error  "You should import AREG engine either as shared or static library"

#endif // wrong imports

#include "areg/base/GEGlobal.h"
#include "googletest/googletest/include/gtest/gtest.h"

#ifdef _WIN32
    #pragma comment(lib, "gtest.lib")
    #pragma comment(lib, "gtest_main.lib")
    #pragma comment(lib, "areg.lib")
#endif // _WIN32

#endif // !AREG_TESTS_UNITS_GUNITTESTS_HPP

//
// Created by mariusjenin on 22/11/22.
//

#ifndef PHOTONEAR_INCLUDE_STBI_H
#define PHOTONEAR_INCLUDE_STBI_H

// Disable pedantic warnings for this external library.
#ifdef _MSC_VER
// Microsoft Visual C++ Compiler
    #pragma warning (push, 0)
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Restore warning levels.
#ifdef _MSC_VER
// Microsoft Visual C++ Compiler
    #pragma warning (pop)
#endif

#endif //PHOTONEAR_INCLUDE_STBI_H

/* Copyright (C) 2022 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/e-rock-json
 */

/*! \defgroup <strict> Strict
    @{
    \file
    \brief File contains usefull macros for objects
*/

#pragma once

#include "erock/strict/types.h"

#define JINT(Name) erock::int_t Name{ #Name }
#define JREAL(Name) erock::real_t Name{ #Name } 
#define JBOOL(Name) erock::bool_t Name{ #Name }
#define JSTRING(Name) erock::string_t Name{ #Name }

#define JARRAY(Name, INNER_TYPE) erock::array_t<INNER_TYPE> Name{ #Name }
#define JOBJECT(Name, INNER_TYPE) erock::object<INNER_TYPE> Name{ #Name }

/*! @} */
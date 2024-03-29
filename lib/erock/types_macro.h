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

#include "erock/types.h"

#define SJINT(Name) erock::strict::int_t Name{ #Name }
#define SJREAL(Name) erock::strict::real_t Name{ #Name } 
#define SJBOOL(Name) erock::strict::bool_t Name{ #Name }
#define SJSTRING(Name) erock::strict::string_t Name{ #Name }

#define SJARRAY(Name, INNER_TYPE) erock::strict::array_t<INNER_TYPE> Name{ #Name }
#define SJOBJECT(Name, INNER_TYPE) erock::strict_object<INNER_TYPE> Name{ #Name }

#define NJINT(Name) erock::nullable::int_t Name{ #Name }
#define NJREAL(Name) erock::nullable::real_t Name{ #Name } 
#define NJBOOL(Name) erock::nullable::bool_t Name{ #Name }
#define NJSTRING(Name) erock::nullable::string_t Name{ #Name }

#define NJARRAY(Name, INNER_TYPE) erock::nullable::array_t<INNER_TYPE> Name{ #Name }
#define NJOBJECT(Name, INNER_TYPE) erock::nullable_object<INNER_TYPE> Name{ #Name }

/*! @} */
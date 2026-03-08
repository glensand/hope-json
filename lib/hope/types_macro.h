/* Copyright (C) 2022 - 2026 Gleb Bezborodov - All Rights Reserved
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

#include "hope/types.h"

#define SJINT(Name) hope::strict::int_t Name{ #Name }
#define SJREAL(Name) hope::strict::real_t Name{ #Name }
#define SJBOOL(Name) hope::strict::bool_t Name{ #Name }
#define SJSTRING(Name) hope::strict::string_t Name{ #Name }

#define SJARRAY(Name, INNER_TYPE) hope::strict::array_t<INNER_TYPE> Name{ #Name }
#define SJOBJECT(Name, INNER_TYPE) hope::strict_object<INNER_TYPE> Name{ #Name }

#define NJINT(Name) hope::nullable::int_t Name{ #Name }
#define NJREAL(Name) hope::nullable::real_t Name{ #Name }
#define NJBOOL(Name) hope::nullable::bool_t Name{ #Name }
#define NJSTRING(Name) hope::nullable::string_t Name{ #Name }

#define NJARRAY(Name, INNER_TYPE) hope::nullable::array_t<INNER_TYPE> Name{ #Name }
#define NJOBJECT(Name, INNER_TYPE) hope::nullable_object<INNER_TYPE> Name{ #Name }

/*! @} */

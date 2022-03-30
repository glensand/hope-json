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
    \brief File contains definitions of the strict json object
*/

#pragma once

#include <string_view>
#include <type_traits>

#include "hope/components/typemap.h"

namespace erock {

    template<typename... Ts>
    class operation {
    protected:
        using map_t = hope::type_map<Ts...>;

        template<typename TPolicy>
        using get_t = typename map_t::template get_t<TPolicy>;
    };

}

/*! @} */

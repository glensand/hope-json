/* Copyright (C) 2021 - 2022 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/e-rock-json
 */

/*! \defgroup <classic> Classic
    @{
    \file
    \brief File contains definitions of the common-known json object
*/

#pragma once

#include <optional>
#include "erock/base_object.h"

namespace erock::classic {

    template<typename TValue>
    using object = base_object<std::optional<TValue>>;

}

/*! @} */

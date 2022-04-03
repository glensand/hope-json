/* Copyright (C) 2022 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/e-rock-json
 */

/*! \defgroup <core> Core
    @{
    \file
    \brief File contains definitions of core json types

*/

#pragma once

#include "hope/typelist/type_list.h"
#include <cmath>
#include <string>
#include <vector>
#include <type_traits>

namespace erock {

    /**
     * Integer type, 64 bit are used to simplify usage in signed-unsigned context
     */
    using raw_int_t = int64_t;

    /**
     * Floating point type, long double are used so that everyone has enough :)
     */
    using raw_real_t = long double;

    /**
     * Boolean type, should be used as is...
     */
    using raw_bool_t = bool;

    /**
     * std::string (not templated, without any allocators or inner chars customization) 'cause the code has to be 
     * as simple as possible (idology of the library)
     */ 
    using raw_string_t = std::string;

    /**
     */ 
    using registered_raw_types_t = hope::type_list<raw_int_t, raw_real_t, raw_bool_t, raw_string_t>;

    /**
     * Alias to erock array template class (uses standard vector 'cause it is quete simple)
     */ 
    template<typename TValue>
    using raw_array_t = std::vector<TValue>;

    template <template<typename> typename TObject>
    struct types final {

        /**
         * Named integer, this type has to be used to load/store integer from json
         */
        using int_t = TObject<raw_int_t>;

        /**
         * Named floating point value, this type has to be used to load/store floating point value from json
         */
        using real_t = TObject<raw_real_t>;

        /**
         * Named boolean value, this type has to be used to load/store boolean value from json
         */
        using bool_t = TObject<raw_bool_t>;

        /**
         * Named string object, this type has to be used to load/store string value from json 
         */
        using string_t = TObject<raw_string_t>;

        /** The only one templated type, represents array(in terms as json); usage is obvious
         */
        template<typename TValue>
        using array_t = TObject<raw_array_t<TValue>>;

    };

}

/*! @} */

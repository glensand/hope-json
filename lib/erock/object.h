/* Copyright (C) 2021 - 2022 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/e-rock-json
 */

/*! \defgroup <core> Core
    @{
    \file
    \brief File contains definitions of the strict json object
*/

#pragma once

#include <string_view>
#include <optional>

#include "erock/types.h"

namespace erock  {

    /**
     * \brief Named field of the JSON's object, the field is also complete object, 'cause object = object | Term
     * This structure should be used to each member of user's type.
     * 
     *  @param TValue Internal value type (e_int_t, e_bool_t or user structure e.t.c)
     */
    template<typename TValue>
    struct base_object {
        using value_t = TValue;
        /**
         * The name of the object(or attribute in terms of xml)
         */ 
        const std::string_view name;

        /**
         * The value of the object
         */
        TValue value;

        /**
         * Implicit conversion operator
         */
        operator const TValue&() const {
            return value;
        }

        operator TValue&() {
            return value;
        }
        
        template<typename TR>
        base_object& operator=(TR&& v){
            value = std::forward<TR>(v);
            return *this;
        }
    };

    template<typename TValue>
    using strict_object = base_object<TValue>;
    
    template<typename TValue>
    using nullable_object = base_object<std::optional<TValue>>;

    using nullable = types<nullable_object>;
    using strict = types<strict_object>;
    
}

/*! @} */

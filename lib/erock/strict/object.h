/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
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

namespace erock  {

    /**
     * \brief Named field of the JSON's object, the field is also complete object, 'cause object = object | Term
     * This structure should be used to each member of user's type.
     * 
     *  @param TValue Internal value type (e_int_t, e_bool_t or user structure e.t.c)
     */
    template<typename TValue>
    struct object final {
        /**
         * The name of the object(or attribute in terms of xml)
         */ 
        std::string_view name;

        /**
         * The value of the object
         */
        TValue value;

        /**
         * Implicit conversion operator
         */
        operator TValue() const {
            return value;
        }

        object& operator=(const TValue& v){
            value = v;
            return *this;
        }
    };

}

/*! @} */

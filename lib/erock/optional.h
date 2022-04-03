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
    \brief 
*/

#pragma once

#include "erock/types.h"
#include "hope/components/user_defined_types.h"

namespace erock  {

    template<typename TValue>
    struct optional {

        static_assert(
            hope::contains<TValue>(registered_raw_types_t{})
            || hope::is_vector_v<TValue>,
        "");

        ~optional(){
            if constexpr (!is_small){
                delete value;
            }
        }
        operator const TValue&() const {
            return get();
        }

        operator TValue&() {
            return get();
        }

        template<typename TR>
        optional& operator=(TR&& v) {
            get() = std::forward<TR>(v);
            return *this;
        }

        mutable TValue* value{ nullptr };
        mutable bool has{ false };
    private:
        using small_t = hope::type_list<raw_int_t, raw_real_t, raw_bool_t>;
        constexpr static bool is_small = hope::contains<TValue>(small_t{});

        TValue& get() const {
            if constexpr (is_small){
                return reinterpret_cast<TValue&>(value); // small value optimization
            }
            if (!has){
                value = new TValue;
                has = true;
            }
            return *value;
        }

    };
    
}

/*! @} */

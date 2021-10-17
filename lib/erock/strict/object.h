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


namespace erock  {

    /**
     */
    template<typename TStruct>
    class object final {
    public:

        const TStruct& get() const noexcept { return m_object; }
        TStruct& get() noexcept { return m_object; }

    private:

        TStruct m_object;
    };

}

/*! @} */

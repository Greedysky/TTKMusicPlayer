#ifndef TTKFUNCTOR_H
#define TTKFUNCTOR_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2026 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include "ttkmoduleexport.h"

/*! @brief The class of the null type.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKNullType { };


/*! @brief The class of the functor interface.
 * @author Greedysky <greedysky@163.com>
 */
template <typename IT, typename OT = TTKNullType, typename PT = TTKNullType>
class TTK_MODULE_EXPORT TTKFunctor
{
public:
    using InputType  = IT;
    using OutputType = OT;
    using OptionType = PT;

public:
    /*!
     * Object constructor.
     */
    TTKFunctor();
    /*!
     * Object destructor.
     */
    virtual ~TTKFunctor();

public:
    /*!
     * Functor execute in unsafe mode.
     */
    OutputType executeUnsafe(const InputType &input);
    /*!
     * Functor execute in unsafe mode.
     */
    OutputType executeUnsafe(const InputType &input, const OptionType &option);

    /*!
     * Functor execute in safe mode.
     */
    bool execute(const InputType &input, const OptionType &option = OptionType());
    /*!
     * Functor execute in safe mode.
     */
    bool execute(const InputType &input, OutputType &output, const OptionType &option = OptionType());

protected:
    /*!
     * Functor execute main interface.
     */
    virtual bool executeMain(OutputType &output, const InputType &input, const OptionType &option) = 0;

};

template <typename IT, typename OT, typename PT>
TTKFunctor<IT, OT, PT>::TTKFunctor()
{

}

template <typename IT, typename OT, typename PT>
TTKFunctor<IT, OT, PT>::~TTKFunctor()
{

}

template <typename IT, typename OT, typename PT>
typename TTKFunctor<IT, OT, PT>::OutputType TTKFunctor<IT, OT, PT>::executeUnsafe(const InputType &input)
{
    OT output;
    executeMain(output, input, OptionType());
    return output;
}

template <typename IT, typename OT, typename PT>
typename TTKFunctor<IT, OT, PT>::OutputType TTKFunctor<IT, OT, PT>::executeUnsafe(const InputType &input, const OptionType &option)
{
    OT output;
    executeMain(output, input, option);
    return output;
}

template <typename IT, typename OT, typename PT>
bool TTKFunctor<IT, OT, PT>::execute(const InputType &input, const OptionType &option)
{
    OT output;
    return executeMain(output, input, option);
}

template <typename IT, typename OT, typename PT>
bool TTKFunctor<IT, OT, PT>::execute(const InputType &input, OutputType &output, const OptionType &option)
{
    return executeMain(output, input, option);
}

#endif // TTKFUNCTOR_H

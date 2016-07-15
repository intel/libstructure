/*
 * Copyright (c) 2016, Intel Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once

#include "structure/structure_export.h"
#include "structure/ValueImporter.hpp"

#include <initializer_list>
#include <memory>

namespace structure
{

class GenericField;
class GenericFieldValue;

class STRUCTURE_EXPORT ValueInitializer : public ValueImporter
{
public:
    /** Constructs a ValueInitializer from a string literal
     *
     * It should coincide with a Field.
     */
    ValueInitializer(const char *atomicValue);

    /** Constructs a ValueInitializer from a boolean
     */
    ValueInitializer(bool atomicValue);

    /** Constructs a ValueInitializer from a long long literal
     */
    ValueInitializer(long long atomicValue, void *);

    /** Constructs a ValueInitializer from a unsigned long long literal
     */
    ValueInitializer(unsigned long long atomicValue, void *);

    /** Constructs a ValueInitializer from a double literal
     */
    ValueInitializer(double atomicValue, void *);

    /** Constructs a ValueInitializer from a signed integer
     */
    template <typename T>
    ValueInitializer(
        T atomicValue,
        typename std::enable_if<std::is_integral<T>::value and std::is_signed<T>::value, T>::type =
            0)
        : ValueInitializer(static_cast<long long>(atomicValue), nullptr)
    {
    }

    /** Constructs a ValueInitializer from an unsigned integer
     */
    template <typename T>
    ValueInitializer(T atomicValue,
                     typename std::enable_if<
                         std::is_integral<T>::value and std::is_unsigned<T>::value, T>::type = 0)
        : ValueInitializer(static_cast<unsigned long long>(atomicValue), nullptr)
    {
    }

    /** Constructs a ValueInitializer from a floating point
     */
    template <typename T>
    ValueInitializer(T atomicValue,
                     typename std::enable_if<std::is_floating_point<T>::value, T>::type = 0)
        : ValueInitializer(static_cast<double>(atomicValue), nullptr)
    {
    }

    /** Constructs a ValueInitializer from a string
     */
    ValueInitializer(const std::string &atomicValue);

    /** Constructs a ValueInitializer from an previously-defined importer
     *
     * This should coincide with a Block.
     */
    ValueInitializer(ValueImporter &importer);

    /** Constructs a ValueInitializer from a recursive initializer list
     *
     * This should coincide with a Block.
     *
     * Example 1:
     * @code
     * PromptImporter<> importer;
     * Block structure("b1", Block("b2", ...), UInt8("u"), UInt8("v"));
     * structure.with({importer, "1", "2"});
     * @endcode
     *
     * Example 2:
     * @code
     * Block structure("b1", Block("b2", ...), UInt8("u"), UInt8("v"));
     * ValueInitializer value = {{"1", "2"}, "3", "4"};
     * structure.with(value);
     * @endcode
     */
    ValueInitializer(std::initializer_list<ValueInitializer> list);

    std::unique_ptr<GenericFieldValue> import(const GenericField &field,
                                              const std::string &path) override;
    void onEnterBlock(const std::string &block) override;
    void onExitBlock(const std::string &block) override;

private:
    // This class needs to be copiable because instances will be copied from initializer_list to
    // list when they are part of a ListImporter. (Copied and not moved because items of a
    // initializer_list can't be moved). That's why we use a shared_ptr.
    std::shared_ptr<ValueImporter> mImporter;
};
}

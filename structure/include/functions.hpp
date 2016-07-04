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

#include "structure_export.h"

#include "type/Structure.hpp"
#include "type/Block.hpp"
#include "type/GenericField.hpp"
#include "type/Field.hpp"
#include "ValueInitializer.hpp"

#include <string>
#include <list>
#include <memory>
#include <functional>
#include <ostream>

namespace structure
{

// Structure

/** Type of functions visiting a Block */
using BlockFunction = std::function<void(const Block &)>;
/** No-op when visiting a Block */
const BlockFunction defaultBlockFunction = [](const Block &) {};

/** Type of functions visiting a Field */
using FieldFunction = std::function<void(const GenericField &)>;
/** No-op when visiting a Field */
const FieldFunction defaultFieldFunction = [](const GenericField &) {};

/** Lightweight implementation of a StructureVisitor
 *
 * @param[in] structure The structure to be visited
 * @param[in] onEnterBlock Function to be called at the beggining of a Block
 * @param[in] onExitBlock Function to be called at the end of a Block
 * @param[in] onEnterField Function to be called on a Field
 * @param[in] recursive Whether children of blocks should be automatically recursively be browsed
 *                      between onEnterBlock and onExitBlock (if not, the browsing logic should
 *                      probably be handled by onEnterBlock)
 */
STRUCTURE_EXPORT void apply(const Structure &structure, BlockFunction onEnterBlock,
                            BlockFunction onExitBlock, FieldFunction onEnterField, bool recursive);

// Value

/** Type of functions visiting a BlockValue */
using BlockValueFunction = std::function<void(const BlockValue &)>;
/** No-op when visiting a BlockValue */
const BlockValueFunction defaultBlockValueFunction = [](const BlockValue &) {};

/** Type of functions visiting a FieldValue */
using FieldValueFunction = std::function<void(const GenericFieldValue &)>;
/** No-op when visiting a FieldValue */
const FieldValueFunction defaultFieldValueFunction = [](const GenericFieldValue &) {};

/** Lightweight implementation of a ValueVisitor
 *
 * @param[in] structure The value to be visited
 * @param[in] onEnterBlock Function to be called at the beggining of a BlockValue
 * @param[in] onExitBlock Function to be called at the end of a BlockValue
 * @param[in] onEnterField Function to be called on a FieldValue
 * @param[in] recursive Whether children of blocks should be automatically recursively be browsed
 *                      between onEnterBlock and onExitBlock (if not, the browsing logic should
 *                      probably be handled by onEnterBlock)
 */
STRUCTURE_EXPORT void apply(const StructureValue &structure, BlockValueFunction onEnterBlock,
                            BlockValueFunction onExitBlock, FieldValueFunction onEnterField,
                            bool recursive);

// Functions
/** Pretty-print a Structure
 *
 * @param[out] outStream The stream on which to print
 * @param[in] structure The structure to be printed
 */
STRUCTURE_EXPORT void print(std::ostream &outStream, const Structure &structure);
/** See the structure::print(std::ostream&, const Structure&) overload. */
STRUCTURE_EXPORT void print(std::ostream &outStream, const std::unique_ptr<Block> &structure);
/** Pretty-print a StructureValue
 *
 * @param[out] outStream The stream on which to print
 * @param[in] value The value to be printed
 */
STRUCTURE_EXPORT void print(std::ostream &outStream, const StructureValue &value);
/** See the structure::print(std::ostream&, const StructureValue&) overload. */
STRUCTURE_EXPORT void print(std::ostream &outStream, const std::unique_ptr<StructureValue> &value);

/** @defgroup FunctionalEquivalents Pure-function equivalent of methods
 *
 * Instead of calling `obj.func(arg)`, it is sometimes convenient calling `func(obj, arg)`. These
 * functions provide that mean for some methods of Structure, StructureValue & co.
 * @{
 */
/** Equivalent to StructureValue::getValue() */
STRUCTURE_EXPORT std::string getValue(const StructureValue &value);
/** See structure::getValue(const StructureValue&) */
STRUCTURE_EXPORT std::string getValue(const std::unique_ptr<StructureValue> &value);

/** Equivalent to Structure::getChild() */
STRUCTURE_EXPORT const Structure &getChild(const Structure &structure, const std::string &path);
/** See structure::getChild(Structure&, const std::string &) */
STRUCTURE_EXPORT const Structure &getChild(const std::unique_ptr<Block> &structure,
                                           const std::string &path);
/** Equivalent to StructureValue::getChild() */
STRUCTURE_EXPORT StructureValue &getChild(const StructureValue &value, const std::string &path);
/** See structure::getChild(StructureValue&, const std::string &) */
STRUCTURE_EXPORT StructureValue &getChild(const std::unique_ptr<StructureValue> &value,
                                          const std::string &path);

/** Equivalent to Structure::build(ValueImporter &) */
STRUCTURE_EXPORT std::unique_ptr<StructureValue> build(const Structure &structure,
                                                       ValueImporter &importer);

/** Equivalent to Block::with(ValueInitializer) */
STRUCTURE_EXPORT std::unique_ptr<StructureValue> with(const Block &block,
                                                      ValueInitializer initializer);

/** Equivalent to constructing a FieldValue
 *
 * @param fieldType The type with which the value will be constructed
 * @param value The actual (in-memory) value
 */
template <class FieldType>
std::unique_ptr<FieldValue<FieldType>> with(FieldType fieldType, typename FieldType::Storage value)
{
    static_assert(is_structure<FieldType>::value,
                  "The FieldType template type must be a Structure");
    return std::make_unique<FieldValue<FieldType>>(fieldType, value);
}
/** Same as structure::with(FieldType, FieldType::Storage) but constructed with string
 * representation */
template <class FieldType>
std::unique_ptr<FieldValue<FieldType>> with(FieldType fieldType, const std::string &value)
{
    static_assert(is_structure<FieldType>::value,
                  "The FieldType template type must be a Structure");
    return std::make_unique<FieldValue<FieldType>>(fieldType, value);
}
/** @} */
}

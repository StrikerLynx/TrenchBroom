/*
 Copyright (C) 2010-2017 Kristian Duske

 This file is part of TrenchBroom.

 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ConfigParserBase.h"

#include "EL/EvaluationContext.h"
#include "EL/Expression.h"
#include "EL/Value.h"
#include "Exceptions.h"

#include <fmt/format.h>

#include <string>

namespace TrenchBroom::IO
{

ConfigParserBase::ConfigParserBase(const std::string_view str, std::filesystem::path path)
  : m_parser{ELParser::Mode::Strict, str}
  , m_path{std::move(path)}
{
}

ConfigParserBase::~ConfigParserBase() = default;

EL::Expression ConfigParserBase::parseConfigFile()
{
  return m_parser.parse();
}

void expectType(const EL::Value& value, const EL::ValueType type)
{
  if (value.type() != type)
  {
    throw ParserException{
      value.line(),
      value.column(),
      fmt::format(
        "Expected value of type '{}', but got type '{}'",
        EL::typeName(type),
        value.typeName())};
  }
}

void expectStructure(const EL::Value& value, const std::string& structure)
{
  auto parser = ELParser{ELParser::Mode::Strict, structure};
  const auto expected = parser.parse().evaluate(EL::EvaluationContext());
  assert(expected.type() == EL::ValueType::Array);

  const auto mandatory = expected[0];
  assert(mandatory.type() == EL::ValueType::Map);

  const auto optional = expected[1];
  assert(optional.type() == EL::ValueType::Map);

  // Are all mandatory keys present?
  for (const auto& key : mandatory.keys())
  {
    const auto typeName = mandatory[key].stringValue();
    if (typeName != "*")
    {
      const auto type = EL::typeForName(typeName);
      expectMapEntry(value, key, type);
    }
  }
}

void expectMapEntry(
  const EL::Value& value, const std::string& key, const EL::ValueType type)
{
  const auto& map = value.mapValue();
  const auto it = map.find(key);
  if (it == std::end(map))
  {
    throw ParserException{
      value.line(), value.column(), fmt::format("Expected map entry '{}'", key)};
  }
  expectType(it->second, type);
}

} // namespace TrenchBroom::IO

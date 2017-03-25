
/*
 *  lab - a general-purpose C++ toolkit
 *  Copyright (C) 2017, Amos Wenger
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details:
 * https://github.com/itchio/lab/blob/master/LICENSE
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#include <lab/env.h>
#include <lab/io.h>
#include <lab/packet.h>
#include <lab/platform.h>
#include <lab/strings.h>
#include <lab/types.h>
#include <lab/paths.h>

#include "lest.hpp"

const lest::test specification[] = {
#if defined(LAB_WINDOWS)
  CASE("lab::strings::{ToWide,FromWide} converts from utf-8 to utf-16 and back") {
    std::string input = "漢字 🐶 hello";
    auto wide = lab::strings::ToWide(input);
    auto result = lab::strings::FromWide(wide);
    EXPECT(result == input);
  },

  CASE("lab::strings::ArgvQuote quotes shell arguments") {
    std::wstring line;
    lab::strings::ArgvQuote(std::wstring(L"hello"), line, false);
    EXPECT(L"hello" == line);

    line = L"";
    lab::strings::ArgvQuote(std::wstring(L"no pressure.exe"), line, false);
    EXPECT(L"\"no pressure.exe\"" == line);

    line = L"";
    lab::strings::ArgvQuote(std::wstring(L"some \\sort of \"quote\", kind of"), line, false);
    EXPECT(L"\"some \\sort of \\\"quote\\\", kind of\"" == line);
  },
#endif // LAB_WINDOWS

  CASE("lab::strings::CContains") {
    EXPECT(true == lab::strings::CContains("booga", "oog"));
    EXPECT(true == lab::strings::CContains("booga", "boo"));
    EXPECT(true == lab::strings::CContains("booga", "oga"));
    EXPECT(false == lab::strings::CContains("booga", "oof"));
    EXPECT(false == lab::strings::CContains("booga", "heh"));
    EXPECT(false == lab::strings::CContains("booga", "yay"));
  },

  CASE("lab::strings::CEquals") {
    EXPECT(true == lab::strings::CEquals("booga", "booga"));
    EXPECT(false == lab::strings::CEquals("booga", "oogah"));
  },

  CASE("lab::env::{Get,Set}") {
    std::string name = "LAB_TEST";
    std::string value = "漢字 🐶 hello";
    lab::env::Set(name, value);
    std::string value2 = lab::env::Get(name);
    EXPECT(value == value2);
  },

#if defined(LAB_WINDOWS)
  CASE("lab::env::Expand") {
    auto result = lab::env::Expand("hello %LOCALAPPDATA% world");
    auto control = "hello " + lab::env::Get("LOCALAPPDATA") + " world";
    EXPECT(control == result);
  },
#endif

  CASE("lab::env::MergeBlocks") {
    const char *a[] = { "A=1", "C=3", nullptr };
    const char *b[] = { "B=2", "D=4", nullptr };
    auto c = lab::env::MergeBlocks(const_cast<char **>(a), const_cast<char **>(b));
    EXPECT(std::string("A=1") == std::string(c[0]));
    EXPECT(std::string("C=3") == std::string(c[1]));
    EXPECT(std::string("B=2") == std::string(c[2]));
    EXPECT(std::string("D=4") == std::string(c[3]));
    EXPECT(nullptr == c[4]);
  },

  CASE("lab::paths::SelfPath") {
    EXPECT(lab::paths::SelfPath().length() > static_cast<size_t>(0));
  },

  CASE("lab::paths::{Join,DirName}") {
#if defined(LAB_WINDOWS)
    auto base = "C:\\Program Files";
    auto control = "C:\\Program Files\\CMake";
#else
    auto base = "/opt";
    auto control = "/opt/cmake";
#endif
    auto result = lab::paths::Join(base, "CMake");
    EXPECT(control == result);
    EXPECT(base == lab::paths::DirName(result));
  },
};

int main (int argc, char *argv[]) {
  return lest::run(specification, argc, argv);
}

// OpenSTA, Static Timing Analyzer
// Copyright (c) 2026, Parallax Software, Inc.
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.
// 
// The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software.
// 
// Altered source versions must be plainly marked as such, and must not be
// misrepresented as being the original software.
// 
// This notice may not be removed or altered from any source distribution.

#pragma once

#include <map>
#include <mutex>
#include <string>
#include <string_view>

#include "Format.hh"
#include "Report.hh"
#include "StringUtil.hh"

namespace sta {

class Report;
class Pin;

using DebugMap = std::map<std::string, int, std::less<>>;

class Debug
{
public:
  Debug(Report *report);
  int level(std::string_view what);
  void setLevel(std::string_view what,
                int level);
  // False unless some debug category is enabled.
  bool debugEnabled() const { return debug_on_; }
  bool check(std::string_view what,
             int level) const
  {
    if (!debug_on_)
      return false;
    return checkEnabled(what, level);
  }
  int statsLevel() const { return stats_level_; }
  template <typename... Args>
  void report(std::string_view what,
              std::string_view fmt,
              Args &&...args)
  {
    std::string msg = sta::format("{}: {}", what,
                                 sta::formatRuntime(fmt, std::forward<Args>(args)...));
    std::unique_lock<std::mutex> lock(buffer_lock_);
    report_->reportLine(msg);
  }

protected:
  Report *report_;
  std::mutex buffer_lock_;
  bool debug_on_{false};
  DebugMap debug_map_;
  int stats_level_{0};
  bool checkEnabled(std::string_view what,
                    int level) const;
};

// Inlining a varargs function would eval the args, which can
// be expensive, so use a macro. The flag test skips the call when
// debug is off, which is the usual timing run.
#define debugPrint(debug, what, level, fmt, ...) \
  if ((debug)->debugEnabled() && (debug)->check(what, level)) { \
    debug->report(what, fmt __VA_OPT__(,) __VA_ARGS__); \
  }

} // namespace sta

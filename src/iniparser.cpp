/*
 * cxxtools - general purpose C++-toolbox
 * Copyright (C) 2007 Tommi Maekitalo
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * is provided AS IS, WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, and
 * NON-INFRINGEMENT.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 *
 */

#include <cxxtools/iniparser.h>
#include <cxxtools/log.h>
#include <cctype>
#include <iostream>
#include <stdexcept>

log_define("cxxtools.iniparser")

namespace cxxtools
{
  bool IniParser::Event::onSection(const std::string& section)
  {
    return false;
  }

  bool IniParser::Event::onKey(const std::string& key)
  {
    return false;
  }

  bool IniParser::Event::onValue(const std::string& key)
  {
    return false;
  }

  bool IniParser::Event::onComment(const std::string& comment)
  {
    return false;
  }

  bool IniParser::Event::onError()
  {
    throw std::runtime_error("parse error in ini-file");
    return true;
  }

  void IniParser::parse(std::istream& in)
  {
    char ch;
    while (in.get(ch) && !parse(ch))
      ;
    end();
  }

  bool IniParser::parse(char ch)
  {
    bool ret = false;
    switch (state)
    {
      case state_0:
        if (ch == '[')
          state = state_section;
        else if (std::isalnum(ch))
        {
          data = ch;
          state = state_key;
        }
        else if (ch == '#')
        {
          state = state_comment;
        }
        else if (std::isspace(ch))
          ;
        else
        {
          log_debug("onError");
          ret = event.onError();
        }
        break;

      case state_section:
        if (ch == ']')
        {
          log_debug("onSection(" << data << ')');
          ret = event.onSection(data);
          data.clear();
          state = state_0;
        }
        else
          data += ch;
        break;

      case state_key:
        if (ch == '=')
        {
          log_debug("onKey(" << data << ')');
          ret = event.onKey(data);
          state = state_value0;
        }
        else if (std::isspace(ch))
        {
          log_debug("onKey(" << data << ')');
          ret = event.onKey(data);
          state = state_key_sp;
        }
        else
          data += ch;
        break;

      case state_key_sp:
        if (ch == '=')
          state = state_value0;
        else if (!std::isspace(ch))
        {
          log_debug("onError");
          ret = event.onError();
        }
        break;

      case state_value0:
        if (ch == '\n')
        {
          log_debug("onValue(\"\")");
          ret = event.onValue(std::string());
          state = state_0;
        }
        else if (!std::isspace(ch))
        {
          data = ch;
          state = state_value;
        }
        break;

      case state_value:
        if (ch == '\n')
        {
          log_debug("onValue(" << data << ')');
          ret = event.onValue(data);
          data.clear();
          state = state_0;
        }
        else
          data += ch;
        break;

      case state_comment:
        if (ch == '\n')
          state = state_0;
        break;
    }

    return ret;
  }

  void IniParser::end()
  {
    switch (state)
    {
      case state_0:
      case state_comment:
        break;

      case state_section:
      case state_key:
      case state_key_sp:
        log_debug("onError");
        event.onError();
        break;

      case state_value0:
        log_debug("onValue(\"\")");
        event.onValue(std::string());
        break;

      case state_value:
        log_debug("onValue" << data << ')');
        event.onValue(data);
        break;
    }
  }

}

/* cxxtools/syserror.h
 *
 * cxxtools - general purpose C++-toolbox
 * Copyright (C) 2007 Tommi Maekitalo
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef CXXTOOLS_SYSERROR_H
#define CXXTOOLS_SYSERROR_H

#include <cxxtools/api.h>
#include <stdexcept>

namespace cxxtools
{
  class SysError : public std::runtime_error
  {
      int m_errno;

    public:
      explicit SysError(const char* fn);
      SysError(int err, const char* fn);

      int getErrno() const   { return m_errno; }
  };
}

#endif // CXXTOOLS_SYSERROR_H


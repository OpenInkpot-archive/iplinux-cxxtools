/* dynbuffer.cpp
 *
 * cxxtools - general purpose C++-toolbox
 * Copyright (C) 2003 Tommi Maekitalo
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

#include <exception>
#include <iostream>
#include <cxxtools/dynbuffer.h>

int main(int argc, char* argv[])
{
  try
  {
    cxxtools::Dynbuffer<> buffer(512);
    char* data = buffer.data();
    // data points to a 512 bytes buffer, which is freed, when the
    // current scope is left either normally or through a exception.
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}


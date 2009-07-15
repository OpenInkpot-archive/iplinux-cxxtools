/* md5stream.cpp
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

#include "cxxtools/md5stream.h"
#include <cstring>

namespace cxxtools
{

////////////////////////////////////////////////////////////////////////
// Md5streambuf
//
std::streambuf::int_type Md5streambuf::overflow(
  std::streambuf::int_type ch)
{
  if (pptr() == 0)
  {
    // Ausgabepuffer ist leer - initialisieren
    MD5Init(&context);
  }
  else
  {
    // konsumiere Zeichen aus dem Puffer
    MD5Update(&context,
              (const unsigned char*)pbase(),
              pptr() - pbase());
  }

  // setze Ausgabepuffer
  setp(buffer, buffer + bufsize);

  if (ch != traits_type::eof())
  {
    // das Zeichen, welches den overflow ausgel�st hat, stecken
    // wir in den Puffer.
    *pptr() = traits_type::to_char_type(ch);
    pbump(1);
  }

  return 0;
}

std::streambuf::int_type Md5streambuf::underflow()
{
  // nur Ausgabestrom
  return traits_type::eof();
}

int Md5streambuf::sync()
{
  if (pptr() != pbase())
  {
    // konsumiere Zeichen aus dem Puffer
    MD5Update(&context, (const unsigned char*)pbase(), pptr() - pbase());

    // leere Ausgabepuffer
    setp(buffer, buffer + bufsize);
  }

  return 0;
}

void Md5streambuf::getDigest(unsigned char digest_[16])
{
  using namespace std;

  if (pptr())
  {
    if (pptr() != pbase())
    {
      // konsumiere Zeichen aus dem Puffer
      MD5Update(&context, (const unsigned char*)pbase(), pptr() - pbase());
    }

    // deinitialisiere Ausgabepuffer
    setp(0, 0);

    MD5Final(digest, &context);
    memcpy(digest_, digest, 16);
  }
  else
    memset(digest_, 0, 16);
}

////////////////////////////////////////////////////////////////////////
// Md5stream
//
const char* Md5stream::getHexDigest()
{
  static const char hex[] = "0123456789abcdef";
  unsigned char md5[16];
  getDigest(md5);
  int i;
  char* p = hexdigest;
  for (i = 0; i < 16; ++i)
  {
    *p++ = hex[md5[i] >> 4];
    *p++ = hex[md5[i] & 0xf];
  }
  *p = '\0';
  return hexdigest;
}

}

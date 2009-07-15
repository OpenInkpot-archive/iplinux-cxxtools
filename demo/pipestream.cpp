/* pipestream.cpp
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

/**
 * This demo shows, how to create a child-process, use a pipe to signal
 * from child to parent and another pipe to send a datastream.
 */

#include <cxxtools/pipestream.h>
#include <cxxtools/fork.h>

int main(int argc, char* argv[])
{
  try
  {
    // create pipe, where child signals, that he is initialized
    cxxtools::Pipe pipe;

    cxxtools::Pipestream pstream;

    // fork child-process
    cxxtools::Fork fork;

    if (fork.parent())
    {
      pipe.closeWriteFd();
      pstream.closeWriteFd();

      std::cout << "waiting for child to become ready" << std::endl;

      char ch = pipe.read(); // wait for child to get ready
      std::cout << "child is ready - he sent '" << ch << '\'' << std::endl;

      // now we copy everything, the child sends through the stream
      std::cout << pstream.rdbuf() << std::flush;

      fork.wait();

      std::cout << "child terminated normally" << std::endl;
    }
    else // child
    {
      pipe.closeReadFd();
      pstream.closeReadFd();

      // we do some long initialization:
      ::sleep(1);

      pipe.write('a');

      // make another break
      ::sleep(1);

      pstream << "Hello World!" << std::endl;
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

//
// begin license header
//
// This file is part of Pixy CMUcam5 or "Pixy" for short
//
// All Pixy source code is provided under the terms of the
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).
// Those wishing to use Pixy source code, software and/or
// technologies under different licensing terms should contact us at
// cmucam@cs.cmu.edu. Such licensing terms are available for
// all portions of the Pixy codebase presented here.
//
// end license header
//

#include <signal.h>
#include "libpixyusb2.h"

Pixy2        pixy;
static bool  run_flag = true;


void handle_SIGINT(int unused)
{
  // On CTRL+C - abort! //

  run_flag = false;
}

void  get_line_features()
{
  int  Element_Index;

  // Query Pixy for line features //
  pixy.line.getAllFeatures();

  // Were vectors detected? //
  if (pixy.line.numVectors)
  {
    // Blocks detected - print them! //

    printf ("Detected %d vectors(s)\n", pixy.line.numVectors);

    for (Element_Index = 0; Element_Index < pixy.line.numVectors; ++Element_Index)
    {
      printf ("  Vector %d: ", Element_Index + 1); // TODO: find/ change this
      pixy.line.vectors[Element_Index].print();
    }
  }
}

int main()
{
  int  Result;

  // Catch CTRL+C (SIGINT) signals //
  signal (SIGINT, handle_SIGINT);

  // Initialize Pixy2 Connection //
  {
    Result = pixy.init();

    if (Result < 0)
    {
      return Result;
    }
  }

  // Get Pixy2 Version information //
  {
    Result = pixy.getVersion();

    if (Result < 0)
    {
      return Result;
    }

    pixy.version->print();
  }

  // Set Pixy2 to line feature identification program //
  pixy.changeProg("line");

  while(1)
  {
    get_line_features();

    if (run_flag == false)
    {
      // Exit program loop //
      break;
    }
  }
}

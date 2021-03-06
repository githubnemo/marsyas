// TestSum.h

//
// The Sum class sums up various parts of the input
//

//
// (c) 2009 - sness@sness.net - GPL
//

#include <cxxtest/TestSuite.h>

#include <cstdio>
#include "Collection.h"
#include "MarSystemManager.h"
#include "ChromaFilter.h"

#include <vector>

using namespace std;
using namespace Marsyas;

class Sum_runner : public CxxTest::TestSuite
{
public:

  void test_chroma(void) 
  {
	MarSystemManager mng;

	MarSystem* net = mng.create("Series", "series");
	net->addMarSystem(mng.create("SoundFileSource", "src"));
	net->addMarSystem(mng.create("ChromaFilter", "chromafilter"));

    delete net;
  }

};

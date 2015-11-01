/*
 * CSE-main.cc
 *
 *  Created on: 2015年7月19日
 *      Author: weimi_000
 */


#include <stdexcept>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "CSEBase.h"
#include "NSECoAPBinding.h"
#include "CSEResourceStore.h"
#include "CSEHandler.h"
#include "CSEServer.h"
#include "CoAPInt.h"

using namespace MicroWireless::OneM2M;


int main(int argc, char* argv[])
{

  const char * store_fn = "data/.store";

  // Check command line arguments.
  switch (argc)
  {
  case 3: break;
  case 4: store_fn = argv[3];
  	  	  break;
  default:
    std::cerr << "Usage: http_server <address> <port> [<Resource store>]\n";
    std::cerr << "  For IPv4, try:\n";
    std::cerr << "    receiver 0.0.0.0 80 .\n";
    std::cerr << "  For IPv6, try:\n";
    std::cerr << "    receiver 0::0 80 .\n";
    return 1;
  }
/*
  if (!std::ifstream(cse_fn).good()) {
	std::cerr << "CSE resource file not valid: " << cse_fn << std::endl;
	return 1;
  }
*/
  try
  {
	CoAPInt coap_int_(argv[1], argv[2]);
	NSE_CoAP nse_coap_(argv[1], argv[2], coap_int_);
	CSEResourceStore rdb_(store_fn);
	CSEHandler hdl_(nse_coap_, rdb_);
	CSEServer::setCSEServer(rdb_, nse_coap_, hdl_);

	CSEServer::run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }


  return 0;
}


/*
 * CSEResourceStore.h
 *
 *  Created on: 2015年7月27日
 *      Author: weimi_000
 */

#ifndef CSE_RESOURCESTORE_H_
#define CSE_RESOURCESTORE_H_

#include "CSEBase.h"
#include "ResourceStore.h"

namespace MicroWireless {
namespace OneM2M {

using namespace MicroWireless::OneM2M;

class CSEResourceStore: public ResourceStore<CSEBase> {
public:
	CSEResourceStore(const char * rdb_fn) :
		ResourceStore<CSEBase>(rdb_fn)
	{
		if (!setupRoot()) {
			throw runtime_error("CSE store set up root failed.");
		}
		p_cse_ = getRoot();
	};

	CSEBase * cse() { return p_cse_; };
	virtual ~CSEResourceStore() {};

private:
	CSEBase * p_cse_;
};

}	//OneM2M
}	//MicroWireless


#endif /* CSE_RESOURCESTORE_H_ */

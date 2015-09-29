/*
 * CSEHandler.h
 *
 *  Created on: 2015年7月21日
 *      Author: weimi_000
 */

#ifndef CSE_CSEHANDLER_H_
#define CSE_CSEHANDLER_H_

#include <boost/function.hpp>

#include "CommonTypes.h"
#include "CSEBase.h"
#include "CSEResourceStore.h"
#include "RequestHandler.h"

namespace MicroWireless {
namespace OneM2M {

using namespace MicroWireless::OneM2M;

class NSEBase;
class RequestPrim;
class ResponsePrim;

using anncCreator =  boost::function<void(const ResourceBase&, const vector<string>&, const AnncAttr&)>;

class CSEHandler : public RequestHandler {

public:
	CSEHandler(NSEBase& nse, CSEResourceStore& rdb);

	void handleRequest(RequestPrim&);
	void handleResponse(ResponsePrim&);

	bool initiateAnnc(const RequestPrim&, const ResourceBase&, ResponseStatusCode&);

private:
	template <typename SourceType, typename AnncType>
	void createAnnc(const ResourceBase& res, const vector<string>& at, const AnncAttr& oa) {
		AnncType annc_(static_cast<const SourceType&>(res));
		if (oa.size() == 0) {
			annc_.copyAnncFields();
		} else {
			annc_.copyAnncFields(oa);
		}
		string pc_;
		annc_.serializeToString(&pc_);
		for (unsigned int i = 0; i < at.size(); i++) {
			string rqi_;
			generateRequestId(rqi_);
			try {
				RequestPrim reqp_annc_(Operation::CREATE,
					at[i], rdb_.getRoot()->getDomain() + rdb_.getRoot()->getCSEId(), rqi_);
				reqp_annc_.setContent(pc_);
				nse_.send(reqp_annc_, "localhost", 5555);
				reqc_.addRequest(reqp_annc_, rqi_,
						boost::bind(&CSEHandler::postAnnc, this, _1, _2, _3));
			} catch (exception &e) {
				continue;
			}
		}
	}

	void postAnnc(const RequestPrim&, const string&, const ResponsePrim&);

private:
	CSEResourceStore& rdb_;

	static map<SupportedResourceType, anncCreator> annc_create_;
};

}	// OneM2M

}	// MicroWireless

#endif /* CSE_CSEHANDLER_H_ */

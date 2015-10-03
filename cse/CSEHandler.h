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

class CSEHandler : public RequestHandler {

public:
	using anncCreator =  boost::function<void(const ResourceBase&,
			const vector<string>&, const AnncAttr&, const string&)>;

	CSEHandler(NSEBase& nse, CSEResourceStore& rdb);

	void handleRequest(RequestPrim&);
	void handleResponse(ResponsePrim&);

	bool initiateAnnc(const RequestPrim&, const ResourceBase&, ResponseStatusCode&);

private:
	template <typename SourceType, typename AnncType>
	void createAnnc(const ResourceBase& res, const vector<string>& at, const AnncAttr& oa,
			const string& rqi) {
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
				reqc_.addRequest(reqp_annc_, rqi, // original request rqi from AE
						boost::bind(&CSEHandler::postAnnc, this, _1, _2, _3));
				nse_.send_request(reqp_annc_, "localhost", 5555);
			} catch (exception &e) {
				cerr << "CSEHandler::createAnnc: exception: " << e.what() << endl;
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

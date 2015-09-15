/*
 * NSEBase.h
 *
 *  Created on: 2015年7月21日
 *      Author: weimi_000
 */

#ifndef INCLUDE_NSEBASE_H_
#define INCLUDE_NSEBASE_H_

#include <string>
//#include <boost/function.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

namespace MicroWireless {
namespace OneM2M {

using namespace MicroWireless::OneM2M;

class RequestPrim;
class ResponsePrim;

class NSEBase {
public:
	NSEBase(const std::string& ip, const std::string& port);

	//void setRequestHandler(boost::function<void(RequestPrim&)> req_hdl);

	template <typename HandlerType>
	void post(RequestPrim& req, HandlerType* hdl) {
		io_svc_->post(boost::bind(&HandlerType::handleRequest, hdl, req));
	}

	//void post(ResponsePrim& rsp);
	virtual void send(ResponsePrim& rsp, const std::string& addr, uint port);
	virtual void run();

	virtual ~NSEBase();

private:
	void work_thread(boost::shared_ptr<boost::asio::io_service> io_service);

	static const int num_threads_ = 2;

	//boost::function<void(RequestPrim&)> req_h_;
	const std::string ip_, port_;
	boost::shared_ptr<boost::asio::io_service> io_svc_;
	boost::shared_ptr<boost::asio::io_service::work> work_;
	//boost::shared_ptr<boost::asio::io_service::strand> strand_;
	boost::thread_group thread_grp_;
};

}	// OneM2M

}	// MicroWireless

#endif /* INCLUDE_NSEBASE_H_ */

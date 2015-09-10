/*
 * NSEBase.cc
 *
 *  Created on: 2015年9月4日
 *      Author: weimi_000
 */

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
//#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>

#include "RequestPrim.h"
#include "ResponsePrim.h"
#include "NSEBase.h"

namespace MicroWireless {
namespace OneM2M {

using namespace std;
using namespace MicroWireless::OneM2M;

NSEBase::NSEBase(const std::string& ip, const std::string& port)
	: ip_(ip), port_(port)
	, io_svc_(new boost::asio::io_service())
	//,strand_ = new boost::asio::io_service::strand(*io_svc_)
{ }

void NSEBase::setRequestHandler(boost::function<void(RequestPrim&)> req_hdl) {
	req_h_ = req_hdl;
}

void NSEBase::work_thread(boost::shared_ptr<boost::asio::io_service> io_service) {
	while (true) {
		try	{
			boost::system::error_code ec;
			io_service->run(ec);
			if (ec)	{
				cerr << "[" << boost::this_thread::get_id()
					<< "] error from handler: " << ec << endl;
			}
			break;
		} catch (exception &ex)	{
			cerr << "[" << boost::this_thread::get_id()
				<< "] Exception: " << ex.what() << std::endl;
		}
	}
}

void NSEBase::post(RequestPrim& req) {
	io_svc_->post(boost::bind(req_h_, req));
}

void NSEBase::send(ResponsePrim& rsp, const std::string& addr, uint port) { }

void NSEBase::run() {
	boost::shared_ptr<boost::asio::io_service::work> work(
		new boost::asio::io_service::work(*io_svc_));

	for (int x = 0; x < num_threads_; ++x)	{
		thread_grp_.create_thread(boost::bind(&NSEBase::work_thread, this, io_svc_));
	}

	cout << "NSEBase::run:" << num_threads_ << " worker threads created.\n";
}

NSEBase::~NSEBase() {
	io_svc_->stop();
	thread_grp_.join_all();
}

}	// OneM2M

}	// MicroWireless


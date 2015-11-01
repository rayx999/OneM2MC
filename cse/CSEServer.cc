/*
 * CSEServer.cc
 *
 *  Created on: 2015年7月25日
 *      Author: weimi_000
 */
#include <stdexcept>

#include "CSEResourceStore.h"
#include "CommonTypes.h"
#include "NSEBase.h"
#include "CSEHandler.h"
#include "CSEServer.h"
#include "CSEApi.h"

namespace MicroWireless {
namespace OneM2M {

using namespace MicroWireless::OneM2M;

namespace CSEServer {

namespace {
	bool inst_ = false;
	CSEResourceStore* p_rdb_ = NULL;
	NSEBase* p_nse_ = NULL;
	CSEHandler* p_hdl_ = NULL;
}

void setCSEServer(CSEResourceStore& rdb, NSEBase& nse, CSEHandler& hdl) {
	if (inst_)
		throw runtime_error("CSEServer already set.");

	p_rdb_ = &rdb;
	p_nse_ = &nse;
	p_hdl_ = &hdl;
	CSEApi::setCSEApi(rdb, nse, hdl);
	inst_ = true;
}

void clrCSEServer() {
	inst_ = false;
	p_rdb_ = NULL;
	p_nse_ = NULL;
	p_hdl_ = NULL;
}

CSEResourceStore& getCSEResourceStore() {
	if (!inst_)
		throw runtime_error("CSEServer NOT set.");

	return *p_rdb_;
}

NSEBase& getNSEBase() {
	if (!inst_)
		throw runtime_error("CSEServer NOT set.");

	return *p_nse_;
}

CSEHandler& getCSEHandler() {
	if (!inst_)
		throw runtime_error("CSEServer NOT set.");

	return *p_hdl_;
}

void run() {
	if (!inst_)
		throw runtime_error("CSEServer NOT set.");

	p_nse_->run();
}

}	//CSEServer
}	//OneM2M
}	//MicroWireless

/*
 * test_RequestCache.cc
 *
 *  Created on: 2015年9月28日
 *      Author: weimi_000
 */


#include <iostream>
#include <string>
#include <json2pb.h>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread.hpp>
#include <boost/format.hpp>
#include <boost/bind.hpp>
#include "gtest/gtest.h"

#include "CommonTypes.h"
#include "RequestCache.h"
#include "RequestPrim.h"
#include "ResponsePrim.h"

using namespace std;
using namespace MicroWireless::OneM2M;

class RequestCacheTest : public ::testing::Test {
protected:
	static const string reqp_json;

	RequestCache& cache_;

	RequestCacheTest() : cache_(RequestCache::getCache()) {}
	virtual ~RequestCacheTest() {}

	virtual void SetUp() {	}

	virtual void TearDown() {}

public:
	void callback(const RequestPrim& reqp, const std::string& data, const ResponsePrim& rspp) {
		ASSERT_EQ(rspp.getResponseStatusCode(), ResponseStatusCode::OK);
	}

	void addRequest(int i) {
		const string rqi_ = boost::str(boost::format("RQI-%05d") % i);
		RequestPrim reqp_(Operation::CREATE, "//microwireless.com/in-cse-01",
				"//microwireless.com/ae-01", rqi_);
		ASSERT_TRUE(cache_.addRequest(reqp_));
	}

	void getRequest(int i) {
		const string rqi_ = boost::str(boost::format("RQI-%05d") % i);
		while (!cache_.isRequestIdInUse(rqi_)) {
			boost::this_thread::sleep(boost::posix_time::milliseconds(100));
			cout << "getRequest wait for " << rqi_ << endl;
		}
		RequestPrim reqp_;
		ASSERT_TRUE(cache_.getRequest(rqi_, reqp_));
	}
};

const string RequestCacheTest::reqp_json("{"
		"\"op\": 2, "
		"\"to\": \"//microwireless.com/IN-CSE-01/Z0005\", "
		"\"rqi\": \"RQI-1\", "
		"\"fr\": \"//microwireless.com/AE-01\""
"}");

TEST_F(RequestCacheTest, AddRequest) {
	try {
		RequestPrim reqp_(reqp_json);
		ASSERT_TRUE(cache_.addRequest(reqp_));
	} catch (exception &e) {
		cout << "Unexpected exception: " << e.what() << endl;
		ASSERT_TRUE(false);
	}
}

TEST_F(RequestCacheTest, CheckRequest) {
	ASSERT_TRUE(cache_.isRequestIdInUse("RQI-1"));
}

TEST_F(RequestCacheTest, GetRequest) {
	RequestPrim reqp_;

	ASSERT_TRUE(cache_.getRequest("RQI-1", reqp_));
	ASSERT_EQ((int)reqp_.getOperation(), 2);
	ASSERT_STREQ(reqp_.getTo().c_str(), "//microwireless.com/IN-CSE-01/Z0005");
	ASSERT_STREQ(reqp_.getFrom().c_str(), "//microwireless.com/AE-01");
}

TEST_F(RequestCacheTest, CheckRequestNegative) {
	ASSERT_FALSE(cache_.isRequestIdInUse("RQI-1"));
}

TEST_F(RequestCacheTest, AddDuplicateRequest) {
	try {
		RequestPrim reqp_(reqp_json);
		ASSERT_TRUE(cache_.addRequest(reqp_));
		ASSERT_FALSE(cache_.addRequest(reqp_));
	} catch (exception &e) {
		cout << "Unexpected exception: " << e.what() << endl;
		ASSERT_TRUE(false);
	}
}

TEST_F(RequestCacheTest, AddRequest2) {
	try {
		RequestPrim reqp_(reqp_json);
		reqp_.setRequestId("RQI-2");
		const string pc_("1234567890");
		ASSERT_TRUE(cache_.addRequest(reqp_, pc_));
	} catch (exception &e) {
		cout << "Unexpected exception: " << e.what() << endl;
		ASSERT_TRUE(false);
	}
}

TEST_F(RequestCacheTest, GetRequest2) {
	RequestPrim reqp_;
	string pc_;

	ASSERT_TRUE(cache_.getRequest("RQI-2", reqp_, pc_));
	ASSERT_EQ((int)reqp_.getOperation(), 2);
	ASSERT_STREQ(reqp_.getTo().c_str(), "//microwireless.com/IN-CSE-01/Z0005");
	ASSERT_STREQ(reqp_.getFrom().c_str(), "//microwireless.com/AE-01");
	ASSERT_STREQ(pc_.c_str(), "1234567890");
}


TEST_F(RequestCacheTest, AddRequest3) {
	try {
		RequestPrim reqp_(reqp_json);
		reqp_.setRequestId("RQI-3");
		const string pc_("1234567890");

		ASSERT_TRUE(cache_.addRequest(reqp_, pc_,
				boost::bind(&RequestCacheTest::callback, this, _1, _2, _3)));
	} catch (exception &e) {
		cout << "Unexpected exception: " << e.what() << endl;
		ASSERT_TRUE(false);
	}
}

TEST_F(RequestCacheTest, GetRequest3) {
	RequestPrim reqp_;
	string pc_;
	RequestCache::PostRequestFunc f_;

	ASSERT_TRUE(cache_.getRequest("RQI-3", reqp_, pc_, f_));
	ASSERT_EQ((int)reqp_.getOperation(), 2);
	ASSERT_STREQ(reqp_.getTo().c_str(), "//microwireless.com/IN-CSE-01/Z0005");
	ASSERT_STREQ(reqp_.getFrom().c_str(), "//microwireless.com/AE-01");
	ASSERT_STREQ(pc_.c_str(), "1234567890");
	ASSERT_FALSE(f_ == NULL);

	ResponsePrim rspp_(&reqp_, ResponseStatusCode::OK, "//microwireless.com/IN-CSE-01");
	f_(reqp_, pc_, rspp_);
}

TEST_F(RequestCacheTest, ParallelTest) {
	boost::shared_ptr<boost::asio::io_service>
		io_svc_(new boost::asio::io_service);
	boost::shared_ptr<boost::asio::io_service::work>
		work_(new boost::asio::io_service::work(*io_svc_));
	boost::thread_group thread_grp_;

	for (int x = 0; x < 5; ++x)	{
		thread_grp_.create_thread(boost::bind(&boost::asio::io_service::run, io_svc_));
	}

	for (int i = 0; i < 100; i++) {
		io_svc_->post(boost::bind(&RequestCacheTest::addRequest, this, i));
		io_svc_->post(boost::bind(&RequestCacheTest::getRequest, this, i));
	}

	work_.reset();
	thread_grp_.join_all();
}

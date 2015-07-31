/*
 * test_ResourceStore.cc for gtest
 *
 *  Created on: 2015年7月10日
 *      Author: weimi_000
 */
#include <fstream>
#include <sstream>
#include "gtest/gtest.h"
#include "test_utils.h"
#include "CSEBase.h"
#include "ResourceStore.h"
//#include "FakeStore.cc"

using namespace MicroWireless::OneM2M;

static bool _setup_ = false;

class ResourceStoreTest : public ::testing::Test {
	protected:
	ResourceStore<CSEBase> * p_store_;

	ResourceStoreTest() { p_store_ = NULL; }

	virtual void SetUp() {
		if (_setup_) {
			p_store_ = new ResourceStore<CSEBase>("data/.store");
		}
	}

	virtual void TearDown() {
		if (p_store_ != NULL)
			delete p_store_;
	}
};

TEST_F(ResourceStoreTest, CtorInvalidStoreName) {
	ResourceStore<CSEBase> store_("data/.no-store");
	ASSERT_FALSE(store_.setupRoot());
}

TEST_F(ResourceStoreTest, CtorNoRoot) {
	ResourceStore<CSEBase> store_("data");
	ASSERT_FALSE(store_.setupRoot());
}

TEST_F(ResourceStoreTest, TurnOnFixture) {
	_setup_ = true;
}

TEST_F(ResourceStoreTest, CtorValidStore) {
	ASSERT_TRUE(p_store_->setupRoot());
}

TEST_F(ResourceStoreTest, ResourceValid) {
	const string resource_("microwireless.com/IN-CSE-01/CSEBase");
	ASSERT_TRUE(p_store_->isResourceValid(resource_));
}

TEST_F(ResourceStoreTest, ResourceNotExist) {
	const string resource_("microwireless.com/IN-CSE-01/bad");
	ASSERT_FALSE(p_store_->isResourceValid(resource_));
}

TEST_F(ResourceStoreTest, GetRoot) {
	p_store_->setupRoot();
	ASSERT_TRUE(p_store_->getRoot() != NULL);
}

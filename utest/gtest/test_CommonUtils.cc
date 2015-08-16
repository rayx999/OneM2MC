/*
 * test_CommonUtils.cc
 *
 *  Created on: 2015年8月13日
 *      Author: weimi_000
 */

#include <iostream>
#include <string>
#include "gtest/gtest.h"
#include "test_utils.h"
#include "CommonUtils.h"

using namespace std;
using namespace MicroWireless::OneM2M;

class CommonUtilsTest : public ::testing::Test {
protected:
	static const string cse_regex;

	virtual void SetUp() {}
	// virtual void TearDown() {}
};

const string CommonUtilsTest::cse_regex("in-cse-\\d\\d");

TEST_F(CommonUtilsTest, Parse4Subdomains) {
	const string id_("//in-cse-01.o2o.microwireless.com/95847");
	string domain, csi, ri;

	ASSERT_TRUE(parseIds(id_, cse_regex, domain, csi, ri));
	ASSERT_STREQ(domain.c_str(), "//o2o.microwireless.com");
	ASSERT_STREQ(csi.c_str(), "/in-cse-01");
	ASSERT_STREQ(ri.c_str(), "95847");
}

TEST_F(CommonUtilsTest, Parse3Subdomains) {
	const string id_("//in-cse-01.microwireless.com/95847");
	string domain, csi, ri;

	ASSERT_TRUE(parseIds(id_, cse_regex, domain, csi, ri));
	ASSERT_STREQ(domain.c_str(), "//microwireless.com");
	ASSERT_STREQ(csi.c_str(), "/in-cse-01");
	ASSERT_STREQ(ri.c_str(), "95847");
}

TEST_F(CommonUtilsTest, Parse2Subdomains) {
	const string id_("//microwireless.com/in-cse-01/95847");
	string domain, csi, ri;

	ASSERT_TRUE(parseIds(id_, cse_regex, domain, csi, ri));
	ASSERT_STREQ(domain.c_str(), "//microwireless.com");
	ASSERT_STREQ(csi.c_str(), "/in-cse-01");
	ASSERT_STREQ(ri.c_str(), "95847");
}

TEST_F(CommonUtilsTest, Parse4SubdomainsFullRespurcePath) {
	const string id_("//in-cse-01.o2o.microwireless.com/fresh/fish/shark");
	string domain, csi, ri;

	ASSERT_TRUE(parseIds(id_, cse_regex, domain, csi, ri));
	ASSERT_STREQ(domain.c_str(), "//o2o.microwireless.com");
	ASSERT_STREQ(csi.c_str(), "/in-cse-01");
	ASSERT_STREQ(ri.c_str(), "fresh/fish/shark");
}

TEST_F(CommonUtilsTest, Parse3SubdomainsFullResourcePath) {
	const string id_("//in-cse-01.microwireless.com/fresh/fish/shark");
	string domain, csi, ri;

	ASSERT_TRUE(parseIds(id_, cse_regex, domain, csi, ri));
	ASSERT_STREQ(domain.c_str(), "//microwireless.com");
	ASSERT_STREQ(csi.c_str(), "/in-cse-01");
	ASSERT_STREQ(ri.c_str(), "fresh/fish/shark");
}

TEST_F(CommonUtilsTest, Parse2SubdomainsFullResourcePath) {
	const string id_("//microwireless.com/in-cse-01/fresh/fish/shark");
	string domain, csi, ri;

	ASSERT_TRUE(parseIds(id_, cse_regex, domain, csi, ri));
	ASSERT_STREQ(domain.c_str(), "//microwireless.com");
	ASSERT_STREQ(csi.c_str(), "/in-cse-01");
	ASSERT_STREQ(ri.c_str(), "fresh/fish/shark");
}

TEST_F(CommonUtilsTest, IgnoreCSEIdCase) {
	const string id_("//microwireless.com/In-Cse-01/95847");
	string domain, csi, ri;

	ASSERT_TRUE(parseIds(id_, cse_regex, domain, csi, ri));
	ASSERT_STREQ(domain.c_str(), "//microwireless.com");
	ASSERT_STREQ(csi.c_str(), "/In-Cse-01");
	ASSERT_STREQ(ri.c_str(), "95847");
}

TEST_F(CommonUtilsTest, DuplicateCseId) {
	const string id_("//in-cse-01.microwireless.com/in-cse-01/95847");
	string domain, csi, ri;

	ASSERT_TRUE(parseIds(id_, cse_regex, domain, csi, ri));
	ASSERT_STREQ(domain.c_str(), "//microwireless.com");
	ASSERT_STREQ(csi.c_str(), "/in-cse-01");
	ASSERT_STREQ(ri.c_str(), "95847");
}

TEST_F(CommonUtilsTest, ParsePartialId) {
	const string id_("/in-cse-01/95847");
	string domain, csi, ri;

	ASSERT_TRUE(parseIds(id_, cse_regex, domain, csi, ri));
	ASSERT_TRUE(domain.empty());
	ASSERT_STREQ(csi.c_str(), "/in-cse-01");
	ASSERT_STREQ(ri.c_str(), "95847");
}

TEST_F(CommonUtilsTest, ParsePartialIdFullPath) {
	const string id_("/in-cse-01/fresh/fish/shark");
	string domain, csi, ri;

	ASSERT_TRUE(parseIds(id_, cse_regex, domain, csi, ri));
	ASSERT_TRUE(domain.empty());
	ASSERT_STREQ(csi.c_str(), "/in-cse-01");
	ASSERT_STREQ(ri.c_str(), "fresh/fish/shark");
}

TEST_F(CommonUtilsTest, ResourceIdOnly) {
	const string id_("fresh/fish/shark");
	string domain, csi, ri;

	ASSERT_TRUE(parseIds(id_, cse_regex, domain, csi, ri));
	ASSERT_TRUE(domain.empty());
	ASSERT_TRUE(csi.empty());
	ASSERT_STREQ(ri.c_str(), "fresh/fish/shark");
}

// negative cases
TEST_F(CommonUtilsTest, 4SubdomainsNoCseId) {
	const string id_("//fresh.o2o.microwireless.com/95847");
	string domain, csi, ri;

	ASSERT_FALSE(parseIds(id_, cse_regex, domain, csi, ri));
}

TEST_F(CommonUtilsTest, 3SubdomainsNoCseId) {
	const string id_("//o2o.microwireless.com/95847");
	string domain, csi, ri;

	ASSERT_FALSE(parseIds(id_, cse_regex, domain, csi, ri));
}

TEST_F(CommonUtilsTest, 2SubdomainsNoCseId) {
	const string id_("//microwireless.com/95847");
	string domain, csi, ri;

	ASSERT_FALSE(parseIds(id_, cse_regex, domain, csi, ri));
}

TEST_F(CommonUtilsTest, NoRootDomain) {
	const string id_("//microwireless/in-cse-01/95847");
	string domain, csi, ri;

	ASSERT_FALSE(parseIds(id_, cse_regex, domain, csi, ri));
}

TEST_F(CommonUtilsTest, InconsistentCseId) {
	const string id_("//in-cse-10.microwireless.com/in-cse-01/95847");
	string domain, csi, ri;

	ASSERT_FALSE(parseIds(id_, cse_regex, domain, csi, ri));
}

TEST_F(CommonUtilsTest, NoResourceId) {
	const string id_("//in-cse-10.microwireless.com/");
	string domain, csi, ri;

	ASSERT_FALSE(parseIds(id_, cse_regex, domain, csi, ri));
}

TEST_F(CommonUtilsTest, NoResourceId1) {
	const string id_("//microwireless.com/in-cse-01");
	string domain, csi, ri;

	ASSERT_FALSE(parseIds(id_, cse_regex, domain, csi, ri));
}

TEST_F(CommonUtilsTest, NoResourceId2) {
	const string id_("/in-cse-01");
	string domain, csi, ri;

	ASSERT_FALSE(parseIds(id_, cse_regex, domain, csi, ri));
}

TEST_F(CommonUtilsTest, InvalidCseId) {
	const string id_("//microwireless.com/in-cse-X1/95847");
	string domain, csi, ri;

	ASSERT_FALSE(parseIds(id_, cse_regex, domain, csi, ri));
}

TEST_F(CommonUtilsTest, InvalidCseId1) {
	const string id_("//in-cse-X1.microwireless.com/95847");
	string domain, csi, ri;

	ASSERT_FALSE(parseIds(id_, cse_regex, domain, csi, ri));
}

TEST_F(CommonUtilsTest, InvalidCseId2) {
	const string id_("/in-cse-X1/95847");
	string domain, csi, ri;

	ASSERT_FALSE(parseIds(id_, cse_regex, domain, csi, ri));
}

TEST_F(CommonUtilsTest, InvalidCharacter) {
	const string id_("//in-cse-01.microwirele'ss.com/95847");
	string domain, csi, ri;

	ASSERT_FALSE(parseIds(id_, cse_regex, domain, csi, ri));
}

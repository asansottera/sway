/*
Copyright (c) 2011, Andrea Sansottera
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
 - Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
 - Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <boost/test/unit_test.hpp>

#include <sway/configuration.hpp>

using namespace sway;

BOOST_AUTO_TEST_CASE(TestStringSimple) {
	configuration cfg = configuration::from_string("a=10,b=hello,c=1");
	BOOST_CHECK_EQUAL(cfg.get<int>("a"), 10);
	BOOST_CHECK_EQUAL(cfg.get<std::string>("b").compare("hello"), 0);
	BOOST_CHECK_EQUAL(cfg.get<bool>("c"), true);
	std::vector<std::string> unused = cfg.unused();
	BOOST_CHECK_EQUAL(unused.size(), 0u);
}

BOOST_AUTO_TEST_CASE(TestStringUnused) {
	configuration cfg = configuration::from_string("a=10,b=hello,c=1");
	BOOST_CHECK_EQUAL(cfg.get<int>("a"), 10);
	BOOST_CHECK_EQUAL(cfg.get<std::string>("b").compare("hello"), 0);
	std::vector<std::string> unused = cfg.unused();
	BOOST_CHECK_EQUAL(unused.size(), 1u);
	BOOST_CHECK_EQUAL(unused[0].compare("c"), 0);
}

BOOST_AUTO_TEST_CASE(TestStringGroup) {
	configuration cfg = configuration::from_string("a=10,b=hello,c=1,poll.alpha=1.0");
	configuration poll_cfg = cfg.group("poll");
	BOOST_CHECK_CLOSE(poll_cfg.get<double>("alpha"), 1.0, 1e-8);
	std::vector<std::string> poll_unused = poll_cfg.unused();
	BOOST_CHECK_EQUAL(poll_unused.size(), 0u);
}

BOOST_AUTO_TEST_CASE(TestStringGroupUnused) {
	configuration cfg = configuration::from_string("a=10,b=hello,c=1,poll.alpha=1.0,poll.phi=1.0");
	configuration poll_cfg = cfg.group("poll");
	BOOST_CHECK_CLOSE(poll_cfg.get<double>("alpha"), 1.0, 1e-8);
	std::vector<std::string> poll_unused = poll_cfg.unused();
	BOOST_CHECK_EQUAL(poll_unused.size(), 1u);
	BOOST_CHECK_EQUAL(poll_unused[0].compare("phi"), 0);
}

BOOST_AUTO_TEST_CASE(TestStringGroupNested) {
	configuration cfg = configuration::from_string("a=10,b=hello,c=1,poll.alpha=1.0,poll.phi=1.0,poll.linear.eps_limit_max=0.1");
	configuration poll_cfg = cfg.group("poll");
	BOOST_CHECK_CLOSE(poll_cfg.get<double>("alpha"), 1.0, 1e-8);
	BOOST_CHECK_CLOSE(poll_cfg.get<double>("phi"), 1.0, 1e-8);
	configuration poll_linear_cfg = poll_cfg.group("linear");
	BOOST_CHECK_CLOSE(poll_linear_cfg.get<double>("eps_limit_max"), 0.1, 1e-8);
}

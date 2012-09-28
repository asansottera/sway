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

#ifndef SWAY_CONFIGURATION_HPP
#define SWAY_CONFIGURATION_HPP

#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

namespace sway {

class configuration_error : public std::runtime_error {
public:
	configuration_error(const std::string & msg) : std::runtime_error(msg) {
	}
};

class configuration {
private:
	struct option {
		std::string value;
		int use_count;
		option(const std::string & _value) {
			value = _value;
			use_count = 0;
		}
	};
private:
	typedef std::map<std::string, option> option_map;
	typedef boost::shared_ptr<option_map> option_map_ptr;
	typedef std::map<std::string, configuration> child_map;
	typedef boost::shared_ptr<child_map> child_map_ptr;
	mutable option_map_ptr m_options;
	mutable child_map_ptr m_children;
private:
	configuration() {
		m_options.reset(new option_map);
		m_children.reset(new child_map);
	}
private:
	void add_option(const std::vector<std::string> & split, const std::string & value, unsigned int idx) {
		if (idx == split.size()-1) {
			option_map::value_type opt(split[idx], option(value));
			m_options->insert(opt);		
		} else {
			child_map::iterator itr = m_children->find(split[idx]);
			if (itr == m_children->end()) {
				configuration child;
				itr = m_children->insert(child_map::value_type(split[idx], child)).first;
			}
			itr->second.add_option(split, value, idx+1);
		}
	}
	void add_option(const std::string & key, const std::string & value) {
		std::vector<std::string> split;
		boost::split(split, key, boost::is_any_of("."));
		if (split.size() == 1) {
			option_map::value_type opt(key, option(value));
			m_options->insert(opt);
		} else {
			child_map::iterator itr = m_children->find(split[0]);
			if (itr == m_children->end()) {
				configuration child;
				itr = m_children->insert(child_map::value_type(split[0], child)).first;
			}
			itr->second.add_option(split, value, 1);
		}
	}
public:
	/*! Creates a new configuration object parsing the file at the given path. */
	static configuration from_file(const std::string & file_name) {
		configuration cfg;
		std::ifstream f(file_name.data());
		std::string line;
		std::vector<std::string> splitted;
		std::size_t i = 0;
		while (std::getline(f, line)) {
			++i;
			boost::split(splitted, line, boost::is_any_of("="));
			if (splitted.size() != 2) {
				std::stringstream msg;
				msg << "Invalid line \"" << i << "\"";
				throw configuration_error(msg.str());
			}
			cfg.add_option(splitted[0], splitted[1]);
		}
		return cfg;
	}
	/*! Creates a new configuraiton object parsing the given string. */
	static configuration from_string(const std::string & options) {
		configuration cfg;
		std::vector<std::string> tokens;
		boost::split(tokens, options, boost::is_any_of(","));
		std::vector<std::string> kv;
		std::vector<std::string>::iterator itr;
		for (itr = tokens.begin(); itr != tokens.end(); ++itr) {
			if (itr->length() > 0) {
				boost::split(kv, *itr, boost::is_any_of("="));
				if (kv.size() != 2) {
					std::stringstream msg;
					msg << "Invalid option \"" << *itr << "\"";
					throw configuration_error(msg.str());
				}
				cfg.add_option(kv[0], kv[1]);
			}
		}
		return cfg;
	}
	/*! Retrieves the value of the given option, throw an exception if not found. */
	template<class T>
	T get(const std::string & key) {
		option_map::iterator itr = m_options->find(key);
		if (itr == m_options->end()) {
			std::stringstream msg;
			msg << "Option \"" << key << "\" not specified";
			throw configuration_error(msg.str());
		}
		T result = boost::lexical_cast<T>(itr->second.value);
		itr->second.use_count += 1;
		return result;
	}
	/*! Retrieves the value of the given option, returns the default value if not found. */
	template<class T>
	T get(const std::string & key, const T & def) {
		option_map::iterator itr = m_options->find(key);
		if (itr == m_options->end()) {
			return def;
		}
		T result = boost::lexical_cast<T>(itr->second.value);
		itr->second.use_count += 1;
		return result;
	}
	/*! Returns the names of the options whose value has not been read yet. */
	std::vector<std::string> unused() const {
		std::vector<std::string> res;
		option_map::const_iterator i;
		for (i = m_options->begin(); i != m_options->end(); ++i) {
			if (i->second.use_count == 0) {
				res.push_back(i->first);
			}
		}
		return res;
	}
	configuration group(const std::string & name) {
		child_map::const_iterator itr = m_children->find(name);
		if (itr != m_children->end()) {
			return itr->second;
		} else {
			std::stringstream msg;
			msg << "Group \"" << name << "\" not found";
			throw configuration_error(msg.str());
		}
	}
};


}

#endif

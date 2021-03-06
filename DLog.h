/*
 * Debug.h
 *
 *  Created on: Apr 13, 2015
 *
 *
 *
 *
 */

#ifdef DEBUG_FLAG
#define DEBUG_ENABLED 1
#else
#define DEBUG_ENABLED 0
#endif
#define DEBUG(msg) if (!DEBUG_ENABLED) {} \
                   else dbglog() << __FILE__ << ":" << __LINE__ << " " << msg

#ifndef DLOG_H_
#define DLOG_H_

#include <iostream>

/*************************************************|Debugging Helper|**************************************************/

struct dbglog {
		std::ostream &os_;
		mutable bool has_endl_;

		dbglog(std::ostream &os = std::cout) :
						os_(os),
						has_endl_(false) {
			os_ << std::endl;
		}

		~dbglog() {
			if (!has_endl_) os_ << std::endl;
		}

		template<typename T>
		static bool has_endl(const T &) {
			return (false);
		}

		static bool has_endl(char c) {
			return (c == '\n');
		}

		static bool has_endl(std::string s) {
			return (has_endl(*s.rbegin()));
		}

		static bool has_endl(const char *s) {
			return (has_endl(std::string(s)));
		}

		template<typename T>
		static bool same_manip(T & (*m)(T &), T & (*e)(T &)) {
			return (m == e);
		}

		const dbglog& operator <<(std::ostream & (*m)(std::ostream &)) const {
			has_endl_ = same_manip(m, std::endl);
			os_ << m;
			return (*this);
		}

		template<typename T>
		const dbglog& operator <<(const T &v) const {
			has_endl_ = has_endl(v);
			os_ << v;
			return (*this);
		}
};

#endif

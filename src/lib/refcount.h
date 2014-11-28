// -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
// vi:set ts=4 sts=4 sw=4 noet :
//
// Copyright 2010 wkhtmltopdf authors
//
// This file is part of wkhtmltopdf.
//
// wkhtmltopdf is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// wkhtmltopdf is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with wkhtmltopdf.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __REFCOUNT_HH__
#define __REFCOUNT_HH__


#include <wkhtmltox/dllbegin.inc>

namespace wkhtmltopdf {


class RefBase {
private:
	RefBase(): usage(0) {}
	
	inline void incref() {
		++usage;
	}
	
	inline void decref() {
		--usage; 
		if (usage == 0) delete this;
	}
	
private:
	size_t usage;
};


template <typename T>
class RefPtr {
public:
	inline RefPtr(): obj(NULL) {}
	
	inline ~RefPtr() {
		reset(NULL);
	}
	
	template <typename TT>
	RefPtr(TT * x): obj(NULL) {
		reset(x);
	}
	
	template <typename TT>
	RefPtr(const RefPtr<TT> & x): obj(NULL) {
		reset(x->obj);
	}
	
	template <typename TT>
	RefPtr & operator=(const RefPtr<TT> & x) {
		reset(x->obj);
		return *this;
	}
	
	inline void reset(T * other) {
		if (obj) obj->decref();
		if (other) other->incref();
		obj = other;
	}
	
	template <typename TT>
	inline bool operator==(TT * p) {
		return obj == p;
	}
	
	template <typename TT>
	inline bool operator==(const RefPtr<TT> & p) {
		return obj == p.obj;
	}
	
	template <typename TT>
	inline bool operator!=(TT * p) {
		return obj != p;
	}
	
	template <typename TT>
	inline bool operator!=(const RefPtr<TT> & p) {
		return obj != p.obj;
	}
	
	inline T & operator->() {
		return *obj;
	}
	
	inline const T & operator->() const {
		return *obj;
	}
	
	inline T & operator*() {
		return *obj;
	}
	
	inline const T & operator*() const {
		return *obj;
	}
	
private:
	T * obj;
};

#endif //__REFCOUNT_HH__

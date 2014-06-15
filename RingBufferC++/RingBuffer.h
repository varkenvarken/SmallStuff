// ##### BEGIN GPL LICENSE BLOCK #####
//
// RingBuffer, a C++ class implementing a RingBuffer (aka Circular Buffer)
// (c) 2014 Michel J. Anders (varkenvarken)
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// ##### END GPL LICENSE BLOCK #####
#pragma once
#include <array>
#include <exception>
using namespace std;

/// ExtraContainers is intended as a namespace for specialized containers.
namespace ExtraContainers
{
	template<class T, size_t N>
	class RingBuffer
	{

	private:
		class base_iterator
		{
		public:
			typedef base_iterator self_type;
			typedef T value_type;
			typedef T& reference;
			typedef T* pointer;
			typedef int difference_type;
			base_iterator(RingBuffer &buffer, bool begin, bool reverse) :
				buffer_(buffer),
				current_(begin ? (reverse ? 0 : buffer.Count() - 1) : (reverse ? buffer.Count() : -1))
			{}
			reference operator*() { return buffer_[current_]; }
			pointer operator->() { return &(buffer_[current_]); }
			bool operator==(const self_type& rhs) { return current_ == rhs.current_; }
			bool operator!=(const self_type& rhs) { return current_ != rhs.current_; }

		protected:
			int current_;
			RingBuffer &buffer_;
		};

		class reverse_iterator : public base_iterator
		{
		public:
			typedef reverse_iterator self_type;
			reverse_iterator(RingBuffer &buffer, bool begin) : base_iterator(buffer, begin, true) {}
			self_type operator++() { current_++; return *this; }
			self_type operator++(int dummy) { auto tmp = *this; current_++; return tmp; }
		};

		class iterator : public base_iterator
		{
		public:
			typedef iterator self_type;
			iterator(RingBuffer &buffer, bool begin) : base_iterator(buffer, begin, false) {}
			self_type operator++() { current_--; return *this; }
			self_type operator++(int dummy) { auto tmp = *this; current_--; return tmp; }
		};

		array<T, N> buffer;
		size_t cursor = 0;
		size_t count = 0;

	public: 
		RingBuffer(){ if (N == 0) throw exception("0 length RingBuffer"); }
		size_t Count(){ return count; }
		int Oldest() { return count ? -(int)count + 1 : 0; }
		size_t Capacity(){ return N; }
		void Add(T item)
		{
			if (Count() < Capacity()) count++;
			buffer[cursor] = item;
			cursor = (cursor + 1) % Capacity();
		}
		iterator begin(){ return iterator(*this, true); }
		iterator end(){ return iterator(*this, false);}
		reverse_iterator rbegin(){ return reverse_iterator(*this, true); }
		reverse_iterator rend(){ return reverse_iterator(*this, false);	}
		T& operator[](int i){ // not size_t, we need a signed index to specify positions relative to the oldest element
			return index(i);
		}
		const T& operator[](int i) const{
			return index(i);
		}

	private: 
		T& index(int i){
			int m = Count() < Capacity() ? Count() : Capacity();
			if (i < 0) i = -(m + i);
			int p = ((int)cursor - 1 - i) % m; // % is a remainder, not a modulo op, see http://stackoverflow.com/questions/1082917/mod-of-negative-number-is-melting-my-brain
			if (p < 0) p += m;
			return buffer[p];
		}

	};
};

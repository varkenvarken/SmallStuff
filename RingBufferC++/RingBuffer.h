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
	/// A fixed size buffer that will retire the oldest item when adding a new item to a buffer that has reached its capacity.
	/// It provides iterators to iterate over all items, both from newest to oldest and from oldest to newest and an index
	/// operator that accesses items relative to the newest item (for positive indices) or relative to the oldest item (for
	/// negative indices.
	///
	/// *Examples*
	///
	/// In the following example we create a RingBuffer with space for 5 ints, add 12 ints and iterate over the buffer to print them:
	/// ~~~~~~{.c++}
	///     RingBuffer<int, 5> buffer;
	///     for (int i = 0; i < 12; i++){ buffer.Add(i); }
	///     for (auto item : buffer) cout << ' ' << item;
	/// ~~~~~~
	/// The output will be:
	/// ~~~~~~{.c++}
	///     7 8 9 10 11
	/// ~~~~~~
	/// It is also possible to iterate from newest to oldest:
	/// ~~~~~~{.c++}
	///     for (auto rit = buffer.rbegin(); rit != buffer.rend(); cout << ' ' << *rit++) continue;
	/// ~~~~~~
	/// Items can be refered to directly, for example to refer to the oldest item in the buffer:
	/// ~~~~~~{.c++}
	///     auto old = buffer[-1];
	/// ~~~~~~
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

		/// The backing store.
		array<T, N> buffer;

		/// The location where a new item will be inserted.
		size_t cursor = 0;

		/// The number of items in the buffer. Will never exceed the capacity.
		size_t count = 0;

	public:
		/// Create a RingBuffer of a given size. Throws an exception if the size is specified as 0.
		RingBuffer(){ if (N == 0) throw exception("0 length RingBuffer"); }

		/// Return the number of items in the buffer. Will never exceed the capacity.
		size_t Count(){ return count; }

		/// Return the index of the oldest item. This is a negative number with -1 for the oldest item, -2 for the next oldest item etc.
		int Oldest() { return count ? -(int)count + 1 : 0; }

		/// Return the capacity of the RingBuffer. This is the maximum number the buffer will hold before retiring the oldest item if a new item is added.
		size_t Capacity(){ return N; }

		/// Add an item to the Ringbuffer. If the capacity of the RingBuffer is reached, it will retire the oldest item. 
		void Add(T item)
		{
			if (Count() < Capacity()) count++;
			buffer[cursor] = item;
			cursor = (cursor + 1) % Capacity();
		}

		/// Return the start of a forward iterator. A forward iterator will return items from the oldest to the newest.
		iterator begin(){ return iterator(*this, true); }

		/// Return the end of a forward iterator.
		iterator end(){ return iterator(*this, false);}

		/// Return the start of a reverse iterator. A reverse iterator will return items from the newest to the oldest.
		reverse_iterator rbegin(){ return reverse_iterator(*this, true); }
		
		/// Return the end of a reverse iterator.
		reverse_iterator rend(){ return reverse_iterator(*this, false); }

		/// Return a reference to an item in the RingBuffer. 
		/// An index of 0 is the newest item with positive indices returning older items.
		/// Negative indices count from -1 for the oldest item to -2 for the next oldest, etc.
		T& operator[](int i){ // not size_t, we need a signed index to specify positions relative to the oldest element
			return index(i);
		}

		/// Return a const reference to an item in the RingBuffer.
		const T& operator[](int i) const{
			return index(i);
		}

	private:
		/// Common logic shared by both the const and non const versions of the index operators opeator[]
		T& index(int i){
			int m = Count() < Capacity() ? Count() : Capacity();
			if (i < 0) i = -(m + i);
			int p = ((int)cursor - 1 - i) % m; // % is a remainder, not a modulo op, see http://stackoverflow.com/questions/1082917/mod-of-negative-number-is-melting-my-brain
			if (p < 0) p += m;
			return buffer[p];
		}

	};
};

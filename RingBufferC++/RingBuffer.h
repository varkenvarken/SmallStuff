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

/// <summary>
/// ExtraContainers is intended as a namespace for specialized containers.
/// </summary>
namespace ExtraContainers
{
	/// <summary>
	/// A RingBuffer is a circular list with a fixed capacity.
	/// Adding more items to the RingBuffer than its capacity will overwrite the the oldest item.
	/// </summary>
	/// <typeparam name="T">The type of the items in the RingBuffer</typeparam>
	/// <seealso cref="GetEnumerator">
	/// All items in a RingBuffer can be iterated over from oldest to newest by using the enumerator returned by the GetEnumerator method, which allows for the standard foreach syntax.
	/// </seealso>
	/// <seealso cref="GetReverseEnumerator">
	/// It is also possible to iterate over the items from newest to oldest with the enumerator returned by the GetReverseEnumerator method.
	/// </seealso>
	/// <seealso cref="this">
	/// </seealso>
	/// <example>The following example adds 10 ints to a RingBuffer with a capacity for 5. The it lists the contents from oldest to newest:
	/// <code>
	/// var rb = new RingBuffer&lt;int>(5);
	/// for(int i=0; i&lt;10; i++){ rb.Add(i); }
	/// foreach(var item in rb){ Console.WriteLine(item); }
	/// </code>
	/// The result will look like this:
	/// <code>
	/// 6
	/// 7
	/// 8
	/// 9
	/// 10
	/// </code>
	/// </example>


	template<class T, size_t N>
	class RingBuffer
	{

	private:
		class iterator
		{
		public:
			typedef iterator self_type;
			typedef T value_type;
			typedef T& reference;
			typedef T* pointer;
			typedef std::forward_iterator_tag iterator_category;
			typedef int difference_type;
			// begin
			iterator(pointer start, pointer begin, pointer end) : start_(start), begin_(begin), end_(end) { 
				clamp(start_); 
				ptr_ = (end_ != begin_) ? start_ : nullptr ;
			}
			// end
			iterator(): ptr_(nullptr){}
			// post inc
			self_type operator++() { self_type i = *this; ptr_++; roll(ptr_); return i; }
			// pre inc
			self_type operator++(int junk) { ptr_++; roll(ptr_);  return *this; }
			reference operator*() { return *ptr_; }
			pointer operator->() { return ptr_; }
			bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
			bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }

		private:
			void roll(pointer &p){ if (p >= end_) p = begin_; if (p == start_) p = nullptr; }
			void clamp(pointer &p){ if (p < begin_) p = end_ - 1; else if (p >= end_) p = begin_ ; }
			pointer ptr_, start_, begin_, end_;
		};

		array<T, N> buffer;
		size_t cursor = 0;
		size_t count = 0;
	public: RingBuffer(){ if (N == 0) throw exception("0 length RingBuffer"); }
		/// <summary>
		/// The number of items in the RingBuffer.
		/// </summary>
	public: size_t Count(){ return count; }

		/// <summary>
		/// The maximum number of items in the RingBuffer.
		/// </summary>
	public: size_t Capacity(){ return N; }

		/// <summary>
		/// Add an item at the current position of a RingBuffer.
		/// </summary>
		/// <param name="item">The item to add</param>
	public: void Add(T item)
	{
		if (Count() < Capacity()) count++;
		buffer[cursor] = item;
		cursor = (cursor + 1) % Capacity();
	}

		/// <summary>
		/// Return a forward iterator over the contents of the Ringbuffer.
		/// A forward iterator returns all items from the oldest to the newest.
		/// </summary>
		/// <returns></returns>
	public: iterator begin(){
		T* base = &(buffer[0]);
		if (Count() < Capacity()){
			return iterator(base, base, base +cursor);
		}
		else{
			return iterator(base + cursor, base, base + Capacity());
		}
	}

	public: iterator end(){
		return iterator();
	}

	};
};

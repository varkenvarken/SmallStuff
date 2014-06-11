// ##### BEGIN GPL LICENSE BLOCK #####
//
// RingBuffer, a C# class implementing a RingBuffer (aka Circular Buffer)
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

using System;
using System.Collections;
using System.Collections.Generic;

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
    public class RingBuffer<T> : IEnumerable<T>, IEnumerable
    {
        /// <summary>
        /// Constructors specifying no initial capacity or a capacity smaller than this default will be set to the defaultcapacity. 
        /// There is no provision the alter the capacity afterwards.
        /// </summary>
        public static readonly int defaultcapacity = 4; // readonly instead of const, see http://stackoverflow.com/questions/13150343/the-constant-cannot-be-marked-static

        private int cursor = 0;
        
        private List<T> buffer;
        
        /// <summary>
        /// Create an empty RingBuffer with a default capacity.
        /// </summary>
        public RingBuffer() { buffer = new List<T>(defaultcapacity); }

        /// <summary>
        /// Create an empty RingBuffer with a given capacity.
        /// </summary>
        /// <param name="capacity">The apacity of the RingBuffer. Capacities smaller than the default capacity will set to the default.</param>
        public RingBuffer(int capacity) { buffer = new List<T>(capacity > defaultcapacity ? capacity : defaultcapacity); }

        /// <summary>
        /// The number of items in the RingBuffer.
        /// </summary>
        public int Count { get { return buffer.Count;} }

        /// <summary>
        /// The maximum number of items in the RingBuffer.
        /// </summary>
        public int Capacity { get { return buffer.Capacity; } }

        /// <summary>
        /// Add an item at the current position of a RingBuffer.
        /// </summary>
        /// <param name="item">The item to add</param>
        public void Add(T item)
        {
            if (Count == Capacity)
            {
                buffer[cursor] = item;
            }
            else
            {
                buffer.Add(item);
            }
            cursor = (cursor + 1) % Capacity;
        }

        /// <summary>
        /// Return a forward iterator over the contents of the Ringbuffer.
        /// A forward iterator returns all items from the oldest to the newest.
        /// </summary>
        /// <returns></returns>
        public IEnumerator<T> GetEnumerator()
        {
            int end = Count;
            if (Count >= Capacity)
            {
                for (int i = cursor; i < Count; i++)
                {
                    yield return buffer[i];
                }
                end = cursor;
            }
            for (int i = 0; i < end; i++)
            {
                yield return buffer[i];
            }
        }

        /// <summary>
        /// Return a reverse iterator over the contents of the Ringbuffer.
        /// A reverse iterator returns all items from the newest to the oldest.
        /// </summary>
        /// <returns></returns>
        public IEnumerator<T> GetReverseEnumerator()
        {
            int end = 0;
            if (Count >= Capacity)
            {
                for (int i = cursor - 1; i >= 0; i--)
                {
                    yield return buffer[i];
                }
                end = cursor;
            }
            for (int i = Count - 1; i >= end; i--)
            {
                yield return buffer[i];
            }
        }

        /// <summary>
        /// This is just to satisfy the IEnumerable interface, necessary for any foreach syntactic sugar to work
        /// see: http://stackoverflow.com/questions/2981615/error-message-regarding-ienumerable-getenumerator
        /// </summary>
        /// <returns></returns>
        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        /// <summary>
        /// Return an element relative to the newest. Positive indices will index towards increasingly older items,
        /// while negative indices will index towards newer items, i.e. -1 refers to the oldest, -2 the second oldest, ...
        /// </summary>
        /// <param name="i">The index.</param>
        /// <returns></returns>
        public T this[int i]
        {
            get {
                int m = Count < Capacity ? Count : Capacity;
                int p = (cursor - 1 - i)%m; // % is a remainder, not a modulo op, see http://stackoverflow.com/questions/1082917/mod-of-negative-number-is-melting-my-brain
                if (p < 0) p += m;
                return buffer[p];
            }
        }
    }
}

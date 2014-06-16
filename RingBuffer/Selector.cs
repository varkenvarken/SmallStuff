using System;
using System.Collections;
using System.Collections.Generic;

namespace ExtraContainers
{
    /// <summary>
    /// A RingBuffer that provides an extra iterator that will return those items that match a certain condition
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public class RingBufferSelector<T> : RingBuffer<T>
    {
        public RingBufferSelector() : base() { }
        public RingBufferSelector(int n) : base(n) { }

        /// <summary>
        /// returns items that match a certain condition, from oldest to newest.
        /// </summary>
        /// <param name="selector">a delegate to match items</param>
        /// <returns></returns>
        public IEnumerator<T> GetSelector(predicate selector)
        {
            foreach (var item in this)
            {
                if (selector(item))
                {
                    yield return item;
                }
            }
        }

        /// <summary>
        /// A function that takes an item as argument and returns true if it matches a condition.
        /// </summary>
        /// <param name="item"></param>
        /// <returns></returns>
        public delegate bool predicate(T item);
    }
}

using System;
using System.Collections;
using System.Collections.Generic;

namespace ExtraContainers
{
    public class RingBuffer<T> : List<T>
    {
        public static readonly int defaultcapacity = 4; // readonly instead of const, see http://stackoverflow.com/questions/13150343/the-constant-cannot-be-marked-static

        private int cursor = 0;

        public RingBuffer() : base(defaultcapacity){}

        public RingBuffer(int capacity) : base(capacity) { }

        public new void Add(T item)
        {
            if (this.Count == this.Capacity)
            {
                this[cursor] = item;
            }
            else
            {
                base.Add(item);
            }
            cursor = (cursor + 1) % this.Capacity;
        }

        public new IEnumerator<T> GetEnumerator()
        {
            if (Count >= Capacity)
            {
                for (int i = cursor; i < Count; i++)
                {
                    yield return this[i];
                }
                for (int i = 0; i < cursor; i++)
                {
                    yield return this[i];
                }
            }
            else
            {
                for (int i = 0; i < Count; i++)
                {
                    yield return this[i];
                }
            }
        }

        public new IEnumerator<T> GetReverseEnumerator()
        {
            if (Count >= Capacity)
            {
                for (int i = cursor-1; i >= 0; i--)
                {
                    yield return this[i];
                }
                for (int i = Count - 1; i >= cursor; i--)
                {
                    yield return this[i];
                }
            }
            else
            {
                for (int i = Count-1; i >= 0; i--)
                {
                    yield return this[i];
                }
            }
        }

        public void Dump(bool reverse=false)
        {
            Console.Write("Cursor: {0}, Count: {1}, Capacity: {2} , ", cursor, this.Count, this.Capacity);
            if (reverse)
            {
                var re = GetReverseEnumerator();
                while (re.MoveNext())
                {
                    Console.Write("{0} ",re.Current);
                }
            }
            else
            {
                foreach (var i in this)
                {
                    Console.Write("{0} ", i);
                }
            }
            Console.WriteLine("");
        }

    }
}

using System;
using System.Collections.Generic;
using ExtraContainers;

namespace RingBufferTest
{
    class Program
    {

        static void Main(string[] args)
        {
            var rb = new RingBuffer<int>();
            rb.Dump();
            for (int i = 1; i <= 10; i++)
            {
                rb.Add(i);
                rb.Dump();
                rb.Dump(true);
            }
        }
    }
}

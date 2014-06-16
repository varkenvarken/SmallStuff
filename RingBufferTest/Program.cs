// ##### BEGIN GPL LICENSE BLOCK #####
//
// RingBufferTest, A program to test the RingBuffer class
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
using System.Collections.Generic;
using ExtraContainers;

namespace RingBufferTest
{
    class Program
    {
        static void Dump<T>(ref RingBufferSelector<T> rb, bool reverse = false)
        {
            if (reverse)
            {
                var re = rb.GetReverseEnumerator();
                while (re.MoveNext())
                {
                    Console.Write("{0} ", re.Current);
                }
            }
            else
            {
                foreach (var i in rb)
                {
                    Console.Write("{0} ", i);
                }
            }
            Console.WriteLine("");
        }

        static bool even(int number) { return number % 2 == 0; }

        static void Main(string[] args)
        {
            var rb = new RingBufferSelector<int>(5);
            Dump(ref rb);
            for (int i = 0; i < 3; i++)
            {
                rb.Add(i);
            }
            Dump(ref rb);
            Dump(ref rb, true);
            //for (int i = 0; i < rb.Count; i++)
            //    Console.WriteLine("{0} {1} -{0} {2}",i, rb[i],rb[-i]);
            for (int i = 3; i <= 11; i++)
            {
                rb.Add(i);
            }
            Dump(ref rb);
            Dump(ref rb, true);
            //for (int i = 0; i < rb.Count; i++)
            //    Console.WriteLine("{0} {1} -{0} {2}", i, rb[i], rb[-i]);

            var se = rb.GetSelector(even);
            while(se.MoveNext())
            {
                Console.Write("{0} ", se.Current);
            }

            Console.ReadLine();
        }
    }
}

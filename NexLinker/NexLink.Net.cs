using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace NexLinker
{
    public class NexLink
    {
        [DllImport("NexLink.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int init();

        [DllImport("NexLink.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int scandevices();

        [DllImport("NexLink.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int initwithindex(int index);

        [DllImport("NexLink.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void close();

        [DllImport("NexLink.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int receive(byte[] data, int length);

        [DllImport("NexLink.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int transfer(byte[] data, int length);

        [DllImport("NexLink.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int control_get(byte bRequest, ushort wValue, byte[] data, ushort wLength);

        [DllImport("NexLink.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int control_set(byte bRequest, ushort wValue, byte[] data, ushort wLength);

        public static byte[] StructToBytes(object odata)
        {
            int size = Marshal.SizeOf(odata);
            byte[] byteArray = new byte[size];

            IntPtr ptr = Marshal.AllocHGlobal(size);
            try
            {
                Marshal.StructureToPtr(odata, ptr, false);
                Marshal.Copy(ptr, byteArray, 0, size);
            }
            finally
            {
                Marshal.FreeHGlobal(ptr);
            }

            return byteArray;
        }

        public static object BytesToStruct(byte[] byteArray, Type type)
        {
            int size = Marshal.SizeOf(type);
            IntPtr ptr = Marshal.AllocHGlobal(size);
            try
            {
                Marshal.Copy(byteArray, 0, ptr, size);
                return (object)Marshal.PtrToStructure(ptr, type);
            }
            finally
            {
                Marshal.FreeHGlobal(ptr);
            }
        }

        public static int SetDirection(byte dir)
        {
            var rawdata = new byte[1];
            rawdata[0] = (byte)((dir & 3));
            return NexLink.control_set((byte)NEX_BREQ.NEX_SCREEN_SET, 0, rawdata, (ushort)rawdata.Length);
        }

        public static int SetTimestamp()
        {
            DateTime currentTime = DateTime.Now;
            DateTime unixStartTime = new DateTime(1970, 1, 1, 0, 0, 0, 0, DateTimeKind.Local);
            TimeSpan elapsedTime = currentTime - unixStartTime;
            long timestamp = (long)elapsedTime.TotalSeconds;

            var rawdata = BitConverter.GetBytes(timestamp);
            return NexLink.control_set((byte)NEX_BREQ.NEX_TIMESTAMP_SET, 0, rawdata, (ushort)rawdata.Length);
        }

        public static int GetTimestamp(ref long timestamp)
        {
            var rawdata = new byte[BitConverter.GetBytes(timestamp).Length];
            var ret = NexLink.control_get((byte)NEX_BREQ.NEX_TIMESTAMP_GET, 0, rawdata, (ushort)rawdata.Length);
            timestamp = BitConverter.ToInt64(rawdata, 0);
            return ret;
        }

        public static int SetBrightness(nex_brightness_des brides)
        {
            var rawdata = StructToBytes(brides);
            return NexLink.control_set((byte)NEX_BREQ.NEX_BRIGHTNESS_SET, 0, rawdata, (ushort)rawdata.Length);
        }

        public static int GetBrightness(ref nex_brightness_des brides)
        {
            var rawdata = new byte[StructToBytes(brides).Length];
            var ret = NexLink.control_get((byte)NEX_BREQ.NEX_BRIGHTNESS_GET, 0, rawdata, (ushort)rawdata.Length);
            brides = (nex_brightness_des)BytesToStruct(rawdata, typeof(nex_brightness_des));
            return ret;
        }

    }

    public enum NEX_BREQ : Byte
    {
        NEX_BREQ_HOST_FORMAT = 0,
        NEX_TIMESTAMP_SET,
        NEX_TIMESTAMP_GET,
        NEX_BRIGHTNESS_SET,
        NEX_BRIGHTNESS_GET,
        NEX_SCREEN_SET,
        NEX_SCREEN_GET,
        NEX_COMMAND_LEN,
    };

    [StructLayout(LayoutKind.Sequential)]
    public struct nex_brightness_des
    {
        public UInt16 brightness;
        public UInt16 damp;
    };


    public struct nex_usb_des
    {
        public UInt64 timestamp_s;
        public nex_brightness_des brides;
    };
}

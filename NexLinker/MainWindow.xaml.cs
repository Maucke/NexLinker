using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace NexLinker
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        const int SCR_WIDTH = 240;
        const int SCR_HEIGHT = 280;
        const int BLOK_VALID = 960;
        byte[] ScreenGram;

        public MainWindow()
        {
            InitializeComponent();
        }

        private void Window_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Left)
                this.DragMove();
        }

        DispatcherTimer dispatcher = new DispatcherTimer();
        Thread threadsend { get; set; }
        bool USBAlive = false;

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            var ret = NexLink.scandevices();
            if (ret == 0) return;
            ret = NexLink.initwithindex(ret - 1);
            if (ret > 0)
                USBAlive = true;

            dispatcher.Interval = TimeSpan.FromMilliseconds(1);
            dispatcher.Tick += Dispatcher_Tick;
            dispatcher.Start();

            NexLink.SetDirection(0);
            NexLink.SetTimestamp();
            NexLink.SetBrightness(new nex_brightness_des() { brightness = (ushort)100, damp = 5000 });
            threadsend = new Thread(() =>
            {
                while (USBAlive)
                {
                    if (ScreenGram != null)
                        try
                        {
                            SendOnPic(ScreenGram);
                        }
                        catch (Exception)
                        {
                            USBAlive = false;
                            Thread.Sleep(10);
                        }
                    else
                        Thread.Sleep(10);
                }
                NexLink.close();
            })
            { IsBackground = true };
            threadsend.Start();
        }

        private void btn_exit_Click(object sender, RoutedEventArgs e)
        {
            USBAlive = false;
            dispatcher.Stop();
            this.Close();
        }

        private void Dispatcher_Tick(object sender, EventArgs e)
        {
            // 捕获屏幕指定区域的图像
            var point = brd_cap.PointToScreen(new System.Windows.Point(0, 0));
            System.Drawing.Rectangle rectangle = new System.Drawing.Rectangle((int)point.X, (int)point.Y, (int)brd_cap.ActualWidth, (int)brd_cap.ActualHeight);
            try
            {
                ScreenGram = CaptureScreenPart(rectangle);
            }
            catch (Exception)
            {

            }
        }

        public void SendOnPic(byte[] data)
        {
            var recvdata = new byte[1024];
            for (int i = 0; i < (SCR_WIDTH * SCR_HEIGHT * 2) / BLOK_VALID; i++)
            {
                for (int p = 0; p < BLOK_VALID; p++)
                {
                    if ((p & 1) == 0)
                    {
                        recvdata[p] = data[i * BLOK_VALID + p + 1];
                    }
                    else
                    {
                        recvdata[p] = data[i * BLOK_VALID + p - 1];
                    }
                }
                NexLink.transfer(recvdata, 1024);
            }
        }

        public byte[] ConvertTo16BitByteArray(Bitmap bitmap)
        {
            // 将图像转换为16位RGB565格式
            BitmapData bmpData = bitmap.LockBits(new System.Drawing.Rectangle(0, 0, bitmap.Width, bitmap.Height), ImageLockMode.ReadOnly, System.Drawing.Imaging.PixelFormat.Format16bppRgb565);
            int byteCount = bmpData.Stride * bitmap.Height;
            byte[] byteArray = new byte[byteCount];
            IntPtr ptr = bmpData.Scan0;

            // 将像素数据复制到字节数组中
            System.Runtime.InteropServices.Marshal.Copy(ptr, byteArray, 0, byteCount);

            bitmap.UnlockBits(bmpData);

            return byteArray;
        }

        public byte[] CaptureScreenPart(System.Drawing.Rectangle bounds)
        {
            // 创建一个与控件大小相同的位图
            Bitmap bitmap = new Bitmap(bounds.Width, bounds.Height);

            // 使用Graphics类绘制控件内容到位图上
            var graphics = Graphics.FromImage(bitmap);
            graphics.CopyFromScreen(new System.Drawing.Point(bounds.X, bounds.Y), System.Drawing.Point.Empty, bounds.Size);
            Bitmap scaledBitmap = new Bitmap(bitmap, new System.Drawing.Size(SCR_WIDTH, SCR_HEIGHT));

            // 将缩小后的图像转换为16位RGB565格式的字节数组
            byte[] byteArray = ConvertTo16BitByteArray(scaledBitmap);

            // 释放资源
            bitmap.Dispose();
            // 释放资源
            scaledBitmap.Dispose();
            return byteArray;
        }
    }
}

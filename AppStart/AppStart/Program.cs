using System.Diagnostics;
using System.Runtime.InteropServices;

class AppStart
{
    [DllImport("user32.dll")]
    static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);

    [DllImport("Kernel32")]
    private static extern IntPtr GetConsoleWindow();

    const int SW_HIDE=0;
    private const int SW_SHOW = 5;

    static void Main(string[] args)
    {
        IntPtr hwnd;
        hwnd=GetConsoleWindow();
        ShowWindow(hwnd,SW_HIDE);

        ProcessStartInfo info = new ProcessStartInfo();             
        info.FileName = @"..\nodeMiddleware.exe";              
        info.Arguments = "";              
        info.WindowStyle = ProcessWindowStyle.Hidden;
        Process pro = Process.Start(info);              
        pro.WaitForExit();
    }
}
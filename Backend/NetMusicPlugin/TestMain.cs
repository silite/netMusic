using System.Collections;
using System.Runtime.InteropServices;
using NetMusicPlugin;

class TestMain
{
    [DllImport("user32.dll")]
    static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);

    [DllImport("Kernel32")]
    private static extern IntPtr GetConsoleWindow();

    const int SW_HIDE=0;
    const int SW_SHOW=5;
    
    static void Main(string[] args)
    {
        IntPtr hwnd;
        hwnd=GetConsoleWindow();
        ShowWindow(hwnd,SW_HIDE);
        
        System.Timers.Timer timer = new System.Timers.Timer(300);
        timer.Elapsed += new System.Timers.ElapsedEventHandler(trigger);
        timer.AutoReset = true; //设置是执行一次（false）还是一直执行(true)； 
        timer.Enabled = true;
        timer.Start();
        while (true)
        {
        }
    }

    private static void trigger(object source, System.Timers.ElapsedEventArgs e)
    {
        ElementTool tools = ElementTool.Instance;
        ArrayList resList = tools.getResList();
        
        ReadFile file = ReadFile.Instance;
    }
}
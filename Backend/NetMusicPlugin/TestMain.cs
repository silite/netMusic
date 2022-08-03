using System.Collections;
using NetMusicPlugin;

class TestMain
{
    static void Main(string[] args)
    {

        System.Timers.Timer timer = new System.Timers.Timer(1000);
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
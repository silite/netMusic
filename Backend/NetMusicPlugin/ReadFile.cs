namespace NetMusicPlugin;

using Newtonsoft.Json.Linq;
using System.IO;

public class ReadFile
{
    private static ReadFile instance = null;
    private static string netMusicPath = Environment.ExpandEnvironmentVariables(@"%localappdata%\Netease\CloudMusic\webdata\file");

    private ReadFile()
    {
        FileSystemWatcher watcher = new FileSystemWatcher();
        watcher.Path = netMusicPath;
        watcher.Changed += new System.IO.FileSystemEventHandler(watcherChange);
        watcher.EnableRaisingEvents = true;
        watcher.Filter = "history";

        void watcherChange(object sender, FileSystemEventArgs e)
        {
            new Thread(() =>
            {
                WebSocket.Instance.send(getActivePlayMusicId(e.FullPath));
            }).Start();
        }
    }

    public static void readFildInit()
    {
        WebSocket.Instance.send(getActivePlayMusicId(netMusicPath + @"\history"));
    }

    private static string getActivePlayMusicId(string path)
    {
        try
        {
            var fileStream = new FileStream(path, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
            var streamReader = new StreamReader(fileStream, System.Text.Encoding.UTF8);
            string originString = streamReader.ReadToEnd();
            streamReader.Close();
            fileStream.Close();

            JArray jsonString = JArray.Parse(originString);

            return "id*ss*" + jsonString.First["tid"];
        }
        catch (Exception e)
        {
            return getActivePlayMusicId(path);
        }
    }

    public static ReadFile Instance
    {
        get { return instance ??= new ReadFile(); }
    }
}
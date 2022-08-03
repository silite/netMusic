namespace NetMusicPlugin;

using Newtonsoft.Json.Linq;
using System.IO;

public class ReadFile
{
    private static ReadFile instance = null;

    private ReadFile()
    {
        WebSocket webSocket = WebSocket.Instance;

        string netMusicPath = @"%localappdata%\Netease\CloudMusic\webdata\file";
        
        webSocket.send(getActivePlayMusicId(netMusicPath + "history"));

        FileSystemWatcher watcher = new FileSystemWatcher();
        watcher.Path = Environment.ExpandEnvironmentVariables(netMusicPath);
        watcher.Changed += new System.IO.FileSystemEventHandler(watcherChange);
        watcher.EnableRaisingEvents = true;
        watcher.Filter = "history";

        void watcherChange(object sender, FileSystemEventArgs e)
        {
            new Thread(() =>
            {
                webSocket.send(getActivePlayMusicId(e.FullPath));
            }).Start();
        }
    }

    private static string getActivePlayMusicId(string path)
    {
        var fileStream = new FileStream(path, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
        var streamReader = new StreamReader(fileStream, System.Text.Encoding.UTF8);
        string originString = streamReader.ReadToEnd();
        streamReader.Close();
        fileStream.Close();

        JArray jsonString = JArray.Parse(originString);

        return "id*ss*" + jsonString.First["tid"];
    }

    public static ReadFile Instance
    {
        get { return instance ??= new ReadFile(); }
    }
}
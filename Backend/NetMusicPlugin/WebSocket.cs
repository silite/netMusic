using Fleck;

namespace NetMusicPlugin;

public class WebSocket
{
    private static List<IWebSocketConnection> allSockets = new List<IWebSocketConnection>();
    private static WebSocket instance = null;
    
    public WebSocket()
    {
        var server = new WebSocketServer("ws://0.0.0.0:12449");
        server.Start(socket =>
        {
            socket.OnOpen = () =>
            {
                Console.WriteLine("server start");
                allSockets.Add(socket);
                ReadFile.readFildInit();
            };

            socket.OnClose = () =>
            {
                allSockets.Remove(socket);
            };

            socket.OnMessage = message =>
            {
                allSockets.ForEach(s => s.Send(message));
            };
        });
        
    }
    
    public static WebSocket Instance
    {
        get { return instance ??= new WebSocket(); }
    }

    public void send(string message)
    {
        foreach (var socket in allSockets)
        {
            socket.Send(message);
        }
    }
}
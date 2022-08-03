using System.Collections;
using System.Diagnostics;

namespace NetMusicPlugin;
using System.Runtime.InteropServices;
using System.Windows.Automation;

public class ElementTool
{
    [DllImport("User32.dll", EntryPoint = "FindWindow")]
    private static extern IntPtr FindWindow(string lpClassName, string lpWindowName);

    private static ElementTool instance = null;

    private static string getNameProperty(AutomationElement element)
    {
        return element.GetCurrentPropertyValue(AutomationElement.NameProperty) as string;
    }

    private static AutomationElement getElement()
    {
        try
        {
            IntPtr mainHwnd = FindWindow("OrpheusBrowserHost", null);
            AutomationElement element = AutomationElement.FromHandle(mainHwnd);

            return element;
        }
        catch (Exception e)
        {
            WebSocket.Instance.send("error");
            throw;
        }
    }

    private AutomationElementCollection getControlPanels()
    {
        try
        {
            AutomationElementCollection controlPanel =
                getElement().FindFirst(TreeScope.Children,
                        new PropertyCondition(AutomationElement.ClassNameProperty, "CefBrowserWindow"))
                    .FindFirst(TreeScope.Children, Condition.TrueCondition)
                    .FindFirst(TreeScope.Children, new PropertyCondition(AutomationElement.NameProperty, "网易音乐"))
                    .FindFirst(TreeScope.Children, Condition.TrueCondition)
                    .FindAll(TreeScope.Children, Condition.TrueCondition)[2]
                    .FindAll(TreeScope.Children, Condition.TrueCondition)[4]
                    .FindAll(TreeScope.Children, Condition.TrueCondition);

            return controlPanel;
        }
        catch (Exception e)
        {
            WebSocket.Instance.send("error");
            throw;
        }
    }

    public ArrayList getResList()
    {
        AutomationElementCollection controlPanels = this.getControlPanels();

        ArrayList paramsList = new ArrayList
        {
            "title*ss*" + getNameProperty(getElement()),
            "isPlaying*ss*" + !getNameProperty(controlPanels[1]).Contains("播放"),
            "playTime*ss*" + getNameProperty(controlPanels[3].FindFirst(TreeScope.Children, Condition.TrueCondition)),
            "totalTime*ss*" + getNameProperty(controlPanels[5].FindFirst(TreeScope.Children, Condition.TrueCondition))
        };

        foreach (var item in paramsList)
        {
            WebSocket.Instance.send(item as string);
        }

        return paramsList;
    }

    public static ElementTool Instance
    {
        get { return instance ??= new ElementTool(); }
    }
}
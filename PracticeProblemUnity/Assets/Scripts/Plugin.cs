using System.Runtime.InteropServices;
using UnityEngine;

public static class Plugin
{
    [DllImport("Plugin.dll")]
    public static extern float CalculateTerrainDistance();
}
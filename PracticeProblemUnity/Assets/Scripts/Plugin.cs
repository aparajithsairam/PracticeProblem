using System.Runtime.InteropServices;
using UnityEngine;

public static class Plugin
{
    [DllImport("Plugin.dll")]
    public static extern float CalculateTerrainDistance(
        byte[] heightMap,
        int aX, int aY,
        int bX, int bY);
}
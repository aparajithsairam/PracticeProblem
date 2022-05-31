using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Threading.Tasks;
using UnityEngine;
using UnityEngine.UI;

public class TerrainDistanceCalculator : MonoBehaviour
{
    Task<byte[]> preHeightMapReaderTask, postHeightMapReaderTask;
    public Text preDistText, postDistText, differenceText;
    public Button calcTerrainDistButton;

    public Text aXText, aYText, bXText, bYText;
    void Start()
    {
        preHeightMapReaderTask = ReadHeightMap(Path.Combine(Application.streamingAssetsPath, "pre.data"));
        postHeightMapReaderTask = ReadHeightMap(Path.Combine(Application.streamingAssetsPath, "post.data"));
    }

    async Task<byte[]> ReadHeightMap(string filePath)
    {
        return await Task.Run(() =>
        {
            return File.ReadAllBytes(filePath);
        });
    }

    public async void CalculateTerrainDistance()
    {
        calcTerrainDistButton.interactable = false;
        int aX = int.Parse(aXText.text);
        int aY = int.Parse(aYText.text);
        int bX = int.Parse(bXText.text);
        int bY = int.Parse(bYText.text);

        var t1 = CalculateTerrainDistance(preHeightMapReaderTask, aX, aY, bX, bY);
        _ = t1.ContinueWith((t) => 
            { preDistText.text = t.Result.ToString(); },
            TaskScheduler.FromCurrentSynchronizationContext());
        var t2 = CalculateTerrainDistance(postHeightMapReaderTask, aX, aY, bX, bY);
        _ = t2.ContinueWith((t) =>
            { postDistText.text = t.Result.ToString(); },
            TaskScheduler.FromCurrentSynchronizationContext());
        _ = Task.WhenAll(t1, t2).ContinueWith((t) =>
            {
                differenceText.text = (t1.Result - t2.Result).ToString();
                calcTerrainDistButton.interactable = true;
            },
            TaskScheduler.FromCurrentSynchronizationContext());
    }

    public async Task<float> CalculateTerrainDistance(
        Task<byte[]> htMapReaderTask,
        int aX, int aY, int bX, int bY)
    {
        var hts = await htMapReaderTask;
        Debug.Log("Reader task complete");

        return await Task.Run(() => Plugin.CalculateTerrainDistance(hts, aX, aY, bX, bY) );
    }
}

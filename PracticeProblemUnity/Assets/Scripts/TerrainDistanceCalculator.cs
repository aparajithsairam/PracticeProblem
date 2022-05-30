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
        var t1 = CalculateTerrainDistance(preHeightMapReaderTask);
        _ = t1.ContinueWith((t) => 
            { preDistText.text = 1.ToString(); },
            TaskScheduler.FromCurrentSynchronizationContext());
        var t2 = CalculateTerrainDistance(postHeightMapReaderTask);
        _ = t2.ContinueWith((t) =>
            { postDistText.text = 2.ToString(); },
            TaskScheduler.FromCurrentSynchronizationContext());
        _ = Task.WhenAll(t1, t2).ContinueWith((t) =>
            {
                differenceText.text = 3.ToString();
                calcTerrainDistButton.interactable = true;
            },
            TaskScheduler.FromCurrentSynchronizationContext());
    }

    public async Task<float> CalculateTerrainDistance(Task<byte[]> htMapReaderTask)
    {
        var hts = await htMapReaderTask;
        Debug.Log("Reader task complete");
        await Task.Delay(2000);
        return 1;
    }
}

using GooglePlayGames;
using System.Collections;

using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class Title : MonoBehaviour
{
    public void Execute()
    {
        AudioManager.Instance.Emit(Sound.Button);

        SceneryManager.Instance.LoadScene("Game");
    }

    public void ReaderBoard()
    {
        PlayGamesPlatform.Instance.ShowLeaderboardUI();
    }

    public void Achievements()
    {
        PlayGamesPlatform.Instance.ShowAchievementsUI();
    }

    public void Guide()
    {
        Application.OpenURL("file:///C:/GameGuide/Guide.html");
    }
}

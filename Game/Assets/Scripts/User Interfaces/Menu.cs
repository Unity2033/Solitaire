using GooglePlayGames;
using UnityEngine;

public class Menu : MonoBehaviour
{
    public void Execute()
    {
        AudioManager.Instance.Emit(Sound.Button);

        SceneryManager.Instance.LoadScene("Game");
    }

    public void ReaderBoard()
    {
        AudioManager.Instance.Emit(Sound.Button);

        PlayGamesPlatform.Instance.ShowLeaderboardUI();
    }

    public void Achievements()
    {
        AudioManager.Instance.Emit(Sound.Button);

        PlayGamesPlatform.Instance.ShowAchievementsUI();
    }

    public void Guide()
    {
        AudioManager.Instance.Emit(Sound.Button);

        Application.OpenURL("https://unity2033.github.io/Solitaire/");
    }
}

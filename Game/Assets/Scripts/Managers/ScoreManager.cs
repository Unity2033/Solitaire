using GooglePlayGames;
using GooglePlayGames.BasicApi;
using TMPro;
using UnityEngine;
using UnityEngine.SocialPlatforms.Impl;

public class ScoreManager : MonoBehaviour
{
    [SerializeField] int score;

    [SerializeField] int streak;

    [SerializeField] TextMeshProUGUI scoreText;

    [SerializeField] TextMeshProUGUI [ ] streakTexts;

    const string leaderBoard = "CggIz_bK8VIQAhAA";

    private void Awake()
    {
        Load();
    }

    public void Succeed()
    {
        streak++;

        if (streak > streakTexts.Length)
        {
            for (int i = 0; i < streakTexts.Length; i++)
            {
                streakTexts[i].text = "-";
            }

            streak = 1; 
        }

        streakTexts[streak - 1].text = "x" + streak;

        score += 10 * streak;

        scoreText.text = score.ToString();
    }

    public void Failed()
    {
        for (int i = 0; i < streak; i++)
        {
            streakTexts[i].text = "-";
        }

        streak = 0;

        DataManager.Instance.Session.selections = 0;
    }

    public void Save()
    {
        DataManager.Instance.Session.score += score;

        AchievementManager.Instance.Achieve(Achievements.First, DataManager.Instance.Session);

        PlayGamesPlatform.Instance.ReportScore(DataManager.Instance.Session.score, leaderBoard, success => { Debug.Log(success); });
    }

    public void Load()
    {
        PlayGamesPlatform.Instance.LoadScores
        (
            leaderBoard,
            LeaderboardStart.PlayerCentered,
            1,
            LeaderboardCollection.Public,
            LeaderboardTimeSpan.AllTime,
            data =>
            {
                if (!data.Valid || data.Scores.Length == 0)
                    return;

                int leaderboardScore = (int)data.Scores[0].value;

                DataManager.Instance.Session.score = Mathf.Max( DataManager.Instance.Session.score, leaderboardScore);
            }
        );
    }
}

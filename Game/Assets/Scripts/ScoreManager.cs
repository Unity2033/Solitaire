using GooglePlayGames;
using GooglePlayGames.BasicApi;
using TMPro;
using UnityEngine;

public class ScoreManager : Singleton<ScoreManager>
{
    [SerializeField] int score;

    [SerializeField] int record;

    [SerializeField] int streak;

    [SerializeField] TextMeshProUGUI scoreText;

    [SerializeField] TextMeshProUGUI[] streakTexts;

    private void Start()
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
    }

    public void Save()
    {
        record += score;

        PlayGamesPlatform.Instance.ReportScore(record, "Leader Board", success => { Debug.Log(success); });
    }

    public void Load()
    {
        PlayGamesPlatform.Instance.LoadScores
        (
            "Leader Board",
            LeaderboardStart.PlayerCentered,
            1,
            LeaderboardCollection.Public,
            LeaderboardTimeSpan.AllTime,
            data => 
            {  
                 if (data.Valid && data.Scores.Length > 0)
                 {
                     record = (int)data.Scores[0].value;
                 }
                 else
                 {
                     record = 0;
                 }             
            }   
        );
    }
}

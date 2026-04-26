using TMPro;
using UnityEngine;

public class ScoreManager : Singleton<ScoreManager>
{
    [SerializeField] int score;

    [SerializeField] int streak;

    [SerializeField] TextMeshProUGUI scoreText;

    [SerializeField] TextMeshProUGUI[] streakTexts;

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
}

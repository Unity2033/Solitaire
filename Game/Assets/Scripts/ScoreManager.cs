using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class ScoreManager : Singleton<ScoreManager>
{
    [SerializeField] int score;
    [SerializeField] TextMeshProUGUI scoreText;

    [SerializeField] int combo;
    [SerializeField] int maximumValue;

    public void Increase()
    {
        combo = combo + 1;

        score += 10 * combo;

        if(combo >= maximumValue)
        {
            combo = 0;
        }

        scoreText.text = score.ToString();
    }

    public void Reset()
    {
        combo = 0;
    }
}

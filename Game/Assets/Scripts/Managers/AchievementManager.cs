using GooglePlayGames;
using UnityEngine;

public class AchievementManager : Singleton<AchievementManager>
{
    [SerializeField] Achievements achievements;

    private void Unlock(int index)
    {
        PlayGamesPlatform.Instance.UnlockAchievement(achievements.missions[index]);
    }

    public void ScoreReached(Session session)
    {
        if(session.score >= 10000)
        {
            Unlock(0);
        }
    }
}

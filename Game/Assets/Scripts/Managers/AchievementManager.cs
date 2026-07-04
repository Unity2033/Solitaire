using GooglePlayGames;
using UnityEngine;

public class AchievementManager : Singleton<AchievementManager>
{
    [SerializeField] string[] missions = new string[3] 
    {
        GPGSIds.achievement_point_hunter,
        GPGSIds.achievement_smooth_selection, 
        GPGSIds.achievement_chain_reaction
    };

    private void Unlock(Achievements achievements)
    {
        PlayGamesPlatform.Instance.UnlockAchievement(missions[(int)achievements]);
    }

    public void Achieve(Achievements achievements, Session session)
    {
        switch(achievements)
        {
            case Achievements.First : if (session.score >= 10000) { Unlock(Achievements.First); }          
                break;
            case Achievements.Second: if (session.selections >= 10) { Unlock(Achievements.Second); }
                break;
            case Achievements.Third : if (session.draws >= 10) { Unlock(Achievements.Third); }
                break;
        }
    }
}

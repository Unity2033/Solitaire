using GooglePlayGames;
using GooglePlayGames.BasicApi;
using UnityEngine;
using UnityEngine.SocialPlatforms;

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

    public void Compare()
    {
        AudioManager.Instance.Emit(Sound.Button);

        PlayGamesPlatform.Instance.localUser.LoadFriends(success =>
        {
            var local = PlayGamesPlatform.Instance.localUser;

            var friends = local.friends;

            string localId = local.id;

            string identifier;
            string name;

            if (friends != null && friends.Length > 0)
            {
                var friend = friends[UnityEngine.Random.Range(0, friends.Length)];

                identifier = friend.id;
                name = friend.userName;
            }
            else
            {
                Debug.Log("No friends → fallback self compare");

                identifier = localId;
                name = local.userName;
            }

            PlayGamesPlatform.Instance.ShowCompareProfileWithAlternativeNameHintsUI(localId, identifier,name,(success) => {Debug.Log("Compare UI closed");});
        });
    }
}

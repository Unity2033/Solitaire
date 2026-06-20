using GooglePlayGames;
using GooglePlayGames.BasicApi;
using GooglePlayGames.BasicApi.SavedGame;
using System;
using System.Text;
using UnityEngine;

public class DataManager : Singleton<DataManager>
{
    public Session Session => session;

    [SerializeField] Session session = new Session();

    private ISavedGameClient SavedGameClient => PlayGamesPlatform.Instance.SavedGame;

    private void Start()
    {
        Load(Read);
    }

    public void Save()
    {
        SavedGameClient.OpenWithAutomaticConflictResolution("Data", DataSource.ReadCacheOrNetwork, ConflictResolutionStrategy.UseLongestPlaytime, Access);
    }

    private void Access(SavedGameRequestStatus status, ISavedGameMetadata game)
    {
        if (status != SavedGameRequestStatus.Success)
        {
            Debug.LogError("Save Open Failed");

            return;
        }

        byte [ ] bytes = Encoding.UTF8.GetBytes(JsonUtility.ToJson(session));

        var update = new SavedGameMetadataUpdate.Builder().Build();

        PlayGamesPlatform.Instance.SavedGame.CommitUpdate
        (
            game,
            update,
            bytes,
            (status, game) => Debug.Log("Save")
        );
    }

    public void Load(Action<SavedGameRequestStatus, ISavedGameMetadata> callback)
    {
        SavedGameClient.OpenWithAutomaticConflictResolution
        (
            "Data",
            DataSource.ReadCacheOrNetwork,
            ConflictResolutionStrategy.UseLongestPlaytime,
            callback
        );
    }

    private void Read(SavedGameRequestStatus status, ISavedGameMetadata game)
    {
        if (status != SavedGameRequestStatus.Success)
        {
            Debug.LogError("Load Open Failed");

            return;
        }

        SavedGameClient.ReadBinaryData(game, (readStatus, data) =>
        {
            if (readStatus != SavedGameRequestStatus.Success || data == null)
            {
                Debug.LogError("Load Failed");

                return;
            }

            session = JsonUtility.FromJson<Session>(Encoding.UTF8.GetString(data));
        });
    }
}

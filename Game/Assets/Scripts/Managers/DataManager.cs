using GooglePlayGames;
using GooglePlayGames.BasicApi;
using GooglePlayGames.BasicApi.SavedGame;
using System;
using System.Text;
using UnityEngine;

public class DataManager : Singleton<DataManager>
{
    public Session Session { get; } = new Session();

    public State State { get; private set; } = State.NotLoaded;

    private ISavedGameClient SavedGameClient => PlayGamesPlatform.Instance.SavedGame;

    private const string SaveName = "Data";

    public void Load()
    {
        if (PlayGamesPlatform.Instance.localUser.authenticated == false)
        {
            Debug.LogWarning("GPGS not authenticated yet");
            State = State.Failed;
            return;
        }

        State = State.Loading;

        SavedGameClient.OpenWithAutomaticConflictResolution(
            SaveName,
            DataSource.ReadCacheOrNetwork,
            ConflictResolutionStrategy.UseLongestPlaytime,
            OnLoadOpened
        );
    }

    private void OnLoadOpened(SavedGameRequestStatus status, ISavedGameMetadata game)
    {
        if (status != SavedGameRequestStatus.Success)
        {
            Debug.LogError("Save Open Failed");
            State = State.Failed;
            return;
        }

        SavedGameClient.ReadBinaryData(game, OnDataRead);
    }

    private void OnDataRead(SavedGameRequestStatus status, byte[] data)
    {
        if (status != SavedGameRequestStatus.Success || data == null || data.Length == 0)
        {
            Debug.Log("No save found. Using default session.");
            State = State.Ready;
            return;
        }

        try
        {
            string json = Encoding.UTF8.GetString(data);

            // Session 객체를 유지하고 내용만 덮어쓴다.
            JsonUtility.FromJsonOverwrite(json, Session);

            State = State.Ready;

            Debug.Log("Data Load Complete");
        }
        catch (Exception exception)
        {
            Debug.LogError($"Deserialize Failed : {exception}");
            State = State.Ready;
        }
    }

    public void Save()
    {
        if (State == State.NotLoaded || State == State.Loading)
            return;

        if (!PlayGamesPlatform.Instance.localUser.authenticated)
            return;

        SavedGameClient.OpenWithAutomaticConflictResolution(
            SaveName,
            DataSource.ReadCacheOrNetwork,
            ConflictResolutionStrategy.UseLongestPlaytime,
            OnSaveOpened
        );
    }

    private void OnSaveOpened(SavedGameRequestStatus status, ISavedGameMetadata game)
    {
        if (status != SavedGameRequestStatus.Success)
        {
            Debug.LogError("Save Open Failed");
            return;
        }

        byte[] bytes = Encoding.UTF8.GetBytes(JsonUtility.ToJson(Session));

        var update = new SavedGameMetadataUpdate.Builder().Build();

        SavedGameClient.CommitUpdate(game, update, bytes, (status, metadata) =>
        {
            if (status == SavedGameRequestStatus.Success)
                Debug.Log("Save Complete");
            else
                Debug.LogError($"Save Failed : {status}");
        });
    }
}
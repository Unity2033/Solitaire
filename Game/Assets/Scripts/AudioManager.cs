using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class AudioManager : Singleton<AudioManager>
{
    [SerializeField] AudioSource effectAudioSource;
    [SerializeField] AudioSource sceneryAudioSource;

    [SerializeField] Dictionary<Sound, AudioClip> dictionary;

    void Start()
    {
        dictionary = new Dictionary<Sound, AudioClip>();
    }

    void OnEnable()
    {
        SceneManager.sceneLoaded += OnSceneLoaded;
    }


    public void Emit(Sound name)
    {
        if (dictionary.TryGetValue(name, out AudioClip audioClip) == false)
        {
            audioClip = Resources.Load<AudioClip>(name.ToString());

            if (audioClip == null)
            {
                Debug.LogWarning($"Audio not found: {name}");

                return;
            }

            dictionary.Add(name, audioClip);
        }

        effectAudioSource.PlayOneShot(audioClip);
    }

    void OnSceneLoaded(Scene scene, LoadSceneMode mode)
    {
        sceneryAudioSource.clip = Resources.Load<AudioClip>(scene.name);

        sceneryAudioSource.Play();
    }

    void OnDisable()
    {
        SceneManager.sceneLoaded -= OnSceneLoaded;
    }

}

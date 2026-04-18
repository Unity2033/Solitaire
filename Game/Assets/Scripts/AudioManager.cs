using System.Collections.Generic;
using UnityEngine;

public class AudioManager : Singleton<AudioManager>
{
    [SerializeField] AudioSource audioSource;

    [SerializeField] Dictionary<string, AudioClip> dictionary;

    void Start()
    {
        dictionary = new Dictionary<string, AudioClip>();
    }

    public void Emit(string name)
    {
        if (!dictionary.TryGetValue(name, out AudioClip audioClip))
        {
            audioClip = Resources.Load<AudioClip>(name);

            if (audioClip == null)
            {
                Debug.LogWarning($"Audio not found: {name}");

                return;
            }

            dictionary.Add(name, audioClip);
        }

        audioSource.PlayOneShot(audioClip);
    }
}

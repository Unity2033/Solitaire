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
        AudioClip audioClip = null;

        if (dictionary.TryGetValue(name, out audioClip) == false)
        {
            dictionary.Add(name, Resources.Load<AudioClip>(name));

            audioClip = dictionary[name];
        }

        audioSource.PlayOneShot(audioClip);
    }
}

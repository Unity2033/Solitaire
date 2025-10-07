using System.Collections.Generic;
using UnityEngine;

public class AudioManager : MonoBehaviour
{
    [SerializeField] AudioSource audioSource;

    [SerializeField] Dictionary<string, AudioClip> dictionary;

    private void Awake()
    {
        dictionary = new Dictionary<string, AudioClip>();
    }

    void Start()
    {
        
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

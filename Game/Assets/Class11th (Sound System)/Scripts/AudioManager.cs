using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AudioManager : MonoBehaviour
{
    [SerializeField] AudioSource audioSource;

    public void Listener(AudioClip audioClip)
    {
        audioSource.PlayOneShot(audioClip);
    }
}

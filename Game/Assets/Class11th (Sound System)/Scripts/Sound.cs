using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Sound : MonoBehaviour
{
    [SerializeField] float time = 5;

    [SerializeField] AudioClip audioClip;

    [SerializeField] AudioManager audioManager;

    private void Start()
    {
        time = Random.Range(1, 6);
    }

    private void Update()
    {
        time -= Time.deltaTime;

        if(time <= 0)
        {
            audioManager.Listener(audioClip);

            time = Random.Range(1, 6);
        }
    }


}

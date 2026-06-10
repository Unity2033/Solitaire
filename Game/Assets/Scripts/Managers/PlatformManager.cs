using DG.Tweening;
using GooglePlayGames;
using GooglePlayGames.BasicApi;
using System;
using System.Collections;
using UnityEngine;
using UnityEngine.UI;

public class PlatformManager : MonoBehaviour
{
    [SerializeField] Slider slider;
    [SerializeField] RectTransform failurePanel;

    void Awake()
    {      
        Connect();
    } 

    void Connect()
    {
        PlayGamesPlatform.Instance.Authenticate(status => { if (status == SignInStatus.Success) { StartCoroutine(Success()); } else {  Failed(); } });
    }

    IEnumerator Transition(float start, float end, float duration = 1.0f)
    {
        float time = 0;

        slider.value = start;

        while (time < duration)
        {
            time += Time.deltaTime;

            slider.value = Mathf.Lerp( start, end, time / duration);

            yield return null;
        }

        slider.value = end;
    }

    public IEnumerator Success()
    {
        yield return StartCoroutine(Transition(0f, 1f));

        SceneryManager.Instance.LoadScene("Title");
    }

    public void Failed()
    {
        failurePanel.DOKill();

        failurePanel.gameObject.SetActive(true);

        failurePanel.localScale = Vector3.zero;

        failurePanel.DOScale(1f, 0.5f).SetEase(Ease.OutBounce);
    }
}


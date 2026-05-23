using DG.Tweening;
using GooglePlayGames;
using GooglePlayGames.BasicApi;
using System;
using System.Collections;
using System.Threading.Tasks;
using UnityEditor.MPE;
using UnityEngine;

public class PlatformManager : MonoBehaviour
{
    [SerializeField] RectTransform failurePanel;

    void Awake()
    {      
        StartCoroutine(Connect());
    } 

    IEnumerator Connect()
    {
        SignInStatus? result = null;

        try
        {
            PlayGamesPlatform.Instance.Authenticate(status => { result = status; });
        }
        catch (Exception exception)
        {
            Debug.LogError(exception);

            yield break;
        }

        yield return new WaitUntil(() => result.HasValue);

        if (result == SignInStatus.Success)
        {
            SceneryManager.Instance.LoadScene("Title");
        }
        else
        {
            failurePanel.gameObject.SetActive(true);

            failurePanel.localScale = Vector3.zero;

            failurePanel.DOScale(1f, 0.5f).SetEase(Ease.OutBounce);
        }
    }
}


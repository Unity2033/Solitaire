using System.Collections;
using UnityEngine;
using UnityEngine.Device;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class SceneryManager : Singleton<SceneryManager>
{
    [SerializeField] Image screen;
    [SerializeField] float duration;

    private Coroutine coroutine;

    private void OnEnable()
    {
        SceneManager.sceneLoaded += OnSceneLoaded;
    }

    public void LoadScene(string sceneName)
    {
        if (coroutine == null)
        {
            coroutine = StartCoroutine(SceneRoutine(sceneName));
        }
    }

    IEnumerator SceneRoutine(string sceneName)
    {
        yield return StartCoroutine(Fade(0, 1));

        AsyncOperation asyncOperation = SceneManager.LoadSceneAsync(sceneName);

        while (asyncOperation.isDone == false)
        {
            yield return null;
        }

        coroutine = null;
    }

    IEnumerator Fade(float start, float end)
    {
        float time = 0;

        Color color = screen.color;

        while (time < duration)
        {
            time += Time.deltaTime;

            color.a = Mathf.Lerp(start, end, time / duration);

            screen.color = color;

            yield return null;
        }

        color.a = end;

        screen.color = color;
    }


    private void OnDisable()
    {
        SceneManager.sceneLoaded -= OnSceneLoaded;
    }

    private void OnSceneLoaded(Scene scene, LoadSceneMode mode)
    {
        StartCoroutine(Fade(1, 0));
    }
}

using UnityEngine;

public class Singleton<T> : MonoBehaviour where T : MonoBehaviour
{
    private static T instance;

    public static T Instance { get { return instance; } }

    protected virtual void Awake()
    {
        if (instance == null)
        {
            instance = (T)FindAnyObjectByType(typeof(T));
        }
        else
        {
            Destroy(gameObject);
        }
    }
}
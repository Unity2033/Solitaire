using UnityEngine;

public class Singleton<T> : MonoBehaviour where T : MonoBehaviour
{
    private static T instance;

    public static T Instance
    {
        get
        {
            if (instance == null)
            {
                instance = (T)FindAnyObjectByType(typeof(T));

                if (instance == null)
                {
                    GameObject clone = new GameObject(typeof(T).Name);

                    instance = clone.AddComponent<T>();
                }

                DontDestroyOnLoad(instance.gameObject);
            }

            DontDestroyOnLoad(instance.gameObject);

            return instance;
        }
    }

    protected void Awake()
    {
        if (instance != null)
        {
            Destroy(gameObject);
        }
    }
}
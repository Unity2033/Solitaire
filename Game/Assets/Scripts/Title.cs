using System.Collections;

using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class Title : MonoBehaviour
{
    public void Execute()
    {
        AudioManager.Instance.Emit(Sound.Button);

        SceneryManager.Instance.LoadScene("Game");
    }

}

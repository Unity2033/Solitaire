using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Selectable : MonoBehaviour
{
    [SerializeField] Text titleText;

    public void Entered()
    {
        titleText.fontSize = 100;
    }
    
    public void Exited()
    {
        titleText.fontSize = 75;
    }

    public void Seleted()
    {
        titleText.fontSize = 50;
    }
}

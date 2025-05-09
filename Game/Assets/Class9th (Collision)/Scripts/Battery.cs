using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Battery : MonoBehaviour
{
    [SerializeField] int temperature;

    public void Increase()
    {
        temperature++;

        Debug.Log("temperature : " + temperature);
    }
}

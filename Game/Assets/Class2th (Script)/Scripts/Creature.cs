using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Creature : MonoBehaviour
{
    private Stat stat = new Stat();

    public Creature()
    {
        stat.Level = 1;
        stat.Health = 100;
        stat.Name = "Bear";

        Debug.Log("Level : " + stat.Level);
        Debug.Log("Health : " + stat.Health);
        Debug.Log("Name : " + stat.Name);
    }
}

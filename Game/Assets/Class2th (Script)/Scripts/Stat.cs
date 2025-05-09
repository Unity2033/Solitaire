using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Stat
{
    private int level;
    private int health;
    private string name;
    
    public int Level 
    { 
        get { return level; }
        set { level = value;  } 
    }

    public int Health { get { return health; } set { health = value; } }

    public string Name { get { return name; } set { name = value; } }
}

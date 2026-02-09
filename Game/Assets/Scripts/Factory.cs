using NUnit.Framework;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Factory
{
    private Image prefab;

    public Factory(Image image) 
    {
        prefab = image;
    }

    public List<Image> Create(int count)
    {
        List<Image> list = new();

        Suit suit = 0;

        for(int i = 0; i < count; i++)
        {
            if(i != 0 && i % 13 == 0)
            {
                suit++;
            }

            Image clone = Object.Instantiate(prefab);

            clone.GetComponent<Card>().Initialize((i % 13) + 1, suit, suit.ToString() + " " + ((i % 13) + 1));

            list.Add(clone);
        }

        return list;
    }
}

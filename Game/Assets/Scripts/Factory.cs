using NUnit.Framework;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Factory
{
    private Card prefab;

    public Factory(Card clone) 
    {
        prefab = clone;
    }

    public List<Card> Create(int count)
    {
        List<Card> list = new();

        Suit suit = 0;

        for(int i = 0; i < count; i++)
        {
            if(i != 0 && i % 13 == 0)
            {
                suit++;
            }

            Card card = Object.Instantiate(prefab);

            card.Initialize((i % 13) + 1, suit, suit.ToString() + " " + ((i % 13) + 1));

            list.Add(card);
        }

        return list;
    }
}

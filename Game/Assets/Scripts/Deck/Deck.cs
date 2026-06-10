using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Deck : MonoBehaviour
{
    [SerializeField] int createCount;

    [SerializeField] Factory factory;

    [SerializeField] List<Card> list;
    
    public int Count => list.Count;

    private void Awake()
    {
        factory = new Factory(Resources.Load<Card>("Prefabs/Card"));

        list.Capacity = createCount;

        list = factory.Create(createCount);

        foreach (Card card in list)
        {
            card.transform.SetParent(transform, false);
        }

        Shuffle();
    }

    public void Shuffle()
    {
        for (int i = 0; i < list.Count; i++)
        {
            int index = Random.Range(0, list.Count);

            (list[i], list[index]) = (list[index], list[i]);
        }
    }

    public Card Deal()
    {
        if (list.Count <= 0)
        {
            return null;
        }

        Card card = list[list.Count - 1];

        list.RemoveAt(list.Count - 1);

        return card; 
    }
}


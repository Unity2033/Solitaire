using System.Collections.Generic;
using UnityEngine;

public class GameManager : MonoBehaviour
{
    [SerializeField] int sum;
    [SerializeField] int index;

    [SerializeField] List<Card> list;

    private static GameManager instance;

    public static GameManager Instance { get { return instance; } }

    private void Awake()
    {
        instance = this;
    }

    public void Calculate(Card card)
    {
        if (card.Rank == 13)
        {
            card.gameObject.SetActive(false);

            return;
        }

        list.Add(card);

        if (list.Count == 2)
        {
            sum = list[0].Rank + list[1].Rank;

            if (sum == 13)
            {
                card.gameObject.SetActive(false);
            }

            list.Clear();
        }
    }
}

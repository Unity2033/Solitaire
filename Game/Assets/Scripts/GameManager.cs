using System.Collections.Generic;
using UnityEngine;

public class GameManager : MonoBehaviour
{
    [SerializeField] int sum;
    [SerializeField] List<Card> list;

    private static GameManager instance;

    public static GameManager Instance { get { return instance; } }

    private void Awake()
    {
        instance = this;
    }

    public void Calculate(Card card)
    {
        list.Add(card);

        if (list.Count > 1)
        {
            sum = list[0].Rank + list[1].Rank;

            if(sum == 13)
            {
                Debug.Log(sum);
            }

            list.Clear();
        }
    }
}

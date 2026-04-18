using System.Collections.Generic;
using System.Security;
using UnityEngine;
using UnityEngine.UI;

public class GameManager : Singleton<GameManager>
{
    [SerializeField] Waste waste;

    public bool Examine(Card card)
    {
        int difference = Mathf.Abs(card.Point - waste.Peek().Point);

        if (difference == 1)
        {
            waste.Push(card);

            ScoreManager.Instance.Increase();

            return true;
        }
        else
        {
            ScoreManager.Instance.Reset();

            return false;
        }
    }


}

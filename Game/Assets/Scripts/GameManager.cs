using System.Collections.Generic;
using System.Security;
using UnityEngine;
using UnityEngine.UI;

public class GameManager : Singleton<GameManager>
{
    [SerializeField] bool state;

    [SerializeField] Deck deck;
    [SerializeField] Waste waste;
    [SerializeField] TableauManager tableauManager;

    public bool Examine(Card card)
    {
        if (Rule.Fits(card, waste.Peek()))
        {
            card.ParentTableau.Remove(card);

            waste.Push(card);

            ScoreManager.Instance.Succeed();
            AudioManager.Instance.Emit(Sound.Slide);

            state = true;
        }
        else
        {
            ScoreManager.Instance.Failed();
            AudioManager.Instance.Emit(Sound.Failure);

            state = false;
        }

        Determine();

        return state;
    }

    public void Determine()
    {
        if (Rule.Resolved(tableauManager.Tableaus))
        {
            Debug.Log("V I C T O R Y");
        }
        else if (Rule.ExistsPlacement(tableauManager.Elements(), waste) == false && deck.Count == 0)
        {
            Debug.Log("D E F E A T");
        }    
    }

}

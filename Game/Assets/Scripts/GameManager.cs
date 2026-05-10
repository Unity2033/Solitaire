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
  
            card.OnSelectionSucceeded();

            waste.Push(card);

            ScoreManager.Instance.Succeed();
            AudioManager.Instance.Emit(Sound.Slide);

            state = true;
        }
        else
        {
            card.OnSelectionFailed();

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
            
        }
        else if (Rule.ExistsPlacement(tableauManager.Elements(), waste) == false && deck.Count == 0)
        {
            SceneryManager.Instance.LoadScene("Result");
        }
    }

}

using System.Collections.Generic;
using UnityEngine;

public static class Rule
{
    public static bool Fits(Card card, Card waste)
    {
        return Mathf.Abs(card.Point - waste.Point) == 1;
    }

    public static bool ExistsPlacement(IEnumerable<Card> cards , Waste waste)
    {
        if (waste == null) return false;

        foreach (Card card in cards)
        {
            if (card == null)
            {
                continue;
            }

            if (Fits(card, waste.Peek()))
            {
                return true;
            }
        }

        return false;
    }

    public static bool Resolved(IEnumerable<Tableau> tableaus)
    {
        foreach (Tableau tableau in tableaus)
        {
            if (tableau.Count > 0)
            {
                return false;
            }
        }

        return true;
    }
}

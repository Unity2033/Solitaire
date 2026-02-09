using System.Collections.Generic;
using System.Security;
using UnityEngine;

public class GameManager : Singleton<GameManager>
{
    [SerializeField] List<Card> list;

    public void Calculate(Card card)
    {
        if (list.Count >= 1)
        {
            if (list[0].Same(card))
            {
                list[0].GetComponent<Card>().Revert();

                list.RemoveAt(0);

                return;
            }
        }
            
        if (card.Point == 13)
        {
            card.gameObject.SetActive(false);

            return;
        }

        list.Add(card);

        if (list.Count >= 2)
        {
            if ((list[0].Point + list[1].Point) == 13)
            {
                list[0].gameObject.SetActive(false);
                list[1].gameObject.SetActive(false);

                list.Clear();
            }
            else
            {
                list[0].GetComponent<Card>().Revert();
              
                list.RemoveAt(0);
            }
        }
    }
}

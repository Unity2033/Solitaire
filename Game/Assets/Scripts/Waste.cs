using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Waste : MonoBehaviour
{
    [SerializeField] Deck deck;

    [SerializeField] Stack<Card> stack = new Stack<Card>();

    void Start()
    {
        Push(deck.Deal());
    }

    public Card Peek()
    {
        return stack.Peek();
    }

    public void Push(Card card)
    {
        card.transform.SetParent(transform);

        RectTransform rectTransform = card.GetComponent<RectTransform>();

        card.transform.localPosition = rectTransform.anchoredPosition; 

        rectTransform.anchoredPosition = Vector2.zero;

        stack.Push(card);
    }
}

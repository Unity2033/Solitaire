using DG.Tweening;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Waste : MonoBehaviour
{
    [SerializeField] Deck deck;

    [SerializeField] Stack<Card> stack = new Stack<Card>();

    [SerializeField] RectTransform slot;

    private void Awake()
    {
        slot = GetComponent<RectTransform>();
    }

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
        card.Animate(slot, Vector2.zero);

        stack.Push(card);
    }
}

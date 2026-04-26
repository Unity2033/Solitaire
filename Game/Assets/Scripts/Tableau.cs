using System;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using UnityEngine.UI;


public class Tableau : MonoBehaviour
{
    [SerializeField] Deck deck;

    [SerializeField] List<Card> cards = new();

    [SerializeField] int createCount = 5;

    public int Count => cards.Count;

    public Card Peek  => cards[cards.Count - 1];

    void Start()
    {
        deck = FindAnyObjectByType<Deck>();

        Initialize();

        Bind();
    }

    public void Initialize()
    {
        for (int i = 0; i < createCount; i++)
        {
            Card card = deck.Deal();

            cards.Add(card);

            card.ParentTableau = this;

            card.transform.SetParent(transform);

            RectTransform rectTransform = card.GetComponent<RectTransform>();

            rectTransform.anchoredPosition = new Vector2(0, -i * 100);
        }

        cards[cards.Count - 1].EnableSelection();
    }

    public void Bind()
    {
        for (int i = 1; i < createCount; i++)
        {
            cards[i].SetHierarchy(cards[i - 1]);
        }
    }

    public void Remove(Card card)
    {
        cards.Remove(card);

        if (cards.Count > 0)
        {
            cards[cards.Count - 1].EnableSelection();
        }
    }
}

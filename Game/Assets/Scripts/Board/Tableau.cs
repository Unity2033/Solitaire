using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using UnityEngine.UI;


public class Tableau : MonoBehaviour
{
    [SerializeField] Deck deck;

    [SerializeField] List<Card> cards = new();

    [SerializeField] int createCount = 5;

    [SerializeField] RectTransform rectTransform;

    public int Count => cards.Count;

    public Card Peek  => cards[cards.Count - 1];

    private void Awake()
    {
        rectTransform = GetComponent<RectTransform>();
    }

    void Start()
    {
        deck = FindAnyObjectByType<Deck>();

        StartCoroutine(Initialize());
    }

    private IEnumerator Initialize()
    {
        cards.Capacity = createCount;

        for (int i = 0; i < createCount; i++)
        {
            Card card = deck.Deal();

            cards.Add(card);

            card.ParentTableau = this;

            card.Animate(rectTransform, new Vector2(0, -i * 100));

            yield return new WaitForSeconds(0.1f);
        }

        cards[cards.Count - 1].EnableSelection();

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

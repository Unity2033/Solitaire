using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Tableau : MonoBehaviour
{
    [SerializeField] Deck deck;

    [SerializeField] List<Card> cards = new();

    [SerializeField] int createCount = 5;

    [SerializeField] RectTransform rectTransform;

    [SerializeField] float spacing = 100f;

    public int Count => cards.Count;

    public Card Peek  => cards[cards.Count - 1];

    float scale = Mathf.Clamp(Screen.height / 1920f, 0.9f, 1.2f);

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

            card.Animate(rectTransform, new Vector2(0, -i * spacing * scale));

            yield return new WaitForSeconds(0.1f);
        }

        cards[cards.Count - 1].SetSelection(true);
    }

    public void Remove(Card card)
    {
        cards.Remove(card);

        if (cards.Count > 0)
        {
            cards[cards.Count - 1].SetSelection(true);
        }
    }
}

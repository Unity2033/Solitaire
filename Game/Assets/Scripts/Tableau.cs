using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;


public class Tableau : MonoBehaviour
{
    [SerializeField] Deck deck;

    [SerializeField] List<Card> cards = new();

    [SerializeField] int count = 5;

    void Start()
    {
        deck = FindAnyObjectByType<Deck>();

        Initialize();

        Bind();
    }

    public void Initialize()
    {
        for (int i = 0; i < count; i++)
        {
            Card card = deck.Deal();

            cards.Add(card);

            card.transform.SetParent(transform);

            RectTransform rectTransform = card.GetComponent<RectTransform>();

            rectTransform.anchoredPosition = new Vector2(0, -i * 100);
        }

        cards[cards.Count - 1].EnableSelection();
    }

    public void Bind()
    {
        for (int i = 1; i < count; i++)
        {
            cards[i].SetHierarchy(cards[i - 1]);
        }
    }
}

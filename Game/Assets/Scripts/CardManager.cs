using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

[RequireComponent(typeof(PlacementManager))]
public class CardManager : MonoBehaviour
{
    [SerializeField] int createCount;

    [SerializeField] string cardName;

    [SerializeField] List<Image> list;
    [SerializeField] PlacementManager placementManager;

    [SerializeField] Suit suit;

    private void Awake()
    {
        placementManager = GetComponent<PlacementManager>();
    }

    void Start()
    {
        Create();

        placementManager.Placement(list);
    }

    void Create()
    {
        for (int i = 0; i < createCount; i++)
        {
            Image card = Instantiate(Resources.Load<Image>("Card"));

            if (i != 0 && i % 13 == 0)
            {
                suit++;
            }

            cardName = suit.ToString() + "_" + ((i % 13) + 1);

            card.GetComponent<Card>().Initialize((i % 13) + 1, suit, cardName);

            list.Add(card);
        }
    }
}


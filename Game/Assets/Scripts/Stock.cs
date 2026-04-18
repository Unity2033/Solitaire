using NUnit.Framework;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Stock : MonoBehaviour
{
    [SerializeField] Deck deck;
    [SerializeField] Waste waste;

    [SerializeField] Button button;

    void Start()
    {
        deck = FindAnyObjectByType<Deck>();
        waste = FindAnyObjectByType<Waste>();
    }

    public void Draw()
    { 
        waste.Push(deck.Deal());

        ScoreManager.Instance.Reset();
        
        if(deck.Count <= 0)
        {
            button.interactable = false;
        }         
    }
}

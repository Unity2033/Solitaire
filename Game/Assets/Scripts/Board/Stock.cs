using NUnit.Framework;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.Events;
using UnityEngine.UI;

public class Stock : MonoBehaviour
{
    [SerializeField] Deck deck;
    [SerializeField] Waste waste;

    [SerializeField] Button button;

    [SerializeField] TextMeshProUGUI textCount;

    public void Draw()
    {
        waste.Push(deck.Deal());

        GameManager.Instance.Determine();

        textCount.text = deck.Count.ToString();

        AudioManager.Instance.Emit(Sound.Draw);
        
        if(deck.Count <= 0)
        {
            button.gameObject.SetActive(false);
        }         
    }
}

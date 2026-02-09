using System;
using UnityEngine;
using UnityEngine.U2D;
using UnityEngine.UI;

public enum Suit
{
    Spade, Heart, Diamond, Club
}

public class Card : MonoBehaviour
{
    [SerializeField] Data data;

    [SerializeField] Image sprite;
    [SerializeField] Outline outLine;

    [SerializeField] Image [ ] childrens;

    [SerializeField] SpriteAtlas spriteAtlas;
    
    public event Action OnDisabled;

    public int Point { get { return data.Rank; } }

    private void Awake()
    {
        sprite = GetComponent<Image>();

        outLine = GetComponent<Outline>();
    }

    private void OnDisable()
    {
        OnDisabled?.Invoke();
    }

    public void Initialize(int rank, Suit suit, string spriteName)
    {
        data.Rank = rank;
        data.Suit = suit;

        sprite.sprite = spriteAtlas.GetSprite(spriteName);
    }

    public bool Same(Card card)
    {
        if(card.Point == data.Rank && data.Suit == card.data.Suit)
        {
            return true;
        }  
        
        return false;
    }

    public void SetHierarchy(Card left, Card right)
    {
        childrens[0] = left.GetComponent<Image>();
        childrens[1] = right.GetComponent<Image>();

        left.OnDisabled += Recalculate;     
        right.OnDisabled += Recalculate;

        Recalculate();
    }

    public void Recalculate()
    {
        foreach(var element in childrens)
        {
            if(element.gameObject.activeSelf == true)
            {
                sprite.raycastTarget = false;

                return;
            }
        }

        sprite.raycastTarget = true;
    }

    public void Select()
    {
        outLine.enabled = true;

        GameManager.Instance.Calculate(this);
    }

    public void Revert()
    {
        outLine.enabled = false;
    }
}

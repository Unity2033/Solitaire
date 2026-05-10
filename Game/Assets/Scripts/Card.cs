using System;
using UnityEngine;
using UnityEngine.U2D;
using UnityEngine.UI;
using DG.Tweening;

public enum Suit
{
    Spade, Heart, Diamond, Club
}

public class Card : MonoBehaviour
{
    [SerializeField] Data data;

    [SerializeField] Image sprite;

    [SerializeField] Card parent;

    [SerializeField] SpriteAtlas spriteAtlas;
    
    public Tableau ParentTableau { get; set; }

    public int Point { get { return data.Rank; } }

    private void Awake()
    {
        sprite = GetComponent<Image>();
    }

    public void Initialize(int rank, Suit suit, string spriteName)
    {
        data.Rank = rank;
        data.Suit = suit;

        sprite.sprite = spriteAtlas.GetSprite(spriteName);
    }

    public void SetHierarchy(Card parentCard)
    {
        parent = parentCard;
    }

    public void EnableSelection()
    {
        sprite.raycastTarget = true;
    }

    public void OnSelectionFailed()
    {
        transform.DOKill();

        DOTween.Sequence()
        .Append(transform.DORotate(new Vector3(0, 0, 15f), 0.1f))
        .Append(transform.DORotate(new Vector3(0, 0, -10f), 0.1f))
        .Append(transform.DORotate(Vector3.zero, 0.2f));
    }

    public void OnSelectionSucceeded()
    {
        if (parent != null)
        {
            parent.sprite.raycastTarget = true;
        }

        sprite.raycastTarget = false;
    }

    public void Animate(RectTransform slot, Vector2 destination)
    {
        RectTransform rectTransform = GetComponent<RectTransform>();

        rectTransform.SetParent(slot, true);

        rectTransform.DOKill();

        rectTransform.DOAnchorPos(destination, 0.5f);
    }
}

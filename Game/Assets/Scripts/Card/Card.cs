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
    [SerializeField] Image sprite;

    [SerializeField] SpriteAtlas spriteAtlas;
    
    public Tableau ParentTableau { get; set; }

    public int Point { get; set; }

    public Suit Suit { get; set; }

    private void Awake()
    {
        sprite = GetComponent<Image>();
    }

    public void Initialize(int point, Suit suit, string spriteName)
    {
        Suit = suit;
        Point = point;

        sprite.sprite = spriteAtlas.GetSprite(spriteName);
    }

    public void SetSelection(bool state)
    {
        sprite.raycastTarget = state;
    }

    public void OnSelectionFailed()
    {
        transform.DOKill();

        DOTween.Sequence()
        .Append(transform.DORotate(new Vector3(0, 0, 15f), 0.1f))
        .Append(transform.DORotate(new Vector3(0, 0, -10f), 0.1f))
        .Append(transform.DORotate(Vector3.zero, 0.2f));
    }

    public void Animate(RectTransform slot, Vector2 destination)
    {
        RectTransform rectTransform = GetComponent<RectTransform>();

        rectTransform.SetParent(slot, true);

        rectTransform.DOKill();

        rectTransform.DOAnchorPos(destination, 0.5f);
    }
}

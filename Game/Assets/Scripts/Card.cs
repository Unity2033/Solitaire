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

    [SerializeField] Card parent;
    
    [SerializeField] Animator animator;

    [SerializeField] SpriteAtlas spriteAtlas;
    
    public Tableau ParentTableau { get; set; }

    public int Point { get { return data.Rank; } }

    private void Awake()
    {
        sprite = GetComponent<Image>();

        animator = GetComponent<Animator>();
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

    public void Select()
    {
        if (GameManager.Instance.Examine(this))
        {
            if (parent != null)
            {
                parent.sprite.raycastTarget = true;
            }

            sprite.raycastTarget = false;
        }
        else
        {
            animator.Play("Selection Failed");
        }
    }
}

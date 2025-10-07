using UnityEngine;
using UnityEngine.U2D;
using UnityEngine.UI;

using UnityEngine.EventSystems;
using Unity.VisualScripting;
public enum Suit
{
    Spade, Heart, Diamond, Club
}

public class Card : MonoBehaviour
{
    [SerializeField] Suit suit;

    [SerializeField] Image sprite;
    [SerializeField] Image [ ] childrens;

    [SerializeField] SpriteAtlas spriteAtlas;

    [field: SerializeField] public int Rank { get; private set; }

    private void Awake()
    {
        sprite = GetComponent<Image>();
    }

    public void Initialize(int rank, Suit suit, string spriteName)
    {
        Rank = rank;
        this.suit = suit;

        sprite.sprite = spriteAtlas.GetSprite(spriteName);
    }

    public void SetHierarchy(Image left, Image right)
    {
        childrens[0] = left;
        childrens[1] = right;
    }

    public void Select()
    {
        GameManager.Instance.Calculate(this);
    }
}

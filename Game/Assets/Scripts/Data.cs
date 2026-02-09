using UnityEngine;

[System.Serializable]
public class Data
{
    [SerializeField] int rank;
    [SerializeField] Suit suit;

    public int Rank { set { rank = value; } get { return rank; } }

    public Suit Suit { set { suit = value; } get { return suit; } }
}

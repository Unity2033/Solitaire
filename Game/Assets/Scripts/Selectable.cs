using UnityEngine;

public class Selectable : MonoBehaviour
{
    [SerializeField] Card card;

    public void Awake()
    {
        card = GetComponent<Card>();
    }

    public void Select()
    {
        GameManager.Instance.Examine(card);
    }
}

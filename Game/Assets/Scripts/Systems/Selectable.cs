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
        DataManager.Instance.Session.draws = 0;

        GameManager.Instance.Examine(card);
    }
}

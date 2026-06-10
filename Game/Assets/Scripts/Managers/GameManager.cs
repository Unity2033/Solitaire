using DG.Tweening;
using UnityEngine;
using UnityEngine.UI;

public class GameManager : MonoBehaviour
{
    [SerializeField] Deck deck;
    [SerializeField] Waste waste;
    [SerializeField] ScoreManager scoreManager;
    [SerializeField] TableauManager tableauManager;

    [SerializeField] Image summary;
    [SerializeField] RectTransform resultPanel;

    public static GameManager Instance { get; private set; }

    private void Awake()
    {
        if(Instance != null && Instance != this)
        {
            Destroy(gameObject);
        }
        else
        {
            Instance = this;
        }
    }

    public void Examine(Card card)
    {
        if (Rule.Fits(card, waste.Peek()))
        {
            card.ParentTableau.Remove(card);
  
            card.OnSelectionSucceeded();

            waste.Push(card);

            scoreManager.Succeed();

            AudioManager.Instance.Emit(Sound.Slide);
        }
        else
        {
            card.OnSelectionFailed();

            scoreManager.Failed();

            AudioManager.Instance.Emit(Sound.Incorrect);
        }

        Determine();
    }

    public void Determine()
    {
        if (Rule.Resolved(tableauManager.Tableaus))
        {
            AudioManager.Instance.Emit(Sound.Success);

            Animate("Sprites/Success", Ease.OutBack);

            scoreManager.Save();
        }
        else if (Rule.ExistsPlacement(tableauManager.Elements(), waste) == false && deck.Count == 0)
        {
            AudioManager.Instance.Emit(Sound.Failure);

            Animate("Sprites/Failure", Ease.OutBounce);

            scoreManager.Save();
        }
    }

    private void Animate(string result, Ease ease)
    {
        summary.sprite = Resources.Load<Sprite>(result);

        resultPanel.gameObject.SetActive(true);

        resultPanel.localScale = Vector3.zero;

        resultPanel.DOScale(1f, 0.5f).SetEase(ease);
    }

    public void Resume()
    {
        AudioManager.Instance.Emit(Sound.Button);

        SceneryManager.Instance.LoadScene("Title");
    }

}

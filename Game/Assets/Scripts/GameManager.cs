using DG.Tweening;
using System.Collections.Generic;
using System.Security;
using UnityEngine;
using UnityEngine.UI;
using static UnityEngine.Rendering.DebugUI;

public class GameManager : Singleton<GameManager>
{
    [SerializeField] bool state;

    [SerializeField] Deck deck;
    [SerializeField] Waste waste;
    [SerializeField] TableauManager tableauManager;

    [SerializeField] Image summary;
    [SerializeField] RectTransform resultPanel;

    public bool Examine(Card card)
    {
        if (Rule.Fits(card, waste.Peek()))
        {
            card.ParentTableau.Remove(card);
  
            card.OnSelectionSucceeded();

            waste.Push(card);

            ScoreManager.Instance.Succeed();

            AudioManager.Instance.Emit(Sound.Slide);

            state = true;
        }
        else
        {
            card.OnSelectionFailed();

            ScoreManager.Instance.Failed();
            AudioManager.Instance.Emit(Sound.Failure);

            state = false;
        }

        Determine();

        return state;
    }

    public void Determine()
    {
        if (Rule.Resolved(tableauManager.Tableaus))
        {
            Animate("Success", Ease.OutBack);

            ScoreManager.Instance.Save();
        }
        else if (Rule.ExistsPlacement(tableauManager.Elements(), waste) == false && deck.Count == 0)
        {
            Animate("Failure", Ease.OutBounce);

            ScoreManager.Instance.Save();
        }
    }

    private void Animate(string result, Ease ease)
    {
        summary.sprite = Resources.Load<Sprite>(result);

        resultPanel.gameObject.SetActive(true);

        resultPanel.localScale = Vector3.zero;

        resultPanel.DOScale(1f, 0.5f).SetEase(ease);
    }

}

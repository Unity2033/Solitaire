using System;
using System.Collections.Generic;
using System.Threading;
using UnityEngine;
using UnityEngine.UI;

public class TableauManager : MonoBehaviour
{
    [SerializeField] int createCount = 7;

    [SerializeField] float offest = 450.0f;

    [SerializeField] List<Tableau> tableaus;

    public IEnumerable<Tableau> Tableaus => tableaus;

    private void Awake()
    {
        Placement();
    }

    public void Placement()
    {
        tableaus.Capacity = createCount;

        for (int i = 0; i < createCount; i++)
        {
            Tableau clone = Instantiate(Resources.Load<Tableau>("Prefabs/Tableau"), transform);

            RectTransform rectTransform = clone.GetComponent<RectTransform>();

            rectTransform.anchoredPosition = new Vector2(-offest + i * 150, 0);

            tableaus.Add(clone);
        }
    }

    public IEnumerable<Card> Elements()
    {
        foreach (Tableau tableau in tableaus)
        {
            if (tableau.Count <= 0)
            {
                continue;
            }

            yield return tableau.Peek;
        }
    }
}

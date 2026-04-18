using System.Collections.Generic;
using System.Threading;
using UnityEngine;
using UnityEngine.UI;

public class TableauManager : MonoBehaviour
{
    [SerializeField] int createCount = 7;

    [SerializeField] float offest = 450.0f;

    [SerializeField] List<RectTransform> Tableaus;

    private void Awake()
    {
        Placement();
    }

    public void Placement()
    {
        for(int i = 0; i < createCount; i++)
        {
            RectTransform clone = Instantiate(Resources.Load<RectTransform>("Tableau"), transform);

            clone.anchoredPosition = new Vector2(-offest + i * 150, 0);

            Tableaus.Add(clone);
        }
    }

    public bool Determine()
    {
        foreach(RectTransform tableau in Tableaus)
        {
            if(Tableaus.Count > 0)
            {
                return false;
            }
        }

        return true;
    }

}

using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class RelationManager : MonoBehaviour
{
    [SerializeField] int count;

    [SerializeField] List<List<Image>> hierarchy;

    void Awake()
    {
        hierarchy = new List<List<Image>>();
    }

    public void SetChildren(int layer = 6)
    {
        for (int i = 0; i < layer; i++)
        {
            for (int j = 0; j <= i; j++)
            {
                Image left = hierarchy[i + 1][j];

                Image right = hierarchy[i + 1][j + 1];

                hierarchy[i][j].GetComponent<Card>().SetHierarchy(left.GetComponent<Card>(), right.GetComponent<Card>());
            }
        }
    }

    public void SetHierarchy(List<Image> images,int row = 7)
    {
        for (int i = 0; i < row; i++)
        {
            hierarchy.Add(new List<Image>());

            for (int j = 0; j <= i; j++)
            {
                hierarchy[i].Add(images[count++]);
            }
        }
    }
}

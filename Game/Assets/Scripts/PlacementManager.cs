using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PlacementManager : MonoBehaviour
{
    [SerializeField] int count;

    [SerializeField] float offset;

    [SerializeField] float positionX = 0;

    [SerializeField] Transform parentTransform;

    public void Placement(List<Image> images, int row = 7)
    {
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j <= i; j++)
            {
                if (j != 0)
                {
                    positionX += 100;
                }
                else
                {
                    positionX = -offset * i;
                }

                images[count].transform.SetParent(parentTransform);

                images[count++].rectTransform.anchoredPosition = new Vector3(positionX, offset * -i, 0);
            }
        }
    }
}

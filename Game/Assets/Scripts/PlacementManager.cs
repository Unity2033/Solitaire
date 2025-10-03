using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PlacementManager : MonoBehaviour
{
    [SerializeField] int random;

    [SerializeField] float offset;

    [SerializeField] float positionX = 0;

    [SerializeField] Transform parentTransform;

    public void Placement(List<Image> images, int count = 7)
    {
        for (int i = 0; i < count; i++)
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

                random = Random.Range(0, images.Count);

                images[random].transform.SetParent(parentTransform);

                images[random].rectTransform.anchoredPosition = new Vector3(positionX, offset * -i, 0);

                images.Remove(images[random]);
            }
        }
    }
}

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Mouse : MonoBehaviour
{
    [SerializeField] Ray ray;
    [SerializeField] LayerMask layerMask;
    [SerializeField] RaycastHit rayCastHit;

    void Update()
    {
        if (Input.GetMouseButtonDown(0))
        {
            ray = Camera.main.ScreenPointToRay(Input.mousePosition);

            if(Physics.Raycast(ray, out rayCastHit, Mathf.Infinity, layerMask))
            {
                GameObject prefab = rayCastHit.collider.gameObject;

                if(prefab.transform.GetChild(0).gameObject.activeSelf)
                {
                    prefab.transform.GetChild(0).gameObject.SetActive(false);
                }
                else
                {
                    prefab.transform.GetChild(0).gameObject.SetActive(true);
                }
            }
        }
    }
}

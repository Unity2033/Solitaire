using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

[RequireComponent(typeof(RelationManager))]
[RequireComponent(typeof(PlacementManager))]
public class Deck : MonoBehaviour
{
    [SerializeField] int createCount;

    [SerializeField] Factory factory;

    [SerializeField] List<Image> list;

    [SerializeField] RelationManager relationManager;
    [SerializeField] PlacementManager placementManager;

    private void Awake()
    {
        relationManager = GetComponent<RelationManager>();

        placementManager = GetComponent<PlacementManager>();

        factory = new Factory(Resources.Load<Image>("Card"));

        list = factory.Create(createCount);
    }

    void Start()
    {
        Shuffle(); 

        placementManager.Placement(list);

        relationManager.SetHierarchy(list);

        relationManager.SetChildren();
    }

    public void Shuffle()
    {
        for (int i = 0; i < list.Count; i++)
        {
            int index = Random.Range(0, list.Count);

            (list[i], list[index]) = (list[index], list[i]);
        }
    }
}


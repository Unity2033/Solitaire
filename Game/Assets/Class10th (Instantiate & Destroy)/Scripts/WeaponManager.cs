using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WeaponManager : MonoBehaviour
{
    [SerializeField] int count = 0;

    [SerializeField] GameObject [] weapons;
    
    [SerializeField] Transform parentPosition;

    [SerializeField] List<GameObject> weaponsList;


    void Start()
    {
        for(int i = 0; i < weapons.Length; i++)
        {
            GameObject clone = Instantiate(weapons[i], parentPosition);

            clone.gameObject.SetActive(false);

            weaponsList.Add(clone);
        }

        weaponsList[count].gameObject.SetActive(true);
    }

    private void Update()
    {
        if (Input.GetKeyDown(KeyCode.Space))
        {
            Swap();
        }

        if (Input.GetMouseButtonDown(0))
        {
            Attack();
        }
    }

    public void Swap()
    {
        weaponsList[count].gameObject.SetActive(false);

        count = (count + 1) % weaponsList.Count;

        weaponsList[count].gameObject.SetActive(true);
    }

    public void Attack()
    {
        weaponsList[count].GetComponent<Weapon>().Attack();
    }
}
